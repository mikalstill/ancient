
/* 
 *   UI for the CEP program
 *   Copyright (C) Michael Still                    2002
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
#include "cepCore.h"
#include "cepWxErrorHandler.h"
#include "cepEliminateDialog.h"

class wxDocManager;

// Define a new application
class cepApp:public wxApp
{
public:
  cepApp (void);
  bool OnInit (void);
  int OnExit (void);

  wxMDIChildFrame *CreateChildFrame (wxDocument * doc, wxView * view,
                                     bool isCanvas);

protected:
  cepConfiguration *config;
  wxDocManager * m_docManager;
  cepErrorHandler *errHandler;
  cepError m_error;
};

DECLARE_APP (cepApp)
  // Define a new frame
class cepCanvas;
class cepFrame:public wxDocMDIParentFrame
{
DECLARE_CLASS (cepFrame) 
public:
  wxMenu * editMenu;

  cepFrame (wxDocManager * manager, wxFrame * frame,
            const wxString & title, const wxPoint & pos,
            const wxSize & size, long type);

  void OnAbout (wxCommandEvent & event);

  cepCanvas *CreateCanvas (wxView * view, wxFrame * parent);
  void OnClose (wxCloseEvent & evt);

  DECLARE_EVENT_TABLE ()

protected:
  cepConfiguration *config;
  cepWxErrorHandler *errHandler;
};

extern cepFrame *
GetMainFrame (void);

#define CEPMENU_CUTSEGMENT     1
#define CEPMENU_ABOUT   2
#define CEPMENU_AVERAGE 3
#define CEPMENU_COLORAXES 4
#define CEPMENU_COLORLINE 5
#define CEPMENU_COLORAVERAGE 6
#define CEPMENU_ELIMINATEOUTLIERS 7
#define CEPMENU_VIEWCENTERED 8
#define CEPMENU_VIEWZOOMED 9

extern bool singleWindowMode;

#endif
