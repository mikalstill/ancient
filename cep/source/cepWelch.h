/*
 * cepWelch.h
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

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "cepMatrix.h"
#include "cepError.h"
#include "cepLs.h"
#include "cepInterp.h"
#include "cepDataWindower.h"
#include "cepCfft.h"
#include "cepDFT.h"
#include "cepPSD.h"



#ifndef __CEP_WELCH_H
#define __CEP_WELCH_H


/****
DOCBOOK START
cep Interpolation class

This class

DOCBOOK END
****/
class cepWelch
{

public:

cepWelch();

/* cepMatrix<double> welchFromPSD(cepMatrix<double> & psd)
  Method: welchAverage
  Computes the welch average of a set of psd windows

  Import:
    psd: 3d dimension matrix of psd datasets
  Exports:
    Welch average of the dataset*/
cepMatrix<double> welchFromPSD(cepMatrix<double> & psd);



/* cepMatrix<double> welchFromTime(cepMatrix<double> timeSeries,
      int winLength, int overlap, double sampleRate, bool detrend)
  Method: welchFromTime
  Computes the welch average of a raw time series dataset.
  If the data is irregular then gaps will be filled using
  linear interpolation.

  Imports:
    timeSeries: (Safe variable) orignal time series matrix
    sampleRate: Sample rate of the dataset in days
    interpType: type of interpolation used to make the dataset regular
	 	input: Is a single window matrix, with date as the first row and
						location values as the second row.
		sampleRate: The sample rate that the data is going to be interpolated
						to in decimal years.
		interpType: Type of interpolation to be used:
						NEAREST_INTERP: (1) Nearest neigbour interpolation
						LINEAR_INTERP:  (2) Linear interpolation between points
						NATURAL_SPLINE_INTERP:  (3) Natural spline interpolation
						CUBIC_SPLINE_INTERP: (4) p spline interpolation
						DIVIDED_INTERP: (5) newton divided differences
	  winLength: Length of each data window used in samples
		winOverlap(optional): number of samples that windows are to overlap by
    winType: Type windowing to use:
        cepDataWindower::WINDOW_RECTANGULAR;
        cepDataWindower::WINDOW_HAMMING;
        cepDataWindower::WINDOW_HANNING;
        cepDataWindower::WINDOW_BLACKMAN;
        cepDataWindower::WINDOW_CHEBYSHEV;
    detrend: boolean set true if the uses wishes to detrend each window
  Exports:
    Welch average of the dataset*/
cepMatrix<double> welchFromTime(cepMatrix<double> & timeSeries,
    double sampleRate, int interpType, int winLength, int winOverlap,
    cepWindow windowType, bool detrend, double chebBandWidth = 0.2);

private:

void initialisePSDWelchMatrix(cepMatrix<double> & welchAverage, cepMatrix<double> & psd);

void sumWindows(cepMatrix<double> & welchAverage, cepMatrix<double> & psd);

void setToIdentity(cepMatrix<double> & P);

bool sizeIsPowerOf2(cepMatrix<double> matrix);

cepMatrix <double> getFFT(cepMatrix <double> & regularData);

cepMatrix <double> getDFT(cepMatrix <double> & regularData);

void getDetrendedData(cepMatrix<double> & regularData);

void setToWindow(cepMatrix<double> & window, int winNum, cepMatrix<double> & dataSet);

void setWindowTo(cepMatrix<double> & window, int winNum, cepMatrix<double> & dataSet);

};

#endif // ! __CEP_WELCH_H
