
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
#include "cepLs.h"

#include <stdlib.h>
#include <unistd.h>

#include <wx/colour.h>
#include <wx/colordlg.h>

IMPLEMENT_DYNAMIC_CLASS (cepView, wxView)
BEGIN_EVENT_TABLE (cepView, wxView) 
  EVT_MENU (CEPMENU_AVERAGE, cepView::OnToggleAverage)
  EVT_MENU (CEPMENU_ERRORS, cepView::OnToggleErrors)
  EVT_MENU (CEPMENU_COLORAXES, cepView::OnColorAxes)
  EVT_MENU (CEPMENU_COLORLINE, cepView::OnColorLine)
  EVT_MENU (CEPMENU_COLORAVERAGE, cepView::OnColorAverage)
  EVT_MENU (CEPMENU_COLORERROR, cepView::OnColorError)
  EVT_MENU (CEPMENU_ELIMINATEOUTLIERS, cepView::OnEliminateOutliers)
  EVT_MENU (CEPMENU_VIEWCENTERED, cepView::OnViewCentered)
  EVT_MENU (CEPMENU_VIEWZOOMED, cepView::OnViewZoomed)
  EVT_MENU (CEPMENU_SHOWX, cepView::OnToggleX)
  EVT_MENU (CEPMENU_SHOWY, cepView::OnToggleY)
  EVT_MENU (CEPMENU_SHOWZ, cepView::OnToggleZ)
  EVT_MENU (CEPMENU_LS, cepView::OnLeastSquares)
END_EVENT_TABLE ()
  
cepView::cepView ():
  m_plotfailed(false)
{
  canvas = (cepCanvas *) NULL;
  frame = (wxFrame *) NULL;
  m_config = (cepConfiguration *)&cepConfiguration::getInstance();

  // The matrices are initially NULL
  m_x = NULL;
  m_y = NULL;
  m_z = NULL;
}

cepView::~cepView ()
{
  if(m_x != NULL)
    delete m_x;
  if(m_y != NULL)
    delete m_y;
  if(m_z != NULL)
    delete m_z;
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

  // Extract the dataset
  cepDoc *theDoc = (cepDoc *) GetDocument ();
  cepDataset *theDataset = theDoc->getDataset ();

  // We can only handle this event if we are ready
  if (theDataset && theDataset->isReady())
  {
    // If the dataset was malformed, then we close the tab here
    if (!theDataset->isWellFormed())
    {
      OnClose();
      return;
    }

    // Get the matrices if we don't have them yet
    if(m_x == NULL){
      m_x = theDataset->getMatrix(cepDataset::dirX);
    }

    if(m_y == NULL){
      m_y = theDataset->getMatrix(cepDataset::dirY);
    }

    if(m_z == NULL){
      m_z = theDataset->getMatrix(cepDataset::dirZ);
    }

    // Graph the matrices
    int width, height, gCount = 0;
    frame->GetSize (&width, &height);
    bool showX, showY, showZ;
    
    cepError err;
    err = m_config->getValue("ui-viewmenu-showx", true, showX);
    if(err.isReal()) err.display();
    err = m_config->getValue("ui-viewmenu-showy", true, showY);
    if(err.isReal()) err.display();
    err = m_config->getValue("ui-viewmenu-showz", true, showZ);
    if(err.isReal()) err.display();

    if(showX) gCount++;
    if(showY) gCount++;
    if(showZ) gCount++;

    if(gCount > 0){
      int presHeight = height / gCount - 10;
      int presDrop = 0;
      
      cepDebugPrint ("Dataset valid, so displaying");
      if(showX){
	cepDebugPrint("Displaying x direction graph");
	drawPresentation(theDataset, cepDataset::dirX, presDrop, dc, 
			 presHeight);
	presDrop += presHeight + 10;
      }

      if(showY){
	cepDebugPrint("Displaying y direction graph");
	drawPresentation(theDataset, cepDataset::dirY, presDrop, dc, 
			 presHeight);
	presDrop += presHeight + 10;
      }
      
      if(showZ){
	cepDebugPrint("Displaying z direction graph");
	drawPresentation(theDataset, cepDataset::dirZ, presDrop, dc, 
			 presHeight);
	presDrop += presHeight + 10;
      }
    }
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

  // todo: The following line was causing a segv
  // canvas->Clear ();
  canvas->view = (wxView *) NULL;
  canvas = (cepCanvas *) NULL;

  wxString
  s (wxTheApp->GetAppName ());

  if (frame)
    frame->SetTitle (s);

  SetFrame ((wxFrame *) NULL);
  Activate (FALSE);

  if (deleteWindow)
  {
    cepDebugPrint ("Close the window");
    delete frame;
    return TRUE;
  }
  return TRUE;
}

void cepView::drawPresentation(cepDataset *theDataset, cepDataset::direction dir, int top, wxDC *dc, int presHeight)
{
  // If plotting has failed before, then it will fail now...
  if(m_plotfailed && !m_dirty)
    return;

  // Being marked dirty makes us regenerate the images
  if(m_dirty && (m_pngCache[(int) dir] != "")){
    unlink(m_pngCache[(int) dir].c_str());
    m_pngCache[(int) dir] = "";
  }

  // We cache images so we go faster
  if(m_pngCache[(int) dir] == ""){
    cepPresentation pres (theDataset->getData (dir).size () + 20,
			  presHeight);
    char *cfname = strdup("/tmp/cep.XXXXXX");;
    int fd;
    fd = mkstemp(cfname);
    close(fd);
    cepDebugPrint ("Temporary file for image is " + string(cfname));
    cepDebugPrint ("There are " +
                   cepToString (theDataset->getData (dir).size ()) +
                   " data points to add");

    for (unsigned int i = 0; i < theDataset->getData (dir).size (); i++)
    {
      pres.addDataPoint (i,
			 (long)(theDataset->getData (dir)[i].
				sample * 10000),
			 (long)(theDataset->getData (dir)[i].
				error * 10000));
      cepDebugPrint ("Data point: " +
                     cepToString ((long)
                              (theDataset->getData (dir)[i].
                               sample * 10000)));
    }

    cepError err;
    int red = 0, green = 0, blue = 0;
    bool toggle;
    
    err = m_config->getValue("ui-viewmenu-showaverages", false, toggle);
    if(err.isReal()) err.display();
    pres.useAverage(toggle);

    err = m_config->getValue("ui-viewmenu-showerrors", true, toggle);
    if(err.isReal()) err.display();    
    pres.useErrors(toggle);

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

    err = m_config->getValue("ui-graph-color-error-r", 127, red);
    if(err.isReal()) err.display();
    err = m_config->getValue("ui-graph-color-error-g", 127, green);
    if(err.isReal()) err.display();
    err = m_config->getValue("ui-graph-color-error-b", 127, blue);
    if(err.isReal()) err.display();
    pres.setErrorColor(red, green, blue);

    int currentView;
    err = m_config->getValue("ui-viewmenu-currentview", 
			     (int) cepPresentation::viewCentered, currentView);
    if(err.isReal()) err.display();
    pres.setView((cepPresentation::view) currentView);

    err = pres.createPNG (cfname);
    if (err.isReal ()){
      err.display ();
      m_plotfailed = true;
      return;
    }

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
  m_config->setValue("ui-viewmenu-showaverages", pevt.IsChecked());
  m_dirty = true;

  // todo_mikal: this doesn't work at the moment...
  // Force a repaint of the window
  wxPaintEvent evt(0);
  wxPostEvent(frame, evt);
}

void cepView::OnToggleErrors (wxCommandEvent &pevt)
{
  m_config->setValue("ui-viewmenu-showerrors", pevt.IsChecked());
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

void
cepView::OnColorError (wxCommandEvent & WXUNUSED (event))
{
  wxColourDialog picker(NULL);
  if(picker.ShowModal() == wxID_OK){
    wxColourData data = picker.GetColourData();
    wxColour color = data.GetColour();
    m_config->setValue("ui-graph-color-error-r", color.Red());
    m_config->setValue("ui-graph-color-error-g", color.Green());
    m_config->setValue("ui-graph-color-error-b", color.Blue());

    m_dirty = true;
    // todo_mikal: post paint event
  }
}

void cepView::OnEliminateOutliers(wxCommandEvent& event)
{
  // todo_mikal: not implemented at this time (and disabled in the menu)
  cepEliminateDialog elim;
  elim.display();
}

void cepView::OnViewCentered(wxCommandEvent& event)
{
  wxMenuBar *bar = frame->GetMenuBar();
  if(bar){
    wxMenu* view_menu = bar->GetMenu(bar->FindMenu("View"));
    if(view_menu){
      view_menu->Check(CEPMENU_VIEWCENTERED, true);
      view_menu->Check(CEPMENU_VIEWZOOMED, false);

      m_config->setValue("ui-viewmenu-currentview",
			 cepPresentation::viewCentered);
      m_dirty = true;
    }
  }
}

void cepView::OnViewZoomed(wxCommandEvent& event)
{
  wxMenuBar *bar = frame->GetMenuBar();
  if(bar){
    wxMenu* view_menu = bar->GetMenu(bar->FindMenu("View"));
    if(view_menu){
      view_menu->Check(CEPMENU_VIEWCENTERED, false);
      view_menu->Check(CEPMENU_VIEWZOOMED, true);

      m_config->setValue("ui-viewmenu-currentview",
			 cepPresentation::viewZoomed);
      m_dirty = true;
    }
  }
}

void cepView::OnToggleX (wxCommandEvent &pevt)
{
  m_config->setValue("ui-viewmenu-showx", pevt.IsChecked());
  m_dirty = true;

  // todo_mikal: this doesn't work at the moment...
  // Force a repaint of the window
  cepDebugPrint("Posting a paint event for the window " +
		cepToString(frame->GetId()));
  wxPaintEvent event( frame->GetId() );
  wxPostEvent(frame, event);
}

void cepView::OnToggleY (wxCommandEvent &pevt)
{
  m_config->setValue("ui-viewmenu-showy", pevt.IsChecked());
  m_dirty = true;

  // todo_mikal: this doesn't work at the moment...
  // Force a repaint of the window
  cepDebugPrint("Posting a paint event for the window " +
		cepToString(frame->GetId()));
  wxPaintEvent event( frame->GetId() );
  wxPostEvent(frame, event);
}

void cepView::OnToggleZ (wxCommandEvent &pevt)
{
  m_config->setValue("ui-viewmenu-showz", pevt.IsChecked());
  m_dirty = true;

  // todo_mikal: this doesn't work at the moment...
  // Force a repaint of the window
  wxPaintEvent evt(0);
  wxPostEvent(frame, evt);
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

void
cepCanvas::OnMouseEvent (wxMouseEvent & event)
{
  if (!view)
    return;

  wxClientDC dc (this);

  PrepareDC (dc);
  dc.SetPen (*wxBLACK_PEN);
  // wxPoint pt (event.GetLogicalPosition (dc));
}

// Perform a least squares regression on the dataset (in all directions)
void cepView::OnLeastSquares (wxCommandEvent &pevt)
{
  m_dirty = true;

  LeastSquares(m_x, "x");
  LeastSquares(m_y, "y");
  LeastSquares(m_z, "z");
  
  // todo_mikal post redraw
}

void cepView::LeastSquares(cepMatrix<double> *mat, string direction)
{
  // Do we have any data?
  if(mat == NULL){
    cepError err("Cannot perform least squares regression on the " + 
		 direction + "matrix, because it is empty",
		 cepError::sevErrorRecoverable);
    err.display();
    return;
  }

  // Build a temporary matrix P matrix
  // todo_mikal: Build a UI
  cepMatrix<double> pmatrix(mat->getNumRows(), mat->getNumRows());

  // Do the LS
  cepLs myLs;
  myLs.cepDoVCV(*mat, pmatrix);
}
