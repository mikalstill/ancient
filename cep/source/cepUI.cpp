/* 
   UI for the CEP program, replaces the cepMain.cpp file
   Copyright (C) Michael Still                    2002

   Based on a demo which originally had this notice:

   ////////////////////////////////////////////////////////////////////////////
   // Name:        docview.cpp
   // Purpose:     Document/view demo
   // Author:      Julian Smart
   // Created:     04/01/98
   // Copyright:   (c) Julian Smart and Markus Holzem
   // Licence:   	wxWindows license
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

#define GLOBALS_HERE
#include "core.h"

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

#include <stdlib.h>

cepFrame *frame = (cepFrame *) NULL;

// Start up our application. NOTE THAT YOU CAN'T DISPLAY MESSAGE BOXES HERE
// BECAUSE WXWINDOWS ISN'T READY YET. Hence the m_error thing...
IMPLEMENT_APP (cepApp) cepApp::cepApp (void)
{
  m_docManager = (wxDocManager *) NULL;

  // Open our configuration
  char *homedir = getenv("HOME");
  string config("");

  if(homedir == NULL)
    {
      cepError error("Unable to determine your home directory. Defaulting to the current working directory", cepError::sevInformational);
      m_error = error;
      error.clear();
    }
  else
    {
      config = string(homedir);
      config += "/";
    }

  config += ".cep";
  gConfiguration = new cepConfiguration(config);

  if(!m_error.isReal()){
    cepError dbg("Configuration database is located at: " + config,
  		 cepError::sevDebug);
    m_error = dbg;
    dbg.clear();
  }
  
  // Define some simple default options for the handling of cepErrors
  //  gOptions.errorDisplay[cepError::sevDebug] = false;
  //  gOptions.errorDisplay[cepError::sevInformational] = true;
  //  gOptions.errorDisplay[cepError::sevWarning] = true;
  //  gOptions.errorDisplay[cepError::sevErrorRecoverable] = true;
  //  gOptions.errorDisplay[cepError::sevErrorFatal] = true;
}

bool
cepApp::OnInit (void)
{
  // Create a document manager
  m_docManager = new wxDocManager;

  // Create a template relating drawing documents to their views
  (void) new wxDocTemplate ((wxDocManager *) m_docManager, "Dataset", "*.dat1",
			    "", "dat1", "Dataset", "Dataset View",
			    CLASSINFO (cepDoc),
			    CLASSINFO (cepView));

  // Create the main frame window
  frame =
    new cepFrame ((wxDocManager *) m_docManager, (wxFrame *) NULL,
		  (const wxString) "Techtonic Information Transform System", 
		  wxPoint (0, 0), wxSize (1000, 700),
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
  help_menu->Append (DOCVIEW_ABOUT, "&About\tF1");

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
  if(m_error.isReal()){
    m_error.display();
    m_error.clear();
  }

  // Display tips on startup
  // todo_mikal: make tips work
  // todo_mikal: turn off startup tips sometimes
  // todo_mikal: should the tips be stored in a tdb?
  if ( 1 ) 
    { 
      //      wxTipProvider *tipProvider = wxCreateFileTipProvider("tips.txt", 0); 
      //      wxShowTip(windowParent, tipProvider); 
      //      delete tipProvider; 
    }

  return TRUE;
}

int
cepApp::OnExit (void)
{
  delete m_docManager;
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

  if (isCanvas)
    {
      edit_menu = new wxMenu;
      edit_menu->Append (wxID_UNDO, "&Undo");
      edit_menu->Append (wxID_REDO, "&Redo");
      edit_menu->AppendSeparator ();
      edit_menu->Append (DOCVIEW_CUT, "&Cut last segment");

      doc->GetCommandProcessor ()->SetEditMenu (edit_menu);
    }

  wxMenu *help_menu = new wxMenu;
  help_menu->Append (DOCVIEW_ABOUT, "&About");

  wxMenuBar *menu_bar = new wxMenuBar;

  menu_bar->Append (file_menu, "&File");
  if (isCanvas)
    menu_bar->Append (edit_menu, "&Edit");
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
EVT_MENU (DOCVIEW_ABOUT, cepFrame::OnAbout) 
EVT_CLOSE(cepFrame::OnClose)
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
  wxMessageBox
    ("Techtonic Information Transform System\n\nA GPS, VLBI and SLR dataset manipulation tool by\n\tMichael Still\n\tDaniel Fernandez\n\tBlake Swadling\n\tNick Wheatstone\n\tand Kristy Van Der Vlist\n\nPortions copyright: Julian Smart julian.smart@ukonline.co.uk\n\nReleased under the terms of the GNU GPL",
     "About Techtonic Information Transform System");
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
void cepFrame::OnClose(wxCloseEvent& evt)
{
  // Save the window size to the configuration database
  int width, height;
  GetSize(&width, &height);

  cepError err;
  err = gConfiguration->setValue("mainwindow-size-x", width);
  if(err.isReal()){
    err.display();
    err.clear();
  }
  else{
    err = gConfiguration->setValue("mainwindow-size-y", height);
    if(err.isReal()){
      err.display();
      err.clear();
    }
  }

  // Close the window
  wxFrame::OnCloseWindow(evt);
}
