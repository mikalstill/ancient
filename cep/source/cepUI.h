
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
#include "cepCore.h"
#include "cepWxErrorHandler.h"
#include "cepTextErrorHandler.h"

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

  wxDocManager * m_docManager;

protected:
  cepConfiguration *m_config;
  cepErrorHandler *errHandler;
  cepError m_error;
};

DECLARE_APP (cepApp)
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
  void OnTestErrors (wxCommandEvent & event);

  void OnOpen (wxCommandEvent &event);

  cepCanvas *CreateCanvas (wxView * view, wxFrame * parent);
  void OnClose (wxCloseEvent & evt);

  DECLARE_EVENT_TABLE ()

protected:
  cepConfiguration *m_config;
  cepErrorHandler *errHandler;
};

extern cepFrame *
GetMainFrame (void);

#define CEPMENU_CUTSEGMENT     1
#define CEPMENU_ABOUT   2
//#define CEPMENU_AVERAGE 3
#define CEPMENU_COLORAXES 4
//#define CEPMENU_COLORLINE 5
//#define CEPMENU_COLORAVERAGE 6
//#define CEPMENU_ELIMINATEOUTLIERS 7
//#define CEPMENU_VIEWCENTERED 8
//#define CEPMENU_VIEWZOOMED 9
#define CEPMENU_TESTERRORS 10
#define CEPMENU_SHOWX 11
#define CEPMENU_SHOWY 12
#define CEPMENU_SHOWZ 13
#define CEPMENU_COLORERROR 14
#define CEPMENU_ERRORS 15
#define CEPMENU_LS_VCV 16
#define CEPMENU_LS_RW 17
#define CEPMENU_LS 18
#define CEPMENU_WINDOW_BLACKMAN 19
#define CEPMENU_WINDOW_CHEBYSHEV 20
#define CEPMENU_WINDOW_HAMMING 21
#define CEPMENU_WINDOW_HANNING 50
#define CEPMENU_WINDOW_RECT 22
#define CEPMENU_WINDOW 23
#define CEPMENU_INTERP_NEAREST 24
#define CEPMENU_INTERP_LINEAR 25
#define CEPMENU_INTERP_NATURALSPLINE 26
#define CEPMENU_INTERP_CUBICSPLINE 27
#define CEPMENU_INTERP_DIVIDED 28
#define CEPMENU_INTERP 29
#define CEPBTN_DIR_SUBMIT 30
#define CEPBTN_DIR_CANCEL 31
#define CEPBTN_READ_SUBMIT 32
#define CEPBTN_READ_CANCEL 33
#define CEPBTN_RATE_SUBMIT 34
#define CEPBTN_RATE_CANCEL 35
#define CEPBTN_ATT_SUBMIT 34
#define CEPBTN_ATT_CANCEL 35
#define CEPBTN_WEIGHT_SUBMIT 36
#define CEPBTN_WEIGHT_CANCEL 37
#define CEPBTN_WEIGHT_GO 38
#define CEPBTN_DATE_SUBMIT 39
#define CEPBTN_DATE_CANCEL 40
#define CEPMENU_SELECTFONT 41
#define CEPMENU_FFT 42
#define CEPMENU_COLORFONT 43
#define CEPMENU_GRID 44
#define CEPMENU_COLORGRID 45
#define CEPMENU_COLORLS 46
#define CEPMENU_COLORREMOVE 47
#define CEPMENU_COLORLINE1 48
#define CEPMENU_COLORLINE2 49
#define CEPMENU_COLORLINE3 50

extern bool singleWindowMode;     

#endif
