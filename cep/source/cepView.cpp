/* 
   UI for the CEP program
   Copyright (C) Michael Still                    2002

   Based on a demo which originally had this notice:
  
   ////////////////////////////////////////////////////////////////////////////
   // Name:        view.cpp
   // Purpose:     View classes
   // Author:      Julian Smart
   // Created:     04/01/98
   // Copyright:   (c) Julian Smart and Markus Holzem
   // Licence:     wxWindows license
   ////////////////////////////////////////////////////////////////////////////

   [The wxWindows license is compatible with the GNU GPL, as it is the GNU
   LGPL]
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifdef __GNUG__
// #pragma implementation
#endif

// For compilers that support precompilation, includes "wx/wx.h".
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

#include "cepUI.h"
#include "doc.h"
#include "view.h"

IMPLEMENT_DYNAMIC_CLASS (cepView, wxView)
BEGIN_EVENT_TABLE (cepView, wxView)
END_EVENT_TABLE ()
// What to do when a view is created. Creates actual
// windows for displaying the view.
     bool
     cepView::OnCreate (wxDocument * doc, long WXUNUSED (flags))
{
  frame = wxGetApp ().CreateChildFrame (doc, this, TRUE);
  frame->SetTitle ("cepView");

  canvas = GetMainFrame ()->CreateCanvas (this, frame);
#ifdef __X__
  // X seems to require a forced resize
  int x, y;
  frame->GetSize (&x, &y);
  frame->SetSize (-1, -1, x, y);
#endif
  frame->Show (TRUE);
  Activate (TRUE);

  return TRUE;
}

// Sneakily gets used for default print/preview
// as well as drawing on the screen.
void
cepView::OnDraw (wxDC * dc)
{
  dc->SetFont (*wxNORMAL_FONT);
  dc->SetPen (*wxBLACK_PEN);
}

void
cepView::OnUpdate (wxView * WXUNUSED (sender), wxObject * WXUNUSED (hint))
{
  if (canvas)
    canvas->Refresh ();

/* Is the following necessary?
#ifdef __WXMSW__
  if (canvas)
    canvas->Refresh();
#else
  if (canvas)
    {
      wxClientDC dc(canvas);
      dc.Clear();
      OnDraw(& dc);
    }
#endif
*/
}

// Clean up windows used for displaying the view.
bool
cepView::OnClose (bool deleteWindow)
{
  if (!GetDocument ()->Close ())
    return FALSE;

  // Clear the canvas in  case we're in single-window mode,
  // and the canvas stays.
  canvas->Clear ();
  canvas->view = (wxView *) NULL;
  canvas = (MyCanvas *) NULL;

  wxString s (wxTheApp->GetAppName ());
  if (frame)
    frame->SetTitle (s);

  SetFrame ((wxFrame *) NULL);

  Activate (FALSE);

  if (deleteWindow)
    {
      delete frame;
      return TRUE;
    }
  return TRUE;
}

/*
 * Window implementations
 */

BEGIN_EVENT_TABLE (MyCanvas, wxScrolledWindow)
EVT_MOUSE_EVENTS (MyCanvas::OnMouseEvent) END_EVENT_TABLE ()
// Define a constructor for my canvas
MyCanvas::MyCanvas (wxView * v, wxFrame * frame, const wxPoint & pos, const wxSize & size, long style):
wxScrolledWindow (frame, -1, pos, size, style)
{
  view = v;
}

// Define the repainting behaviour
void
MyCanvas::OnDraw (wxDC & dc)
{
  if (view)
    view->OnDraw (&dc);
}

// This implements a tiny doodling program. Drag the mouse using
// the left button.
void
MyCanvas::OnMouseEvent (wxMouseEvent & event)
{
  if (!view)
    return;

  wxClientDC dc (this);
  PrepareDC (dc);
  dc.SetPen (*wxBLACK_PEN);
  wxPoint pt (event.GetLogicalPosition (dc));
}

// Define a constructor for my text subwindow
MyTextWindow::MyTextWindow (wxView * v, wxFrame * frame, const wxPoint & pos,
			    const wxSize & size, long style):
wxTextCtrl (frame, -1, "", pos, size, style)
{
  view = v;
}
