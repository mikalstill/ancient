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
#include "cepWxErrorHandler.h"
#include "cepTextErrorHandler.h"
#include "cepDataset.h"
#include "cepUI.h"

class cepCanvas:public wxScrolledWindow
{
public:
  cepCanvas (wxView * v, wxFrame * frame, const wxPoint & pos,
             const wxSize & size, long style);

  virtual void OnDraw (wxDC & dc);
  void OnMouseEvent (wxMouseEvent & event);

  cepError graphStatus(bool& x, bool& y, bool& z, int& count);
  cepDataset::direction determineGraph(int y, string& name);
  cepError graphPlacement(cepDataset::direction dir, int& top, int& bottom, int& width);

  wxView * m_view;
  bool m_showx, m_showy, m_showz;
  float m_scale[cepDataset::dirUnknown];
  long m_minval[cepDataset::dirUnknown];

 private:
  DECLARE_EVENT_TABLE ()
    
    wxButton m_button;
  cepConfiguration *m_config;
  int m_selectXStart, m_selectXPrevious, m_selectXEnd;
  cepDataset::direction m_selDir;
  string m_selDirString;
};

#endif
