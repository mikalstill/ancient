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

#include "doc.h"
#include "view.h"

IMPLEMENT_DYNAMIC_CLASS (cepDoc, wxDocument)

cepDoc::cepDoc (void)
{
}

cepDoc::~cepDoc (void)
{
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

#if wxUSE_STD_IOSTREAM
istream & cepDoc::LoadObject (istream & stream)
{
  printf("Wibble");
  wxDocument::LoadObject (stream);
  return stream;
}
#else
wxInputStream & cepDoc::LoadObject (wxInputStream & stream)
{
  printf("Wobble\n");
  wxDocument::LoadObject (stream);
  return stream;
}
#endif
