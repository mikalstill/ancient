/////////////////////////////////////////////////////////////////////////////
// Name:        doccepView.h
// Purpose:     Document/view demo
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: cepUI.h,v 1.10 2002-08-09 14:21:51 u982087 Exp $
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
// #pragma interface "doccepView.h"
#endif

#ifndef __DOCVIEWSAMPLEH__
#define __DOCVIEWSAMPLEH__

#include "wx/docmdi.h"
#include "cepCore.h"
#include "cepWxErrorHandler.h"

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
DECLARE_CLASS (cepFrame) public:
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

extern cepFrame *GetMainFrame (void);

#define DOCVIEW_CUT     1
#define DOCVIEW_ABOUT   2

extern bool singleWindowMode;

#endif
