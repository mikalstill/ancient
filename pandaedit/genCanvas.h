/* 
 *   Canvas for the CEP program
 *   Copyright (C) Michael Still                    2002
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

#ifndef __CEPCANVASH__
#define __CEPCANVASH__

#include "wx/docview.h"
#include "genWxErrorHandler.h"
#include "genUI.h"

/******************************************************************************
DOCBOOK START

FUNCTION genCanvas
PURPOSE owns all drawing to user visible areas

SYNOPSIS START
#include&lt;genCanvas.h&gt;

SYNOPSIS END

DESCRIPTION Set the starting point for the sequence of curves and lines that it to be drawn on the current page. This call is compulsory for almost all of the line drawing functions. It is not required for the <command>panda_rectangle</command> call.

EXAMPLE START
#include&lt;libplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

EXAMPLE END
SEEALSO 
DOCBOOK END
******************************************************************************/

class genCanvas:public wxScrolledWindow
{
public:
  genCanvas (wxView * v, wxFrame * frame, const wxPoint & pos,
             const wxSize & size, long style);

  virtual void OnDraw (wxDC & dc);
  void OnMouseEvent (wxMouseEvent & event);

  wxView * m_view;

 private:
  DECLARE_EVENT_TABLE ()
  wxFrame *m_frame;
};

#endif
