/////////////////////////////////////////////////////////////////////////////
// Name:        cepView.h
// Purpose:     View classes
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: cepView.h,v 1.10 2002-08-10 13:19:18 u964076 Exp $
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
// #pragma interface
#endif

#ifndef __VIEWSAMPLEH__
#define __VIEWSAMPLEH__

#include "wx/docview.h"
#include "cepWxErrorHandler.h"

class cepCanvas:public wxScrolledWindow
{
public:
  wxView * view;

  cepCanvas (wxView * v, wxFrame * frame, const wxPoint & pos,
             const wxSize & size, long style);
  virtual void OnDraw (wxDC & dc);
  void OnMouseEvent (wxMouseEvent & event);

private:
    DECLARE_EVENT_TABLE ()

    wxButton m_button;
};

class cepView:public wxView
{
public:
  wxFrame * frame;
  cepCanvas *canvas;

  cepView();
  ~cepView();

  bool OnCreate (wxDocument * doc, long flags);
  void OnDraw (wxDC * dc);
  void OnUpdate (wxView * sender, wxObject * hint = (wxObject *) NULL);
  bool OnClose (bool deleteWindow = TRUE);

  void OnCut (wxCommandEvent & event);

  void OnToggleAverage (wxCommandEvent& event);
  void OnColorAxes  (wxCommandEvent& event);
  void OnColorLine  (wxCommandEvent& event);
  void OnColorAverage  (wxCommandEvent& event);
  void OnEliminateOutliers (wxCommandEvent& event);

private:
DECLARE_DYNAMIC_CLASS (cepView) DECLARE_EVENT_TABLE ()
  void drawPresentation(cepDataset *ds, cepDataset::direction dir, int top,
			wxDC *dc);
 string m_pngCache[3]; 
 bool m_showAverages;
 bool m_dirty;
 cepConfiguration *m_config;
 cepWxErrorHandler *errHandler;
};


#endif
