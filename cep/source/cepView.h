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

#include "cepCanvas.h"

class cepView:public wxView
{
public:
  enum cepLsDisplay{
    lsDisplayNone = 0,
      lsDisplayVCV,
      lsDisplayRW
  };

  wxFrame * frame, * m_parentFrame;
  cepCanvas *canvas;

  cepView();
  ~cepView();

  void setParentFrame(wxFrame *parentFrame);

  bool OnCreate (wxDocument * doc, long flags);
  void OnDraw (wxDC * dc);
  void OnUpdate (wxView * sender, wxObject * hint = (wxObject *) NULL);
  bool OnClose (bool deleteWindow = TRUE);

  void OnCut (wxCommandEvent & event);
  //  void OnMouseEvent (wxMouseEvent & event);

  void OnColorAxes  (wxCommandEvent& event);
  void OnColorLine  (wxCommandEvent& event);
  void OnColorAverage  (wxCommandEvent& event);
  void OnColorError  (wxCommandEvent& event);
  void OnViewCentered (wxCommandEvent& event);
  void OnViewZoomed (wxCommandEvent& event);
  void OnToggleX (wxCommandEvent& event);
  void OnToggleY (wxCommandEvent& event);
  void OnToggleZ (wxCommandEvent& event);

  void OnToggleErrors (wxCommandEvent &pevt);

  // Least squares
  void OnLeastSquaresVCV (wxCommandEvent& event);
  void OnLeastSquaresRW (wxCommandEvent& event);

  // Windowing
  void OnWindowBlackman (wxCommandEvent& event);
  void OnWindowChebyshev (wxCommandEvent& event);
  void OnWindowHamming (wxCommandEvent& event);
  void OnWindowRect (wxCommandEvent& event);

  // Interpolation
  void OnInterpNearest (wxCommandEvent& event);
  void OnInterpLinear (wxCommandEvent& event);
  void OnInterpNaturalSpline (wxCommandEvent& event);
  void OnInterpCubicSpline (wxCommandEvent& event);
  void OnInterpDivided (wxCommandEvent& event);

private:
  DECLARE_DYNAMIC_CLASS (cepView) 
  DECLARE_EVENT_TABLE ()

  void drawPresentation(cepDataset *ds, cepDataset::direction dir, int top,
			wxDC *dc, int presWidth, int presHeight);

  void populateMatP(cepMatrix<double> &matP, const double & toDate, const double &fromDate, const double &val, cepMatrix<double> &data);
  string m_pngCache[3];
  bool m_dirty;
  bool m_plotfailed;
  cepLsDisplay m_displayLs;
  cepConfiguration *m_config;
  cepErrorHandler *errHandler;
  int m_x, m_y;
};


#endif
