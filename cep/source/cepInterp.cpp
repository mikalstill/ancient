
/**********************************************************
cepInterp.cpp

Written by:- Nick Wheatstone
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

#include "cepInterp.h"

cepInterp::cepInterp()
{

}

cepMatrix<double> & cepInterp::doInterp(cepMatrix<double> const & input, double sampleRate,
												int interpType, int winSize = 1, double winOverlap = 0.0)
{
	/* get the size of the input set*/
	int inputSize = input.getNumRows();

	/* winLength is the length of a window minus overlap on start edge*/
	double winLength = winSize * (1-winOverlap);

	/* calculate number of required samples after interpolation*/
	int numSamples;
	numSamples = (int)(input.getValue(0,inputSize-1)- input.getValue(0,0))/inputSize;
	numSamples = (int)(numSamples-(numSamples-winSize)/winLength);

	/* build new timescale*/
	cepMatrix<double> timeScale(2,numSamples);
	timeScale.setValue(0,0,input.getValue(0,0));
	for (int i = 1; i < numSamples; i++)
		timeScale.setValue(i,0,timeScale.getValue(i-1,0));

	/* do interpolation and return results */
	return doInterp(input,timeScale,interpType);
}

cepMatrix<double> & cepInterp::doInterp(const cepMatrix<double> & input,
                                 cepMatrix<double> & timeScale,int interpType)
{
	switch(interpType)
	{
		case NEAREST_INTERP:
						return nearestInterp(input, timeScale);
						break;
		case LINEAR_INTERP:
						return linearInterp(input, timeScale);
						break;
		case NATURAL_SPLINE_INTERP:
						return naturalSplineInterp(input, timeScale);
						break;
		case CUBIC_SPLINE_INTERP:
						return cubicSplineInterp(input, timeScale);
						break;
		case DIVIDED_INTERP:
						return dividedInterp(input, timeScale);
						break;
	}
	return timeScale;
}

	cepMatrix<double> & cepInterp::nearestInterp(cepMatrix<double> const & input,
												cepMatrix<double> & timeScale)
{
	int newSize = timeScale.getNumRows();
	int oldSize = input.getNumRows();

	double distA, distB; // distance from adjacent points

	int position = 0; // position on old timeline
	// fill each value on new timeline = nearst old value;
	for (int i = 0; i < newSize; i++)
	{
		if (inBounds(input, timeScale, position, i, newSize, oldSize))
		{
			distA = input.getValue(i,0) - input.getValue(position,0);
			distB = input.getValue(i,0) - input.getValue(position+1,0);
			if (distA < distB)
				timeScale.setValue(i,1,input.getValue(position,1));
			else
				timeScale.setValue(i,1,input.getValue(position+1,1));
		}
		else
		{
			return timeScale;
		}
	}
	return timeScale;
}

	cepMatrix<double> & cepInterp::linearInterp(const cepMatrix<double> & input,
												cepMatrix<double> & timeScale)
{
	int newSize = timeScale.getNumRows();
	int oldSize = input.getNumRows();

	double distDate, distValue;

	int position = 0;
	for (int i = 0; i < newSize; i++)
	{
		if (inBounds(input, timeScale, position, i, newSize, oldSize))
		{
			distDate = input.getValue(position+1,0)-input.getValue(position,0);
			distValue = input.getValue(position+1,1)-input.getValue(position,1);
			timeScale.setValue(i,1,input.getValue(position,1)+distValue/distDate*
												(timeScale.getValue(i,0)-input.getValue(position,0)));
		}
		else
		{
			return timeScale;
		}
	}
	return timeScale;

}

	cepMatrix<double> & cepInterp::naturalSplineInterp(const cepMatrix<double> & input,
												cepMatrix<double> & timeScale)
{
	int i;
	int newSize = timeScale.getNumRows();
	int oldSize = input.getNumRows();
	int n = oldSize - 1;

	// a cubic spline describes a line by drawing a cubic between each 2
	// adjacent points and then matching the first and second derivatives of
	// the cubics
	// a, b, c and d are set of parameters for each of these cubics such that
	// a(x-n)^3+b(x-n)^2+c(x-n) + d = line approximation between 2 points
	cepMatrix<double> a(n,1);
	cepMatrix<double> b(n,1);
	cepMatrix<double> c(n,1);
	cepMatrix<double> d(n,1);

	// h describes the disance accross intervals
	// h[i] = (x[i+1] - x[i]) - set later in loop
	cepMatrix<double> h(n,1);

	// S describes the second derivatives (too complex to describe here)
	cepMatrix<double> s(n+1,1);

	// t is a temporary cooefficent matrix used for intermediate calculations
	// uses a n-1 by 3 matrix to simulate a n-1 by n-1 tridiagonal matrix.
	cepMatrix<double> t(n-1,3);

	for (i = 0; i < n; i++)
	{
		// d is set to the height value at the start of each interval
		d.setValue(i,0,input.getValue(i,1));
		h.setValue(i,0,input.getValue(i+1,0)-input.getValue(i,0));
	}

	// because this is a natural spline the second derivatives
	// of the end points are set to zero
	s.setValue(0,0,0.0);
	s.setValue(n,0,0.0);

	// set initial coefficent matrix and place temp values in the
	// middle n-1 values of s

	// initialise first row
	t.setValue(0,1, 2.0*(h.getValue(0,0)+h.getValue(1,0)));
	t.setValue(0,2, h.getValue(1,0));
	s.setValue(1,0, 6.0*((input.getValue(2,1)-input.getValue(1,1))/h.getValue(1,0) -
										 (input.getValue(1,1)-input.getValue(0,1))/h.getValue(0,0)));
	// initialise middle rows
	for (i = 2; i < n-1; i++)
	{
		t.setValue(i-1,0, h.getValue(i-1,0));
		t.setValue(i-1,1, 2.0*(h.getValue(i-1,0)+h.getValue(i,0)));
		t.setValue(i-1,2, h.getValue(i,0));
		s.setValue(i,0, 6.0*((input.getValue(i+1,1)-input.getValue(i,1))/h.getValue(i,0) -
											 (input.getValue(i,1)-input.getValue(i-1,1))/h.getValue(i-1,0)));
	}
	// initialise last row
		t.setValue(n-2,0, h.getValue(n-2,0));
		t.setValue(n-2,1, 2.0*(h.getValue(n-2,0)+h.getValue(n-1,0)));
		s.setValue(n-1,0, 6.0*((input.getValue(n,1)-input.getValue(n-1,1))/h.getValue(n-1,0) -
											 (input.getValue(n-1,1)-input.getValue(n-2,1))/h.getValue(n-2,0)));

	// augment t with s then row reduce
	rowReduce(t,s,n);

	// use values of s to calculate a, b and c
	calc_abc(a,b,c,s,h,input,n);

	// use a,b,c and d to interpolate the data to the new timescale

	int position = 0;
	for (i = 0; i < newSize; i++)
	{
		if (inBounds(input, timeScale, position, i, newSize, oldSize))
		{
			timeScale.setValue(i,1,
								a.getValue(i,0)*pow((timeScale.getValue(i,0)-input.getValue(i,0)),3.0)+
								b.getValue(i,0)*pow((timeScale.getValue(i,0)-input.getValue(i,0)),2.0)+
								c.getValue(i,0)*(timeScale.getValue(i,0)-input.getValue(i,0))+
								d.getValue(i,0));
		}
		else
		{
		  // give error once I know how
			return timeScale;
		}
	}
	return timeScale;

}

cepMatrix<double> & cepInterp::cubicSplineInterp(const cepMatrix<double> & input,
												cepMatrix<double> & timeScale)
{
	return timeScale;
}


cepMatrix<double> & cepInterp::dividedInterp(const cepMatrix<double> & input,
												cepMatrix<double> & timeScale)
{
	int i;
	int j;
	int order = 2; // used for marking the point of exit from a loop
	int oldSize = input.getNumRows();
	int newSize = timeScale.getNumRows();

	// resizable datastructure to hold the divided differences table
	vector<cepMatrix <double> * > diffs;

	// vector to hold error estimation values
	vector<double> errors;

	double errorMod;	// interim value for calculating errors
	double errorPos; // Position on the curve used for approximating the error

	// calculate first colum of divided differences table

	diffs.push_back(new cepMatrix<double>(oldSize-1,1));
	for (j = 0; j <= oldSize - 1; j++)
	{
		diffs[0]->setValue(j,0, (input.getValue(j+1,1)-input.getValue(j,1))/
							(input.getValue(j+1,0) - input.getValue(j,0)));
	}
	// calculate error approximation for first value
	errorPos = (input.getValue(0,0)+input.getValue(1,0))*0.5;
	errorMod = errorPos-input.getValue(0,0);
	errors.push_back(errorMod * diffs[0]->getValue(0,0));



	// calculate rest of divided differences table
	for (i = 2; i < oldSize; i++)
	{
		order = i;
		diffs.push_back(new cepMatrix<double> (oldSize-i,1));
		for (j = 0; j <= oldSize - i; j++)
		{
			diffs[i-1]->setValue(j,0, (diffs[i-2]->getValue(j+1,0)-diffs[i-2]->getValue(j,0))/
								(input.getValue(j+1,0) - input.getValue(j,0)));
		}
		// calculate error for order i-2 divided difference table
		errorMod = errorMod * (errorPos - input.getValue(i-1,0));
		errors.push_back(errorMod * diffs[i-1]->getValue(0,0));

		// check error limits for early exit
		if (errors[i-1] > errors[i-2])
		{
			// trigger exit from for loop
			i = oldSize + 2;
		}
	}

	//if (i > oldSize) // if divided difference table loop exited early
	//{
	//	order = order - 1; // decrease order of approx by 1
	//}

	int position = 0; // position on old timeline (input)
	// loop to create table of answer values (timeScale(i,1)
	// see Gerald wheatley, Applied numerical analysis, pg 232 eq (3.6)
	for (i = 0; i < newSize; i++)
	{
		double tempValue;
		tempValue = 1.0;
		timeScale.setValue(i,1, input.getValue(position,0));
		for (j = 1; j < order; j++)
		{
			if (inBounds(input, timeScale, position, i, newSize, oldSize))
			{
				tempValue = tempValue * (timeScale.getValue(i,0) - input.getValue(position,0));
				timeScale.setValue(i,1, timeScale.getValue(i,1) + tempValue * diffs[j]->getValue(i,0));
			}
			else
			{
				for (int k = 0; k < order; k++)
					delete diffs[k];
			  // give error once I know how
				return timeScale;
			}
		}
	}
	for (int k = 0; k < order; k++)
		delete diffs[k];
	return timeScale;
}

bool cepInterp::inBounds(const cepMatrix<double> & input, cepMatrix<double> & timeScale,
								 int & position, int & i, int newSize, int oldSize)
{

	if (input.getValue(position,0) < timeScale.getValue(0,i))
	{
		if (position+1 >= oldSize)
		{
			while (i < newSize)
			{
				// fills remainder with repeat values
				// Remove this part before final version
				timeScale.setValue(i,1,timeScale.getValue(i-1,1)) ;
				i++;
			}
			return false;
		}
		else
		{
			position ++;
		}
	}

	return true;
}

void cepInterp::rowReduce(cepMatrix<double> & t, cepMatrix<double> & s, int n)
// Imports:
// 			t: a tridiagonal matix represented by a n-1 by 3 matrix
//			s: an n+1 long matrix vector
//			n: int size of matrices
// augment t with the middle n-1 values in s then row reduce
// this
// Exports:
//			t: identity matrix
//			s: real values for s
{
	int i;

	for (i = 1; i < n-1; i++)
	{
		// row divided by first element
		t.setValue(i-1,2, t.getValue(i-1,2)/t.getValue(i-1,1));
		s.setValue(i,0, s.getValue(i,0)/t.getValue(i-1,1));
		t.setValue(i-1,1, 0.0);

		// subtractraction to eliminate first value in next row
		t.setValue(i,1, t.getValue(i,1)-t.getValue(i-1,2)*t.getValue(i,0));
		s.setValue(i+1,0, s.getValue(i+1,0)- s.getValue(i,0)*t.getValue(i,0));
		t.setValue(i,0, 0.0);
	}
  // for last row
	// row divided by first element
	t.setValue(n-2,2, t.getValue(n-2,2)/t.getValue(n-2,1));
	s.setValue(n-1,0, s.getValue(n-1,0)/t.getValue(n-2,1));
	t.setValue(n-2,1, 0.0);

	// Work back up the matrix zeroing right most values in t
	for (i = n-1; i > 1; i--)
	{
		s.setValue(i-1,0, s.getValue(i-1,0) - s.getValue(i,0)*t.getValue(i-2,2));
		t.setValue(i-2,2, 0.0);
	}
}

void cepInterp::calc_abc(cepMatrix<double> & a,cepMatrix<double> & b,cepMatrix<double> & c,
							cepMatrix<double> & s,cepMatrix<double> & h,const cepMatrix<double> & input, int n)
{
	int i;

	for (i = 0; i < n; i++)
	{
		a.setValue(i,0, (s.getValue(i+1,0)-s.getValue(i,0))/(6.0*h.getValue(i,0)));
		b.setValue(i,0, s.getValue(i,0));
		c.setValue(i,0, (input.getValue(i+1,0)-input.getValue(i,0))/h.getValue(i,0) -
							(h.getValue(i,0)*(2.0*s.getValue(i,0) + s.getValue(i+1,0)))/6.0);
	}
}
