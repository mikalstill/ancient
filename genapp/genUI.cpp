
/* 
 *   UI for the CEP program, replaces the genMain.cpp file
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

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>

fstream gLog;

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

#include "genUI.h"
#include "genDoc.h"
#include "genView.h"
#include <wx/image.h>

#include <stdlib.h>
#include <sstream>

genFrame *frame = (genFrame *) NULL;

// Start up our application. NOTE THAT YOU CAN'T DISPLAY MESSAGE BOXES HERE
// BECAUSE WXWINDOWS ISN'T READY YET. Hence the m_error thing...
IMPLEMENT_APP (genApp) genApp::genApp (void)
{
  m_docManager = (wxDocManager *) NULL;
  errHandler = (genWxErrorHandler *) NULL;
  gLog.open ("gen.log", ios::out);
}

bool
genApp::OnInit (void)
{
  // Subscribe a wx windows based error handler
  errHandler = new genWxErrorHandler();
  genError::addErrorHandler( *errHandler );

  // Create a document manager
  m_docManager = new wxDocManager;

  // Let the user be able to open new application modules
  (void)new
    wxDocTemplate ((wxDocManager *) m_docManager, "Application modules", "*.mdl",
                   "", "mdl", "Module", "Module View",
                   CLASSINFO (genDoc), CLASSINFO (genView));

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
  frame =
    new genFrame ((wxDocManager *) m_docManager, (wxFrame *) NULL,
                  (const wxString)"genapp",
                  wxPoint (0, 0), wxSize (100, 100),
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
  frame->CreateStatusBar(3);

  // Center on the window and make it visible
  frame->Centre (wxBOTH);
  frame->Show (TRUE);

  SetTopWindow (frame);

  // We can process command line options here if we want
  return TRUE;
}

int
genApp::OnExit (void)
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
genApp::CreateChildFrame (wxDocument * doc, wxView * view, bool isCanvas)
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

  wxMenu *help_menu = new wxMenu;
  help_menu->Append (CEPMENU_ABOUT, "&About");

  wxMenuBar *menu_bar = new wxMenuBar;
  menu_bar->Append (file_menu, "&File");
  menu_bar->Append (help_menu, "&Help");

  // Associate the menu bar with the frame
  subframe->SetMenuBar (menu_bar);
  return subframe;
}

/*
 * This is the top-level window of the application.
 */

IMPLEMENT_CLASS (genFrame, wxDocMDIParentFrame)
BEGIN_EVENT_TABLE (genFrame, wxDocMDIParentFrame)
  EVT_MENU (CEPMENU_ABOUT, genFrame::OnAbout)
  EVT_MENU (CEPMENU_TESTERRORS, genFrame::OnTestErrors)
EVT_CLOSE (genFrame::OnClose)
END_EVENT_TABLE ()

genFrame::genFrame (wxDocManager * manager, wxFrame * frame, 
		    const wxString & title, const wxPoint & pos, 
		    const wxSize & size, long type):
wxDocMDIParentFrame (manager, frame, -1, title, pos, size, type, "myFrame")
{
  editMenu = (wxMenu *) NULL;
}

// Display the about box the for application. This is a modal dialog, which
// is displayed over the rest of the user interface
void
genFrame::OnAbout (wxCommandEvent & WXUNUSED (event))
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

void
genFrame::OnTestErrors (wxCommandEvent & WXUNUSED (event))
{
  if(wxMessageBox("Are you sure you want to do this? It should cause the user interface to exit, loosing all of your work...", "Are you sure?", wxYES_NO) == wxYES){
    for(int i = 0; i < genError::sevMax; i++){
      genError e("Testing 123", (genError::severity) i);
      e.display();
    }
  }
}

// Creates a canvas. Called from view.cpp when a new drawing
// view is created.
genCanvas *
genFrame::CreateCanvas (wxView * view, wxFrame * parent)
{
  int width, height;

  parent->GetClientSize (&width, &height);

  // Non-retained canvas
  genCanvas *canvas =
    new genCanvas (view, parent, wxPoint (0, 0), wxSize (width, height), 0);
  // canvas->SetCursor (wxCursor (wxCURSOR_PENCIL));

  // Give it scrollbars
  //  canvas->SetScrollbars (20, 20, 50, 50);

  return canvas;
}

genFrame *
GetMainFrame (void)
{
  return frame;
}

// Capture the window close event, so we can save config info about the window
// todo_mikal: not called on close of application...
void
genFrame::OnClose (wxCloseEvent & evt)
{
  // Save the window size to the configuration database
  int width, height;
  GetSize (&width, &height);
  wxFrame::OnCloseWindow (evt);
}
