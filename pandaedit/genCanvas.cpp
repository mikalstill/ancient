/* 
 *   Canvas for pandaedit
 *   Copyright (C) Michael Still                    2002
 *
 *   Based on a demo which originally had this notice:
 *  
 *   /////////////////////////////////////////////////////////////////////////
 *   // Name:        view.cpp
 *   // Purpose:     View classes
 *   // Author:      Julian Smart
 *   // Created:     04/01/98
 *   // Copyright:   (c) Julian Smart and Markus Holzem
 *   // Licence:     wxWindows license
 *   /////////////////////////////////////////////////////////////////////////
 *
 *   [The wxWindows license is compatible with the GNU GPL, as it is the GNU
 *   LGPL]
 *   
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *   
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *   
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_DOC_VIEW_ARCHITECTURE
#error You must set wxUSE_DOC_VIEW_ARCHITECTURE to 1 in setup.h!
#endif

#include "genUI.h"
#include "pdfDoc.h"
#include "pdfView.h"

#include <stdlib.h>
#include <unistd.h>

#include <wx/colour.h>
#include <wx/colordlg.h>

#include "genCanvas.h"
#include "verbosity.h"
#include "configuration.h"
#include "idmangle.h"

#define CONTROLSIZE 2
#define GRIDSPACING 10

BEGIN_EVENT_TABLE (genCanvas, wxScrolledWindow)
EVT_MOUSE_EVENTS (genCanvas::OnMouseEvent) END_EVENT_TABLE ()

// Define a constructor for my canvas
genCanvas::genCanvas (wxView * v, wxFrame * frame, const wxPoint & pos,
		      const wxSize & size, long style):
  wxScrolledWindow (frame, -1, pos, size, style),
  m_view (v),
  m_frame (frame),
  m_editting(false),
  m_editTarget(-1)
{
}

// Define the repainting behaviour
void
genCanvas::OnDraw (wxDC & dc)
{
  if (m_view)
    m_view->OnDraw (&dc);
}

// Process mouse events, these can include mouse moves, and clicks
void
genCanvas::OnMouseEvent (wxMouseEvent & event)
{
  wxClientDC dc (this);
  PrepareDC (dc);
  wxPoint pt (event.GetLogicalPosition (dc));

  // Check the point is valid
  if(pt.x < 0)
    return;
  if(pt.y < 0)
    return;
  if(pt.x > m_width)
    return;
  if(pt.y > m_height)
    return;

  // Find out if we're over an object, and process a selection event if there
  // is one
  string over("");
  char *selRaster;
  if(m_view && ((selRaster = ((pdfView *) m_view)->getSelectRaster()) != NULL))
    {
      // This assumes the bit depth of the raster returned, and will break
      // if the rules change
      unsigned long inset = (pt.y * m_width + pt.x) * 3;
      int demangled = demangle(selRaster[inset], selRaster[inset + 1], 
			       selRaster[inset + 2]);
      if(demangled < 16777215)
	{
	  // Selection event
	  if(event.RightIsDown())
	    {
	      endTool();

	      // TODO mikal: we don't use the command type here...
	      object::commandType type;
	      m_controlPoints = ((pdfView *) m_view)->getCommand(demangled - 1,
								 type);
	      debug(dlTrace, string("User selected object ") +
		    toString(demangled));
	      debug(dlTrace, string("Number of selected control points: ") +
		    toString(m_controlPoints.size()));
	      over += " selection";
	      m_editting = true;
	      m_editTarget = demangled - 1;

	      // We also need to paint the control points onto the canvas...
	      showControlPoints(dc, false);
	    }

	  over += string(" over object id ") + toString(demangled);
	  ((pdfView *) m_view)->setHoverTarget(demangled);
	}
      else
	((pdfView *) m_view)->setHoverTarget(-1);
    }

  // If we're snapping to a grid, find the closest grid point, and change to
  // that
  configuration *config;
  bool gridSnap;
  config = (configuration *) & configuration::getInstance ();
  config->getValue("pref-snaptogrid", false, gridSnap);
  if(gridSnap){
    int xmod, ymod;
    xmod = pt.x % GRIDSPACING;
    ymod = pt.y % GRIDSPACING;

    if(xmod > 5) pt.x += (GRIDSPACING - xmod);
    else pt.x -= xmod;
    if(ymod > 5) pt.y += (GRIDSPACING - ymod);
    else pt.y -= ymod;
  }

  // Tell the world what is happening
  string msg = toString(pt.x) + string(", ") + toString(pt.y) + 
    string(" of ") + toString(m_width) + string(", ") + toString(m_height) +
    over;
  ((wxFrame *) wxGetApp ().GetTopWindow ())->
	SetStatusText (msg.c_str(), 0);
 
  // End the current instance of a tool, but not the tool
  if(event.LeftIsDown() && event.ControlDown())
    endTool();
  
  // Continue with the current tool
  else if(event.LeftIsDown())
    {
      // This sets the device context so that our drawing causes an inversion, 
      // lines are drawn with black, and polygons are filled with black.
      dc.SetLogicalFunction (wxINVERT);
      dc.SetPen (*wxBLACK_PEN);
      dc.SetBrush (*wxBLACK_BRUSH);
      
      if(m_controlPoints.size() == 0)
	dc.DrawPoint(pt);
      else{
	dc.DrawLine(m_controlPoints[m_controlPoints.size() - 1], pt);
      }

      m_controlPoints.push_back(pt);
    }
}

void
genCanvas::setHeight(int height)
{
  debug(dlTrace, string("Canvas height set to: ") + toString(height));
  m_height = height;
}

void
genCanvas::setWidth(int width)
{
  debug(dlTrace, string("Canvas width set to: ") + toString(width));
  m_width = width;
}

void
genCanvas::endTool()
{
  debug(dlTrace, "Tool instance ended, tool still selected");
  if(m_controlPoints.size() == 0)
    return;

  if(m_view)
    {
      if(!m_editting)
	{
	  ((pdfView *) m_view)->setHeight(m_height);
	  ((pdfView *) m_view)->appendCommand(object::cLine,
					      m_controlPoints);
	}
      else
	{
	  ((pdfView *) m_view)->rewriteCommand(m_editTarget,
					       object::cLine,
					       m_controlPoints);
	  m_editting = false;
	  m_editTarget = -1;
	}
      ((pdfView *) m_view)->setDirty();
      Refresh();
    }
  m_controlPoints.clear();
}

void
genCanvas::showControlPoints(wxClientDC & dc, bool onlyMostRecent)
{
  dc.SetLogicalFunction (wxINVERT);
  dc.SetPen (*wxBLACK_PEN);
  dc.SetBrush (*wxBLACK_BRUSH);
  for(unsigned int i = (onlyMostRecent ? m_controlPoints.size() - 1 : 0); 
      i < m_controlPoints.size(); i++)
    {
      // TODO mikal: make nicer?
      dc.DrawLine(wxPoint(m_controlPoints[i].x - CONTROLSIZE,
			  m_controlPoints[i].y - CONTROLSIZE),
		  wxPoint(m_controlPoints[i].x + CONTROLSIZE,
			  m_controlPoints[i].y + CONTROLSIZE));
      dc.DrawLine(wxPoint(m_controlPoints[i].x - CONTROLSIZE,
			  m_controlPoints[i].y + CONTROLSIZE),
		  wxPoint(m_controlPoints[i].x + CONTROLSIZE,
			  m_controlPoints[i].y - CONTROLSIZE));
    }
}
