/* 
 *   Canvas for the CEP program
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
  if(event.LeftIsDown())
    {
      debug(dlTrace, "Obtaining device context");
      wxClientDC dc (this);
      PrepareDC (dc);
      debug(dlTrace, "Device context ready");
      
      // This sets the device context so that our drawing causes an inversion, 
      // lines are drawn with black, and polygons are filled with black.
      dc.SetLogicalFunction (wxINVERT);
      dc.SetPen (*wxBLACK_PEN);
      dc.SetBrush (*wxBLACK_BRUSH);
      wxPoint pt (event.GetLogicalPosition (dc));
      
      if(m_controlPoints.size() == 0)
	dc.DrawPoint(pt);
      else{
	dc.DrawLine(m_controlPoints[m_controlPoints.size() - 1], pt);
      }

      m_controlPoints.push_back(pt);
    }
}
