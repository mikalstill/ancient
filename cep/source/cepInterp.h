

/**********************************************************
cepInterp.h

Written By:- Nick Wheatstone
Description:- Interpolation toolkit.  Provides various methods of
interpolation.
***********************************************************/
 /*
  * Imp for the interpolation class
  * Copyright (C) Nick Wheatstone             2002
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

#include <iostream.h>
#include <math.h>
#include <stdlib.h>
#include "cepMatrix.h"

#ifndef __CEP_INTERP_H
#define __CEP_INTERP_H

const int NEAREST_INTERP = 1;
const int LINEAR_INTERP  = 2;
const int NATURAL_SPLINE_INTERP = 3;
const int CUBIC_SPLINE_INTERP = 4;
const int DIVIDED_INTERP = 5;

class cepInterp
{

public:

	cepInterp();
/*******
DOCBOOK START
	 doInterp method 1: Interpolates data to a set sample rate

  cepMatrix<double> & doInterp(const cepMatrix<double> & input, double sampleRate,
									int interpType, int winSize = 1, double winOverlap = 0);

	 Imports:
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
	  winSize(optional): Used for getting datasets that divide into complete
						windows.  Data value is the length of the window in number
	 					of samples.
		winOverlap(optional): Used for getting complete windows.  Data value is
						the proportion of overlap between windows.  This is to be given
						as a number: 0.0 <= winOverlap < 1.0.
	 Export:
	  return value: Interpolated cepMatrix.
DOCBOOK END
*/
  cepMatrix<double> & doInterp(const cepMatrix<double> & input, double sampleRate,
									int interpType, int winSize = 1, double winOverlap = 0);

/****
DOCBOOK START

	 doInterp method 2: Interpolates data to a new timescale

	cepMatrix<double> & doInterp(const cepMatrix<double> & input, cepMatrix<double> & timeScale,
												int interpType);

	 Imports:
		input: Is a single window matrix, with date as the first column and
				location values as the second column.
	  timescale: 2 column matrix where the first column defines a new timescale
				and the second column is blank. Note: This value is changed
				(can also be used as a return value)
		interpType: Type of interpolation to be used:
						NEAREST_INTERP: (1) Nearest neigbour interpolation
						LINEAR_INTERP:  (2) Linear interpolation between points
						NATURAL_SPLINE_INTERP:  (3) Natural spline interpolation
						CUBIC_SPLINE_INTERP: (4) p spline interpolation
						DIVIDED_INTERP: (5) newton divided differences
	 Exports:
		return value: 2 column matrix where the first column is the timescale and
				the second column is the interpolated data points to fit that timescale
DOCBOOK END
*/
	cepMatrix<double> & doInterp(const cepMatrix<double> & input, cepMatrix<double> & timeScale,
												int interpType);


private:

 // internal interpolation methods
	cepMatrix<double> & nearestInterp(const cepMatrix<double> & input,
												cepMatrix<double> & timeScale,	int interpType);
	cepMatrix<double> & linearInterp(const cepMatrix<double> & input,
												cepMatrix<double> & timeScale, int interpType);
	cepMatrix<double> & naturalSplineInterp(const cepMatrix<double> & input,
												cepMatrix<double> & timeScale, int interpType);
	cepMatrix<double> & cubicSplineInterp(const cepMatrix<double> & input,
												cepMatrix<double> & timeScale, int interpType);
	cepMatrix<double> & dividedInterp(const cepMatrix<double> & input,
												cepMatrix<double> & timeScale, int interpType);

	bool inBounds(const cepMatrix<double> & input, cepMatrix<double> & timeScale,
									 int & position, int & i, int newSize, int oldSize);
	void rowReduce(cepMatrix<double> & t, cepMatrix<double> & s);
	void calc_abc(cepMatrix<double> & a,cepMatrix<double> & b,cepMatrix<double> & c,
								cepMatrix<double> & s,cepMatrix<double> & h,cepMatrix<double> & input, int n);

};

#endif // ! __CEP_INTERP_H
