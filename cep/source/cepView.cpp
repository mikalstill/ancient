
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

#include "cepApplicator.h"

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
  EVT_MENU (CEPMENU_COLORLINE1, cepView::OnColorLine1)
  EVT_MENU (CEPMENU_COLORLINE2, cepView::OnColorLine2)
  EVT_MENU (CEPMENU_COLORLINE3, cepView::OnColorLine3)
  EVT_MENU (CEPMENU_COLORREMOVE, cepView::OnColorRemove)
  EVT_MENU (CEPMENU_COLORERROR, cepView::OnColorError)
  EVT_MENU (CEPMENU_COLORLS, cepView::OnColorLs)
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
  EVT_MENU (CEPMENU_INTERP_LS, cepView::OnInterpLs)
  EVT_MENU (CEPMENU_FFT, cepView::OnFFT)
  EVT_MENU (CEPMENU_WINDOWNEXT, cepView::OnNextWindow)
  EVT_MENU (CEPMENU_WINDOWPREV, cepView::OnPrevWindow)
  EVT_MENU (CEPMENU_SELECTFONT, cepView::OnSelectFont)
  EVT_MENU (CEPMENU_FONTSIZE6, cepView::OnSelectFontSize6)
  EVT_MENU (CEPMENU_FONTSIZE8, cepView::OnSelectFontSize8)
  EVT_MENU (CEPMENU_FONTSIZE9, cepView::OnSelectFontSize9)
  EVT_MENU (CEPMENU_FONTSIZE10, cepView::OnSelectFontSize10)
  EVT_MENU (CEPMENU_FONTSIZE11, cepView::OnSelectFontSize11)
  EVT_MENU (CEPMENU_FONTSIZE12, cepView::OnSelectFontSize12)
  EVT_MENU (CEPMENU_FONTSIZE14, cepView::OnSelectFontSize14)
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

  m_currentWindow = -1;
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
		 theDataset->isFreqDomain(), theDataset->getEnergy(dir),
		 m_currentWindow);
    m_plotfailed = plot.getFailed();
    canvas->m_isFreq[(int) dir] = theDataset->isFreqDomain();
    
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
cepView::OnColorLine1 (wxCommandEvent & WXUNUSED (event))
{
  wxColourDialog picker(NULL);
  if(picker.ShowModal() == wxID_OK){
    wxColourData data = picker.GetColourData();
    wxColour color = data.GetColour();
    m_config->setValue("ui-graph-color-line1-r", color.Red());
    m_config->setValue("ui-graph-color-line1-g", color.Green());
    m_config->setValue("ui-graph-color-line1-b", color.Blue());

    m_dirty = true;
    canvas->Refresh();
  }
}

void
cepView::OnColorLine2 (wxCommandEvent & WXUNUSED (event))
{
  wxColourDialog picker(NULL);
  if(picker.ShowModal() == wxID_OK){
    wxColourData data = picker.GetColourData();
    wxColour color = data.GetColour();
    m_config->setValue("ui-graph-color-line2-r", color.Red());
    m_config->setValue("ui-graph-color-line2-g", color.Green());
    m_config->setValue("ui-graph-color-line2-b", color.Blue());

    m_dirty = true;
    canvas->Refresh();
  }
}

void
cepView::OnColorLine3 (wxCommandEvent & WXUNUSED (event))
{
  wxColourDialog picker(NULL);
  if(picker.ShowModal() == wxID_OK){
    wxColourData data = picker.GetColourData();
    wxColour color = data.GetColour();
    m_config->setValue("ui-graph-color-line3-r", color.Red());
    m_config->setValue("ui-graph-color-line3-g", color.Green());
    m_config->setValue("ui-graph-color-line3-b", color.Blue());

    m_dirty = true;
    canvas->Refresh();
  }
}

void
cepView::OnColorRemove (wxCommandEvent & WXUNUSED (event))
{
  wxColourDialog picker(NULL);
  if(picker.ShowModal() == wxID_OK){
    wxColourData data = picker.GetColourData();
    wxColour color = data.GetColour();
    m_config->setValue("ui-graph-color-remove-r", color.Red());
    m_config->setValue("ui-graph-color-remove-g", color.Green());
    m_config->setValue("ui-graph-color-remove-b", color.Blue());

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
cepView::OnColorLs (wxCommandEvent & WXUNUSED (event))
{
  wxColourDialog picker(NULL);
  if(picker.ShowModal() == wxID_OK){
    wxColourData data = picker.GetColourData();
    wxColour color = data.GetColour();
    m_config->setValue("ui-graph-color-ls-r", color.Red());
    m_config->setValue("ui-graph-color-ls-g", color.Green());
    m_config->setValue("ui-graph-color-ls-b", color.Blue());

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
  cepDoc *theDoc = (cepDoc *) GetDocument ();
  cepDataset *theDataset = theDoc->getDataset ();
  if (theDataset && theDataset->isReady() && theDataset->isWellFormed()){
    // Prompt for processing options
    cepLsUi lsUi;
    lsUi.showIsReweight();
    
    // User cancelled
    if(lsUi.getIsReweight() == -1){
      canvas->Refresh();
      return;
    }
    
    // Reweight if required
    if(lsUi.getIsReweight () == -1){
      canvas->Refresh();
      return;
    }
    
    cepDataset normal;
    cepDataset residual;
    //    processLsVCV(theDataset, lsUi.getIsReweight(), normal, residual);

    return;

    {
      char *cfname = strdup("/tmp/cep.XXXXXX");
      int fd;
      fd = mkstemp(cfname);
      close(fd);
      
      string newcfname(string(cfname) + "~" + theDataset->getName());
      normal.write(newcfname.c_str());
      
      wxGetApp().m_docManager->CreateDocument(string(newcfname + ".dat1").c_str(), wxDOC_SILENT);
      free(cfname);
    }

    {
      char *cfname = strdup("/tmp/cep.XXXXXX");
      int fd;
      fd = mkstemp(cfname);
      close(fd);
      
      string newcfname(string(cfname) + "~" + theDataset->getName());
      residual.write(newcfname.c_str());
      
      wxGetApp().m_docManager->CreateDocument(string(newcfname + ".dat1").c_str(), wxDOC_SILENT);
      free(cfname);
    }
  }
  canvas->Refresh();
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
  cepDoc *theDoc = (cepDoc *) GetDocument ();
  cepDataset *theDataset = theDoc->getDataset ();
  if (theDataset && theDataset->isReady() && theDataset->isWellFormed()){
    processLsRW(theDataset);
  }
  canvas->Refresh();
}

void cepView::OnWindowBlackman (wxCommandEvent& event)
{
  uiProcessWindow(cepDataWindower::WINDOW_BLACKMAN, "Blackman Window");
}

void cepView::OnWindowChebyshev (wxCommandEvent& event)
{
  uiProcessWindow(cepDataWindower::WINDOW_CHEBYSHEV, "Chebyshev Window");
}

void cepView::OnWindowHamming (wxCommandEvent& event)
{
  uiProcessWindow(cepDataWindower::WINDOW_HAMMING, "Hamming Window");

}

void cepView::OnWindowHanning (wxCommandEvent& event)
{
  uiProcessWindow(cepDataWindower::WINDOW_HANNING, "Hanning Window");

}

void cepView::OnWindowRect (wxCommandEvent& event)
{
  uiProcessWindow(cepDataWindower::WINDOW_RECTANGULAR, "Rect Window");
}

void cepView::uiProcessInterp(const int iType, string desc){
  cepInterpUi interpUi;
  
  cepDoc *theDoc = (cepDoc *) GetDocument ();
  cepDataset *theDataset = theDoc->getDataset ();
  if (theDataset && theDataset->isReady() && theDataset->isWellFormed()){
    interpUi.showSampleRate(1);
    cepDebugPrint(string("Interp sample rate: ") + cepToString(interpUi.getSampleRate()));
    if(interpUi.getSampleRate() == -1){
      canvas->Refresh();
      return;
    }

    char *cfname = strdup("/tmp/cep.XXXXXX");
    int fd;
    fd = mkstemp(cfname);
    close(fd);
    
    string newcfname(string(cfname) + "~" + theDataset->getName());
    cepError err = processInterp(theDataset, iType, desc, interpUi.getSampleRate(), newcfname);
    if(err.isReal()){
      err.display();
      canvas->Refresh();
      return;
    }

    wxGetApp().m_docManager->CreateDocument(string(newcfname + ".dat1").c_str(), wxDOC_SILENT);
    free(cfname);
    
    canvas->Refresh();
  }
}

void cepView::uiProcessWindow(const cepWindow wType, string desc){
  cepDoc *theDoc = (cepDoc *) GetDocument ();
  cepDataset *theDataset = theDoc->getDataset ();
  if (theDataset && theDataset->isReady() && theDataset->isWellFormed()){
    cepWindowUi windowUi;

    bool ok = false;
    do
    {
        cepDebugPrint("Prompt for Window information");
        cepError err;
        if(wType == cepDataWindower::WINDOW_CHEBYSHEV) {        
            err = windowUi.showChebyshev();
        } else {
            err = windowUi.show();
        }
       
	cepDebugPrint("Checking if operation cancelled");
        if( windowUi.cancelled() ){
	  cepDebugPrint("Operation cancelled");
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

    char *cfname = strdup("/tmp/cep.XXXXXX");
    int fd;
    fd = mkstemp(cfname);
    close(fd);
    
    string newcfname(string(cfname) + "~" + theDataset->getName());
    cepError err = processWindow(theDataset, wType, desc, windowUi.getSize(), windowUi.getOverlap(),
				 newcfname);
    if(err.isReal()){
      err.display();
      canvas->Refresh();
      return;
    }

    wxGetApp().m_docManager->CreateDocument(string(newcfname + ".dat1").c_str(), wxDOC_SILENT);
    free(cfname);
    
    // Actually force the graphs to redraw
    canvas->Refresh();
  }
}

void cepView::OnInterpNearest (wxCommandEvent& event)
{    
  uiProcessInterp(NEAREST_INTERP, "NN Interp");
}

void cepView::OnInterpLinear (wxCommandEvent& event)
{
  uiProcessInterp(LINEAR_INTERP, "Lin Interp");
}

void cepView::OnInterpLs (wxCommandEvent& event)
{
  cepDoc *theDoc = (cepDoc *) GetDocument ();
  cepDataset *theDataset = theDoc->getDataset ();
  if (theDataset && theDataset->isReady() && theDataset->isWellFormed()){
    cepInterpUi interpUi;
    
    if(!theDataset->getHaveLs(cepDataset::dirX)){
      cepError err("Please perform a least squares regression first", cepError::sevInformational);
      err.display();
      canvas->Refresh();
      return;
    }
    
    interpUi.showSampleRate(1);
    cepDebugPrint(string("Interp sample rate: ") + cepToString(interpUi.getSampleRate()));
    if(interpUi.getSampleRate() == -1){
      canvas->Refresh();
      return;
    }
    
    char *cfname = strdup("/tmp/cep.XXXXXX");
    int fd;
    fd = mkstemp(cfname);
    close(fd);
    
    string newcfname(string(cfname) + "~" + theDataset->getName());
    cepError err = processInterpLs(theDataset, interpUi.getSampleRate(), newcfname);
    if(err.isReal()){
      err.display();
      canvas->Refresh();
      return;
    }
    
    wxGetApp().m_docManager->CreateDocument(string(newcfname + ".dat1").c_str(), wxDOC_SILENT);
    free(cfname);
  }
  canvas->Refresh();
}

void cepView::OnInterpNaturalSpline (wxCommandEvent& event)
{
  uiProcessInterp(NATURAL_SPLINE_INTERP, "NS Interp");
}

void cepView::OnInterpCubicSpline (wxCommandEvent& event)
{
  uiProcessInterp(CUBIC_SPLINE_INTERP, "CS Interp");
}

void cepView::OnInterpDivided (wxCommandEvent& event)
{
  uiProcessInterp(DIVIDED_INTERP, "Div Interp");
}

void cepView::OnFFT (wxCommandEvent& event)
{
  cepDoc *theDoc = (cepDoc *) GetDocument ();
  cepDataset *theDataset = theDoc->getDataset ();
  if (theDataset && theDataset->isReady() && theDataset->isWellFormed()){
    char *cfname = strdup("/tmp/cep.XXXXXX");
    int fd;
    fd = mkstemp(cfname);
    close(fd);
    
    string newcfname(string(cfname) + "~" + theDataset->getName());
    cepError err = processFFT(theDataset, newcfname);
    if(err.isReal()){
      err.display();
      canvas->Refresh();
      return;
    }

    wxGetApp().m_docManager->CreateDocument(string(newcfname + ".dat1").c_str(), wxDOC_SILENT);
    free(cfname);
  }
  canvas->Refresh();
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

void cepView::OnSelectFontSize6 (wxCommandEvent& event)
{
  cepError err = m_config->setValue("ui-graph-font-size", 6);
  if(err.isReal())
    err.display();

  m_dirty = true;
  canvas->Refresh();
}

void cepView::OnSelectFontSize8 (wxCommandEvent& event)
{
  cepError err = m_config->setValue("ui-graph-font-size", 8);
  if(err.isReal())
    err.display();

  m_dirty = true;
  canvas->Refresh();
}

void cepView::OnSelectFontSize9 (wxCommandEvent& event)
{
  cepError err = m_config->setValue("ui-graph-font-size", 9);
  if(err.isReal())
    err.display();

  m_dirty = true;
  canvas->Refresh();
}

void cepView::OnSelectFontSize10 (wxCommandEvent& event)
{
  cepError err = m_config->setValue("ui-graph-font-size", 10);
  if(err.isReal())
    err.display();

  m_dirty = true;
  canvas->Refresh();
}

void cepView::OnSelectFontSize11 (wxCommandEvent& event)
{
  cepError err = m_config->setValue("ui-graph-font-size", 11);
  if(err.isReal())
    err.display();

  m_dirty = true;
  canvas->Refresh();
}

void cepView::OnSelectFontSize12 (wxCommandEvent& event)
{
  cepError err = m_config->setValue("ui-graph-font-size", 12);
  if(err.isReal())
    err.display();

  m_dirty = true;
  canvas->Refresh();
}

void cepView::OnSelectFontSize14 (wxCommandEvent& event)
{
  cepError err = m_config->setValue("ui-graph-font-size", 14);
  if(err.isReal())
    err.display();

  m_dirty = true;
  canvas->Refresh();
}

void cepView::OnPrevWindow(wxCommandEvent& event)
{
  cepDoc *theDoc = (cepDoc *) GetDocument ();
  cepDataset *theDataset = theDoc->getDataset ();
  if (theDataset && theDataset->isReady() && theDataset->isWellFormed()){
    if(m_currentWindow > 0){
      m_currentWindow--;
      m_dirty = true;
    }
  }

  canvas->Refresh();
}

void cepView::OnNextWindow(wxCommandEvent& event)
{
  cepDoc *theDoc = (cepDoc *) GetDocument ();
  cepDataset *theDataset = theDoc->getDataset ();
  if (theDataset && theDataset->isReady() && theDataset->isWellFormed()){
    if(m_currentWindow < theDataset->getMatrix(cepDataset::dirX)->getNumTables()){
      m_currentWindow++;
      m_dirty = true;
    }
  }

  canvas->Refresh();
}
