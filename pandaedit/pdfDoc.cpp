
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
  return TRUE;
}

static int gNewDocumentCount = 1;

bool
pdfDoc::OnNewDocument ()
{
  debug(dlTrace, "New document");
  m_filename = string("New PDF document ") + toString(gNewDocumentCount);
  gNewDocumentCount++;
  m_pdf = NULL;
  m_ready = true;

  return TRUE;
}

bool
pdfDoc::OnOpenDocument (const wxString & filename)
{
  // todo_mikal: truncate filename?
  m_filename = filename;

  m_progressCount = 0;
  m_progressMax = 10;
  m_progress = new wxProgressDialog ("Loading PDF file",
				     "Please wait while the PDF file is parsed");

  // todo_mikal: check return code
  if ((m_pdf = pandaedit ((char *) filename.c_str (), ds_progressCallback)) != 
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
pdfDoc::appendCommand(int pageNum, string commandString)
{
  debug(dlTrace, "Appending the following command to a page of the PDF");
  debug(dlTrace, "Start command");
  debug(dlTrace, commandString);
  debug(dlTrace, "End command");
  while(pageNum >= m_pages.size())
    appendPage();

  m_pages[pageNum].appendCommand(commandString);
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
	dictitem di(dictitem::diTypeName, "Type", m_pdf);
	di.setValue("Pages");
	newpages.getDict().add(di);
      }
      m_pdf->addObject(newpages);

      object newcatalog(objNumAppended, objNumAppended);
      {
	dictitem di(dictitem::diTypeObjectReference, "Pages", m_pdf);
	di.setValue(newpages.getNumber(), newpages.getGeneration());
	newcatalog.getDict().add(di);
      }
      {
	dictitem di(dictitem::diTypeName, "Type", m_pdf);
	di.setValue("Catalog");
	newcatalog.getDict().add(di);
      }
      m_pdf->addObject(newcatalog);      
    }

  object newpage(objNumAppended, objNumAppended);
  m_pdf->addObject(newpage);
  m_pdf->appendPage(newpage);
  m_pages.push_back(newpage, m_pdf);
}

// A scary global progress handler
void
ds_progressCallback ()
{
  gProgressDoc->incrementProgress ();
}
