
/* 
 *   Imp for the CEP data presentation
 *
 *   This class owns the presentation of the data on disc... There are
 *   several presentation formats available -- pdf, tiff, gif...
 *
 *   Copyright (C) Michael Still                    2002
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

#ifndef CEP_PRESENTATION_HEADER
#define CEP_PRESENTATION_HEADER

#include "cepDataset.h"

class cepError;

class cepPresentation
{
public:
  cepPresentation (long width, long height, cepMatrix<double> *ds, double b1, double b2, bool haveLs,
		   string offset);

  void xAxisTitle (const string & title);
  void yAxisTitle (const string & title);

  void useErrors(bool yesno);
  void useGrid(bool yesno);

  void setAxesColor(char red, char green, char blue);
  void setLineColor(char red, char green, char blue);
  void setErrorColor(char red, char green, char blue);
  void setGridColor(char red, char green, char blue);
  void setFontColor(char red, char green, char blue);

  cepError createPDF (const string & filename);
  cepError createBitmap (float& xscale, float& yscale, long& xminval, long& yminval, long& yrange);
  cepError createPNG (const string & filename, float& xscale, float& yscale, long& xminval, long& yminval,
		      long& yrange);

private:
  typedef struct{
    char red;
    char green;
    char blue;
  } color;

  long m_width;
  long m_height;

  string m_xTitle;
  string m_yTitle;

  long m_xminval;
  long m_xmaxval;

  long m_yminval;
  long m_ymaxval;

  long m_emaxval;

  color m_axesColor;
  color m_lineColor;
  color m_errorColor;
  color m_gridColor;
  color m_fontColor;

  bool m_useErrors;
  bool m_useGrid;
  char *m_raster;

  cepMatrix<double> *m_ds;
  bool m_haveMaxima;

  double m_b1;
  double m_b2;
  bool m_haveLs;
  string m_offset;

  cepConfiguration *m_config;
};

#endif
