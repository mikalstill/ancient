/*
 * cepPSD.cpp
 *
 * Description:- Prepares frequency domain data for plotting by doing making
 *               a power spectral density data set
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

#include "cepPSD.h"

cepPSD::cepPSD()
{
}



cepMatrix<double> cepPSD::makePSD(cepMatrix<ComplexDble> & freqSeries)
{
  int winSize = freqSeries.getNumRows();
  int numCols = freqSeries.getNumCols();
  int numWindows = freqSeries.getNumTables();
  cepMatrix<double> psd(winSize,numCols,numWindows);
  int i,j,k;

  for (k = 0; k < numWindows; k++)
  {
    for (i = 0; i < winSize; i++)
    {
      for (j = 0; j < numCols; j++)
      {
        if (j == 1)
        {
          psd.setValue(i,j,k, sqrt(norm(freqSeries.getValue(i,j,k))));
        }
        else
        {
          psd.setValue(i,j,k, real(freqSeries.getValue(i,j,k)));
        }
      }
    }
  }
  return psd;
}
