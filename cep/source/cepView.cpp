
/* 
 *   UI for the CEP program
 *   Copyright (C) Michael Still                    2002
 *   Copyright (C) Kristy Van Der Vlist             2002
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
#include "cepPresentation.h"
#include "cepPlot.h"
#include "cepLsUi.h"
#include "cepInterpUi.h"
#include "cepWindowUi.h"
#include "cepwindowchebyshev.h"

#include "cepMatrixIO.h"

#include "cepLs.h"
#include "cepInterp.h"


#include <stdlib.h>
#include <unistd.h>

#include <wx/colour.h>
#include <wx/colordlg.h>

IMPLEMENT_DYNAMIC_CLASS (cepView, wxView)
BEGIN_EVENT_TABLE (cepView, wxView)
  EVT_MENU (CEPMENU_ERRORS, cepView::OnToggleErrors)
  EVT_MENU (CEPMENU_COLORAXES, cepView::OnColorAxes)
  EVT_MENU (CEPMENU_COLORLINE, cepView::OnColorLine)
  EVT_MENU (CEPMENU_COLORERROR, cepView::OnColorError)
  EVT_MENU (CEPMENU_SHOWX, cepView::OnToggleX)
  EVT_MENU (CEPMENU_SHOWY, cepView::OnToggleY)
  EVT_MENU (CEPMENU_SHOWZ, cepView::OnToggleZ)
  EVT_MENU (CEPMENU_LS_VCV, cepView::OnLeastSquaresVCV)
  EVT_MENU (CEPMENU_LS_RW, cepView::OnLeastSquaresRW)
  EVT_MENU (CEPMENU_WINDOW_BLACKMAN, cepView::OnWindowBlackman)
  EVT_MENU (CEPMENU_WINDOW_CHEBYSHEV, cepView::OnWindowChebyshev)
  EVT_MENU (CEPMENU_WINDOW_HAMMING, cepView::OnWindowHamming)
  EVT_MENU (CEPMENU_WINDOW_RECT, cepView::OnWindowRect)
  EVT_MENU (CEPMENU_INTERP_NEAREST, cepView::OnInterpNearest)
  EVT_MENU (CEPMENU_INTERP_LINEAR, cepView::OnInterpLinear)
  EVT_MENU (CEPMENU_INTERP_NATURALSPLINE, cepView::OnInterpNaturalSpline)
  EVT_MENU (CEPMENU_INTERP_CUBICSPLINE, cepView::OnInterpCubicSpline)
  EVT_MENU (CEPMENU_INTERP_DIVIDED, cepView::OnInterpDivided)
END_EVENT_TABLE ()

cepView::cepView ():
  m_plotfailed(false)
{
  canvas = (cepCanvas *) NULL;
  frame = (wxFrame *) NULL;
  m_parentFrame = (wxFrame *) NULL;
  m_config = (cepConfiguration *)&cepConfiguration::getInstance();

  // Do we want to display a LS regression?
  cepError err;
  int currentLs;
  err = m_config->getValue("ui-mathmenu-displayls",
			   (int) lsDisplayNone,
			   currentLs);
  if(err.isReal()){
    err.display();
  }
  else{
    m_displayLs = (cepLsDisplay) currentLs;
  }
}

void cepView::setParentFrame(wxFrame *parentFrame)
{
  m_parentFrame = parentFrame;
}

cepView::~cepView ()
{
}

// What to do when a view is created. Creates actual
// windows for displaying the view.
bool cepView::OnCreate (wxDocument * doc, long WXUNUSED (flags))
{
  frame = wxGetApp ().CreateChildFrame (doc, this, TRUE);
  frame->SetTitle ("");

  //  m_parentFrame = wxGetApp().GetMainFrame();
  canvas = GetMainFrame()->CreateCanvas (this, frame);

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
      
      // Set the title of the tab if we haven't already
      if(frame->GetTitle() == "")
	frame->SetTitle(string(theDataset->getName() + " - " + theDataset->getProcHistory()).c_str());
      
      // Graph the matrices
      cepError err;
      int top, bottom, width;
      
      m_config->getValue("ui-viewmenu-showx", true, canvas->m_showx).display();
      m_config->getValue("ui-viewmenu-showy", true, canvas->m_showy).display();
      m_config->getValue("ui-viewmenu-showz", true, canvas->m_showz).display();
      
      for(int dir = (int) cepDataset::dirX; dir < (int) cepDataset::dirUnknown; dir++){
	err = canvas->graphPlacement((cepDataset::direction) dir, top, bottom, width);
	if(err.isReal()){
	  err.display();
	  return;
	}
	drawPresentation(theDataset, (cepDataset::direction) dir, top, dc, 
			 width, bottom - top);
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
  canvas->m_view = (wxView *) NULL;
  canvas = (cepCanvas *) NULL;

  if (frame)
    frame->SetTitle (wxTheApp->GetAppName ());

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

void cepView::drawPresentation(cepDataset *theDataset, cepDataset::direction dir, int top, wxDC *dc, 
			       int presWidth, int presHeight)
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
    char *cfname = strdup("/tmp/cep.XXXXXX");;
    int fd;
    fd = mkstemp(cfname);
    close(fd);
    cepPlot plot(theDataset, dir, cfname, presWidth, presHeight, canvas->m_scale[(int) dir],
		 canvas->m_minval[(int) dir]);
    m_plotfailed = plot.getFailed();
    
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

void cepView::OnToggleErrors (wxCommandEvent &pevt)
{
  m_config->setValue("ui-viewmenu-showerrors", pevt.IsChecked());
  m_dirty = true;
  canvas->Refresh();  
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
    canvas->Refresh();
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
    canvas->Refresh();
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
    canvas->Refresh();
  }
}

void cepView::OnToggleX (wxCommandEvent &pevt)
{
  m_config->setValue("ui-viewmenu-showx", pevt.IsChecked());
  m_dirty = true;
  canvas->Refresh();
}

void cepView::OnToggleY (wxCommandEvent &pevt)
{
  m_config->setValue("ui-viewmenu-showy", pevt.IsChecked());
  m_dirty = true;
  canvas->Refresh();
}

void cepView::OnToggleZ (wxCommandEvent &pevt)
{
  m_config->setValue("ui-viewmenu-showz", pevt.IsChecked());
  m_dirty = true;
  canvas->Refresh();
}

// Perform a least squares regression on the dataset (in all directions)
void cepView::OnLeastSquaresVCV (wxCommandEvent &pevt)
{
  cepMatrix <double> residuals, matP;

  cepDoc *theDoc = (cepDoc *) GetDocument ();
  cepDataset *theDataset = theDoc->getDataset ();
  if (theDataset && theDataset->isReady() && theDataset->isWellFormed())
  {
    // Force a redraw of the graphs later
    m_dirty = true;
      
    cepLsUi lsUi;
    lsUi.showIsReweight();
    if(lsUi.getIsReweight() != -1)
	  {
	    lsUi.showWhichDir();
	  
	    for(int i = 0; i < cepDataset::dirUnknown; i++)
	    {
	      cepDebugPrint("Reweighting in the " + cepToString(i) + " direction");

	      const char dirNames[] = {'x', 'y', 'z'};
	      if (lsUi.getWhichDir (dirNames[i]) == true)
		    {
		      cepDebugPrint("User selected to reweight in this direction");

		      cepLs thisLs;

          if (lsUi.getIsReweight () == 1)
		      {
		        thisLs.cepDoVCV (*theDataset->getMatrix ((cepDataset::direction) i));
		        if(thisLs.getError().isReal() == true)
            {
              thisLs.getError().display();
              goto END;
            }  
			residuals = thisLs.getResidual ();
		        cepDebugPrint("Resolved LS regression");

		        cout << "equation of the line is " << endl;
		        cout << "y=" << thisLs.getB1 () << "x+" << thisLs.getB2 () << endl;
		        cout << "residuals are: " << endl;
		        for (int i = 0; i < residuals.getNumRows (); i++)
			      {
			        cout << residuals.getValue (i, 0) << " ";
		  	    }
		        cout << endl;
		      
		      }
		      else
		      {
		        const char *dirStrings[] = {"x (North)", "y (East)", "z (Up"};
		        lsUi.showIsReadP (dirStrings[i]);
		        if (lsUi.getIsReadP () == 1)
			      {
			        lsUi.showGetfNameP ();
			        if (lsUi.getfNameP () != "")
			        {
			          cout << "x selected: no rewight" << endl;
			          cout << "file read is: " << lsUi.getfNameP () << endl;
			          matP = cepReadMatrix (lsUi.getfNameP ());
			          for (int i = 0; i < matP.getNumRows (); i++)
				        {
				          for (int j = 0; j < matP.getNumCols (); j++)
				          {
				            cout << matP.getValue (i, j) << " ";
				          }
				          cout << endl;
				        }
			          thisLs.cepDoVCV (*theDataset->getMatrix (cepDataset::dirX), matP);
			          	          thisLs.cepDoVCV (*theDataset->getMatrix (cepDataset::dirX), matP);
                if(thisLs.getError().isReal() == true)
                {
                  thisLs.getError().display();
                  goto END;
                }
				  residuals = thisLs.getResidual ();
			      
			          cout << "equation of the line is " << endl;
			          cout << "y=" << thisLs.getB1 () << "x+" << thisLs.getB2 () << endl;
			          cout << "residuals are: " << endl;
			          for (int i = 0; i < residuals.getNumRows (); i++)
				        {
				          cout << residuals.getValue (i, 0) << " ";
				        }
			          cout << endl;
			        }
			      }
		        else
			      {
			        if (lsUi.getIsReadP () == 0)
			        {
			          cout << "re-weight graph thingie goes here " << endl;
			        }
			      }
		      }
		    }
	      else
		    {
		      cepDebugPrint("User did not request a LS regression in this direction");
		    }
	    }
	  }
      // Actually force the graphs to redraw
      canvas->Refresh();
  }
END: cout << ""; 
}

// Perform a least squares regression on the dataset (in all directions)
void cepView::OnLeastSquaresRW (wxCommandEvent &pevt)
{
//   m_dirty = true;

//   LeastSquaresVCV(m_x, "x");
//   LeastSquaresVCV(m_y, "y");
//   LeastSquaresVCV(m_z, "z");

//   m_displayLs = lsDisplayVCV;
//   cepError err;
//   err = m_config->setValue("ui-mathmenu-displayls", (int) m_displayLs);
//   if(err.isReal()){
//     err.display();
//   }

//   canvas->Refresh();
}

void cepView::LeastSquaresRW(cepMatrix<double> *mat, string direction)
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

void cepView::OnWindowBlackman (wxCommandEvent& event)
{}

void cepView::OnWindowChebyshev (wxCommandEvent& event)
{
  cepWindowUi windowUi;

  windowUi.showBandwidth();

  while( isnan( windowUi.getBandwidth()))
  {
    cepError("Error. Attenuation must be a number", cepError::sevWarning).display();
    windowUi.showBandwidth();
  }

  double bw = windowUi.getBandwidth();
  if( bw != cepWindowBandwidth::UNINITIALISED ){
    cepError err = cepWindowChebyshev::setTransitionBandwidth( bw );
    if( err.isReal() ) {
      err.display();
      return;
    }
    cout << "value accepted" << endl;
  }
}

void cepView::OnWindowHamming (wxCommandEvent& event)
{}

void cepView::OnWindowRect (wxCommandEvent& event)
{}

void cepView::OnInterpNearest (wxCommandEvent& event)
{
  cepInterpUi interpUi;

  interpUi.showSampleRate();

  while(interpUi.getSampleRate() == -2)
  {
    cepError("Error. Sample rate must be a number", cepError::sevWarning).display();
    interpUi.showSampleRate();
  }

  if(interpUi.getSampleRate()!= -1){
    cout << "call Nearst Neighbour" << endl;
  }
}

void cepView::OnInterpLinear (wxCommandEvent& event)
{
  cepInterpUi interpUi;

  interpUi.showSampleRate();

  while(interpUi.getSampleRate() == -2)
  {
    cepError("Error. Sample rate must be a number", cepError::sevWarning).display();
    interpUi.showSampleRate();
  }

  if(interpUi.getSampleRate()!= -1){
    cout << "call LInerar" << endl;
  }
}

void cepView::OnInterpNaturalSpline (wxCommandEvent& event)
{
  cepInterpUi interpUi;

  interpUi.showSampleRate();

  while(interpUi.getSampleRate() == -2)
  {
    cepError("Error. Sample rate must be a number", cepError::sevWarning).display();
    interpUi.showSampleRate();
  }

  if(interpUi.getSampleRate()!= -1){
    cout << "call natural spline" << endl;
  }
}

void cepView::OnInterpCubicSpline (wxCommandEvent& event)
{
    cepInterpUi interpUi;

  interpUi.showSampleRate();

  while(interpUi.getSampleRate() == -2)
  {
    cepError("Error. Sample rate must be a number", cepError::sevWarning).display();
    interpUi.showSampleRate();
  }

  if(interpUi.getSampleRate()!= -1){
    cout << "call cubic spline" << endl;
  }
}

void cepView::OnInterpDivided (wxCommandEvent& event)
{
    cepInterpUi interpUi;

  interpUi.showSampleRate();

  while(interpUi.getSampleRate() == -2)
  {
    cepError("Error. Sample rate must be a number", cepError::sevWarning).display();
    interpUi.showSampleRate();
  }

  if(interpUi.getSampleRate()!= -1){
    cout << "call Newton Divided differences" << endl;
  }
}
