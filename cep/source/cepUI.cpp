/* 
 *   UI for the CEP program, replaces the cepMain.cpp file
 *
 *   Copyright (C) Michael Still                    2002
 *   Copyright (C) Blake Swadling                   2002
 *
 *   Based on a demo which originally had this notice:
 *
 *   /////////////////////////////////////////////////////////////////////////
 *   // Name:        docview.cpp
 *   // Purpose:     Document/view demo
 *   // Author:      Julian Smart
 *   // Created:     04/01/98
 *   // Copyright:   (c) Julian Smart and Markus Holzem
 *   // Licence:   	wxWindows license
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

#define GLOBALS_HERE
#include "cepCore.h"
#include "cepPresentation.h"
#include "cepPlot.h"
#include "cepStringArray.h"
#include "cepTmpClean.h"
#include "cepDataWindower.h"
#include <unistd.h>

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_DOC_VIEW_ARCHITECTURE
#error You must set wxUSE_DOC_VIEW_ARCHITECTURE to 1 in setup.h!
#endif

#if !wxUSE_MDI_ARCHITECTURE
#error You must set wxUSE_MDI_ARCHITECTURE to 1 in setup.h!
#endif

#include "cepUI.h"
#include "cepDoc.h"
#include "cepView.h"
#include <wx/image.h>

#include <stdlib.h>
#include <sstream>

cepFrame *frame = (cepFrame *) NULL;

// Start up our application. NOTE THAT YOU CAN'T DISPLAY MESSAGE BOXES HERE
// BECAUSE WXWINDOWS ISN'T READY YET. Hence the m_error thing...
IMPLEMENT_APP (cepApp) cepApp::cepApp (void)
{
  m_docManager = (wxDocManager *) NULL;
  m_config = (cepConfiguration *) NULL;
  errHandler = (cepWxErrorHandler *) NULL;
  gLog.open ("cep.log", ios::out);
}

bool
cepApp::OnInit (void)
{
  // Get a reference to the configuration
  m_config = (cepConfiguration *) & cepConfiguration::getInstance ();

  // Subscribe a wx windows based error handler
  errHandler = new cepWxErrorHandler ();
  cepError::addErrorHandler (*errHandler);
  cepDataWindower::init ();

  // Create a document manager
  m_docManager = new wxDocManager;

  // Create a template relating drawing documents to their views
  (void) new
    wxDocTemplate ((wxDocManager *) m_docManager, "Dataset", "*.dat1",
		   "", "dat1", "Dataset", "Dataset View",
		   CLASSINFO (cepDoc), CLASSINFO (cepView));

  // Initialise bitmap handlers (we need these for the presentation layer)
#if wxUSE_LIBPNG
  wxImage::AddHandler (new wxPNGHandler);
#endif

#if wxUSE_LIBJPEG
  wxImage::AddHandler (new wxJPEGHandler);
#endif

#if wxUSE_LIBTIFF
  wxImage::AddHandler (new wxTIFFHandler);
#endif

#if wxUSE_GIF
  wxImage::AddHandler (new wxGIFHandler);
#endif

#if wxUSE_PCX
  wxImage::AddHandler (new wxPCXHandler);
#endif

#if wxUSE_PNM
  wxImage::AddHandler (new wxPNMHandler);
#endif

  // Create the main frame window
  int windowx, windowy;
  m_config->getValue ("ui-mainwindow-size-x", 1000, windowx);
  m_config->getValue ("ui-mainwindow-size-y", 700, windowy);
  cepDebugPrint ("Main frame size is " + cepToString (windowx) + " by " +
		 cepToString (windowy));

  frame =
    new cepFrame ((wxDocManager *) m_docManager, (wxFrame *) NULL,
		  (const wxString) "Geodetic Data Modelling System",
		  wxPoint (0, 0), wxSize (windowx, windowy),
		  wxDEFAULT_FRAME_STYLE);

  // Give it an icon (this is ignored in MDI mode: uses resources)
#ifdef __WXMSW__
  frame->SetIcon (wxIcon ("doc"));
#endif
#ifdef __X__
  frame->SetIcon (wxIcon ("doc.xbm"));
#endif

  // Make a menubar
  wxMenu *file_menu = new wxMenu;
  wxMenu *edit_menu = (wxMenu *) NULL;

  // This is magic, the shortcut keys just work from the menu name...
  file_menu->Append (wxID_OPEN, "&Open...\tCtrl-O");
  file_menu->AppendSeparator ();
  file_menu->Append (wxID_EXIT, "E&xit\tCtrl-Q");

  // A nice touch: a history of files visited. Use this menu.
  m_docManager->FileHistoryUseMenu (file_menu);

  wxMenu *help_menu = new wxMenu;

  help_menu->Append (CEPMENU_ABOUT, "&About\tF1");

  wxMenuBar *menu_bar = new wxMenuBar;

  menu_bar->Append (file_menu, "&File");
  if (edit_menu)
    menu_bar->Append (edit_menu, "&Edit");
  menu_bar->Append (help_menu, "&Help");

  // Associate the menu bar with the frame
  frame->SetMenuBar (menu_bar);

  // We also have a status bar (3 is the number of fields)
  frame->CreateStatusBar (3);

  // Center on the window and make it visible
  frame->Centre (wxBOTH);
  frame->Show (TRUE);

  SetTopWindow (frame);

  // We can process command line options here if we want
  cepDebugPrint ("Starting to parse command line options");
  int optchar;
  string filename;
  cepError err;

  while ((optchar = getopt (argc, argv, "d:e:x:y:z:")) != -1)
    {
      cepDebugPrint ("CLI Options character: " +
		     cepToString ((char) optchar));
      switch (optchar)
	{
	case 'd':
	  // Open this dataset
	  cepDebugPrint ("CLI Dataset: " + string (optarg));
	  filename = optarg;
	  break;

	case 'e':
	  // Show errors?
	  cepDebugPrint ("CLI Show errors: " + string (optarg));
	  m_config->setValue ("ui-viewmenu-showerrors",
			      string (optarg) == "yes");
	  break;

	case 'x':
	  // Show the x plot?
	  cepDebugPrint ("CLI Show X: " + string (optarg));
	  m_config->setValue ("ui-viewmenu-showx", string (optarg) == "yes");
	  break;

	case 'y':
	  // Show the y plot?
	  cepDebugPrint ("CLI Show Y: " + string (optarg));
	  m_config->setValue ("ui-viewmenu-showy", string (optarg) == "yes");
	  break;

	case 'z':
	  // Show the z plot?
	  cepDebugPrint ("CLI Show Z: " + string (optarg));
	  m_config->setValue ("ui-viewmenu-showz", string (optarg) == "yes");
	  break;

	default:
	case '?':
	  err = cepError ("Unknown command line arguement",
			  cepError::sevErrorRecoverable);
	  err.display ();
	  // todo_mikal: A usage message
	  break;
	}
    }

  // Are there old errors?
  if (m_error.isReal ())
    {
      m_error.display ();
      m_error.clear ();
    }

  // Cleanup temp
  cepTmpClean cleaner ("/tmp", "cep*");
  int deleted;
  cepError ce;
  ce = cleaner.execute (deleted, true);

  if (ce.isReal ())
    {
      ce.display ();
    }

  cepDebugPrint ("Cleaned up " + cepToString (deleted) +
		 " temporary items from previous runs");

  // Open the dataset as requested
  if (filename != "")
    {
      m_docManager->CreateDocument (string (filename + ".dat1").c_str (),
				    wxDOC_SILENT);
    }
  return TRUE;
}

int
cepApp::OnExit (void)
{
  // TODO BS 06/08/2002 - removed as this segfaults if data has been loaded.
  // why? i dunno :)
  // delete m_docManager;

  return 0;
}

/*
 * Centralised code for creating a document frame.
 * Called from view.cpp, when a view is created.
 */

wxMDIChildFrame *
cepApp::CreateChildFrame (wxDocument * doc, wxView * view, bool isCanvas)
{
  // Make a child frame
  wxDocMDIChildFrame *subframe =
    new wxDocMDIChildFrame (doc, view, GetMainFrame (), -1, "Child Frame",
			    wxPoint (10, 10), wxSize (300, 300),
			    wxDEFAULT_FRAME_STYLE);

#ifdef __WXMSW__
  subframe->SetIcon (wxString (isCanvas ? "chart" : "notepad"));
#endif
#ifdef __X__
  subframe->SetIcon (wxIcon ("doc.xbm"));
#endif

  // Make a menubar
  wxMenu *file_menu = new wxMenu;

  file_menu->Append (wxID_OPEN, "&Open...");
  file_menu->Append (wxID_CLOSE, "&Close");
  file_menu->Append (wxID_SAVE, "&Save");
  file_menu->Append (wxID_SAVEAS, "Save &As...");

  if (isCanvas)
    {
      file_menu->AppendSeparator ();
      file_menu->Append (wxID_PRINT, "&Print...");
      file_menu->Append (wxID_PRINT_SETUP, "Print &Setup...");
      file_menu->Append (wxID_PREVIEW, "Print Pre&view");
    }

  file_menu->AppendSeparator ();
  file_menu->Append (wxID_EXIT, "E&xit");

  wxMenu *edit_menu = (wxMenu *) NULL;
  wxMenu *view_menu = (wxMenu *) NULL;
  wxMenu *maths_menu = (wxMenu *) NULL;
  wxMenu *size_submenu = (wxMenu *) NULL;
  wxMenu *ls_submenu = (wxMenu *) NULL;
  wxMenu *window_submenu = (wxMenu *) NULL;
  wxMenu *interp_submenu = (wxMenu *) NULL;
  wxMenu *dev_menu = (wxMenu *) NULL;
  wxMenu *goto_menu = (wxMenu *) NULL;

  if (isCanvas)
    {
      edit_menu = new wxMenu;
      edit_menu->Append (wxID_UNDO, "&Undo");
      edit_menu->Append (wxID_REDO, "&Redo");
      edit_menu->AppendSeparator ();
      doc->GetCommandProcessor ()->SetEditMenu (edit_menu);

      /////////////////////////////////////////////////////////////////////////
      // The view menu -- the get their initial state from the config db
      bool confval;
      cepError err;

      m_config = (cepConfiguration *) & cepConfiguration::getInstance ();
      view_menu = new wxMenu (wxMENU_TEAROFF);

      view_menu->Append (CEPMENU_ERRORS, "Show error bars",
			 "Toggle whether the error bars are shown on graphs",
			 TRUE);
      err = m_config->getValue ("ui-viewmenu-showerrors", true, confval);
      if (err.isReal ())
	{
	  view_menu->Check (CEPMENU_ERRORS, true);
	  err.display ();
	}
      else
	view_menu->Check (CEPMENU_ERRORS, confval);

      ////
      view_menu->Append (CEPMENU_GRID, "Show grid",
			 "Toggle whether a grid is shown on graphs", TRUE);
      err = m_config->getValue ("ui-viewmenu-showgrid", true, confval);
      if (err.isReal ())
	{
	  view_menu->Check (CEPMENU_GRID, true);
	  err.display ();
	}
      else
	view_menu->Check (CEPMENU_GRID, confval);

      ////
      view_menu->AppendSeparator ();

      ////
      view_menu->Append (CEPMENU_SHOWX, "Show North",
			 "Show the north direction graph", TRUE);
      err = m_config->getValue ("ui-viewmenu-showx", true, confval);
      if (err.isReal ())
	{
	  view_menu->Check (CEPMENU_SHOWX, true);
	  err.display ();
	}
      else
	view_menu->Check (CEPMENU_SHOWX, confval);

      ////
      view_menu->Append (CEPMENU_SHOWY, "Show East",
			 "Show the east direction graph", TRUE);
      err = m_config->getValue ("ui-viewmenu-showy", true, confval);
      if (err.isReal ())
	{
	  view_menu->Check (CEPMENU_SHOWY, true);
	  err.display ();
	}
      else
	view_menu->Check (CEPMENU_SHOWY, confval);

      ////
      view_menu->Append (CEPMENU_SHOWZ, "Show Up",
			 "Show the up direction graph", TRUE);
      err = m_config->getValue ("ui-viewmenu-showz", true, confval);
      if (err.isReal ())
	{
	  view_menu->Check (CEPMENU_SHOWZ, true);
	  err.display ();
	}
      else
	view_menu->Check (CEPMENU_SHOWZ, confval);

      ////
      view_menu->AppendSeparator ();

      ////
      view_menu->Append (CEPMENU_COLORAXES, "Axes color",
			 "The color of the axes on the graph", FALSE);

      view_menu->Append (CEPMENU_COLORLINE1, "Graph color 1",
			 "The main color of the data line on the graph",
			 FALSE);

      view_menu->Append (CEPMENU_COLORLINE2, "Graph color 2",
			 "The color of the data line on the graph (window 1)",
			 FALSE);

      view_menu->Append (CEPMENU_COLORLINE3, "Graph color 3",
			 "The color of the data line on the graph (window 2)",
			 FALSE);

      view_menu->Append (CEPMENU_COLORERROR, "Error color",
			 "The color of the error bars on the graph", FALSE);

      view_menu->Append (CEPMENU_COLORLS, "LS line color",
			 "The color of the LS regression line on the graph",
			 FALSE);

      view_menu->Append (CEPMENU_COLORREMOVE, "Removed points color",
			 "The color of the points which have been removed by a mathematical operation",
			 FALSE);

      view_menu->AppendSeparator ();

      view_menu->Append (CEPMENU_COLORFONT, "Font color",
			 "The color of the font on the graph", FALSE);

      view_menu->Append (CEPMENU_COLORGRID, "Grid color",
			 "The color of the grid on the graph", FALSE);

      ////
      view_menu->AppendSeparator ();

      view_menu->Append (CEPMENU_SELECTFONT, "Select graph font",
			 "The font which is used for the graph plots", FALSE);

      ////

      size_submenu = new wxMenu (wxMENU_TEAROFF);
      size_submenu->Append (CEPMENU_FONTSIZE6, "6",
			    "Set the font size to 6 points", FALSE);
      size_submenu->Append (CEPMENU_FONTSIZE8, "8",
			    "Set the font size to 8 points", FALSE);
      size_submenu->Append (CEPMENU_FONTSIZE9, "9",
			    "Set the font size to 9 points", FALSE);
      size_submenu->Append (CEPMENU_FONTSIZE10, "10",
			    "Set the font size to 10 points", FALSE);
      size_submenu->Append (CEPMENU_FONTSIZE11, "11",
			    "Set the font size to 11 points", FALSE);
      size_submenu->Append (CEPMENU_FONTSIZE12, "12",
			    "Set the font size to 12 points", FALSE);
      size_submenu->Append (CEPMENU_FONTSIZE14, "14",
			    "Set the font size to 14 points", FALSE);

      view_menu->Append (CEPMENU_LS, "Font size", size_submenu,
			 "Select a font size");

      /////////////////////////////////////////////////////////////////////////
      // The maths menu
      maths_menu = new wxMenu (wxMENU_TEAROFF);

      ////
      ls_submenu = new wxMenu (wxMENU_TEAROFF);
      ls_submenu->Append (CEPMENU_LS_VCV, "Variance Co-variance",
			  "Perform a VCV least squares regression", FALSE);

      ls_submenu->Append (CEPMENU_LS_RW, "Random Walk",
			  "Perform a RW least squares regression", FALSE);

      maths_menu->Append (CEPMENU_LS, "Least squares", ls_submenu,
			  "Least squares regression options");

      ////
      window_submenu = new wxMenu (wxMENU_TEAROFF);
      window_submenu->Append (CEPMENU_WINDOW_BLACKMAN, "Blackman",
			      "Blackman windowing", FALSE);

      window_submenu->Append (CEPMENU_WINDOW_CHEBYSHEV, "Chebyshev",
			      "Chebyshev windowing", FALSE);

      window_submenu->Append (CEPMENU_WINDOW_HAMMING, "Hamming",
			      "Hamming windowing", FALSE);

      window_submenu->Append (CEPMENU_WINDOW_HANNING, "Hanning",
			      "Hamming windowing", FALSE);

      window_submenu->Append (CEPMENU_WINDOW_RECT, "Rectangular",
			      "Rectangular windowing", FALSE);

      maths_menu->Append (CEPMENU_WINDOW, "Windowing", window_submenu,
			  "Dataset windowing");

      ////
      interp_submenu = new wxMenu (wxMENU_TEAROFF);
      interp_submenu->Append (CEPMENU_INTERP_NEAREST, "Nearest Neighbour",
			      "Nearest neighbour interpolation", FALSE);

      interp_submenu->Append (CEPMENU_INTERP_LINEAR, "Linear",
			      "Linear interpolation", FALSE);

      interp_submenu->Append (CEPMENU_INTERP_NATURALSPLINE, "Natural Spline",
			      "Natural spline interpolation", FALSE);

      interp_submenu->Append (CEPMENU_INTERP_CUBICSPLINE, "Cubic Spline",
			      "Cubic spline interpolation", FALSE);

      interp_submenu->Append (CEPMENU_INTERP_DIVIDED,
			      "Newton Divided Difference",
			      "Newton Divided Difference interpolation",
			      FALSE);

      interp_submenu->Append (CEPMENU_INTERP_LS, "Least Squares",
			      "Least Squares interpolation", FALSE);

      maths_menu->Append (CEPMENU_INTERP, "Interpolation", interp_submenu,
			  "Dataset interpolation");

      ////
      maths_menu->Append (CEPMENU_FFT, "Fast Fourier Transform",
			  "Calculate an FFT for this dataset", FALSE);

      ///
      maths_menu->Append (CEPMENU_DFT, "Discrete Fourier Transform",
			  "Calculate a DFT for this dataset", FALSE);
      
      ////
      maths_menu->Append (CEPMENU_WELCH, "Welch Average",
			  "Calculate the Welch average of the dataset", FALSE);
    }

  /////////////////////////////////////////////////////////////////////////////
  // The developer's menu
  dev_menu = new wxMenu (wxMENU_TEAROFF);
  dev_menu->Append (CEPMENU_TESTERRORS, "Test error handling",
		    "This will test that the UI is handling errors correctly",
		    FALSE);

  /////////////////////////////////////////////////////////////////////////////
  // The window selection menu
  goto_menu = new wxMenu (wxMENU_TEAROFF);
  goto_menu->Append (CEPMENU_WINDOWPREV, "Previous Window",
		     "Display the previous window", FALSE);

  goto_menu->Append (CEPMENU_WINDOWNEXT, "Next Window",
		     "Display the next window", FALSE);

  /////////////////////////////////////////////////////////////////////////////
  wxMenu *help_menu = new wxMenu;
  help_menu->Append (CEPMENU_ABOUT, "&About");

  wxMenuBar *menu_bar = new wxMenuBar;
  menu_bar->Append (file_menu, "&File");
  if (isCanvas)
    {
      menu_bar->Append (edit_menu, "&Edit");
      menu_bar->Append (view_menu, "View");
      menu_bar->Append (maths_menu, "Maths");
      menu_bar->Append (goto_menu, "Goto");
    }
  //menu_bar->Append (dev_menu, "Developers");
  menu_bar->Append (help_menu, "&Help");

  // Associate the menu bar with the frame
  subframe->SetMenuBar (menu_bar);

  return subframe;
}

/*
 * This is the top-level window of the application.
 */

IMPLEMENT_CLASS (cepFrame, wxDocMDIParentFrame) BEGIN_EVENT_TABLE (cepFrame, wxDocMDIParentFrame) EVT_MENU (CEPMENU_ABOUT, cepFrame::OnAbout) EVT_MENU (CEPMENU_TESTERRORS, cepFrame::OnTestErrors) EVT_CLOSE (cepFrame::OnClose) END_EVENT_TABLE ()cepFrame::cepFrame (wxDocManager * manager, wxFrame * frame, const wxString & title, const wxPoint & pos, const wxSize & size, long type):
wxDocMDIParentFrame (manager, frame, -1, title, pos, size, type, "myFrame")
{
  editMenu = (wxMenu *) NULL;
}

// Display the about box the for application. This is a modal dialog, which
// is displayed over the rest of the user interface
void
cepFrame::OnAbout (wxCommandEvent & WXUNUSED (event))
{
  ostringstream msg;

  msg << "Geodetic Data Modelling System\n\n";
  msg << "A GPS, VLBI and SLR dataset manipulation tool by\n";
  msg << "    Daniel Fernandez\n";
  msg << "    Michael Still\n";
  msg << "    Blake Swadling\n";
  msg << "    Nick Wheatstone\n";
  msg << "    Kristy Van Der Vlist\n\n";
  msg << "Portions copyright: Julian Smart julian.smart@ukonline.co.uk\n\n";
  msg << "Released under the terms of the GNU GPL";

  wxMessageBox
    ((const wxString &) msg.str ().c_str (),
     "About Geodetic Data Modelling System");
}

void
cepFrame::OnTestErrors (wxCommandEvent & WXUNUSED (event))
{
  if (wxMessageBox
      ("Are you sure you want to do this? It should cause the user interface to exit, loosing all of your work...",
       "Are you sure?", wxYES_NO) == wxYES)
    {
      for (int i = 0; i < cepError::sevMax; i++)
	{
	  cepError e ("Testing 123", (cepError::severity) i);
	  e.display ();
	}
    }
}

// Creates a canvas. Called from view.cpp when a new drawing
// view is created.
cepCanvas *
cepFrame::CreateCanvas (wxView * view, wxFrame * parent)
{
  int width, height;

  parent->GetClientSize (&width, &height);

  // Non-retained canvas
  cepCanvas *canvas =
    new cepCanvas (view, parent, wxPoint (0, 0), wxSize (width, height), 0);
  // canvas->SetCursor (wxCursor (wxCURSOR_PENCIL));

  // Give it scrollbars
  //  canvas->SetScrollbars (20, 20, 50, 50);

  return canvas;
}

cepFrame *
GetMainFrame (void)
{
  return frame;
}

// Capture the window close event, so we can save config info about the window
// todo_mikal: not called on close of application...
void
cepFrame::OnClose (wxCloseEvent & evt)
{
  // Save the window size to the configuration database
  int width, height;

  GetSize (&width, &height);

  cepError err;

  m_config = (cepConfiguration *) & cepConfiguration::getInstance ();
  err = m_config->setValue ("ui-mainwindow-size-x", width);
  if (err.isReal ())
    {
      err.display ();
      err.clear ();
    }
  else
    {
      err = m_config->setValue ("ui-mainwindow-size-y", height);
      if (err.isReal ())
	{
	  err.display ();
	  err.clear ();
	}
    }

  // Close the window
  cepDebugPrint ("Finished with the UI");
  wxFrame::OnCloseWindow (evt);
  cepDebugPrint ("Finished cleaning up the main frame");
}
