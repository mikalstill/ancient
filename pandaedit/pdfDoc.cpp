
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
#include <stdio.h>

IMPLEMENT_DYNAMIC_CLASS (pdfDoc, wxDocument)

// This is needed for the progess callback to get to us
pdfDoc *gProgressDoc;

pdfDoc::pdfDoc (void)
{
  m_progress = NULL;
  gProgressDoc = this;
}

pdfDoc::~pdfDoc (void)
{
}

bool pdfDoc::OnSaveDocument(const wxString& filename)
{
  return TRUE;
}
 
bool pdfDoc::OnOpenDocument(const wxString& filename)
{
  m_progressCount = 0;
  m_progressMax = 10;
  m_progress = new wxProgressDialog ("Loading PDF file",
                                     "Please wait while the PDF file is parsed");

  // todo_mikal: check return code
  pandaedit((char *) filename.c_str(), m_pdf, ds_progressCallback);

  // Cleanup
  delete m_progress;
  m_progress = NULL;
  return TRUE;
}

void
pdfDoc::incrementProgress ()
{
  m_progressCount++;
  if(m_progressCount > m_progressMax)
    m_progressMax *= 2;
  printf("DEBUG: Progress %d of %d\n", m_progressCount, m_progressMax);
  m_progress->Update (m_progressCount * 100 / m_progressMax);
}

// A scary global progress handler
void
ds_progressCallback ()
{
  gProgressDoc->incrementProgress ();
}
