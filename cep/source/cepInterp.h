

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

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include "cepMatrix.h"
#include "cepError.h"

#ifndef __CEP_INTERP_H
#define __CEP_INTERP_H

const int NEAREST_INTERP = 1;
const int LINEAR_INTERP  = 2;
const int NATURAL_SPLINE_INTERP = 3;
const int CUBIC_SPLINE_INTERP = 4;
const int DIVIDED_INTERP = 5;

/****
DOCBOOK START
cep Interpolation class

This class has been designed for the purpose of interpolating
geodesy data matrices to new timescales.  The forseen use of
this class is to simulate datasets with a standard sanpling rate.

To this end the class has 2 use methods, both called do interp.  One
where you provide a sample rate and a second where a custom
timescale may be provided.

DOCBOOK END
****/
class cepInterp
{

public:

	cepInterp();
/*******
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
  cepMatrix<double> doInterp(cepMatrix<double> & input, double sampleRate,
									int interpType, int winSize = 1, double winOverlap = 0.0);

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
	cepMatrix<double> doInterp(cepMatrix<double> & input, cepMatrix<double> & timeScale,
												int interpType);


private:

// Member variable for catching errors
cepError m_error;

 // internal interpolation implementation methods

/*
Basic usage for ALL internal interpolation classes:

Imports:
		input: Is a single window matrix, with date as the first column and
				location values as the second column.
	  timescale: 2 column matrix where the first column defines a new timescale
				and the second column is blank. Note: This value is changed
				(can also be used as a return value)
Exports:
		return value: 2 column matrix where the first column is the timescale and
				the second column is the interpolated data points to fit that timescale
*/

// Nearest neighbour interpolation
// Estimates points to be equal to nearest point
	cepMatrix<double> nearestInterp(cepMatrix<double> & input,
												cepMatrix<double> & timeScale);
// Linear interpolation
// Estimates new points as a linear interp of the 2 nearest points
	cepMatrix<double> linearInterp(cepMatrix<double> & input,
												cepMatrix<double> & timeScale);
// Natural spline interpolation
// Estimates new points using a natural spline
// (Natural spline: second derivatives of end points = 0)
	cepMatrix<double> naturalSplineInterp(cepMatrix<double> & input,
												cepMatrix<double> & timeScale);
// Cubic spline interpolation
// Estimates new points using a P spline
// (P spline: second derivative of end points = second to end points)
	cepMatrix<double> cubicSplineInterp(cepMatrix<double> & input,
												cepMatrix<double> & timeScale);
// Divided difference interpolation
// Estimates new points using newton divided differences
// (order of divided differences optimized for minimum error)
	cepMatrix<double> dividedInterp(cepMatrix<double> & input,
												cepMatrix<double> & timeScale);

// Used for keeping fill command within bounds while interpolation
// (also icrements counter)
	bool inBounds(cepMatrix<double> & input, cepMatrix<double> & timeScale,
									 int & position, int & i, int newSize, int oldSize);

// RowReduce: (used in spline interpolation)
// Row reduces a tridiagonal matrix t augmented by the middle n-1 rows of s
// to save memory t is stored as a 3 by n-1 matrix
	void rowReduce(cepMatrix<double> & t, cepMatrix<double> & s, int n);

// calc_abc: (used in spline interpolation)
// Calculates the the a, b and c parameter of a spline model (see spline documentation)
// Imports: s, h, import, n
// Exports: a, b, c
	void calc_abc(cepMatrix<double> & a,cepMatrix<double> & b,cepMatrix<double> & c,
								cepMatrix<double> & s,cepMatrix<double> & h,cepMatrix<double> & input, int n);

// setColour: (used to set different colours to new points)
// Used at generation time of an interpolated matrix
// Imports: input (original matrix)
//          timeScale (interped matrix to be coloured)
//          position (current position in input)
//          i (current position in timeScale)
  void setColour(cepMatrix<double> & input, cepMatrix<double> & timeScale, int position, int i);


  double yearsToJulian(double year);
  // Converts the decimal years format standard to this project to a linear truncated
  // Julian day number.
  // These two date conversion methods were added at a very long date to fix the problem 
  // of the decimal date format not being linear.
  //
  // Limits: yearsToJulian always assumes that each sample has been taken at exactly
  // 12 o'clock on a given day.  This makes interpolation for input frequencies that
  // are multiples of daily.  However it won't be able to cope with input datasets with
  // sampling rates of less than daily without modification of these functions.
  //
  double julianToYears(double julian);


};

#endif // ! __CEP_INTERP_H
