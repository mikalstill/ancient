
/* 
 *   UI for the CEP program
 *   Copyright (C) Michael Still                    2002
 *   Copyright (C) Kristy Van Der Vlist             2002
 *   Copyright (C) Blake swadling                   2002
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

#include "cepDate.h"
#include "cepDateUi.h"
#include "cepMatrixIO.h"

#include "cepLs.h"
#include "cepInterp.h"

#include "cepCfft.h"

#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <iomanip>

#include <wx/colour.h>
#include <wx/colordlg.h>

IMPLEMENT_DYNAMIC_CLASS (cepView, wxView)
BEGIN_EVENT_TABLE (cepView, wxView)
  EVT_MENU (CEPMENU_ERRORS, cepView::OnToggleErrors)
  EVT_MENU (CEPMENU_GRID, cepView::OnToggleGrid)
  EVT_MENU (CEPMENU_COLORAXES, cepView::OnColorAxes)
  EVT_MENU (CEPMENU_COLORLINE, cepView::OnColorLine)
  EVT_MENU (CEPMENU_COLORERROR, cepView::OnColorError)
  EVT_MENU (CEPMENU_COLORGRID, cepView::OnColorGrid)
  EVT_MENU (CEPMENU_COLORFONT, cepView::OnColorFont)
  EVT_MENU (CEPMENU_SHOWX, cepView::OnToggleX)
  EVT_MENU (CEPMENU_SHOWY, cepView::OnToggleY)
  EVT_MENU (CEPMENU_SHOWZ, cepView::OnToggleZ)
  EVT_MENU (CEPMENU_LS_VCV, cepView::OnLeastSquaresVCV)
  EVT_MENU (CEPMENU_LS_RW, cepView::OnLeastSquaresRW)
  EVT_MENU (CEPMENU_WINDOW_BLACKMAN, cepView::OnWindowBlackman)
  EVT_MENU (CEPMENU_WINDOW_CHEBYSHEV, cepView::OnWindowChebyshev)
  EVT_MENU (CEPMENU_WINDOW_HAMMING, cepView::OnWindowHamming)
  EVT_MENU (CEPMENU_WINDOW_HANNING, cepView::OnWindowHanning)
  EVT_MENU (CEPMENU_WINDOW_RECT, cepView::OnWindowRect)
  EVT_MENU (CEPMENU_INTERP_NEAREST, cepView::OnInterpNearest)
  EVT_MENU (CEPMENU_INTERP_LINEAR, cepView::OnInterpLinear)
  EVT_MENU (CEPMENU_INTERP_NATURALSPLINE, cepView::OnInterpNaturalSpline)
  EVT_MENU (CEPMENU_INTERP_CUBICSPLINE, cepView::OnInterpCubicSpline)
  EVT_MENU (CEPMENU_INTERP_DIVIDED, cepView::OnInterpDivided)
  EVT_MENU (CEPMENU_FFT, cepView::OnFFT)
  EVT_MENU (CEPMENU_SELECTFONT, cepView::OnSelectFont)
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

  frame->GetSize (&m_x, &m_y);

#ifdef __X__
  // X seems to require a forced resize
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
      if(frame->GetTitle() == ""){
	frame->SetTitle(string(theDataset->getName() + " - " + 
			       theDataset->getProcHistory()).c_str());
      }
      
      // If the size of the window has changed, then declare that we need to regenerate...
      int x, y;
      frame->GetSize(&x, &y);
      if((x != m_x) || (y != m_y)){
	cepDebugPrint("Detected window resize event");
	frame->GetSize(&m_x, &m_y);
	cepDebugPrint("Resized to: " + cepToString(m_x) + " x " + cepToString(m_y));
	m_dirty = true;
      }

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

	cepDebugPrint("Graph placement is: " + cepToString(top) + ", " + cepToString(bottom) +
		      ", " + cepToString(width));

	// A width of -1 means invisible
	if(width != -1){
	  drawPresentation(theDataset, (cepDataset::direction) dir, top, dc, 
			   width, bottom - top);
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
  m_dirty = true;
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

    cepDebugPrint("Requesting plot: " + cepToString(presWidth) + " x " + cepToString(presHeight));
    cepPlot plot(theDataset, dir, cfname, presWidth, presHeight, 
		 canvas->m_vertScale[(int) dir], canvas->m_horizScale[(int) dir],
		 canvas->m_xminval[(int) dir], canvas->m_yminval[(int) dir],
		 canvas->m_yrange[(int) dir], theDataset->getHaveLs(dir),
		 theDataset->isFreqDomain(), theDataset->getEnergy(dir));
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

void cepView::OnToggleGrid (wxCommandEvent &pevt)
{
  m_config->setValue("ui-viewmenu-showgrid", pevt.IsChecked());
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

void
cepView::OnColorGrid (wxCommandEvent & WXUNUSED (event))
{
  wxColourDialog picker(NULL);
  if(picker.ShowModal() == wxID_OK){
    wxColourData data = picker.GetColourData();
    wxColour color = data.GetColour();
    m_config->setValue("ui-graph-color-grid-r", color.Red());
    m_config->setValue("ui-graph-color-grid-g", color.Green());
    m_config->setValue("ui-graph-color-grid-b", color.Blue());

    m_dirty = true;
    canvas->Refresh();
  }
}

void
cepView::OnColorFont (wxCommandEvent & WXUNUSED (event))
{
  wxColourDialog picker(NULL);
  if(picker.ShowModal() == wxID_OK){
    wxColourData data = picker.GetColourData();
    wxColour color = data.GetColour();
    m_config->setValue("ui-graph-color-font-r", color.Red());
    m_config->setValue("ui-graph-color-font-g", color.Green());
    m_config->setValue("ui-graph-color-font-b", color.Blue());

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
  cepMatrix <double> residuals[cepDataset::dirUnknown], data[cepDataset::dirUnknown];
  double b1s[cepDataset::dirUnknown], b2s[cepDataset::dirUnknown];

  cepDoc *theDoc = (cepDoc *) GetDocument ();
  cepDataset *theDataset = theDoc->getDataset ();
  if (theDataset && theDataset->isReady() && theDataset->isWellFormed())
    {
      // Prompt for processing options
      cepLsUi lsUi;
      lsUi.showIsReweight();
      
      // User cancelled
      if(lsUi.getIsReweight() == -1)
	return;

      // todo_mikal: remove? lsUi.showWhichDir();
      
      // For each direction
      for(int i = 0; i < cepDataset::dirUnknown; i++)
	{
	  cepDebugPrint("Reweighting in the " + cepToString(i) + " direction");
	  
	  // All directions are currently processed. The lsUi.getWhichDir (dirNames[i]) call can be used
	  // to make this optional later (where dirNames are: 'x', 'y', 'z')
	  cepLs thisLs;
	  
	  // Reweight if required
	  if(lsUi.getIsReweight () == -1)
	    return;

	  if(lsUi.getIsReweight () == 1)
	    {
	      thisLs.cepDoVCV (*theDataset->getMatrix ((cepDataset::direction) i));
	      if(thisLs.getError().isReal() == true)
		{
		  thisLs.getError().display();
		  return;
		}  
	      
	      residuals[i] = thisLs.getResidual ();
	      data[i] = thisLs.getDataset();
	      b1s[i] = thisLs.getB1();
	      b2s[i] = thisLs.getB2();
	    }
	  
	  // Otherwise, don't reweight
	  else
	    {
	      const char *dirStrings[] = {"x (North)", "y (East)", "z (Up"};
	      lsUi.showIsReadP (dirStrings[i]);
	      if(lsUi.getIsReadP () == -1)
		return;

	      if (lsUi.getIsReadP () == 1)
		{
		  lsUi.showGetfNameP ();
		  if (lsUi.getfNameP () != "")
		    {
		      cepMatrix<double> matP;
		      
		      matP = cepReadMatrix (lsUi.getfNameP ());
		      thisLs.cepDoVCV (*theDataset->getMatrix ((cepDataset::direction) i), matP);
		      if(thisLs.getError().isReal() == true)
			{
			  thisLs.getError().display();
			  return;
			}
		      residuals[i] = thisLs.getResidual ();
		      data[i] = thisLs.getDataset();
		      b1s[i] = thisLs.getB1();
		      b2s[i] = thisLs.getB2();
		    }
		}
	      else if (lsUi.getIsReadP () == 0)
		{
		  cepMatrix<double> matP((theDataset->getMatrix((cepDataset::direction) i))->
					 getNumRows(),
					 (theDataset->getMatrix((cepDataset::direction) i))->
					 getNumRows());
		  cepLs thisLs;
		  
		  // Init P matrix to 1 on diagonal
		  for(int j = 0; j < matP.getNumRows(); j ++)
		    {
		      for(int k = 0; k < matP.getNumCols(); k ++)
			{
			  if(j == k)
			    {
			      matP.setValue(j,k,1);
			    }
			  else
			    {
			      matP.setValue(j,k,0);
			    }
			}
		    }
		  
		  lsUi.showWeight((theDataset->getMatrix((cepDataset::direction) i))->
				  getValue(0,0),
				  (theDataset->getMatrix((cepDataset::direction) i))->
				  getValue((theDataset->getMatrix((cepDataset::direction) i))->
					   getNumRows() -1,0),
				  1.0);
		  
		  double toDate = lsUi.getToDate(),
		         fromDate = lsUi.getFromDate(),
		         val = lsUi.getWeight();
		  bool isDoVCV = lsUi.getDoVCV();
      while(isDoVCV == false)
      {
        if((toDate != -1.0) || (fromDate != -1.0) || (val != -1.0))
        {
          populateMatP(matP, toDate, fromDate, val, *theDataset->getMatrix((cepDataset::direction) i));
        }
        else
        {
          //if user hits cancel
          return;
        }

        lsUi.showWeight((theDataset->getMatrix((cepDataset::direction) i))->getValue(0,0),
                        (theDataset->getMatrix((cepDataset::direction) i))->getValue((theDataset->getMatrix((cepDataset::direction) i))->getNumRows() -1,0),
                         1.0);

        toDate = lsUi.getToDate();
        fromDate = lsUi.getFromDate();
        val = lsUi.getWeight();
        isDoVCV = lsUi.getDoVCV();
      }
 
	    if(lsUi.getDoVCV())
	    {
	      thisLs.cepDoVCV(*theDataset->getMatrix((cepDataset::direction) i), matP);
	      residuals[i] = thisLs.getResidual ();
	      data[i] = thisLs.getDataset();
	      b1s[i] = thisLs.getB1();
	      b2s[i] = thisLs.getB2();
	    }
		}
	    }
	}
        
      //////////////////////////////////////////////////////////////////////////////////////////
      // Now we can process the results of the LS regression (this includes popping up a new tab)
      
      // Actual data
      {
	cepDebugPrint("Creating LS data dataset");
	cepDataset newds(theDataset->getMatrix((cepDataset::direction) 0),
			 theDataset->getMatrix((cepDataset::direction) 1),
			 theDataset->getMatrix((cepDataset::direction) 2),
			 theDataset->getOffset((cepDataset::direction) 0), 
			 theDataset->getOffset((cepDataset::direction) 1), 
			 theDataset->getOffset((cepDataset::direction) 2),
			 theDataset->getProcHistory() + " : LS VCV", 
			 theDataset->getHeader((cepDataset::direction) 0), 
			 theDataset->getHeader((cepDataset::direction) 1), 
			 theDataset->getHeader((cepDataset::direction) 2),
			 b1s[0], b1s[1], b1s[2], b2s[0], b2s[1], b2s[2],
			 true, true, true);
	
	char *cfname = strdup("/tmp/cep.XXXXXX");
	int fd;
	fd = mkstemp(cfname);
	close(fd);
	
	string newcfname(string(cfname) + "~" + theDataset->getName());
	newds.write(newcfname.c_str());
	
	wxGetApp().m_docManager->CreateDocument(string(newcfname + ".dat1").c_str(), wxDOC_SILENT);
	free(cfname);
      }
      
      // Residuals
      {
	cepDebugPrint("Creating residuals dataset");
	cepDataset newds(&residuals[0], &residuals[1], &residuals[2], 
			 theDataset->getOffset((cepDataset::direction) 0), 
			 theDataset->getOffset((cepDataset::direction) 1), 
			 theDataset->getOffset((cepDataset::direction) 2),
			 theDataset->getProcHistory() + " : LS VCV Residuals", 
			 theDataset->getHeader((cepDataset::direction) 0), 
			 theDataset->getHeader((cepDataset::direction) 1), 
			 theDataset->getHeader((cepDataset::direction) 2));
	
	char *cfname = strdup("/tmp/cep.XXXXXX");
	int fd;
	fd = mkstemp(cfname);
	close(fd);
	
	string newcfname(string(cfname) + "~" + theDataset->getName());
	newds.write(newcfname.c_str());
	
	wxGetApp().m_docManager->CreateDocument(string(newcfname + ".dat1").c_str(), wxDOC_SILENT);
	free(cfname);
      }
      
      // Actually force the graphs to redraw
      canvas->Refresh();
    }
  else
    {
      cepError("You cannot perform that operation on this dataset at this time", 
	       cepError::sevInformational).display();
    }
}

void cepView::populateMatP(cepMatrix<double> &matP, const double &toDate, const double &fromDate, 
			   const double &val, cepMatrix<double> &data)
{
  cout << "#######IN POPULATE MATRIX" << endl;
  cout << "fromDate " << setprecision(10)<< fromDate << endl;
  cout << "toDate " << setprecision(10) << toDate << endl;
  cout << "vaule " << setprecision(10) << val << endl;

  for(int i = 0; i < matP.getNumRows(); i++)
  {
    cout << endl << "Date is: " << setprecision(10) << data.getValue(i, 0) << endl;
    if((data.getValue(i, 0) >= fromDate) && (data.getValue(i,0) <= toDate))
    {
      cout << "value at " << i << " is" << endl;
      matP.setValue(i,i, val);
    }
  }
  
}
// Perform a least squares regression on the dataset (in all directions)
void cepView::OnLeastSquaresRW (wxCommandEvent &pevt)
{
  cepLsUi lsUi;
  cepLs thisLs;
  const char *dirStrings[] = {"x (North)", "y (East)", "z (Up)"};

  cepDoc *theDoc = (cepDoc *) GetDocument ();
  cepDataset *theDataset = theDoc->getDataset ();
  if (theDataset && theDataset->isReady() && theDataset->isWellFormed())
  {
    lsUi.showWhichDir();

    // For each direction
    for(int i = 0; i < cepDataset::dirUnknown; i++)
    {
      lsUi.showIsReadP (dirStrings[i]);
      
      if (lsUi.getIsReadP () == 1)
        {
          lsUi.showGetfNameP ();
          if (lsUi.getfNameP () != "")
	    {
	      cepMatrix<double> matP;
	      
	      matP = cepReadMatrix (lsUi.getfNameP ());
	      thisLs.cepDoRW (*theDataset->getMatrix ((cepDataset::direction) i), matP);
	      //getdata matrix and residual matrix here!!!!!
	      if(thisLs.getError().isReal() == true)
		{
		  thisLs.getError().display();
		  return;
		}
	    }
        }
      else
        {
          wxMessageBox("The random walk functionality has not been implemented at this time.",
		       "Sorry", wxOK);
        }
    }
  }
}

void cepView::OnWindowBlackman (wxCommandEvent& event)
{
  processWindow(cepDataWindower::WINDOW_BLACKMAN, "Blackman Window");
}

void cepView::OnWindowChebyshev (wxCommandEvent& event)
{
  processWindow(cepDataWindower::WINDOW_CHEBYSHEV, "Chebyshev Window");
}

void cepView::OnWindowHamming (wxCommandEvent& event)
{
  processWindow(cepDataWindower::WINDOW_HAMMING, "Hamming Window");

}

void cepView::OnWindowHanning (wxCommandEvent& event)
{
  processWindow(cepDataWindower::WINDOW_HANNING, "Hanning Window");

}

void cepView::OnWindowRect (wxCommandEvent& event)
{
  processWindow(cepDataWindower::WINDOW_RECTANGULAR, "Rect Window");
}

void
cepView::processWindow(const cepWindow wType, string desc)
{
  cepMatrix<double> windowed[cepDataset::dirUnknown];

  cepDoc *theDoc = (cepDoc *) GetDocument ();
  cepDataset *theDataset = theDoc->getDataset ();
  if (theDataset && theDataset->isReady() && theDataset->isWellFormed()){
    cepWindowUi windowUi;

    cepDebugPrint("Prompt for Window information");
    bool ok = false;
    do
    {
        cepError err;
        if(wType == cepDataWindower::WINDOW_CHEBYSHEV) {        
            err = windowUi.showChebyshev();
        } else {
            err = windowUi.show();
        }
          
        if( windowUi.cancelled() ){
	  canvas->Refresh();
	  return;
	}

        if( err.isReal() ) {
            err.display();
        } else {
            ok = true;
        }
    } while( !ok  );
    
    if(wType == cepDataWindower::WINDOW_CHEBYSHEV){
        double bw = windowUi.getBandwidth();
        // this should never occur as this is only set if we abort
        if( bw != cepWindowBandwidth::UNINITIALISED_FLOAT ){
            cepError err = cepWindowChebyshev::setTransitionBandwidth( bw );
            if( err.isReal() ) {
                err.display();
		canvas->Refresh();
                return;
            }
        }
    }
    
    cepDebugPrint("Set window type");
    cepDataWindower::setWindowType(wType , windowUi.getSize(), windowUi.getOverlap() );
    
    // Now actually window
    for(int i = 0; i < cepDataset::dirUnknown; i++)
    {
      cepDebugPrint("Window: " + cepToString(i));
      cepError werr = cepDataWindower::window(*theDoc->getDataset()->
					      getMatrix((cepDataset::direction) i),
					      windowed[i]);
      if(werr.isReal()){
        werr.display();
	canvas->Refresh();
        return;
      }
    }
    
    // Now we can process the results
    cepDebugPrint("Display results");
    cepDataset newds(&windowed[0], &windowed[1], &windowed[2], 
		     theDataset->getOffset((cepDataset::direction) 0), 
		     theDataset->getOffset((cepDataset::direction) 1), 
		     theDataset->getOffset((cepDataset::direction) 2),
		     theDataset->getProcHistory() + " : " + desc, 
		     theDataset->getHeader((cepDataset::direction) 0), 
		     theDataset->getHeader((cepDataset::direction) 1), 
		     theDataset->getHeader((cepDataset::direction) 2));
    
    char *cfname = strdup("/tmp/cep.XXXXXX");
    int fd;
    fd = mkstemp(cfname);
    close(fd);
    
    string newcfname(string(cfname) + "~" + theDataset->getName());
    newds.write(newcfname.c_str());
    
    wxGetApp().m_docManager->CreateDocument(string(newcfname + ".dat1").c_str(), wxDOC_SILENT);
    free(cfname);
    
    // Actually force the graphs to redraw
    canvas->Refresh();
  }
}

void cepView::OnInterpNearest (wxCommandEvent& event)
{
  processInterp(NEAREST_INTERP, "NN Interp");
}

void cepView::OnInterpLinear (wxCommandEvent& event)
{
  processInterp(LINEAR_INTERP, "Lin Interp");
}

void cepView::OnInterpNaturalSpline (wxCommandEvent& event)
{
  processInterp(NATURAL_SPLINE_INTERP, "NS Interp");
}

void cepView::OnInterpCubicSpline (wxCommandEvent& event)
{
  processInterp(CUBIC_SPLINE_INTERP, "CS Interp");
}

void cepView::OnInterpDivided (wxCommandEvent& event)
{
  processInterp(DIVIDED_INTERP, "Div Interp");
}

void
cepView::processInterp(const int iType, string desc)
{
  cepInterpUi interpUi;
  cepMatrix<double> interped[cepDataset::dirUnknown];

  cepDoc *theDoc = (cepDoc *) GetDocument ();
  cepDataset *theDataset = theDoc->getDataset ();
  if (theDataset && theDataset->isReady() && theDataset->isWellFormed()){
    interpUi.showSampleRate(1);
    cepDebugPrint(string("Interp sample rate: ") + cepToString(interpUi.getSampleRate()));
    if(interpUi.getSampleRate() == -1){
      return;
    }
    
    for(int i = 0; i < cepDataset::dirUnknown; i++)
      {
	cepInterp myInterp;
	interped[i] = myInterp.doInterp(*theDoc->getDataset ()->getMatrix((cepDataset::direction) i), 
					interpUi.getSampleRate(), 
					iType);
      }
    
    // Now we can process the results
    cepDataset newds(&interped[0], &interped[1], &interped[2], 
		   theDataset->getOffset((cepDataset::direction) 0), 
		     theDataset->getOffset((cepDataset::direction) 1), 
		     theDataset->getOffset((cepDataset::direction) 2),
		     theDataset->getProcHistory() + " : " + desc, 
		     theDataset->getHeader((cepDataset::direction) 0), 
		     theDataset->getHeader((cepDataset::direction) 1), 
		     theDataset->getHeader((cepDataset::direction) 2));
    
    char *cfname = strdup("/tmp/cep.XXXXXX");
    int fd;
    fd = mkstemp(cfname);
    close(fd);
    
    string newcfname(string(cfname) + "~" + theDataset->getName());
    newds.write(newcfname.c_str());
    
    wxGetApp().m_docManager->CreateDocument(string(newcfname + ".dat1").c_str(), wxDOC_SILENT);
    free(cfname);
    
    // Actually force the graphs to redraw
    canvas->Refresh();
  }
}

void cepView::OnFFT (wxCommandEvent& event)
{
  cepMatrix<double> ffted[cepDataset::dirUnknown];
  float energies[cepDataset::dirUnknown];
    
  cepDoc *theDoc = (cepDoc *) GetDocument ();
  cepDataset *theDataset = theDoc->getDataset ();
  if (theDataset && theDataset->isReady() && theDataset->isWellFormed()){
    for(int i = 0; i < cepDataset::dirUnknown; i++)
      {
	cepDebugPrint("Performing FFT in " + cepToString(i) + " direction");

	// We need to copy add the data across into complex land...
	cepMatrix<ComplexDble> input(theDataset->getMatrix((cepDataset::direction) i)->getNumRows(),
				     theDataset->getMatrix((cepDataset::direction) i)->getNumCols());
	
	// I assume that an FFT is only on a 2d matrix
	for(int row = 0; 
	    row < theDataset->getMatrix((cepDataset::direction) i)->getNumRows(); row++){
	  for(int col = 0; 
	      col < theDataset->getMatrix((cepDataset::direction) i)->getNumCols(); col++){
	    input.setValue(row, col, theDataset->getMatrix((cepDataset::direction) i)->
	      getValue(row, col));

	    if(theDataset->getMatrix((cepDataset::direction) i)->getError().isReal()){
	      cepDebugPrint("FFT data conversion, extract from dataset");
	      theDataset->getMatrix((cepDataset::direction) i)->getError().display();
	      canvas->Refresh();
	      return;
	    }

	    if(input.getError().isReal()){
	      cepDebugPrint("FFT data conversion, push to input");
	      input.getError().display();
	      canvas->Refresh();
	      return;
	    }
	  }
	}
	
	// Determine how many items we are going to perform an FFT on
	// It has to be a power of two
	cepDebugPrint("Dataset contains " + 
		      cepToString(theDataset->getMatrix((cepDataset::direction) i)->getNumRows()) +
		      " elements");
	int fftScale = theDataset->getMatrix((cepDataset::direction) i)->getNumRows();
	for (int k = 0;; ++k)
	  {
	    // The size is already a power of two
	    if ((1 << k) == fftScale)
	      break;

	    // It is not a power of two
	    if (k == 14 || (1 << k) > fftScale){
	      // Always go low for now...
	      fftScale = 1 << (k - 1);
	      break;
	    }
	  }
	cepDebugPrint("FFT applied to " + cepToString(fftScale) + " elements");

	cepCfft<ComplexDble> myFFT(fftScale);
	if(myFFT.getError().isReal()){
	  cepDebugPrint("Error from FFT initialization function");
	  myFFT.getError().display();
	  canvas->Refresh();
	  return;
	}

	cepMatrix<ComplexDble> output = myFFT.matrixFft(input, 1);
	if(myFFT.getError().isReal()){
	  cepDebugPrint("Error from FFT calculation function");
	  myFFT.getError().display();
	  canvas->Refresh();
	  return;
	}

	// Make a new matrix to put this into
	ffted[i] = cepMatrix<double> (output.getNumRows() - 1, output.getNumCols());
	if(output.getError().isReal()){
	  cepDebugPrint("Error determining size of FFT output");
	  output.getError().display();
	  canvas->Refresh();
	  return;
	}

	// Now we need to get the data back to what we want
	cepDebugPrint("The output matrix is " + cepToString(output.getNumRows()) + " x " +
		      cepToString(output.getNumCols()));
	for(int row = 0; row < output.getNumRows(); row++){
	  for(int col = 0; col < output.getNumCols(); col++){
	    cepDebugPrint("Getting output: row = " + cepToString(row) + " col = " +
			  cepToString(col));

	    // The first row of the FFT output is treated separately...
	    if(row == 0){
	      if(col == 1){
		energies[i] = real(output.getValue(row, col, 0));
	      }
	    }
	    else{
	      // Again, I have assumed that there is only one table
	      ffted[i].setValue(row - 1, col, real(output.getValue(row, col, 0)));
	      if(output.getError().isReal()){
		cepDebugPrint("FFT data conversion, get output");
		output.getError().display();
		canvas->Refresh();
		return;
	      }
	      
	      if(ffted[i].getError().isReal()){
		cepDebugPrint("FFT data conversion, push to new dataset");
		ffted[i].getError().display();
		canvas->Refresh();
		return;
	      }
	    }
	  }
	}
      }
    
    // Now we can process the results
    cepDataset newds(&ffted[0], &ffted[1], &ffted[2], 
		   theDataset->getOffset((cepDataset::direction) 0), 
		     theDataset->getOffset((cepDataset::direction) 1), 
		     theDataset->getOffset((cepDataset::direction) 2),
		     theDataset->getProcHistory() + " : FFT", 
		     theDataset->getHeader((cepDataset::direction) 0), 
		     theDataset->getHeader((cepDataset::direction) 1), 
		     theDataset->getHeader((cepDataset::direction) 2));
    newds.setFreqDomain(true);
    newds.setFreqEnergies(energies[0], energies[1], energies[2]);
    
    char *cfname = strdup("/tmp/cep.XXXXXX");
    int fd;
    fd = mkstemp(cfname);
    close(fd);
    
    string newcfname(string(cfname) + "~" + theDataset->getName());
    newds.write(newcfname.c_str());
    
    wxGetApp().m_docManager->CreateDocument(string(newcfname + ".dat1").c_str(), wxDOC_SILENT);
    free(cfname);
    
    // Actually force the graphs to redraw
    canvas->Refresh();
  }
}

void cepView::OnSelectFont (wxCommandEvent& event)
{
  wxFileDialog dlg(NULL, "Select a display font", "", "", 
		   "TrueType fonts (*.ttf)|*.ttf|Postscript fonts (*.pfb)|*.pfb", 
		   wxOPEN, wxPoint(-1, -1));
  dlg.Centre();

  if(dlg.ShowModal() == wxID_OK){
      cepError err = m_config->setValue("ui-graph-font", string(dlg.GetPath().c_str()));
      if(err.isReal())
	err.display();

      m_dirty = true;
      canvas->Refresh();
      cepDebugPrint(string("Set graphing font to: ") + dlg.GetPath().c_str());
  }
}
