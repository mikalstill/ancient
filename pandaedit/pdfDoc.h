
/* 
 *   UI for the CEP program
 *   Copyright (C) Michael Still                    2002
 *
 *   Based on a demo which originally had this notice:
 *
 *   /////////////////////////////////////////////////////////////////////////
 *   // Name:        doc.h
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
// #pragma interface
#endif

#ifndef __DOCSAMPLEH__
#define __DOCSAMPLEH__

#include "wx/docview.h"
#include <wx/progdlg.h>

#include "pandaedit.h"

class pdfDoc:public wxDocument
{
DECLARE_DYNAMIC_CLASS (pdfDoc) public:
  pdfDoc (void);
   ~pdfDoc (void);

  bool OnOpenDocument (const wxString & filename);
  bool OnSaveDocument (const wxString & filename);

  void incrementProgress ();
  bool isReady();
  pdf *getPDF();
  unsigned int getPageCount();

private:
  wxProgressDialog * m_progress;
  long m_progressCount, m_progressMax;
  pdf *m_pdf;
  objectlist m_pages;
  bool m_ready;
};
#endif

void ds_progressCallback ();
