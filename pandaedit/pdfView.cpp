
/* 
 *   UI for pandaedit
 *   Copyright (C) Michael Still                    2002, 2003
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

#include "genUI.h"
#include "pdfDoc.h"
#include "pdfView.h"
#include "render.h"
#include "verbosity.h"
#include "utility.h"

#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <iomanip>

#include <wx/colour.h>
#include <wx/colordlg.h>

IMPLEMENT_DYNAMIC_CLASS (pdfView, wxView)
  BEGIN_EVENT_TABLE (pdfView, wxView)
  EVT_MENU (GENMENU_NEXTPAGE, pdfView::OnNextPage)
  EVT_MENU (GENMENU_PREVPAGE, pdfView::OnPrevPage)
  EVT_MENU (GENMENU_LINETOOL, pdfView::OnLineTool)
  EVT_MENU (GENMENU_DOCINFO, pdfView::OnAboutDocument)
  EVT_MENU (GENMENU_NEWPAGE, pdfView::OnNewPage)
  END_EVENT_TABLE ()
  
pdfView::pdfView ()
{
  canvas = (genCanvas *) NULL;
  frame = (wxFrame *) NULL;
  m_parentFrame = (wxFrame *) NULL;
  m_page = 0;
  m_dirty = false;
}

void
pdfView::setParentFrame (wxFrame * parentFrame)
{
  m_parentFrame = parentFrame;
}

pdfView::~pdfView ()
{
}

// What to do when a view is created. Creates actual
// windows for displaying the view.
bool
pdfView::OnCreate (wxDocument * doc, long WXUNUSED (flags))
{
  frame = wxGetApp ().CreateChildFrame (doc, this, TRUE);
  frame->SetTitle ("");

  //  m_parentFrame = wxGetApp().GetMainFrame();
  canvas = GetMainFrame ()->CreateCanvas (this, frame);

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
pdfView::OnDraw (wxDC * dc)
{
  dc->SetFont (*wxNORMAL_FONT);
  dc->SetPen (*wxBLACK_PEN);

  pdfDoc *theDoc = (pdfDoc *) GetDocument ();
  if(!theDoc){
    debug(dlTrace, "No document");
    return;
  }

  if(!theDoc->isReady()){
    debug(dlTrace, "Draw ignored because PDF document not ready");
    return;
  }

  // Set the title of the tab if we haven't already
  if(frame->GetTitle() == ""){
    frame->SetTitle(theDoc->getFilename().c_str());
  }
  
  // Show the number of the page we are currently on
  ((wxFrame *) wxGetApp ().GetTopWindow ())->
    SetStatusText (string(string("Displaying page ") +
		   (toString (m_page + 1))).c_str (), 1);

  // Show the currently selected tool
  ((wxFrame *) wxGetApp ().GetTopWindow ())->
    SetStatusText (m_currentToolDesc.c_str (), 2);

  string& filename = m_renders[m_page];
  debug(dlTrace, string("Page render cache names \"") + filename + 
	string("\""));
  if(theDoc->getPDF() != NULL)
    { 
      if(((filename == "") || (m_dirty == true)) && 
	 populatePageFromPDF(theDoc->getPDF(), filename))
	{
	  // And now we can assume that there is a PNG somewhere out there we
	  // can paint...
	  // todo_mikal: shrinking options
	  // todo_mikal: center if smaller than window
	  debug(dlTrace, string("Painting PNG \"") + filename + string("\""));
	  if(filename != ""){
	    try
	      {
		wxImage theImage (filename.c_str(), wxBITMAP_TYPE_PNG);
		wxBitmap theBitmap (theImage.ConvertToBitmap ());
		dc->DrawBitmap (theBitmap, 0, 0);
		m_dirty = false;
	      }
	    catch (...)
	      {
		debug(dlError, "Exception caught in the graph draw routine");
	      }
	  }
	}

    // TODO mikal: also need to redraw the output of the current tool
    }
}

bool
pdfView::populatePageFromPDF(pdf *thePDF, string& filename)
{
  try{
    object foo(objNumNoSuch, objNumNoSuch);
    
    // Find the catalog -- I could probably miss this step, but it seems like
    // a good idea for now...
    debug(dlTrace, "Extracting the catalog");
    object& catalog = foo;
    if(!thePDF->findObject(dictitem::diTypeName, "Type", "Catalog", catalog)){
      debug(dlError, "Bad PDF document: No catalog");
      return false;
    }
    
    // Now find the pages object as refered to by the catalog
    if(!catalog.hasDictItem(dictitem::diTypeObjectReference, "Pages")){
      debug(dlError, "Bad PDF: No pages object refered to in catalog");
      return false;
    }
    
    object& pages = foo;
    if(!catalog.getDict().getValue("Pages", *thePDF, pages)){
      debug(dlError, 
	    "Bad PDF: Could not get pages object, but the catalog references it!");
      return false;
      }
    
    // Now find all the page objects referenced in the pages object
    string kids;
    if(!pages.getDict().getValue("Kids", kids)){
      debug(dlError, "Bad PDF: No pages found in PDF");
      return false;
    }
    
    // Find the pages, and then display the relevant page
    objectlist pagelist(kids, thePDF);
    if(kids.size() < m_page){
      debug(dlError, "Request for a page which doesn't exist");
      return false;
    }

    pdfRender renPage(*thePDF, pagelist[m_page], pages, m_page);
    if(!renPage.render()){
      debug(dlError, "Page render failed");
      return false;
    }
    
    // Now push that into the cache -- this should only be called once
    filename = renPage.getPNGfile();
  }
  catch(...){
    debug(dlError, "Exception caught");
    return false;
  }

    return true;
}

void
pdfView::OnUpdate (wxView * WXUNUSED (sender), wxObject * WXUNUSED (hint))
{
  if (canvas)
    canvas->Refresh ();
}

// Clean up windows used for displaying the view.
bool
pdfView::OnClose (bool deleteWindow)
{
  if (!GetDocument ()->Close ())
    return FALSE;

  // Clear the canvas in case we're in single-window mode,
  // and the canvas stays.
  debug(dlTrace, "Clean up the canvas");

  // todo: The following line was causing a segv
  // canvas->Clear ();
  canvas->m_view = (wxView *) NULL;
  canvas = (genCanvas *) NULL;

  if (frame)
    frame->SetTitle (wxTheApp->GetAppName ());

  SetFrame ((wxFrame *) NULL);
  Activate (FALSE);

  if (deleteWindow)
    {
      debug(dlTrace, "Close the window");
      delete frame;
      return TRUE;
    }
  return TRUE;
}

// Display some basic information about the document
void
pdfView::OnAboutDocument (wxCommandEvent & WXUNUSED (event))
{
  ostringstream msg;
  pdfDoc *theDoc = (pdfDoc *) GetDocument ();
  if(!theDoc->isReady()){
    debug(dlTrace, "Ignoring info request until document is parsed");
    return;
  }

  // todo_mikal: finish this
  msg << "Filename: " << theDoc->getFilename() << endl << endl;
  msg << "Document Title: ";
  msg << "Document Subject: ";
  msg << "Document Author: ";
  msg << "Document Keywords: ";
  msg << "Binding: TODO" << endl;
  msg << "Creator: ";
  msg << "Producer: ";
  msg << "Creation date: ";
  msg << "Last modified: ";
  msg << "Security: ";
  msg << "PDF Specification version: ";
  msg << "Linearized (fast web view): ";
  msg << "Tagged: ";

  wxMessageBox
    ((const wxString &) msg.str ().c_str (),
     "About this PDF");
}

void
pdfView::OnNewPage(wxCommandEvent&)
{
  pdfDoc *theDoc = (pdfDoc *) GetDocument ();
  if(!theDoc->isReady()){
    debug(dlTrace, "Page change ignored because PDF document not ready");
    return;
  }

  theDoc->appendPage();
}

void
pdfView::OnNextPage(wxCommandEvent&)
{
  pdfDoc *theDoc = (pdfDoc *) GetDocument ();
  if(!theDoc->isReady()){
    debug(dlTrace, "Page change ignored because PDF document not ready");
    return;
  }

  if(m_page + 1 < theDoc->getPageCount()){
    m_page++;
    canvas->Refresh();
  }
  else{
    debug(dlTrace, "No more pages");
  }
}

void
pdfView::OnPrevPage(wxCommandEvent&)
{
  pdfDoc *theDoc = (pdfDoc *) GetDocument ();
  if(!theDoc->isReady()){
    debug(dlTrace, "Page change ignored because PDF document not ready");
    return;
  }

  if(m_page != 0){
    m_page--;
    canvas->Refresh();
  }
  else{
    debug(dlTrace, "Already at the start of the document");
  }
}

void
pdfView::OnLineTool(wxCommandEvent&)
{
  m_currentToolDesc = "Line Tool";
  m_currentTool = pdfView::line;
}

void
pdfView::appendCommand(string commandString)
{
  pdfDoc *theDoc = (pdfDoc *) GetDocument ();
  if(!theDoc->isReady()){
    debug(dlTrace, 
	  "Drawing command addition ignored because PDF document not ready");
    return;
  }

  theDoc->appendCommand(m_page, commandString);
}


void
pdfView::setDirty()
{
  m_dirty = true;
}
