/* 
   Imp for the CEP data presentation

   This class owns the presentation of the data on disc... There are
   several presentation formats available -- pdf, tiff, gif...

   Copyright (C) Michael Still                    2002
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef CEP_PRESENTATION_HEADER
#define CEP_PRESENTATION_HEADER

class cepError;

class cepPresentation
{
public:
  cepPresentation (long width, long height);

  void xAxisTitle (const string& title);
  void yAxisTitle (const string& title);
  void xAxisScale (int unitsPerCentiPixel);
  void yAxisScale (int unitsPerCentiPixel);

  cepError addDataPoint (long x, long y);

  cepError createPDF (const string& filename);
  cepError createBitmap ();
  cepError createPNG(const string& filename);

private:
  void interpolate ();

  long m_width;
  long m_height;

  string m_xTitle;
  string m_yTitle;

  int m_xUnit;
  int m_xminval;
  int m_xmaxval;

  int m_yUnit;
  int m_yminval;
  int m_ymaxval;

  vector < long > m_data;

  char *m_raster;
};

#endif
