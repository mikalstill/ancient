
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
#include "cepwindowchebyshev.h"
#include "cepDataWindower.h"

#include "cepDate.h"
#include "cepDateUi.h"
#include "cepMatrixIO.h"

#include "cepLs.h"
#include "cepInterp.h"


#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <iomanip>

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
      if(frame->GetTitle() == "")
	frame->SetTitle(string(theDataset->getName() + " - " + theDataset->getProcHistory()).c_str());
      
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
    cepPlot plot(theDataset, dir, cfname, presWidth, presHeight, canvas->m_scale[(int) dir],
		 canvas->m_minval[(int) dir],
		 theDataset->getHaveLs(dir));
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
		  
		  while(!lsUi.getDoVCV() &&
			((fromDate != -2) && (toDate != -2) && (val != -2)))
		    {
		      if((fromDate != -1) && (toDate != -1))
			{
			  if(fromDate <= toDate)
			    {
			      if(!isnan(val))
				{
				  populateMatP(matP, toDate, fromDate, val, 
					       *theDataset->getMatrix((cepDataset::direction) i));
				}
			      else
				{
				  cepError("Weighting value must be a number", 
					   cepError::sevWarning).display();
				  return;
				}
			    }
			  else
			    {
			      cepError("Start date is after finish date", 
				       cepError::sevWarning).display();
			      return;
			    }
			}
		      
		      lsUi.showWeight((theDataset->getMatrix((cepDataset::direction) i))->
				      getValue(0,0),
				      (theDataset->getMatrix((cepDataset::direction) i))->
				      getValue((theDataset->getMatrix((cepDataset::direction) i))->
					       getNumRows() -1,0),
				      1.0);
		      
		      toDate = lsUi.getToDate();
		      fromDate = lsUi.getFromDate();
		      val = lsUi.getWeight();
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
  
      // Now we can process the results of the LS regression (this includes popping up a new tab)
      cepDataset newds(&residuals[0], &residuals[1], &residuals[2], 
		       theDataset->getOffset((cepDataset::direction) 0), 
		       theDataset->getOffset((cepDataset::direction) 1), 
		       theDataset->getOffset((cepDataset::direction) 2),
		       theDataset->getProcHistory() + " : Residuals", 
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
      
      // Actually force the graphs to redraw
      m_dirty = true;
      canvas->Refresh();
    }
  else
    {
      cepError("You cannot perform that operation on this dataset at this time", 
	       cepError::sevInformational).display();
    }
}

void cepView::populateMatP(cepMatrix<double> &matP, const double &toDate, const double &fromDate, const double &val, cepMatrix<double> &data)
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
          cout << "re-weigting thingie goes here" << endl;
        }
    }
  }
}

void cepView::OnWindowBlackman (wxCommandEvent& event)
{
  cepWindowUi windowUi;
  bool ok = false;

  do
  {
    windowUi.show();
    if( windowUi.cancelled() ) return;
    if( windowUi.checkValues().isReal() ) {
      windowUi.checkValues().display();
    } else {
      ok = true;
    }
  } while( !ok  );

  cepDataWindower::setWindowType( cepDataWindower::WINDOW_BLACKMAN, windowUi.getSize(), windowUi.getOverlap());
}

void cepView::OnWindowChebyshev (wxCommandEvent& event)
{
  cepWindowUi windowUi;
  bool ok = false;

  do
  {
    windowUi.showBandwidth();
    if( windowUi.cancelled() ) return;
    if( windowUi.checkValues().isReal() ) {
      windowUi.checkValues().display();
    } else {
      ok = true;
    }
  } while( !ok  );

  double bw = windowUi.getBandwidth();
  if( bw != cepWindowBandwidth::UNINITIALISED_FLOAT ){
    cepError err = cepWindowChebyshev::setTransitionBandwidth( bw );
    if( err.isReal() ) {
      err.display();
      return;
    }
  cepDataWindower::setWindowType( cepDataWindower::WINDOW_CHEBYSHEV, windowUi.getSize(), windowUi.getOverlap() );
  }
}

void cepView::OnWindowHamming (wxCommandEvent& event)
{
  cepWindowUi windowUi;
  bool ok = false;

  do
  {
    windowUi.show();
    if( windowUi.cancelled() ) return;
    if( windowUi.checkValues().isReal() ) {
      windowUi.checkValues().display();
    } else {
      ok = true;
    }
  } while( !ok  );

  cepDataWindower::setWindowType( cepDataWindower::WINDOW_HAMMING, windowUi.getSize(), windowUi.getOverlap() );

}

void cepView::OnWindowRect (wxCommandEvent& event)
{
  cepWindowUi windowUi;
  bool ok = false;
  do
  {
    windowUi.show();
    if( windowUi.cancelled() ) return;
    if( windowUi.checkValues().isReal() ) {
      windowUi.checkValues().display();
    } else {
      ok = true;
    }
  } while( !ok  );
  
  cepDataWindower::setWindowType( cepDataWindower::WINDOW_RECTANGULAR, windowUi.getSize(), windowUi.getOverlap() );

}

void cepView::OnInterpNearest (wxCommandEvent& event)
{
  cepInterpUi interpUi;

  cepDoc *theDoc = (cepDoc *) GetDocument ();
  cepDataset *theDataset = theDoc->getDataset ();
  const char *dirStrings[] = {"x (North)", "y (East)", "z (Up)"};

  for(int i = 0; i < cepDataset::dirUnknown; i++)
  {
    interpUi.showSampleRate(fabs(((theDataset->getMatrix((cepDataset::direction) i))->getValue(0,1) +
                            (theDataset->getMatrix((cepDataset::direction) i))->getValue(1,1))/2.0), dirStrings[i]);

    cout << "sample Rate returned is " << interpUi.getSampleRate() << endl;
    //if operation not canceled
    if(interpUi.getSampleRate()!= -1){

      //dataset = cepInterp.doInterp(current dataset, sample rate, interptype)
      cout << "call Nearst Neighbour" << endl;
    }
    else
    {
      //if user canceled in any direction bail out
      break;
    }
  }
}

void cepView::OnInterpLinear (wxCommandEvent& event)
{
  cepInterpUi interpUi;

  cepDoc *theDoc = (cepDoc *) GetDocument ();
  cepDataset *theDataset = theDoc->getDataset ();
  const char *dirStrings[] = {"x (North)", "y (East)", "z (Up)"};

  for(int i = 0; i < cepDataset::dirUnknown; i++)
  {
    interpUi.showSampleRate(fabs(((theDataset->getMatrix((cepDataset::direction) i))->getValue(0,1) +
                            (theDataset->getMatrix((cepDataset::direction) i))->getValue(1,1))/2.0), dirStrings[i]);

    cout << "sample Rate returned is " << interpUi.getSampleRate() << endl;
    //if operation not canceled
    if(interpUi.getSampleRate()!= -1){

      //dataset = cepInterp.doInterp(current dataset, sample rate, interptype)
      cout << "call Liner Interp" << endl;
    }
    else
    {
      //if user canceled in any direction bail out
      break;
    }
  }
}

void cepView::OnInterpNaturalSpline (wxCommandEvent& event)
{
  cepInterpUi interpUi;

  cepDoc *theDoc = (cepDoc *) GetDocument ();
  cepDataset *theDataset = theDoc->getDataset ();
  const char *dirStrings[] = {"x (North)", "y (East)", "z (Up)"};

  for(int i = 0; i < cepDataset::dirUnknown; i++)
  {
    interpUi.showSampleRate(fabs(((theDataset->getMatrix((cepDataset::direction) i))->getValue(0,1) +
                            (theDataset->getMatrix((cepDataset::direction) i))->getValue(1,1))/2.0), dirStrings[i]);

    cout << "sample Rate returned is " << interpUi.getSampleRate() << endl;
    //if operation not canceled
    if(interpUi.getSampleRate()!= -1){

      //dataset = cepInterp.doInterp(current dataset, sample rate, interptype)
      cout << "call Natural spline" << endl;
    }
    else
    {
      //if user canceled in any direction bail out
      break;
    }
  }
}

void cepView::OnInterpCubicSpline (wxCommandEvent& event)
{
  cepInterpUi interpUi;

  cepDoc *theDoc = (cepDoc *) GetDocument ();
  cepDataset *theDataset = theDoc->getDataset ();
  const char *dirStrings[] = {"x (North)", "y (East)", "z (Up)"};

  for(int i = 0; i < cepDataset::dirUnknown; i++)
  {
    interpUi.showSampleRate(fabs(((theDataset->getMatrix((cepDataset::direction) i))->getValue(0,1) +
                            (theDataset->getMatrix((cepDataset::direction) i))->getValue(1,1))/2.0), dirStrings[i]);

    cout << "sample Rate returned is " << interpUi.getSampleRate() << endl;
    //if operation not canceled
    if(interpUi.getSampleRate()!= -1){

      //dataset = cepInterp.doInterp(current dataset, sample rate, interptype)
      cout << "call Cubic Spline" << endl;
    }
    else
    {
      //if user canceled in any direction bail out
      break;
    }
  }
}

void cepView::OnInterpDivided (wxCommandEvent& event)
{
  cepInterpUi interpUi;

  cepDoc *theDoc = (cepDoc *) GetDocument ();
  cepDataset *theDataset = theDoc->getDataset ();
  const char *dirStrings[] = {"x (North)", "y (East)", "z (Up)"};

  for(int i = 0; i < cepDataset::dirUnknown; i++)
  {
    interpUi.showSampleRate(fabs(((theDataset->getMatrix((cepDataset::direction) i))->getValue(0,1) +
                            (theDataset->getMatrix((cepDataset::direction) i))->getValue(1,1))/2.0), dirStrings[i]);

    cout << "sample Rate returned is " << interpUi.getSampleRate() << endl;
    //if operation not canceled
    if(interpUi.getSampleRate()!= -1){

      //dataset = cepInterp.doInterp(current dataset, sample rate, interptype)
      cout << "call Divided diffrences" << endl;
    }
    else
    {
      //if user canceled in any direction bail out
      break;
    }
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
