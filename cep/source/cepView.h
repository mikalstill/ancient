/////////////////////////////////////////////////////////////////////////////
// Name:        cepView.h
// Purpose:     View classes
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: cepView.h,v 1.2 2002-06-02 10:53:16 u964076 Exp $
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
// #pragma interface
#endif

#ifndef __VIEWSAMPLEH__
#define __VIEWSAMPLEH__

#include "wx/docview.h"

class MyCanvas:public wxScrolledWindow
{
public:
  wxView * view;

  MyCanvas (wxView * v, wxFrame * frame, const wxPoint & pos,
	    const wxSize & size, long style);
  virtual void OnDraw (wxDC & dc);
  void OnMouseEvent (wxMouseEvent & event);

private:
  DECLARE_EVENT_TABLE ()};

class MyTextWindow:public wxTextCtrl
{
public:
  wxView * view;

  MyTextWindow (wxView * v, wxFrame * frame, const wxPoint & pos,
		const wxSize & size, long style);
};

class cepView:public wxView
{
public:
  wxFrame * frame;
  MyCanvas *canvas;

    cepView ()
  {
    canvas = (MyCanvas *) NULL;
    frame = (wxFrame *) NULL;
  }
   ~cepView ()
  {
  }

  bool OnCreate (wxDocument * doc, long flags);
  void OnDraw (wxDC * dc);
  void OnUpdate (wxView * sender, wxObject * hint = (wxObject *) NULL);
  bool OnClose (bool deleteWindow = TRUE);

  void OnCut (wxCommandEvent & event);

private:
DECLARE_DYNAMIC_CLASS (cepView) DECLARE_EVENT_TABLE ()};
#endif
