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
#include "wx/image.h"

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
#include "cepDoc.h"
#include "cepView.h"

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

  cepDebugPrint("Drawing the image, png is type " + 
		cepItoa(wxBITMAP_TYPE_PNG));

  // todo: this needs to be worked on -- I want to remove the temporary file...
  cepPresentation myPres(400, 200);
  string filename("/tmp/foo.png"); //mktemp("/tmp/cep.XXXXXX"));

  cepDebugPrint("Adding x direction data points");
  cepDoc *theDoc = (cepDoc *) GetDocument();
  cepDataset *theDataset = theDoc->getDataset();
  vector < cep_datarow > & xdata = theDataset->getDataPtr (cepDataset::dirX);

  // todo_mikal: what is the type of the vector index?
  cepDebugPrint("There are " + 
		cepItoa(theDataset->getDataPtr(cepDataset::dirX).size()) + 
		" data points to add");
  //  for(int i = 0; i < xdata.size(); i++){
  //    cepDebugPrint("Adding: " + cepItoa(i) + ", " + cepFtoa(xdata[i].sample));
  //    myPres.addDataPoint(i, (int) xdata[i].sample);
  //  }
  
  // Draw the sin graph
  //  int x;
  //  double angle;
  //
  //  x = 10;
  //  for(angle = 0.0; angle < 12 * 3.1415; angle += 0.1){
  //    x++;
  //    myPres.addDataPoint(x, (long) (100 - sin(angle) * 90)); 
  //  }

  cepDebugPrint("Temporary file for image is " + filename);
  cepError err = myPres.createPNG(filename);
  if(err.isReal()){
    err.display(); 
  }
  
  try{
    wxImage theImage(filename.c_str(), wxBITMAP_TYPE_PNG);
    wxBitmap theBitmap(theImage.ConvertToBitmap());
    dc->DrawBitmap(theBitmap, 30, 150);
  }
  catch( ... ){
    cepDebugPrint("Exception caught in the graph draw routine");
  }    
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
  cepDebugPrint("Close called for a cepView");
  if (!GetDocument ()->Close ())
    return FALSE;

  // Clear the canvas in  case we're in single-window mode,
  // and the canvas stays.
  cepDebugPrint("Clean up the canvas");
  
  // The following line was causing a segv
  //  canvas->Clear ();
  canvas->view = (wxView *) NULL;
  canvas = (cepCanvas *) NULL;

  wxString s (wxTheApp->GetAppName ());
  if (frame)
    frame->SetTitle (s);

  SetFrame ((wxFrame *) NULL);

  Activate (FALSE);

  cepDebugPrint("Close the window");
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

BEGIN_EVENT_TABLE (cepCanvas, wxScrolledWindow)
EVT_MOUSE_EVENTS (cepCanvas::OnMouseEvent) 
END_EVENT_TABLE ()

// Define a constructor for my canvas
cepCanvas::cepCanvas (wxView * v, wxFrame * frame, const wxPoint & pos, 
		      const wxSize & size, long style):
  wxScrolledWindow (frame, -1, pos, size, style)
{
  view = v;

  // Is this where we create new controls?
  wxPoint pos, size;
  pos.x = 100;
  pos.y = 42;
  size.x = -1;
  size.y = -1;

  //  m_button = new wxButton(v, -1, "This is a button", pos, size, 
}

// Define the repainting behaviour
void
cepCanvas::OnDraw (wxDC & dc)
{
  if (view)
    view->OnDraw (&dc);
}

// This implements a tiny doodling program. Drag the mouse using
// the left button.
void
cepCanvas::OnMouseEvent (wxMouseEvent & event)
{
  if (!view)
    return;

  wxClientDC dc (this);
  PrepareDC (dc);
  dc.SetPen (*wxBLACK_PEN);
  wxPoint pt (event.GetLogicalPosition (dc));
}
