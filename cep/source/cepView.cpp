
/* 
 *   UI for the CEP program
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
#include "cepDoc.h"
#include "cepView.h"
#include <stdlib.h>
#include <unistd.h>

#include <wx/colour.h>
#include <wx/colordlg.h>

#define PRESHEIGHT 200

IMPLEMENT_DYNAMIC_CLASS (cepView, wxView)
BEGIN_EVENT_TABLE (cepView, wxView) 
  EVT_MENU (CEPMENU_AVERAGE, cepView::OnToggleAverage)
  EVT_MENU (CEPMENU_COLORAXES, cepView::OnColorAxes)
  EVT_MENU (CEPMENU_COLORLINE, cepView::OnColorLine)
  EVT_MENU (CEPMENU_COLORAVERAGE, cepView::OnColorAverage)
  EVT_MENU (CEPMENU_ELIMINATEOUTLIERS, cepView::OnEliminateOutliers)
END_EVENT_TABLE ()

cepView::cepView ()
{
  canvas = (cepCanvas *) NULL;
  frame = (wxFrame *) NULL;
  m_config = (cepConfiguration *)&cepConfiguration::getInstance();
}

cepView::~cepView ()
{
}

// What to do when a view is created. Creates actual
// windows for displaying the view.
  bool cepView::OnCreate (wxDocument * doc, long WXUNUSED (flags))
{
  frame = wxGetApp ().CreateChildFrame (doc, this, TRUE);
  frame->SetTitle ("cepView");

  canvas = GetMainFrame ()->CreateCanvas (this, frame);

#ifdef __X__
  // X seems to require a forced resize
  int
    x,
    y;

  frame->GetSize (&x, &y);
  frame->SetSize (-1, -1, x, y);
#endif

  frame->Show (TRUE);
  Activate (TRUE);

  m_showAverages = false;
  m_dirty = true;

  return TRUE;
}

// Sneakily gets used for default print/preview
// as well as drawing on the screen.
void
cepView::OnDraw (wxDC * dc)
{
  dc->SetFont (*wxNORMAL_FONT);
  dc->SetPen (*wxBLACK_PEN);

  cepDoc *theDoc = (cepDoc *) GetDocument ();
  cepDataset *theDataset = theDoc->getDataset ();

  if (theDataset && theDataset->isReady())
  {
    cepDebugPrint ("Dataset valid, so displaying");
    drawPresentation(theDataset, cepDataset::dirX, 0, dc);
    drawPresentation(theDataset, cepDataset::dirY, PRESHEIGHT, dc);
    drawPresentation(theDataset, cepDataset::dirZ, PRESHEIGHT * 2, dc);
    m_dirty = false;
  }
  else
  {
    cepDebugPrint ("Dataset currently invalid, so not displaying");
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
bool cepView::OnClose (bool deleteWindow)
{
  cepDebugPrint ("Close called for a cepView");
  if (!GetDocument ()->Close ())
    return FALSE;

  // Clear the canvas in case we're in single-window mode,
  // and the canvas stays.
  cepDebugPrint ("Clean up the canvas");

  // The following line was causing a segv
  // canvas->Clear ();
  canvas->view = (wxView *) NULL;
  canvas = (cepCanvas *) NULL;

  wxString
  s (wxTheApp->GetAppName ());

  if (frame)
    frame->SetTitle (s);

  SetFrame ((wxFrame *) NULL);

  Activate (FALSE);

  cepDebugPrint ("Close the window");
  if (deleteWindow)
  {
    delete
      frame;

    return TRUE;
  }
  return TRUE;
}

void cepView::drawPresentation(cepDataset *theDataset, cepDataset::direction dir, int top, wxDC *dc)
{
  // Being marked dirty makes us regenerate the images
  if(m_dirty && (m_pngCache[(int) dir] != "")){
    unlink(m_pngCache[(int) dir].c_str());
    m_pngCache[(int) dir] = "";
  }

  // We cache images so we go faster
  if(m_pngCache[(int) dir] == ""){
    cepPresentation pres (theDataset->getData (dir).size () + 20,
			  PRESHEIGHT);
    char *cfname = strdup("/tmp/cep.XXXXXX");;
    int fd;
    fd = mkstemp(cfname);
    close(fd);
    cepDebugPrint ("Temporary file for image is " + string(cfname));
    cepDebugPrint ("There are " +
                   cepItoa (theDataset->getData (dir).size ()) +
                   " data points to add");

    for (int i = 0; i < theDataset->getData (dir).size (); i++)
    {
      pres.addDataPoint (i,
                           (long)(theDataset->getData (dir)[i].
                                  sample * 10000));
      cepDebugPrint ("Data point: " +
                     cepLtoa ((long)
                              (theDataset->getData (dir)[i].
                               sample * 10000)));
    }

    pres.useAverage(m_showAverages);
    
    cepError err;
    int red = 0, green = 0, blue = 0;

    err = m_config->getValue("ui-graph-color-axis-r", 255, red);
    if(err.isReal()) err.display();
    err = m_config->getValue("ui-graph-color-axis-g", 0, green);
    if(err.isReal()) err.display();
    err = m_config->getValue("ui-graph-color-axis-b", 0, blue);
    if(err.isReal()) err.display();
    pres.setAxesColor(red, green, blue);

    err = m_config->getValue("ui-graph-color-line-r", 0, red);
    if(err.isReal()) err.display();
    err = m_config->getValue("ui-graph-color-line-g", 255, green);
    if(err.isReal()) err.display();
    err = m_config->getValue("ui-graph-color-line-b", 0, blue);
    if(err.isReal()) err.display();
    pres.setLineColor(red, green, blue);

    err = m_config->getValue("ui-graph-color-average-r", 0, red);
    if(err.isReal()) err.display();
    err = m_config->getValue("ui-graph-color-average-g", 0, green);
    if(err.isReal()) err.display();
    err = m_config->getValue("ui-graph-color-average-b", 255, blue);
    if(err.isReal()) err.display();
    pres.setAverageColor(red, green, blue);

    err = pres.createPNG (cfname);
    if (err.isReal ()) err.display ();

    m_pngCache[(int) dir] = string(cfname);
  }
  
  try
    {
      wxImage theImage (m_pngCache[(int) dir].c_str(), wxBITMAP_TYPE_PNG);
      wxBitmap theBitmap (theImage.ConvertToBitmap ());
      dc->DrawBitmap (theBitmap, 0, top);
    }
  catch (...)
    {
      cepDebugPrint ("Exception caught in the graph draw routine");
    }
}

void cepView::OnToggleAverage (wxCommandEvent &pevt)
{
  m_showAverages = pevt.IsChecked();
  m_dirty = true;

  // todo_mikal: this doesn't work at the moment...
  // Force a repaint of the window
  wxPaintEvent evt(0);
  wxPostEvent(frame, evt);
}

void
cepView::OnColorAxes (wxCommandEvent & WXUNUSED (event))
{
  wxColourDialog picker(NULL);
  if(picker.ShowModal() == wxID_OK){
    wxColourData data = picker.GetColourData();
    wxColour color = data.GetColour();
    m_config->setValue("ui-graph-color-axis-r", color.Red());
    m_config->setValue("ui-graph-color-axis-g", color.Green());
    m_config->setValue("ui-graph-color-axis-b", color.Blue());

    m_dirty = true;
    // todo_mikal: post paint event
  }
}

void
cepView::OnColorLine (wxCommandEvent & WXUNUSED (event))
{
  wxColourDialog picker(NULL);
  if(picker.ShowModal() == wxID_OK){
    wxColourData data = picker.GetColourData();
    wxColour color = data.GetColour();
    m_config->setValue("ui-graph-color-line-r", color.Red());
    m_config->setValue("ui-graph-color-line-g", color.Green());
    m_config->setValue("ui-graph-color-line-b", color.Blue());

    m_dirty = true;
    // todo_mikal: post paint event
  }
}

void
cepView::OnColorAverage (wxCommandEvent & WXUNUSED (event))
{
  wxColourDialog picker(NULL);
  if(picker.ShowModal() == wxID_OK){
    wxColourData data = picker.GetColourData();
    wxColour color = data.GetColour();
    m_config->setValue("ui-graph-color-average-r", color.Red());
    m_config->setValue("ui-graph-color-average-g", color.Green());
    m_config->setValue("ui-graph-color-average-b", color.Blue());

    m_dirty = true;
    // todo_mikal: post paint event
  }
}

void cepView::OnEliminateOutliers(wxCommandEvent& event)
{
  //  cepEliminateDialog elim;
  //  elim.display();
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

  // m_button = new wxButton(v, -1, "This is a button", pos, size, 
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
