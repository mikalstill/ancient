/* 
   UI for the CEP program
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
#include "doc.h"
#include "view.h"

MyFrame *frame = (MyFrame *) NULL;

IMPLEMENT_APP (MyApp) MyApp::MyApp (void)
{
  m_docManager = (wxDocManager *) NULL;
}

bool
MyApp::OnInit (void)
{
  // Create a document manager
  m_docManager = new wxDocManager;

  // Create a template relating drawing documents to their views
  (void) new wxDocTemplate ((wxDocManager *) m_docManager, "Dataset", "*.dat1",
			    "", "dat", "Dataset", "Dataset View",
			    CLASSINFO (cepDatasetDoc),
			    CLASSINFO (cepDatasetView));

  // Create the main frame window
  frame =
    new MyFrame ((wxDocManager *) m_docManager, (wxFrame *) NULL,
		 (const wxString) "CEPtor", wxPoint (0, 0), wxSize (1000, 700),
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

  file_menu->Append (wxID_NEW, "&New...\tCtrl-N");
  file_menu->Append (wxID_OPEN, "&Open...\tCtrl-X");

  file_menu->AppendSeparator ();
  file_menu->Append (wxID_EXIT, "E&xit\tAlt-X");

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
  return TRUE;
}

int
MyApp::OnExit (void)
{
  delete m_docManager;
  return 0;
}

/*
 * Centralised code for creating a document frame.
 * Called from view.cpp, when a view is created.
 */

wxMDIChildFrame *
MyApp::CreateChildFrame (wxDocument * doc, wxView * view, bool isCanvas)
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

IMPLEMENT_CLASS (MyFrame, wxDocMDIParentFrame) BEGIN_EVENT_TABLE (MyFrame, wxDocMDIParentFrame) EVT_MENU (DOCVIEW_ABOUT, MyFrame::OnAbout) END_EVENT_TABLE ()MyFrame::MyFrame (wxDocManager * manager, wxFrame * frame, const wxString & title, const wxPoint & pos, const wxSize & size, long type):
wxDocMDIParentFrame (manager, frame, -1, title, pos, size, type, "myFrame")
{
  editMenu = (wxMenu *) NULL;
}

void
MyFrame::OnAbout (wxCommandEvent & WXUNUSED (event))
{
  (void)
    wxMessageBox
    ("CEPtor\n\nA GPS, VLBI and SLR dataset manipulation tool by\n\tMichael Still\n\tDaniel Fernandez\n\tBlake Swadling\n\tNick Wheatstone\n\tand Kristy Van Der Vlist\n\nPortions copyright: Julian Smart julian.smart@ukonline.co.uk\n\nReleased under the terms of the GNU GPL",
     "About CEPtor");
}

// Creates a canvas. Called from view.cpp when a new drawing
// view is created.
MyCanvas *
MyFrame::CreateCanvas (wxView * view, wxFrame * parent)
{
  int width, height;
  parent->GetClientSize (&width, &height);

  // Non-retained canvas
  MyCanvas *canvas =
    new MyCanvas (view, parent, wxPoint (0, 0), wxSize (width, height), 0);
  canvas->SetCursor (wxCursor (wxCURSOR_PENCIL));

  // Give it scrollbars
  canvas->SetScrollbars (20, 20, 50, 50);

  return canvas;
}

MyFrame *
GetMainFrame (void)
{
  return frame;
}
