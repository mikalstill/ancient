
/* 
 *   UI for the CEP program, replaces the cepMain.cpp file
 *   Copyright (C) Michael Still                    2002
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
  config = (cepConfiguration *) NULL;
  errHandler = (cepWxErrorHandler *) NULL;
  gLog.open ("cep.log", ios::out);
}

bool
cepApp::OnInit (void)
{
  // Create a document manager
  m_docManager = new wxDocManager;

  // Get a reference to the configuration
  config = (cepConfiguration *)&cepConfiguration::getInstance();

  // subscribe a wx windows based error handler
  errHandler = new cepWxErrorHandler();
  cepError::addErrorHandler( *errHandler );

  // Create a template relating drawing documents to their views
  (void)new
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
  config->getValue ("ui-mainwindow-size-x", 1000, windowx);
  config->getValue ("ui-mainwindow-size-y", 700, windowy);
  cepDebugPrint ("Main frame size is " + cepItoa (windowx) + " by " +
                 cepItoa (windowy));

  frame =
    new cepFrame ((wxDocManager *) m_docManager, (wxFrame *) NULL,
                  (const wxString)"Geodetic Data Modelling System",
                  wxPoint (0, 0), wxSize (windowx, windowy),
                  wxDEFAULT_FRAME_STYLE);

  // Give it an icon (this is ignored in MDI mode: uses resources)
  // todo_mikal: We should have our own icon
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

  frame->Centre (wxBOTH);
  frame->Show (TRUE);

  SetTopWindow (frame);

  // Are there old errors?
  if (m_error.isReal ())
  {
    m_error.display ();
    m_error.clear ();
  }

  // Display tips on startup
  // todo_mikal: make tips work
  // todo_mikal: turn off startup tips sometimes
  // todo_mikal: should the tips be stored in a tdb?
  if (1)
  {
    // wxTipProvider *tipProvider = wxCreateFileTipProvider("tips.txt", 0); 
    // wxShowTip(windowParent, tipProvider); 
    // delete tipProvider; 
  }

  return TRUE;
}

int
cepApp::OnExit (void)
{
  // BS 06/08/2002 - removed as this segfaults if data has been loaded.
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

  file_menu->Append (wxID_NEW, "&New...");
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

  if (isCanvas)
  {
    edit_menu = new wxMenu;
    edit_menu->Append (wxID_UNDO, "&Undo");
    edit_menu->Append (wxID_REDO, "&Redo");
    edit_menu->AppendSeparator ();
    edit_menu->Append (CEPMENU_CUTSEGMENT, "&Cut last segment");
    doc->GetCommandProcessor ()->SetEditMenu (edit_menu);

    ///////////////////////////////////////////////////////////////////////////
    // The view menu
    view_menu = new wxMenu(wxMENU_TEAROFF);
    view_menu->Append (CEPMENU_AVERAGE, "Show averages",
		       "Toggle whether the average value is shown on graphs",
		       TRUE);
    view_menu->Check(CEPMENU_AVERAGE, false);

    view_menu->Append (CEPMENU_ELIMINATEOUTLIERS, "Eliminate outlying samples",
		       "Removes samples which are outside a given tolerance",
		       FALSE);
    view_menu->Enable(CEPMENU_ELIMINATEOUTLIERS, false);

    view_menu->AppendSeparator();

    view_menu->Append (CEPMENU_VIEWCENTERED, "View centered graphs",
		       "Center the graphs around the horizontal axes",
		       TRUE);
    view_menu->Check(CEPMENU_VIEWCENTERED, true);
    
    view_menu->Append (CEPMENU_VIEWZOOMED, "View zoomed graphs",
		       "Zoom in on the interesting elements in the graph",
		       TRUE);
    view_menu->Check(CEPMENU_VIEWZOOMED, false);    

    view_menu->AppendSeparator();

    view_menu->Append (CEPMENU_COLORAXES, "Axes color",
		       "The color of the axes on the graph", FALSE);
    
    view_menu->Append (CEPMENU_COLORLINE, "Graph color",
		       "The color of the data line on the graph", FALSE);
    
    view_menu->Append (CEPMENU_COLORAVERAGE, "Average color",
		       "The color of the average line on the graph", FALSE);
  }

  wxMenu *help_menu = new wxMenu;

  help_menu->Append (CEPMENU_ABOUT, "&About");

  wxMenuBar *menu_bar = new wxMenuBar;

  menu_bar->Append (file_menu, "&File");
  if (isCanvas){
    menu_bar->Append (edit_menu, "&Edit");
    menu_bar->Append (view_menu, "View");
  }
  menu_bar->Append (help_menu, "&Help");

  // Associate the menu bar with the frame
  subframe->SetMenuBar (menu_bar);

  return subframe;
}

/*
 * This is the top-level window of the application.
 */

IMPLEMENT_CLASS (cepFrame, wxDocMDIParentFrame)
BEGIN_EVENT_TABLE (cepFrame, wxDocMDIParentFrame)
  EVT_MENU (CEPMENU_ABOUT, cepFrame::OnAbout)
EVT_CLOSE (cepFrame::OnClose)
END_EVENT_TABLE ()

cepFrame::cepFrame (wxDocManager * manager, wxFrame * frame, 
		    const wxString & title, const wxPoint & pos, 
		    const wxSize & size, long type):
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
    ((const wxString &)msg.str ().c_str (),
     "About Geodetic Data Modelling System");
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
  canvas->SetScrollbars (20, 20, 50, 50);

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

  config = (cepConfiguration *)&cepConfiguration::getInstance();
  err = config->setValue ("ui-mainwindow-size-x", width);
  if (err.isReal ())
  {
    err.display ();
    err.clear ();
  }
  else
  {
    err = config->setValue ("ui-mainwindow-size-y", height);
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
