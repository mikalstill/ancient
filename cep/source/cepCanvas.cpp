
/* 
 *   Canvas for the CEP program
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

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_DOC_VIEW_ARCHITECTURE
#error You must set wxUSE_DOC_VIEW_ARCHITECTURE to 1 in setup.h!
#endif

#include "cepUI.h"
#include "cepDoc.h"
#include "cepView.h"
#include "cepPresentation.h"
#include "cepPlot.h"
#include "cepLsUi.h"

#include "cepLs.h"

#include <stdlib.h>
#include <unistd.h>

#include <wx/colour.h>
#include <wx/colordlg.h>

#include "cepCanvas.h"

BEGIN_EVENT_TABLE (cepCanvas, wxScrolledWindow)
EVT_MOUSE_EVENTS (cepCanvas::OnMouseEvent) 
END_EVENT_TABLE ()

// Define a constructor for my canvas
cepCanvas::cepCanvas (wxView * v, wxFrame * frame, const wxPoint & pos, 
		      const wxSize & size, long style):
  wxScrolledWindow (frame, -1, pos, size, style),
  m_selectXStart(-1)
{
  view = v;

  // Is this where we create new controls?
  wxPoint pos, size;

  pos.x = 100;
  pos.y = 42;
  size.x = -1;
  size.y = -1;

  m_config = (cepConfiguration *)&cepConfiguration::getInstance();
}

// Define the repainting behaviour
void
cepCanvas::OnDraw (wxDC & dc)
{
  if (view)
    view->OnDraw (&dc);
}

// Process mouse events, these can include mouse moves, and clicks
void
cepCanvas::OnMouseEvent (wxMouseEvent & event)
{
  wxClientDC dc (this);
  PrepareDC (dc);

  dc.SetLogicalFunction(wxINVERT);
  dc.SetPen (*wxBLACK_PEN);
  wxPoint pt (event.GetLogicalPosition (dc));

  string msg("Mouse location: " + cepToString(pt.x) + ", " + cepToString(pt.y));
  ((cepFrame *) wxGetApp().GetTopWindow())->SetStatusText(msg.c_str(), 0);
  
  string dirName;
  cepDataset::direction selDir = determineGraph(pt.y, dirName);
  
  // Is the mouse down?
  if(event.LeftIsDown()){
    if(m_selectXStart == -1){
      m_selectXStart = pt.x;
      m_selDir = selDir;
      m_selDirString = dirName;

      cepDebugPrint("Mouse down in " + cepToString((int) m_selDir) + " (" + m_selDirString + ")");

      string sel = string(m_selDirString + " " + cepToString(m_selectXStart));
      ((cepFrame *) wxGetApp().GetTopWindow())->SetStatusText(sel.c_str(), 2);
    }
    else{
      m_selectXEnd = pt.x;
      string sel = string(m_selDirString + " " + cepToString(m_selectXStart) + 
			  " to " + m_selDirString + " " + cepToString(m_selectXEnd));
      ((cepFrame *) wxGetApp().GetTopWindow())->SetStatusText(sel.c_str(), 2);
    }
  }
  else if(m_selectXStart != -1){
    // Process the selection, and then move on with our lives...
    // todo_mikal: actually process the selection
    int selectXStart = m_selectXStart;
    m_selectXStart = -1;
    wxMessageBox(string("Process this mouse selection: " + m_selDirString + " " + 
			cepToString(selectXStart) + " to " + m_selDirString + " " + 
			cepToString(m_selectXEnd)).c_str());
    ((cepFrame *) wxGetApp().GetTopWindow())->SetStatusText("", 2);
  }
}

// Determine which graph we are over
cepDataset::direction
cepCanvas::determineGraph(int y, string& name){
  if(y < 0){
    ((cepFrame *) wxGetApp().GetTopWindow())->SetStatusText("No graph selected", 1);
    name = "Unknown";
    return cepDataset::dirUnknown;
  }

  bool showX, showY, showZ;
  int graphCount;
  cepError err;
  
  err = graphStatus(showX, showY, showZ, graphCount);
  if(err.isReal())
    err.display();
  
  if(graphCount == 0){
    ((cepFrame *) wxGetApp().GetTopWindow())->SetStatusText("No graphs visible", 1);
    name = "Unknown";
    return cepDataset::dirUnknown;
  }
  
  int width, height, presHeight, graph;
  GetSize(&width, &height);
  presHeight = height / graphCount;
  graph = y / presHeight;
  
  if(showX && (graph-- == 0)){
    ((cepFrame *) wxGetApp().GetTopWindow())->SetStatusText("North", 1);
    name = "North";
    return cepDataset::dirX;
  }
  if(showY && (graph-- == 0)){
    ((cepFrame *) wxGetApp().GetTopWindow())->SetStatusText("East", 1);
    name = "East";
    return cepDataset::dirY;
  }
  if(showZ && (graph-- == 0)){
    ((cepFrame *) wxGetApp().GetTopWindow())->SetStatusText("Up", 1);
    name = "Up";
    return cepDataset::dirZ;
  }
  
  ((cepFrame *) wxGetApp().GetTopWindow())->SetStatusText("Unknown graph", 1);
  name = "Unknown";
  return cepDataset::dirUnknown;
}

// Determine what graphs are currenly being drawn, and a total count
cepError
cepCanvas::graphStatus(bool& x, bool& y, bool& z, int& count)
{
  cepError err;
  err = m_config->getValue("ui-viewmenu-showx", true, x);
  if(err.isReal()) return err;

  err = m_config->getValue("ui-viewmenu-showy", true, y);
  if(err.isReal()) return err;
  
  err = m_config->getValue("ui-viewmenu-showz", true, z);
  if(err.isReal()) return err;

  count = 0;
  if(x) count++;
  if(y) count++;
  if(z) count++;

  return cepError();
}

// Find the rectangle on the canvas occupied by a given graph
cepError 
cepCanvas::graphPlacement(cepDataset::direction dir, int& top, int& bottom, int& width){
  // Get the total size of the canvas
  int cwidth, cheight;
  GetSize (&cwidth, &cheight);
 
  // Determine how many graphs we are showing, and which ones
  bool show[(int) cepDataset::dirUnknown];
  int graphCount;
  cepError err = graphStatus(show[(int) cepDataset::dirX], show[(int) cepDataset::dirY],
			     show[(int) cepDataset::dirZ], graphCount);
  if(err.isReal())
    return err;

  // No graphs showing
  if(graphCount == 0){
    top = bottom = width = -1;
    return cepError();
  }

  // This graph not showing
  if(!show[(int) dir]){
    top = bottom = width = -1;
    return cepError();
  }

  int presDrop = 0;
  int presHeight = cheight / graphCount;
  
  if(dir == cepDataset::dirX){
    top = presDrop;
    bottom = presDrop + presHeight;
    width = cwidth;
    return cepError();
  }

  if(show[cepDataset::dirX]){
    presDrop += presHeight;
  }

  if(dir == cepDataset::dirY){
    top = presDrop;
    bottom = presDrop + presHeight;
    width = cwidth;
    return cepError();
  }

  if(show[cepDataset::dirY]){
    presDrop += presHeight;
  }

  if(dir == cepDataset::dirZ){
    top = presDrop;
    bottom = presDrop + presHeight;
    width = cwidth;
    return cepError();
  }

  top = bottom = width = -1;
  return cepError();
}

  
