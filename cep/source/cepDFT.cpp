/*
 * cepDFT.cpp
 *
 * Description:- Discrete forier transform algorithm
 *
 *   Copyright (C) Nick Wheatstone                  2002
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc., 675
 * Mass Ave, Cambridge, MA 02139, USA.
 */

#include "cepDFT.h"

cepDFT::cepDFT()
{
  firstDate = 0.0;
}


cepMatrix<ComplexDble > cepDFT::DFT(cepMatrix<double> & timeSeries)
{
  const double PI = 3.141592654;
  int row,window,j;
  int winSize = timeSeries.getNumRows();
  int numWindows = timeSeries.getNumTables();

  // storage matrix for frequency domain data - freq
  cepMatrix<ComplexDble > freqSeries(winSize, timeSeries.getNumCols(),numWindows);
  initialiseFreq(timeSeries, freqSeries);

  for (window = 0; window < numWindows; window++)
  {
    for (j = 0; j < winSize; j++)
      for(row = 0; row < winSize; row++)
      {
        ComplexDble c(0.0,-1.0);
        freqSeries.setValue(j,1,window, freqSeries.getValue(j,1,window) +
                    timeSeries.getValue(row,1,window) *
                    exp(c * 2 * PI * row * j/winSize));
      }
  }

  return freqSeries;
}

cepMatrix<double> cepDFT::iDFT(cepMatrix<ComplexDble > & freqSeries)
{
  const double PI = 3.141592654;
  int row,window,j;
  int winSize = freqSeries.getNumRows();
  int numWindows = freqSeries.getNumTables();

  // storage matrix for frequency domain data - freq
  cepMatrix<double> timeSeries(winSize, freqSeries.getNumCols(),numWindows);
  initialiseTimeSeries(freqSeries, timeSeries);

  for (window = 0; window < numWindows; window++)
  {
    for (j = 0; j < winSize; j++)
      for(row = 0; row < winSize; row++)
      {
        ComplexDble c(0.0,1.0);
        timeSeries.setValue(j,1,window, timeSeries.getValue(j,1,window) +
                    real((1/winSize) * freqSeries.getValue(row,1,window) *
                    exp(c * 2 * PI * row * j/winSize)));
      }
  }
  return timeSeries;
}

void cepDFT::initialiseFreq(cepMatrix <double> & timeSeries, cepMatrix<ComplexDble > & freq)
{
  // save Date of first sample so that time scale can be recovered
  firstDate = timeSeries.getValue(0,0,0);

  int numWindows = timeSeries.getNumTables();
  int winSize = timeSeries.getNumRows();
  int halfWinSize = winSize/2 + winSize%2; //half winSize (rounding up)
  double sampleFreq = getSampleFreq(timeSeries);
  for (int window = 0; window < numWindows; window++)
  {
    for (int row = 0; row < winSize; row++)
    {
      if (row < halfWinSize)
      {
        freq.setValue(row,0,window, (sampleFreq*row)/winSize);
      }
      else
      {
        freq.setValue(row,0,window, (sampleFreq*(halfWinSize-(row-halfWinSize)))/winSize);
      }
      freq.setValue(row,1,window, 0.0);
      freq.setValue(row,2,window, 0.0);
      freq.setValue(row,3,window, 0.0);
    }
  }

}

double cepDFT::getSampleFreq(cepMatrix<double> & matrix)
{
  double DAYSINYEAR = 365.25;
	double sampleRate = abs(matrix.getValue(0,0,0)-matrix.getValue(1,0,0))*DAYSINYEAR;
  return 1.0 / sampleRate;
}

void cepDFT::initialiseTimeSeries(cepMatrix <ComplexDble > & freqSeries, cepMatrix<double> & timeSeries)
{
  int numWindows = timeSeries.getNumTables();
  int winSize = timeSeries.getNumRows();
  double sampleRate = getSampleRate(freqSeries);
  for (int window = 0; window < numWindows; window++)
  {
    for (int row = 0; row < winSize; row++)
    {
      timeSeries.setValue(row,0,window, sampleRate*row+firstDate);
      timeSeries.setValue(row,1,window, 0.0);
      timeSeries.setValue(row,2,window, 0.0);
      timeSeries.setValue(row,3,window, 0.0);
    }
  }

}

double cepDFT::getSampleRate(cepMatrix<ComplexDble > & matrix)
{
  double DAYSINYEAR = 365.25;
	double sampleFreq = abs(real(matrix.getValue(0,0,0)-matrix.getValue(1,0,0)));
  return (1.0 / sampleFreq)/DAYSINYEAR;
}
