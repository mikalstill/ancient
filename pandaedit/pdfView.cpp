
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
#include "configuration.h"
#include "dlgPageZoom.h"

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
  EVT_MENU (GENMENU_SAVEPAGESTREAM, pdfView::OnSavePageStream)
  EVT_MENU (GENMENU_FORCEREFRESH, pdfView::OnForceRefresh)
  EVT_MENU (GENMENU_FORCEREPARSE, pdfView::OnForceReparse)

  EVT_MENU (GENMENU_NEWPAGE, pdfView::OnNewPage)
  EVT_MENU (GENMENU_ZOOM, pdfView::OnZoom)

  EVT_MENU (GENMENU_NEXTPAGE, pdfView::OnNextPage)
  EVT_MENU (GENMENU_PREVPAGE, pdfView::OnPrevPage)
  EVT_MENU (GENMENU_LINETOOL, pdfView::OnLineTool)

  EVT_MENU (GENMENU_LINECOLOR, pdfView::OnLineColor)
  EVT_MENU (GENMENU_CONTROLCOLOR, pdfView::OnControlColor)

  EVT_MENU (GENMENU_DOCINFO, pdfView::OnAboutDocument)
  END_EVENT_TABLE ()
  
pdfView::pdfView ()
{
  canvas = (genCanvas *) NULL;
  frame = (wxFrame *) NULL;
  m_parentFrame = (wxFrame *) NULL;
  m_page = 0;
  m_dirty = false;
  m_selraster = NULL;
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

  // Change the size of the window to match the size of the page
  debug(dlTrace, string("Setting the window size to that of page ") +
	toString(m_page));
  unsigned int x, y;
  int cx, cy;
  ((wxFrame *) wxGetApp ().GetTopWindow ())->GetSize(&cx, &cy);
  if(theDoc->getPageSize(m_page, x, y))
    {
      canvas->setWidth(x);
      canvas->setHeight(y);

      configuration *config;
      config = (configuration *) & configuration::getInstance ();
      config->getValue("user-pagezoom-x", 100.0, m_xscale);
      config->getValue("user-pagezoom-y", 100.0, m_yscale);
      m_xscale /= 100.0;
      m_yscale /= 100.0;
      x = max((unsigned int) (x * m_xscale), 600);
      y = max((unsigned int) (y * m_yscale), 200);

      if((x != cx) || (y != cy))
	{
	  debug(dlTrace, "Resizing window");
	  ((wxFrame *) wxGetApp ().GetTopWindow ())->SetSize(x + 20, y + 120);
	}
    }

  string& filename = m_renders[m_page];
  debug(dlTrace, string("Page render cache names \"") + filename + 
	string("\""));
  if(m_dirty || (filename == ""))
    populatePageFromPDF(theDoc, filename);
  m_dirty = false;
  
  if(filename != "")
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
	  }
	catch (...)
	  {
	    debug(dlError, "Exception caught in the graph draw routine");
	  }
      }
    }
  
  // TODO mikal: also need to redraw the output of the current tool
}

bool
pdfView::populatePageFromPDF(pdfDoc* theDoc, string& filename)
{
  try{
    object pages(objNumNoSuch, objNumNoSuch);
    if(theDoc->getPagesObject(pages))
      {
	// Now find all the page objects referenced in the pages object
	string kids;
	if(pages.getDict().getValue("Kids", kids)){
	  // Find the pages, and then display the relevant page
	  objectlist pagelist(kids, theDoc->getPDF());
	  if(kids.size() < m_page){
	    debug(dlError, "Request for a page which doesn't exist");
	    return false;
	  }
	}
      }

    // Create a render, including scaling it...
    configuration *config;
    config = (configuration *) & configuration::getInstance ();
    config->getValue("user-pagezoom-x", 100.0, m_xscale);
    config->getValue("user-pagezoom-y", 100.0, m_yscale);
    m_xscale /= 100.0;
    m_yscale /= 100.0;
    pdfRender renPage(theDoc, m_page, m_xscale, m_yscale);
    if(!renPage.render()){
      debug(dlError, "Page render failed");
      return false;
    }

    // Put the selection raster into the relevant variable
    // This has to use free, as libmplot uses malloc
    if(m_selraster != NULL)
      free(m_selraster);
    m_selraster = renPage.getSelectRaster();
    
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
  msg << "Document Title: " << endl;
  msg << "Document Subject: " << endl;
  msg << "Document Author: " << endl;
  msg << "Document Keywords: " << endl;
  msg << "Binding: TODO" << endl << endl;
  msg << "Creator: " << endl;
  msg << "Producer: " << endl;
  msg << "Creation date: " << endl;
  msg << "Last modified: " << endl;
  msg << "Security: " << endl;
  msg << "PDF Specification version: " << endl;
  msg << "Linearized (fast web view): " << endl;
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
pdfView::OnLineColor (wxCommandEvent & WXUNUSED (event))
{
  showColorDialog("line");
}

void
pdfView::OnControlColor (wxCommandEvent & WXUNUSED (event))
{
  showColorDialog("control");
}

void
pdfView::showColorDialog(string configTag)
{
  configuration *config;
  config = (configuration *) & configuration::getInstance ();
  unsigned char r, g, b;

  // Get the current value of the colour
  config->getValue(string(string("color-") + configTag + 
			  string("-r")).c_str(), 0, r);
  config->getValue(string(string("color-") + configTag + 
			  string("-g")).c_str(), 0, g);
  config->getValue(string(string("color-") + configTag + 
			  string("-b")).c_str(), 0, b);  

  // Prime the colour selection dialog and show
  wxColour color(r, g, b);
  wxColourData data;
  data.SetColour(color);
  wxColourDialog picker(NULL, &data);
  if(picker.ShowModal() == wxID_OK){
    data = picker.GetColourData();
    color = data.GetColour();

    // Set config value
    config->setValue(string(string("color-") + configTag + 
			      string("-r")).c_str(), color.Red());
    config->setValue(string(string("color-") + configTag + 
			      string("-g")).c_str(), color.Green());
    config->setValue(string(string("color-") + configTag + 
			      string("-b")).c_str(), color.Blue());
  }
}

void
pdfView::appendCommand(command cmd)
{
  pdfDoc *theDoc = (pdfDoc *) GetDocument ();
  if(!theDoc->isReady()){
    debug(dlTrace, 
	  "Drawing command addition ignored because PDF document not ready");
    return;
  }

  theDoc->appendCommand(m_page, cmd);
}

// void
// pdfView::rewriteCommand(int index, object::commandType type, 
// 			vector<cmdControlPoint> points)
// {
//   pdfDoc *theDoc = (pdfDoc *) GetDocument ();
//   if(!theDoc->isReady()){
//     debug(dlTrace, 
// 	  "Drawing command modification ignored because PDF document not ready");
//     return;
//   }

//   theDoc->rewriteCommand(m_page, index, type, points);
// }

//vector<cmdControlPoint>
// pdfView::getCommand(int index, object::commandType & type)
// {
//   pdfDoc *theDoc = (pdfDoc *) GetDocument ();
//   if(!theDoc->isReady()){
//     vector<cmdControlPoint> none;
//     debug(dlTrace, 
// 	  "Drawing command lookup ignored because PDF document not ready");
//     return none;
//   }

//   return theDoc->getCommand(m_page, index, type);
// }

void
pdfView::setHeight(int height)
{
  pdfDoc *theDoc = (pdfDoc *) GetDocument ();
  if(!theDoc->isReady()){
    debug(dlTrace, 
	  "Cannot set height with invalid document");
    return;
  }

  theDoc->setHeight(m_page, height);
}

void
pdfView::setDirty()
{
  m_dirty = true;
}

char *
pdfView::getSelectRaster()
{
  return m_selraster;
}

void
pdfView::setHoverTarget(int target)
{
  m_hoverTarget = target;
}

// Save a page's description stream to a file for debugging
void
pdfView::OnSavePageStream (wxCommandEvent & event)
{
  pdfDoc *theDoc = (pdfDoc *) GetDocument ();
  if(!theDoc->isReady()){
    debug(dlError, "Cannot save streams for invalid document");
    return;
  }

  // TODO mikal: is there something more I want to do with this dialog?
  wxFileDialog dlg(NULL, "Choose a filename", "", "", 
                   "TextFiles (*.txt)|*.txt", 
                   wxSAVE, wxPoint(-1, -1));
  dlg.Centre();

  if(dlg.ShowModal() == wxID_OK){
    // Open the output stream
    fstream pageFile;
    debug(dlTrace, string("Saving page stream to ") +
	  string(dlg.GetPath().c_str()));
    pageFile.open(dlg.GetPath().c_str(), ios::out);

    // Find all the contents objects
    objectlist contents;
    object pobj(objNumNoSuch, objNumNoSuch);
  object& page = pobj;
    if(!theDoc->getPage(m_page, page))
      return;
    
    page.getDict().getValue("Contents", *(theDoc->getPDF()), contents);
    
    debug(dlTrace, string("Commence stream parsing for a ") + 
	  toString(contents.size()) + string(" object stream"));

    enum lineEnding
      {
	leUnknown = 0,
	leN,
	leRN,
	leR
      };
    lineEnding le(leUnknown);
    bool firstFile(true);
    string leString = "\n";

    for(unsigned int i = 0; i < contents.size(); i++){
      // Read the stream and action any commands
      char *stream;
      unsigned long length;
      
      object cont(objNumNoSuch, objNumNoSuch);
      if(!contents.item(i, cont))
	{
	  debug(dlError, "Could not extract contents entry");
	  return;
	}

      stream = cont.getStream (length);
      if(le == leUnknown)
	{
	  for(unsigned int j = 0; j < length; j++)
	    {
	      if(stream[j] == '\n')
		{
		  le = leN;
		  leString = "\n";
		}
	      if(stream[j] == '\r')
		{
		  if((j + 1 < length) && (stream[j + 1] == '\n'))
		    {
		      le = leRN;
		      leString = "\r\n";
		    }
		  else
		    {
		      le = leR;
		      leString = "\n";
		    }
		}
	    }
	}

      if(length != 0){
	// leN and leRN are acceptable, leR is not
	debug(dlTrace, string("Line ending is: ") + toString(le));
	if(firstFile)
	  {
	    pageFile << "Page description stream extracted by PandaEdit" 
		     << leString;
	    pageFile << "  http://www.stillhq.com" << leString;
	    firstFile = false;
	  }

	if(le == leR)
	  {
	    for(unsigned int j = 0; j < length; j++)
	      if(stream[j] == '\r') stream[j] = '\n';
	    pageFile << "Line endings automatically converted" << leString;
	  }

	pageFile << leString << "(Object " << cont.getNumber()
		 << " " << cont.getGeneration() << ")"
		 << leString << leString;
	for(unsigned int j = 0; j < length; j++)
	  pageFile << stream[j];
      }
      
      delete[]stream;
    }
    
    pageFile.close();
  }
}

void
pdfView::OnZoom(wxCommandEvent&)
{
  dlgPageZoom psize;
  m_dirty = true;
  canvas->Refresh();
}

// Force a repaint of the page
void
pdfView::OnForceRefresh (wxCommandEvent & event)
{
  m_dirty = true;
  canvas->Refresh();
}

// Force a reparse of the page
void
pdfView::OnForceReparse (wxCommandEvent & event)
{
    pdfDoc *theDoc = (pdfDoc *) GetDocument ();
  if(!theDoc->isReady()){
    debug(dlTrace, 
	  "Drawing command addition ignored because PDF document not ready");
    return;
  }

  theDoc->clearCommands(m_page);

  m_dirty = true;
  canvas->Refresh();
}
