
/* 
 *   UI for the CEP program
 *   Copyright (C) Michael Still                    2002
 *   Copyright (C) Blake Swadling                   2002
 *   Copyright (C) Kristy Van Der Vlist             2002
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

#ifndef __DOCVIEWSAMPLEH__
#define __DOCVIEWSAMPLEH__

#include "wx/docmdi.h"
#include <wx/image.h>
#include "genErrorHandler.h"
#include "genWxErrorHandler.h"

class wxDocManager;

// Define a new application
class genApp:public wxApp
{
public:
  genApp (void);
  bool OnInit (void);
  int OnExit (void);

  wxMDIChildFrame *CreateChildFrame (wxDocument * doc, wxView * view,
				     bool isCanvas);

  wxDocManager *m_docManager;

protected:
    genErrorHandler * errHandler;
};

DECLARE_APP (genApp)
     class genCanvas;
     class genFrame:public wxDocMDIParentFrame
     {
     DECLARE_CLASS (genFrame) public:
       wxMenu * editMenu;

       genFrame (wxDocManager * manager, wxFrame * frame,
		 const wxString & title, const wxPoint & pos,
		 const wxSize & size, long type);

       void OnAbout (wxCommandEvent & event);
       void OnTestErrors (wxCommandEvent & event);

       void OnOpen (wxCommandEvent & event);

       genCanvas *CreateCanvas (wxView * view, wxFrame * parent);
       void OnClose (wxCloseEvent & evt);

     DECLARE_EVENT_TABLE () protected:
         genErrorHandler * errHandler;
     };

     extern genFrame *GetMainFrame (void);

#define GENMENU_ABOUT   2

     extern bool singleWindowMode;

#endif
