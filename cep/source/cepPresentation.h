
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

class cepError;

class cepPresentation
{
public:
  enum view{
    viewCentered = 0,
      viewZoomed
      };

  cepPresentation (long width, long height);

  void xAxisTitle (const string & title);
  void yAxisTitle (const string & title);

  cepError addDataPoint (long x, long y, long error = 0);

  void useAverage(bool yesno);
  void useErrors(bool yesno);
  void recalculateAverage();
  long getAverage();

  void setAxesColor(char red, char green, char blue);
  void setLineColor(char red, char green, char blue);
  void setAverageColor(char red, char green, char blue);
  void setErrorColor(char red, char green, char blue);
  void setView(view v);

  cepError createPDF (const string & filename);
  cepError createBitmap ();
  cepError createPNG (const string & filename);

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
  color m_averageColor;
  color m_errorColor;

  bool m_useAverage;
  long m_average;

  view m_currentView;

  vector < long > m_data;

  bool m_useErrors;
  vector < long > m_errors;
  char *m_raster;
};

#endif
