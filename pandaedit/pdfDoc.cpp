
/* 
 *   UI for the CEP program
 *   Copyright (C) Michael Still                    2002
 *
 *   Based on a demo which originally had this notice:
 *
 *   /////////////////////////////////////////////////////////////////////////
 *   // Name:        doc.cpp
 *   // Purpose:     Implements document functionality
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
#include "wx/txtstrm.h"

#if !wxUSE_DOC_VIEW_ARCHITECTURE
#error You must set wxUSE_DOC_VIEW_ARCHITECTURE to 1 in setup.h!
#endif

#include "pdfDoc.h"
#include "pdfView.h"
#include "objectmodel.h"
#include "utility.h"
#include "verbosity.h"
#include "stringArray.h"
#include "dlgPageSize.h"
#include "configuration.h"

IMPLEMENT_DYNAMIC_CLASS (pdfDoc, wxDocument)

// This is needed for the progess callback to get to us
pdfDoc *gProgressDoc;

pdfDoc::pdfDoc (void):
  m_ready(false)
{
  m_progress = NULL;
  gProgressDoc = this;
}

pdfDoc::~pdfDoc (void)
{
}

bool
pdfDoc::OnSaveDocument (const wxString & filename)
{
  // todo_mikal: truncate filename?
  m_filename = filename;
  debug(dlTrace, string("Saving file: ") + filename.c_str());

  // The current tactic is to ignore completely the structure that is provided
  // by the in-memory objects -- this is too hard to integrate with Panda.
  // Instead, we just use Panda to creata a whole new PDF. The exception is
  // that streams are pushed straight into the new Panda objects...
#if defined HAVE_LIBPANDA
  panda_pdf *output;
  panda_page *pg;

  panda_init();
  if((output = panda_open((char *) filename.c_str(), "w")) == NULL)
    {
      debug(dlError, "Could not open PDF file for output");
      return FALSE;
    }

  // If the user has asked for output compression, then turn it on and set it
  // to the highest level. This will take the most CPU to compress, but will
  // result in the smallest documents.
  bool outputCompressOn;
  configuration *config;
  config = (configuration *) & configuration::getInstance ();
  config->getValue ("pref-outputcompress", false, outputCompressOn);
  
  if(outputCompressOn)
    {
      panda_setproperty (output->pages, panda_scope_cascade,
                         panda_object_property_compress, panda_true);
      panda_setproperty (output->pages, panda_scope_cascade,
                         panda_object_property_compress_level, 9);
    }

  debug(dlTrace, string("Saving ") + toString(m_pages.size()) +
	string(" pages"));
  for(unsigned int count = 0; count < m_pages.size(); count++)
    {
      debug(dlTrace, string("Saving page ") + toString(count));
      string mediaBox;
      getPageSize(count, mediaBox);
      pg = panda_newpage(output, (char *) mediaBox.c_str());

      // Push the drawing commands into the pages stream (skip the Panda
      // creator functions, but use Panda to keep track of bytes etc)
      object pobj(objNumNoSuch, objNumNoSuch);
  object& page = pobj;
      if(m_pages.item(count, page))
	{
	  for(int cmdcnt = 0; cmdcnt < page.getCommandCount(); cmdcnt++)
	    {
	      debug(dlTrace, "Executing page command");
	      page.executeCommand(cmdcnt, pg);
	    }
	}
      else
	{
	  debug(dlError, "Could not extract page to execute");
	}
      debug(dlTrace, "Finished saving page");
    }
  debug(dlTrace, "All pages added");
  panda_close(output);
  debug(dlTrace, "Document completely written");
  return TRUE;
#else
  debug(dlError, "Cannot save because Panda not found at compile time");
  return FALSE;
#endif
}

static int gNewDocumentCount = 1;

bool
pdfDoc::OnNewDocument ()
{
  debug(dlTrace, "New document");
  dlgPageSize psize;
  
  configuration *config;
  config = (configuration *) & configuration::getInstance ();
  config->getValue("user-pagesize-widthdefault", 500, m_width);
  config->getValue("user-pagesize-heightdefault", 500, m_height);

  m_filename = string("New PDF document ") + toString(gNewDocumentCount);
  gNewDocumentCount++;
  m_pdf = NULL;
  m_ready = true;

  // And make the first page
  appendPage();

  return TRUE;
}

bool
pdfDoc::OnOpenDocument (const wxString & filename)
{
  // todo_mikal: truncate filename?
  m_filename = filename;

  m_progressCount = 0;
  m_progressMax = 10;
  debug(dlTrace, "Prompting user for a document to open");
  m_progress = new wxProgressDialog ("Loading PDF file",
				     "Please wait while the PDF file is parsed");

  debug(dlTrace, "Starting PDF parse");
  // todo_mikal: check return code
  if ((m_pdf = parse ((char *) filename.c_str (), ds_progressCallback)) != 
      NULL)
    {
      ((wxFrame *) wxGetApp ().GetTopWindow ())->
	SetStatusText ("PDF document parsed", 0);
      debug(dlTrace, "Parser finished");
      
      // Determine how many pages the PDF contains
      objectlist pages = m_pdf->getPages ();
      m_pages = pages;
      if (m_pages.size () != 0)
	{
	  ((wxFrame *) wxGetApp ().GetTopWindow ())->
	    SetStatusText (string
			   (toString (m_pages.size ()) + " pages").c_str (), 
			   0);
	  m_ready = true;
	}
      else
	{
	  delete m_progress;
	  m_progress = NULL;
	  debug(dlError, "Document contains no pages");
	  return FALSE;
	}
    }
  else
    {
      delete m_progress;
      m_progress = NULL;
      debug(dlError, "Parse failed");
      return FALSE;
    }

  // Cleanup
  delete m_progress;
  m_progress = NULL;
  return TRUE;
}

void
pdfDoc::incrementProgress ()
{
  m_progressCount++;
  if (m_progressCount > m_progressMax)
    m_progressMax *= 2;
  m_progress->Update (m_progressCount * 100 / m_progressMax);
}

bool
pdfDoc::isReady()
{
  return m_ready;
}

pdf *
pdfDoc::getPDF()
{
  return m_pdf;
}

unsigned int
pdfDoc::getPageCount()
{
  return m_pages.size();
}

string
pdfDoc::getFilename()
{
  return m_filename;
}

objectlist&
pdfDoc::getPages()
{
  return m_pages;
}

void
pdfDoc::appendCommand(int pageNum, command cmd)
{
  while(pageNum >= m_pages.size())
    appendPage();

  objectreference objref;
  if(!m_pages.item(pageNum, objref))
    return;

  m_pdf->appendCommand(objref, cmd);
}

void
pdfDoc::rewriteCommand(int pageNum, int index, object::commandType type, 
		      vector<wxPoint> points)
{
  while(pageNum >= m_pages.size())
    {
      debug(dlError, "Rewriting a command on a non existant page?");
      appendPage();
    }

  object pobj(objNumNoSuch, objNumNoSuch);
  object& page = pobj;
  if(m_pages.item(pageNum, page))
    page.rewriteCommand(index, type, points);
  else
    debug(dlError, "Could not rewrite command on non existant page");
}

vector<wxPoint>
pdfDoc::getCommand(int pageNum, int index, object::commandType & type)
{
  while(pageNum >= m_pages.size())
    {
      debug(dlError, "Getting a command from a non existant page?");
      appendPage();
    }

  object pobj(objNumNoSuch, objNumNoSuch);
  object& page = pobj;
  if(m_pages.item(pageNum, page))
    return page.getCommandPoints(index, type);
  else
    {
      debug(dlError, "Could not get a command from a non-existant page");
      vector<wxPoint> empty;
      return empty;
    }
}

void
pdfDoc::setHeight(int pageNum, int height)
{
  while(pageNum >= m_pages.size())
    appendPage();

  object pobj(objNumNoSuch, objNumNoSuch);
  object& page = pobj;
  if(m_pages.item(pageNum, page))
    page.setHeight(height);
  else
    debug(dlError, "Could not set page height of non existant page");
}

void
pdfDoc::appendPage()
{
  debug(dlTrace, "Appending a page to the PDF");

  // This could be a new PDF file which doesn't even have a m_pdf yet.
  // If one is needed, then we make it here...
  if(!m_pdf)
    {
      m_pdf = new pdf();
    }

  // We might also not have a catalog
  object foo(objNumNoSuch, objNumNoSuch);
  object & catalog = foo;
  if (!m_pdf->findObject (dictitem::diTypeName, "Type", "Catalog", catalog))
    {
      debug(dlTrace, "Adding a new catalog and pages object to the PDF");
      object newpages(objNumAppended, objNumAppended);
      {
	dictitem di("Type", m_pdf);
	di.setValue("Pages", true);
	newpages.getDict().add(di);
      }
      {
	dictitem di("Kids", m_pdf);
	di.setValue("[ ]", false);
	newpages.getDict().add(di);
      }
      m_pdf->addObject(newpages);

      object newcatalog(objNumAppended, objNumAppended);
      {
	dictitem di("Pages", m_pdf);
	di.setValue(newpages.getNumber(), newpages.getGeneration());
	newcatalog.getDict().add(di);
      }
      {
	dictitem di("Type", m_pdf);
	di.setValue("Catalog", true);
	newcatalog.getDict().add(di);
      }
      m_pdf->addObject(newcatalog);      
    }

  // Create the new page
  debug(dlTrace, "Creating the new page");
  object newcontents(objNumAppended, objNumAppended);
  {
    dictitem di("Type", m_pdf);
    di.setValue("Contents", true);
    newcontents.getDict().add(di);
  }

  object newpage(objNumAppended, objNumAppended);
  {
    dictitem di("Type", m_pdf);
    di.setValue("Page", true);
    newpage.getDict().add(di);
  }
  {
    dictitem di("Contents", m_pdf);
    di.setValue(newcontents);
    newpage.getDict().add(di);
  }
  {
    object tempobject(objNumNoSuch, objNumNoSuch);
    object & pages = tempobject;
    if(!getPagesObject(pages)){
      debug(dlError, "Could not find pages object");
      return;
    }

    dictitem di("Parent", m_pdf);
    di.setValue(pages);
    newpage.getDict().add(di);
  }
  {
    dictitem di("MediaBox", m_pdf);
    di.setValue(string("[ 0 0 ") + toString(m_width) + string(" ") +
		toString(m_height) + string(" ]"), false);
    newpage.getDict().add(di);
    newpage.setHeight(m_height);
  }

  debug(dlTrace, "Adding the new page");
  m_pdf->addObject(newpage);
  m_pdf->appendPage(newpage);
  m_pages.push_back(newpage, m_pdf);
  debug(dlTrace, "Finished appending the page");
}

// A scary global progress handler
void
ds_progressCallback ()
{
  gProgressDoc->incrementProgress ();
}

bool
pdfDoc::getPage(int pageno, object& outobj)
{
  object pobj(objNumNoSuch, objNumNoSuch);
  object& page = pobj;
  if(m_pages.item(pageno, page))
    {
      return m_pages.item(pageno, outobj);
    }

  debug(dlError, "getPage is returning a non existant page");
  return false;
}

bool
pdfDoc::getPageSize(int pageno, string& mediaBox)
{
  object pobj(objNumNoSuch, objNumNoSuch);
  object& page = pobj;
  if(!getPage(pageno, page))
    return false;

  page.getDict ().getValue ("MediaBox", mediaBox);
  if(mediaBox == ""){
    debug(dlTrace, 
	  "No page size specified in page object, trying pages object...");
    object tempobject(objNumNoSuch, objNumNoSuch);
    object& pages = tempobject;
    if(!getPagesObject(pages))
      return false;
    pages.getDict ().getValue ("MediaBox", mediaBox);
  }
  if(mediaBox == ""){
    debug(dlError, "The document does not specify a page size");
    return false;
  }
  return true;
}

bool
pdfDoc::getPageSize(int pageno, unsigned int& x, unsigned int& y)
{
  string mediaBox;
  if(!getPageSize(pageno, mediaBox))
    return false;

  stringArray boxArgs (mediaBox.substr (1, mediaBox.length () - 2), " ");
  x = atoi (boxArgs[2].c_str ());
  y = atoi (boxArgs[3].c_str ());
  debug(dlTrace, string("Page size is ") + toString(x) + string(" by ") +
	toString(y));
  if((x == 0) || (y == 0)){
    debug(dlError, "Invalid page size");
    return false;
  }

  return true;
}

bool
pdfDoc::getPagesObject(object &obj)
{
  if(m_pdf == NULL)
    return false;
  return m_pdf->getPagesObject(obj);
}
