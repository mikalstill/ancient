/* 
 *   UI for the CEP program
 *   Copyright (C) Michael Still                    2002
 *   Copyright (C) Kristy Van Der Vlist             2002
 *   Copyright (C) Blake Swadling                   2002
 *
 *   Based on a demo which originally had this notice:
 *
 *   /////////////////////////////////////////////////////////////////////////
 *   // Name:        view.h
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

#ifndef __VIEWSAMPLEH__
#define __VIEWSAMPLEH__

#include "genCanvas.h"
#include "pagecache.h"

class pdfView:public wxView
{
public:
  enum tool{
    line = 0
  };

  wxFrame *frame, *m_parentFrame;
  genCanvas *canvas;

    pdfView ();
   ~pdfView ();

  void setParentFrame (wxFrame * parentFrame);
  void setDirty();

  bool OnCreate (wxDocument * doc, long flags);
  void OnDraw (wxDC * dc);
  void OnUpdate (wxView * sender, wxObject * hint = (wxObject *) NULL);
  bool OnClose (bool deleteWindow = TRUE);

  void OnNewPage (wxCommandEvent & event);
  void OnNextPage (wxCommandEvent & event);
  void OnPrevPage (wxCommandEvent & event);
  void OnLineTool (wxCommandEvent & event);

  void OnLineColor (wxCommandEvent & event);
  void OnControlColor (wxCommandEvent & event);

  void OnAboutDocument (wxCommandEvent & event);

  tool getCurrentTool();
  void setHeight(int height);
  void appendCommand(object::commandType type, vector<wxPoint> points);
  void rewriteCommand(int index, object::commandType type, 
		      vector<wxPoint> points);
  vector<wxPoint> getCommand(int index, object::commandType & type);
  char *getSelectRaster();
  void setHoverTarget(int target);

private:
  bool populatePageFromPDF(pdfDoc *theDoc, string& filename);
  void showColorDialog(string configTag);

  pageCache m_renders;
  int m_page;
  int m_hoverTarget;
  bool m_dirty;
  string m_currentToolDesc;
  tool m_currentTool;
  genErrorHandler *errHandler;
  char *m_selraster;

  DECLARE_DYNAMIC_CLASS (pdfView)
  DECLARE_EVENT_TABLE ()
};


#endif
