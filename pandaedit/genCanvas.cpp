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
  m_frame (frame)
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

  // Find out if we're over an object
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
	  over += string(" over object id ") + toString(demangled);
	}
    }

  // If we're snapping to a grid, find the closest grid point, and change to
  // that
  // TODO mikal: should snap be used for object selection as well?
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

  string msg = toString(pt.x) + string(", ") + toString(pt.y) + 
    string(" of ") + toString(m_width) + string(", ") + toString(m_height) +
    over;
  ((wxFrame *) wxGetApp ().GetTopWindow ())->
	SetStatusText (msg.c_str(), 0);
 
  // End the current instance of a tool, but not the tool
  if(event.LeftIsDown() && event.ControlDown())
    {
      debug(dlTrace, "Tool instance ended, tool still selected");
      string commandString(""), controlString(""), selectString("");

      if(m_controlPoints.size() > 0){
	// Start the line we are drawing
	// TODO mikal: fixed line colour
	commandString += "q\n0.0 1.0 0.0 RG\n";
	commandString += toString(m_controlPoints[0].x) + string(" ") +
	  toString(m_height - m_controlPoints[0].y) + string(" m\n");

	// Create a control point blob
	controlString += controlBlob(m_controlPoints[0].x,
				     m_controlPoints[0].y);
      }

      for(unsigned int i = 1; i < m_controlPoints.size(); i++)
	{
	  commandString += toString(m_controlPoints[i].x) + string(" ") +
	    toString(m_height - m_controlPoints[i].y) + string(" l\n");

	  controlString += controlBlob(m_controlPoints[i].x,
				       m_controlPoints[i].y);
	}
      if(m_controlPoints.size() > 0)
	commandString += string("S\nQ\n\n");

      if(m_view)
	{
	  ((pdfView *) m_view)->appendCommand(commandString, controlString,
					      selectString);
	  ((pdfView *) m_view)->setDirty();
	  Refresh();
	}
      m_controlPoints.clear();
    }
  
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

string
genCanvas::controlBlob(unsigned int x, unsigned int y)
{
  // TODO mikal: fixed blob colour
  string rc("q\n1.0 0.0 0.0 RG\n");
  for(unsigned int yc = y - CONTROLSIZE; yc < y + CONTROLSIZE + 1; yc++)
    rc += toString(x - CONTROLSIZE) + string(" ") + toString(m_height - yc) + 
      string(" m\n") +
      toString(x + CONTROLSIZE) + string(" ") + toString(m_height - yc) + 
      string(" l\nS\n");
  rc += "Q\n";
  return rc;
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
