
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

#include "genDoc.h"
#include "genView.h"

IMPLEMENT_DYNAMIC_CLASS (genDoc, wxDocument)

// This is needed for the progess callback to get to us
genDoc *gProgressDoc;

genDoc::genDoc (void)
{
  m_progress = NULL;
  gProgressDoc = this;
}

genDoc::~genDoc (void)
{
}

bool genDoc::OnSaveDocument(const wxString& filename)
{
  return TRUE;
}
 
bool genDoc::OnOpenDocument(const wxString& filename)
{
  m_progressCount = 0;
  m_progress = new wxProgressDialog ("Loading module",
                                     "Please wait while the application behaviour module is loaded");

  // todo: implement progress dialog
  //genBehaviourLoader myLoad(filename, NULL);

  // Cleanup
  delete m_progress;
  m_progress = NULL;
  return TRUE;
}

void
genDoc::incrementProgress ()
{
  m_progressCount++;
  if (m_progressCount > 5000)
    m_progressCount = 0;

  if(m_progressCount % 50 == 0)
    m_progress->Update (m_progressCount / 50);
}

// A scary global progress handler
void
ds_progressCallback (int plane, long line)
{
  gProgressDoc->incrementProgress ();
}
