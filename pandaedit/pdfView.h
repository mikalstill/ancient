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

  bool OnCreate (wxDocument * doc, long flags);
  void OnDraw (wxDC * dc);
  void OnUpdate (wxView * sender, wxObject * hint = (wxObject *) NULL);
  bool OnClose (bool deleteWindow = TRUE);

  void OnNextPage (wxCommandEvent & event);
  void OnPrevPage (wxCommandEvent & event);
  void OnLineTool (wxCommandEvent & event);

  void OnAboutDocument (wxCommandEvent & event);

  tool getCurrentTool();
  void appendCommand(string command);
  

private:
  void populatePageFromPDF(pdf *thePDF, string& filename);

  pageCache m_renders;
  int m_page;
  string m_currentToolDesc;
  tool m_currentTool;

  DECLARE_DYNAMIC_CLASS (pdfView)
  DECLARE_EVENT_TABLE () genErrorHandler *errHandler;
};


#endif
