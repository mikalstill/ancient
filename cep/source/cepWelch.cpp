/*
 * cepWelch.cpp
 *
 * Description:- Welch averaging functionality
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

#include "cepWelch.h"

cepWelch::cepWelch()
{

}


cepMatrix<double> cepWelch::welchFromPSD(cepMatrix<double> & psd)
{
  int matrixSize = psd.getNumRows();
  int numWindows = psd.getNumTables();
  cepMatrix<double> welchAverage(matrixSize,psd.getNumCols());

  initialisePSDWelchMatrix(welchAverage, psd);
  sumWindows(welchAverage, psd);
  //divide each by number of windows to get average
  int row;
  for (row = 0; row < matrixSize; row++)
  {
    welchAverage.setValue(row,1, welchAverage.getValue(row,1)/numWindows);
  }

  return welchAverage;
}

cepMatrix<double> cepWelch::welchFromTime(cepMatrix<double> & timeSeries,
    double sampleRate, int interpType, int winLength, int winOverlap,
    cepWindow windowType, bool detrend, double chebBandWidth)
{
  // Step 1 Do an interp to regularise data
  cepMatrix<double> regularData;
  cepInterp interpolator;
  regularData = interpolator.doInterp(timeSeries,sampleRate,interpType);

  // Step 2 Window the data
  cepMatrix<double> windowedData;
  cepDataWindower::setWindowType (windowType, winLength, winOverlap);

  if (windowType == cepDataWindower::WINDOW_CHEBYSHEV)
  {
    cepDataWindower::setChebBandwidth (chebBandWidth);
  }
	cepDataWindower::window (regularData,windowedData);

  // Step 3 Detrend data if necessary
  if (detrend)
  {
    getDetrendedData(windowedData);
  }

  // Step 4 DFT data and get PSDs

  cepMatrix <double> psdData;
  if (sizeIsPowerOf2(windowedData))
  {    //FFT
    psdData = getFFT(windowedData);
  }
  else
  {    //DFT
    psdData = getDFT(windowedData);
  }

  // Step 5 welch average the data
  return welchFromPSD(psdData);
}

void cepWelch::initialisePSDWelchMatrix(cepMatrix<double> & welchAverage, cepMatrix<double> & psd)
{
  int row, col;
  int psdCols = psd.getNumCols();
  int psdRows = psd.getNumRows();
  for (col = 0; col < psdCols; col++)
  {
    for (row = 0; row < psdRows; row++)
    {
      if (col == 1)
      {
        welchAverage.setValue(row,col, 0.0);
      }
      else
      {
        welchAverage.setValue(row,col, psd.getValue(row,col,0));
      }
    }
  }
}

void cepWelch::sumWindows(cepMatrix<double> & welchAverage, cepMatrix<double> & psd)
// welchAverage = psd win1 except for col 1 which = 0
{
  int table, row;
  int numWindows = psd.getNumTables();
  int matrixSize = psd.getNumRows();

  for (table = 0; table < numWindows; table++)
  {
    for (row = 0; row < matrixSize; row++)
    {
      welchAverage.setValue(row,1, welchAverage.getValue(row,1) +
                                      psd.getValue(row,1,table));
    }
  }
}

void cepWelch::setToIdentity(cepMatrix<double> & P)
{
  int i,j;
  for (i = 0; i < P.getNumRows(); i++)
  {
    for (j = 0; j < P.getNumCols(); j++)
    {
      if (i==j)
      {
        P.setValue(i,j, 1.0);
      }
      else
      {
        P.setValue(i,j, 0.0);
      }
    }
  }
}

bool cepWelch::sizeIsPowerOf2(cepMatrix<double> matrix)
{
  int fftScale = matrix.getNumRows ();
  for (int k = 0;; ++k)
	{
    // The size is a power of two
	  if ((1 << k) == fftScale)
	    return true;

	  // It is not a power of two
	  if (k == 14 || (1 << k) > fftScale)
	  {
	    // Always go low for now...
	    return false;
	  }
	}
}

cepMatrix <double> cepWelch::getFFT(cepMatrix <double>& regularData)
{
  int winSize = regularData.getNumRows();
  int numCols = regularData.getNumCols();
  int numWindows = regularData.getNumTables();
  cepMatrix <ComplexDble> fftData(winSize,numCols,numWindows);
  for (int i = 0; i < winSize; i++)
  {
    for (int j = 0; j < numCols; j++)
    {
      for (int k = 0; k < numWindows; k++)
      {
        fftData.setValue(i,j,k, regularData.getValue(i,j,k));
      }
    }
  }
  cepCfft < ComplexDble > myFFT (regularData.getNumRows());
  fftData = myFFT.matrixFft (fftData, 1);
  cepMatrix <double> psdData(fftData.getNumRows(),fftData.getNumCols(),fftData.getNumTables());

  for (int i = 0; i < winSize; i++)
  {
    for (int j = 0; j < numCols; j++)
    {
      for (int k = 0; k < numWindows; k++)
      {
        psdData.setValue(i,j,k, real(fftData.getValue(i,j,k)));
      }
    }
  }
  return psdData;
}

cepMatrix <double> cepWelch::getDFT(cepMatrix <double> & regularData)
{
  cepDFT myDFT;
  cepMatrix <ComplexDble> fftData = myDFT.DFT(regularData);
  cout << "FFT windows " << fftData.getNumTables();
  cepPSD myPSD;
  return myPSD.makePSD(fftData);
}

void cepWelch::getDetrendedData(cepMatrix<double> & regularData)
{
    cepMatrix<double> dataWindow(regularData.getNumRows(),regularData.getNumCols());
    int numWindows = regularData.getNumTables();
    cepMatrix<double> P (regularData.getNumRows(),regularData.getNumRows());
    setToIdentity(P);
    for (int i = 0; i < numWindows; i++)
    {
      setToWindow(dataWindow,i,regularData);
      cepLs ls;
      ls.cepDoVCV(dataWindow,P);
      dataWindow = ls.getResidual();
      setWindowTo(dataWindow,i,regularData);
    }
}

void cepWelch::setToWindow(cepMatrix<double> & window, int winNum,
                           cepMatrix<double> & dataSet)
{
  int winSize = dataSet.getNumRows();
  int numCols = dataSet.getNumCols();
  for (int i = 0; i < winSize; i++)
  {
    for (int j = 0; j < numCols; j++)
    {
      window.setValue(i,j, dataSet.getValue(i,j,winNum));
    }
  }
}

void cepWelch::setWindowTo(cepMatrix<double> & window, int winNum,
                           cepMatrix<double> & dataSet)
{
  int winSize = dataSet.getNumRows();
  int numCols = dataSet.getNumCols();
  for (int i = 0; i < winSize; i++)
  {
    for (int j = 0; j < numCols; j++)
    {
      dataSet.setValue(i,j,winNum, window.getValue(i,j));
    }
  }
}
