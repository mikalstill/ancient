/* 
   UI for the CEP program
   Copyright (C) Michael Still                    2002

   Based on a demo which originally had this notice:

   ////////////////////////////////////////////////////////////////////////////
   // Name:        doc.cpp
   // Purpose:     Implements document functionality
   // Author:      Julian Smart
   // Created:     04/01/98
   // Copyright:   (c) Julian Smart and Markus Holzem
   // Licence:     wxWindows license
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

#include "core.h"
#include "cepDoc.h"
#include "cepView.h"

IMPLEMENT_DYNAMIC_CLASS (cepDoc, wxDocument)

// This is needed for the progess callback to get to us
cepDoc *gProgressDoc;

cepDoc::cepDoc (void)
{
  m_progress = NULL;
  gProgressDoc = this;
}

cepDoc::~cepDoc (void)
{
  delete m_dataset;
}

#if wxUSE_STD_IOSTREAM
ostream & cepDoc::SaveObject (ostream & stream)
{
  return stream;
}
#else
wxOutputStream & cepDoc::SaveObject (wxOutputStream & stream)
{
  return stream;
}
#endif

// This method is called when the user has selected open from the file menu
// We do the actually opening here, and then lie to wxWindows about the stream
// to the file for it to try to open it
#if wxUSE_STD_IOSTREAM
istream & cepDoc::LoadObject (istream & stream)
#else
wxInputStream & cepDoc::LoadObject (wxInputStream & stream)
#endif
{
  wxMessageBox(GetFilename(), "Banana");
  //wxDocument::LoadObject (stream);

  // We need to change the filename which is displayed in the tab
  SetTitle("Foo");

  // Actually create the dataset
  m_progressCount = 0;
  m_progress = new wxProgressDialog("Loading dataset", 
				    "Please wait while the dataset is loaded");

  m_dataset = new cepDataset("../datasets/mb_ANKR_GPS", ds_progressCallback);
  cepError loadErr = m_dataset->munch();

  if(loadErr.isReal())
    loadErr.display();

  // TODO_Mikal: Handle error condition better

  // Cleanup
  delete m_progress;
  m_progress = NULL;

  return stream;
}

void cepDoc::incrementProgress()
{
  // todo_mikal: 10,000 readings is a lot, do we want to do better?
  m_progressCount++;
  if(m_progressCount > 10000)
    m_progressCount = 0;

  if(m_progressCount % 100 == 0)
    m_progress->Update(m_progressCount / 100);
}

// A scary global progress handler
void
ds_progressCallback (int plane, long line)
{
  gProgressDoc->incrementProgress();
}
