
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

// The following #define macro are for trapping errors detected during matix accesses
// CHECK_ERROR() checks if an error has occurred in a matrix
// CHECK_ERROR_P() does the same but to a matrix pointer
// Both macros cause an error to be logged then halt the interpolation
// It is up to the parent function to check for this error
// Yeah I know this error trapping method is EXPLETIVE DETLETED
#define CHECK_ERROR(MATRIX) {if (MATRIX.getError().isReal()) {cout << "CRASH!!!\n";m_error.setError(MATRIX.getError().getMessage(),cepError::sevErrorRecoverable);return timeScale;}}
#define CHECK_ERROR_P(MATRIX) {if (MATRIX->getError().isReal()){cout << "CRASH!!!\n";m_error.setError(MATRIX->getError().getMessage(),cepError::sevErrorRecoverable); return timeScale;}}


cepInterp::cepInterp()
{
  delta = 0.00001;
}

cepMatrix<double> cepInterp::doInterp(cepMatrix<double> & input, double sampleRate,
												int interpType, int winSize, double winOverlap)
{
	/* winLength is the length of a window minus overlap on start edge*/
	int winLength = (int)(winSize * (1-winOverlap));
  
  // convert the starting matrix to the Julian timeScale
  cepMatrix<double> julianInput(input);
  for (int i = 0; i < julianInput.getNumRows(); i++)
  {
    julianInput.setValue(i,0, yearsToJulian(julianInput.getValue(i,0)));
  }

	/* calculate number of required samples after interpolation*/
	int numSamples;

	numSamples = (int)((julianInput.getValue(julianInput.getNumRows()-1,0) -
                    julianInput.getValue(0,0))/sampleRate)+1;
	numSamples = (int)(numSamples-(numSamples-winSize)%winLength);

	/* build new timescale*/
	cepMatrix<double> timeScale(numSamples,4);
	timeScale.setValue(0,0,julianInput.getValue(0,0));
  CHECK_ERROR(input);
  CHECK_ERROR(timeScale);
	for (int i = 1; i < numSamples; i++)
	{
		timeScale.setValue(i,0,timeScale.getValue(i-1,0)+sampleRate);
    CHECK_ERROR(timeScale);
	}
	for (int i = 0; i < numSamples; i++)
	{
		timeScale.setValue(i,2, 0.0);
    CHECK_ERROR(timeScale);
		timeScale.setValue(i,3, 0.0);
    CHECK_ERROR(timeScale);
	}



/*	for (int i=0; i < timeScale.getNumRows(); i++)
	{
		for (int j=0; j < timeScale.getNumCols(); j++)
			cout << timeScale.getValue(i,j);
		cout << '\n';
	}
*/
	/* do interpolation and return results */
  doInterp(julianInput,timeScale,interpType);
  for(int i = 0; i < timeScale.getNumRows(); i++)
  {
    timeScale.setValue(i,0, julianToYears(timeScale.getValue(i,0)));
  }

	return timeScale;
}

cepMatrix<double> cepInterp::doInterp(cepMatrix<double> & input,
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

	cepMatrix<double> cepInterp::nearestInterp(cepMatrix<double> & input,
												cepMatrix<double> & timeScale)
{
	int newSize = timeScale.getNumRows();
  CHECK_ERROR(timeScale);
	int oldSize = input.getNumRows();
  CHECK_ERROR(input);

	double distA, distB; // distance from adjacent points

	int position = 0; // position on old timeline
	// fill each value on new timeline = nearst old value;
	for (int i = 0; i < newSize; i++)
	{
		if (inBounds(input, timeScale, position, i, newSize, oldSize))
		{
			distA = timeScale.getValue(i,0) - input.getValue(position,0);
      CHECK_ERROR(timeScale);
      CHECK_ERROR(input);
			distB = input.getValue(position+1,0) - timeScale.getValue(i,0);
      CHECK_ERROR(timeScale);
      CHECK_ERROR(input);
			if (distA < distB)
      {
        if (fabs(distA) < delta)
          timeScale.setValue(i,3, 0.0);
        else
          timeScale.setValue(i,3, 1.0);
				timeScale.setValue(i,1,input.getValue(position,1));
        CHECK_ERROR(timeScale);
        CHECK_ERROR(input);
      }
			else
      {
        if (fabs(distB) < delta)
          timeScale.setValue(i,3, 0.0);
        else
          timeScale.setValue(i,3, 1.0);
				timeScale.setValue(i,1,input.getValue(position+1,1));
        CHECK_ERROR(timeScale);
        CHECK_ERROR(input);
      }
		}
		else
		{
			return timeScale;
		}
	}
	return timeScale;
}

	cepMatrix<double> cepInterp::linearInterp(cepMatrix<double> & input,
												cepMatrix<double> & timeScale)
{
	int newSize = timeScale.getNumRows();
  CHECK_ERROR(timeScale);
	int oldSize = input.getNumRows();
  CHECK_ERROR(input);

	double distDate, distValue;

	int position = 0;

	for (int i = 0; i < newSize; i++)
	{
		if (inBounds(input, timeScale, position, i, newSize, oldSize))
		{
      if ((timeScale.getValue(i,0) > input.getValue(position,0) - delta)&&
          (timeScale.getValue(i,0) < input.getValue(position,0) + delta))
      {
        CHECK_ERROR(timeScale);
        CHECK_ERROR(input);
        timeScale.setValue(i,1, input.getValue(position,1));
        CHECK_ERROR(timeScale);
        CHECK_ERROR(input);
        timeScale.setValue(i,3, 0.0);
        CHECK_ERROR(timeScale);
      }
      else if ((timeScale.getValue(i,0) > input.getValue(position+1,0) - delta)&&
               (timeScale.getValue(i,0) < input.getValue(position+1,0) + delta))
      {
        CHECK_ERROR(timeScale);
        CHECK_ERROR(input);
        timeScale.setValue(i,1, input.getValue(position+1,1));
        CHECK_ERROR(timeScale);
        CHECK_ERROR(input);
        timeScale.setValue(i,3, 0.0);
        CHECK_ERROR(timeScale);
      }
      else
      {
        CHECK_ERROR(timeScale);
        CHECK_ERROR(input);
			  distDate = input.getValue(position+1,0)-input.getValue(position,0);
        CHECK_ERROR(input);
			  distValue = input.getValue(position+1,1)-input.getValue(position,1);
        CHECK_ERROR(input);
			  timeScale.setValue(i,1,input.getValue(position,1)+distValue/distDate*
												  (timeScale.getValue(i,0)-input.getValue(position,0)));
        CHECK_ERROR(timeScale);
        CHECK_ERROR(input);
        timeScale.setValue(i,3, 1.0);
      }
		}
		else
		{
			return timeScale;
		}
	}


	return timeScale;

}

	cepMatrix<double> cepInterp::naturalSplineInterp(cepMatrix<double> & input,
												cepMatrix<double> & timeScale)
{
	int i;
	int newSize = timeScale.getNumRows();
  CHECK_ERROR(timeScale);
	int oldSize = input.getNumRows();
  CHECK_ERROR(input);
	int n = oldSize - 1;

	// a cubic spline describes a line by drawing a cubic between each 2
	// adjacent points and then matching the first and second derivatives of
	// the cubics
	// a, b, c and d are set of parameters for each of these cubics such that
	// a(x-n)^3+b(x-n)^2+c(x-n) + d = line approximation between 2 points
	cepMatrix<double> a(n,1);
  CHECK_ERROR(a);
	cepMatrix<double> b(n,1);
  CHECK_ERROR(b);
	cepMatrix<double> c(n,1);
  CHECK_ERROR(c);
	cepMatrix<double> d(n,1);
  CHECK_ERROR(d);

	// h describes the disance accross intervals
	// h[i] = (x[i+1] - x[i]) - set later in loop
	cepMatrix<double> h(n,1);
  CHECK_ERROR(h);

	// S describes the second derivatives (too complex to describe here)
	cepMatrix<double> s(n+1,1);
  CHECK_ERROR(s);

	// t is a temporary cooefficent matrix used for intermediate calculations
	// uses a n-1 by 3 matrix to simulate a n-1 by n-1 tridiagonal matrix.
	cepMatrix<double> t(n-1,3);
  CHECK_ERROR(t);

	for (i = 0; i < n; i++)
	{
		// d is set to the height value at the start of each interval
		d.setValue(i,0,input.getValue(i,1));
    CHECK_ERROR(d);
    CHECK_ERROR(input);
		h.setValue(i,0,input.getValue(i+1,0)-input.getValue(i,0));
    CHECK_ERROR(h);
    CHECK_ERROR(input);
	}

	// because this is a natural spline the second derivatives
	// of the end points are set to zero
	s.setValue(0,0,0.0);
  CHECK_ERROR(s);
	s.setValue(n,0,0.0);
  CHECK_ERROR(s);

	// set initial coefficent matrix and place temp values in the
	// middle n-1 values of s

	// initialise first row
	t.setValue(0,1, 2.0*(h.getValue(0,0)+h.getValue(1,0)));
  CHECK_ERROR(t);
  CHECK_ERROR(h);
	t.setValue(0,2, h.getValue(1,0));
  CHECK_ERROR(t);
  CHECK_ERROR(h);
	s.setValue(1,0, 6.0*((input.getValue(2,1)-input.getValue(1,1))/h.getValue(1,0) -
										 (input.getValue(1,1)-input.getValue(0,1))/h.getValue(0,0)));
  CHECK_ERROR(s);
  CHECK_ERROR(input);
  CHECK_ERROR(h);
	// initialise middle rows
	for (i = 2; i < n-1; i++)
	{
		t.setValue(i-1,0, h.getValue(i-1,0));
    CHECK_ERROR(t);
    CHECK_ERROR(h);
		t.setValue(i-1,1, 2.0*(h.getValue(i-1,0)+h.getValue(i,0)));
    CHECK_ERROR(t);
    CHECK_ERROR(h);
		t.setValue(i-1,2, h.getValue(i,0));
    CHECK_ERROR(t);
    CHECK_ERROR(h);
		s.setValue(i,0, 6.0*((input.getValue(i+1,1)-input.getValue(i,1))/h.getValue(i,0) -
											 (input.getValue(i,1)-input.getValue(i-1,1))/h.getValue(i-1,0)));
    CHECK_ERROR(s);
    CHECK_ERROR(input);
    CHECK_ERROR(h);
	}
	// initialise last row
	t.setValue(n-2,0, h.getValue(n-2,0));
  CHECK_ERROR(t);
  CHECK_ERROR(h);
	t.setValue(n-2,1, 2.0*(h.getValue(n-2,0)+h.getValue(n-1,0)));
  CHECK_ERROR(t);
  CHECK_ERROR(h);
	s.setValue(n-1,0, 6.0*((input.getValue(n,1)-input.getValue(n-1,1))/h.getValue(n-1,0) -
											 (input.getValue(n-1,1)-input.getValue(n-2,1))/h.getValue(n-2,0)));
  CHECK_ERROR(s);
  CHECK_ERROR(input);
  CHECK_ERROR(h);

	// augment t with s then row reduce
	rowReduce(t,s,n);
  CHECK_ERROR(t);
  CHECK_ERROR(s);

	// use values of s to calculate a, b and c
	calc_abc(a,b,c,s,h,input,n);
  CHECK_ERROR(a);
  CHECK_ERROR(b);
  CHECK_ERROR(c);
  CHECK_ERROR(s);
  CHECK_ERROR(h);
  CHECK_ERROR(input);

	// use a,b,c and d to interpolate the data to the new timescale
	int position = 0;
	for (i = 0; i < newSize; i++)
	{
		if (inBounds(input, timeScale, position, i, newSize, oldSize))
		{
      if ((timeScale.getValue(i,0) > input.getValue(position,0) - delta)&&
          (timeScale.getValue(i,0) < input.getValue(position,0) + delta))
      {
        CHECK_ERROR(timeScale);
        CHECK_ERROR(input);
        timeScale.setValue(i,1, input.getValue(position,1));
        CHECK_ERROR(timeScale);
        CHECK_ERROR(input);
        timeScale.setValue(i,3, 0.0);
        CHECK_ERROR(timeScale);
      }
      else if ((timeScale.getValue(i,0) > input.getValue(position+1,0) - delta)&&
               (timeScale.getValue(i,0) < input.getValue(position+1,0) + delta))
      {
        CHECK_ERROR(timeScale);
        CHECK_ERROR(input);
        timeScale.setValue(i,1, input.getValue(position+1,1));
        CHECK_ERROR(timeScale);
        CHECK_ERROR(input);
        timeScale.setValue(i,3, 0.0);
        CHECK_ERROR(timeScale);
      }
      else
      {
        CHECK_ERROR(timeScale);
        CHECK_ERROR(input);
			  timeScale.setValue(i,1,
			        a.getValue(position,0)*pow((timeScale.getValue(i,0)-input.getValue(position,0)),3.0)+
						  b.getValue(position,0)*pow((timeScale.getValue(i,0)-input.getValue(position,0)),2.0)+
						  c.getValue(position,0)*(timeScale.getValue(i,0)-input.getValue(position,0))+
						  d.getValue(position,0));
        CHECK_ERROR(timeScale);
        CHECK_ERROR(input);
        CHECK_ERROR(a);
        CHECK_ERROR(b);
        CHECK_ERROR(c);
        CHECK_ERROR(d);
        timeScale.setValue(i,3, 1.0);
        CHECK_ERROR(timeScale);
      }
		}
		else
		{
      cout << "Out of bounds error!!!!\n";
		  // give error once I know how
			return timeScale;
		}
	}
	return timeScale;

}

cepMatrix<double> cepInterp::cubicSplineInterp(cepMatrix<double> & input,
												cepMatrix<double> & timeScale)
{
	int i;
	int newSize = timeScale.getNumRows();
  CHECK_ERROR(timeScale);
	int oldSize = input.getNumRows();
  CHECK_ERROR(input);
	int n = oldSize - 1;

	// a cubic spline describes a line by drawing a cubic between each 2
	// adjacent points and then matching the first and second derivatives of
	// the cubics
	// a, b, c and d are set of parameters for each of these cubics such that
	// a(x-n)^3+b(x-n)^2+c(x-n) + d = line approximation between 2 points
	cepMatrix<double> a(n,1);
  CHECK_ERROR(a);
	cepMatrix<double> b(n,1);
  CHECK_ERROR(b);
	cepMatrix<double> c(n,1);
  CHECK_ERROR(c);
	cepMatrix<double> d(n,1);
  CHECK_ERROR(d);

	// h describes the disance accross intervals
	// h[i] = (x[i+1] - x[i]) - set later in loop
	cepMatrix<double> h(n,1);
  CHECK_ERROR(h);

	// S describes the second derivatives (too complex to describe here)
	cepMatrix<double> s(n+1,1);
  CHECK_ERROR(s);

	// t is a temporary cooefficent matrix used for intermediate calculations
	// uses a n-1 by 3 matrix to simulate a n-1 by n-1 tridiagonal matrix.
	cepMatrix<double> t(n-1,3);
  CHECK_ERROR(t);

	for (i = 0; i < n; i++)
	{
		// d is set to the height value at the start of each interval
		d.setValue(i,0,input.getValue(i,1));
    CHECK_ERROR(d);
    CHECK_ERROR(input);
		h.setValue(i,0,input.getValue(i+1,0)-input.getValue(i,0));
    CHECK_ERROR(h);
    CHECK_ERROR(input);
	}


	// set initial coefficent matrix and place temp values in the
	// middle n-1 values of s

	// initialise first row
	t.setValue(0,1, 3*h.getValue(0,0)+2*h.getValue(1,0));
  CHECK_ERROR(t);
  CHECK_ERROR(h);
	t.setValue(0,2, h.getValue(1,0));
  CHECK_ERROR(t);
  CHECK_ERROR(h);
	s.setValue(1,0, 6.0*((input.getValue(2,1)-input.getValue(1,1))/h.getValue(1,0) -
										 (input.getValue(1,1)-input.getValue(0,1))/h.getValue(0,0)));
  CHECK_ERROR(s);
  CHECK_ERROR(input);
  CHECK_ERROR(h);
	// initialise middle rows
	for (i = 2; i < n-1; i++)
	{
		t.setValue(i-1,0, h.getValue(i-1,0));
    CHECK_ERROR(t);
    CHECK_ERROR(h);
		t.setValue(i-1,1, 2.0*(h.getValue(i-1,0)+h.getValue(i,0)));
    CHECK_ERROR(t);
    CHECK_ERROR(h);
		t.setValue(i-1,2, h.getValue(i,0));
    CHECK_ERROR(t);
    CHECK_ERROR(h);
		s.setValue(i,0, 6.0*((input.getValue(i+1,1)-input.getValue(i,1))/h.getValue(i,0) -
											 (input.getValue(i,1)-input.getValue(i-1,1))/h.getValue(i-1,0)));
    CHECK_ERROR(s);
    CHECK_ERROR(input);
    CHECK_ERROR(h);
	}
	// initialise last row
		t.setValue(n-2,0, h.getValue(n-2,0));
    CHECK_ERROR(t);
    CHECK_ERROR(h);
		t.setValue(n-2,1, 3*h.getValue(n-2,0)+2*h.getValue(n-1,0));
    CHECK_ERROR(t);
    CHECK_ERROR(h);
		s.setValue(n-1,0, 6.0*((input.getValue(n,1)-input.getValue(n-1,1))/h.getValue(n-1,0) -
											 (input.getValue(n-1,1)-input.getValue(n-2,1))/h.getValue(n-2,0)));
    CHECK_ERROR(s);
    CHECK_ERROR(input);
    CHECK_ERROR(h);

	// because of the spline type the second derivatives
	// of the end points are set equal to the adjacent points
	s.setValue(0,0, s.getValue(1,0));
  CHECK_ERROR(s);
	s.setValue(n,0, s.getValue(n-1,0));
  CHECK_ERROR(s);

	// augment t with s then row reduce
	rowReduce(t,s,n);
  CHECK_ERROR(t);
  CHECK_ERROR(s);

	// use values of s to calculate a, b and c
	calc_abc(a,b,c,s,h,input,n);
  CHECK_ERROR(a);
  CHECK_ERROR(b);
  CHECK_ERROR(c);
  CHECK_ERROR(s);
  CHECK_ERROR(h);
  CHECK_ERROR(input);

	// use a,b,c and d to interpolate the data to the new timescale
	int position = 0;
	for (i = 0; i < newSize; i++)
	{
		if (inBounds(input, timeScale, position, i, newSize, oldSize))
		{
      if ((timeScale.getValue(i,0) > input.getValue(position,0) - delta)&&
          (timeScale.getValue(i,0) < input.getValue(position,0) + delta))
      {
        CHECK_ERROR(timeScale);
        CHECK_ERROR(input);
        timeScale.setValue(i,1, input.getValue(position,1));
        CHECK_ERROR(timeScale);
        CHECK_ERROR(input);
        timeScale.setValue(i,3, 0.0);
        CHECK_ERROR(timeScale);
      }
      else if ((timeScale.getValue(i,0) > input.getValue(position+1,0) - delta)&&
               (timeScale.getValue(i,0) < input.getValue(position+1,0) + delta))
      {
        CHECK_ERROR(timeScale);
        CHECK_ERROR(input);
        timeScale.setValue(i,1, input.getValue(position+1,1));
        CHECK_ERROR(timeScale);
        CHECK_ERROR(input);
        timeScale.setValue(i,3, 0.0);
        CHECK_ERROR(timeScale);
      }
      else
      {
        CHECK_ERROR(input);
        CHECK_ERROR(timeScale);
			  timeScale.setValue(i,1,
								  a.getValue(position,0)*pow((timeScale.getValue(i,0)-input.getValue(position,0)),3.0)+
								  b.getValue(position,0)*pow((timeScale.getValue(i,0)-input.getValue(position,0)),2.0)+
								  c.getValue(position,0)*(timeScale.getValue(i,0)-input.getValue(position,0))+
								  d.getValue(position,0));
        CHECK_ERROR(timeScale);
        CHECK_ERROR(input);
        CHECK_ERROR(a);
        CHECK_ERROR(b);
        CHECK_ERROR(c);
        CHECK_ERROR(d);
        timeScale.setValue(i,3, 1.0);
        CHECK_ERROR(timeScale);
      }
		}
		else
		{
      cout << "Out of bounds error!!!!\n";
		  // give error once I know how
			return timeScale;
		}
	}
	return timeScale;
}


cepMatrix<double> cepInterp::dividedInterp(cepMatrix<double> & input,
												cepMatrix<double> & timeScale)
{
	int i;
	int j;
	int order = 2; // used for marking the point of exit from a loop
	int oldSize = input.getNumRows();
  CHECK_ERROR(input);
	int newSize = timeScale.getNumRows();
  cout << "NewSize:" << newSize << "\n";
  CHECK_ERROR(timeScale);

	// resizable datastructure to hold the divided differences table
	vector<cepMatrix <double> * > diffs;

	// vector to hold error estimation values
	vector<double> errors;

	double errorMod;	// interim value for calculating errors
	double errorPos; // Position on the curve used for approximating the error

	// calculate first colum of divided differences table

	diffs.push_back(new cepMatrix<double>(oldSize-1,1));
  CHECK_ERROR_P(diffs[0]);


	for (j = 0; j < oldSize - 1; j++)
	{
		diffs[0]->setValue(j,0, (input.getValue(j+1,1)-input.getValue(j,1))/
							(input.getValue(j+1,0) - input.getValue(j,0)));
    CHECK_ERROR_P(diffs[0]);
    CHECK_ERROR(input);
	}
	// calculate error approximation for first value
	errorPos = (input.getValue(0,0)+input.getValue(1,0))*0.5;
  CHECK_ERROR(input);

	errorMod = (errorPos-input.getValue(0,0));
  CHECK_ERROR(input);

  cout << "Errors: \n";
	errors.push_back( errorMod * diffs[0]->getValue(0,0));
  cout << errors[0] << '\n';
  CHECK_ERROR_P(diffs[0]);


	// calculate rest of divided differences table
	for (i = 2; i < oldSize; i++)
	{
    double sum;
    sum = 0.0;
		order = i;
		diffs.push_back(new cepMatrix<double> (oldSize-i,1));
    CHECK_ERROR_P(diffs[i-1]);
		for (j = 0; j < oldSize - i; j++)
		{
			diffs[i-1]->setValue(j,0, (diffs[i-2]->getValue(j+1,0)-diffs[i-2]->getValue(j,0))/
								(input.getValue(j+i,0) - input.getValue(j,0)));
      CHECK_ERROR_P(diffs[i-1]);
      CHECK_ERROR_P(diffs[i-2]);
      CHECK_ERROR(input);
//      sum += fabs(diffs[i-1]->getValue(j,0));
      CHECK_ERROR_P(diffs[i-1]);
		}

		// calculate error for order i-2 divided difference table
		errorMod = errorMod * (errorPos - input.getValue(i-1,0));
    CHECK_ERROR(input);
		errors.push_back(errorMod * diffs[i-1]->getValue(0,0));
    cout << errors[i-1] << '\n';
    CHECK_ERROR_P(diffs[i-1]);

		// check error limits for early exit
		if ((pow(errors[i-1],2) > pow(errors[i-2],2)))
		{
			// trigger exit from for loop
			i = oldSize + 2;
		}
    // check diff table hasn't zeroed out
    for (j = 0; j < oldSize - i; j++)
    {
      if (diffs[i-1]->getValue(j,0)==0)
        sum++;
    }
    if (sum == oldSize-i)
      i = oldSize+2;
	}

	if (i > oldSize) // if divided difference table loop exited early
	{
		order = order - 1; // decrease order of approx by 2
    cout << "order:" << order << '\n';
    cout << "OldSize:" << oldSize << '\n';
	}

	int position = 0; // position on old timeline (input)
	// loop to create table of answer values (timeScale(i,1)
	// see Gerald wheatley, Applied numerical analysis, pg 232 eq (3.6)
	for (i = 0; i < newSize; i++)
	{
		double tempValue;
		tempValue = 1.0;
		if (inBounds(input, timeScale, position, i, newSize, oldSize))
		{
      CHECK_ERROR(input);
      CHECK_ERROR(timeScale);
  		timeScale.setValue(i,1, input.getValue(position,1));
      CHECK_ERROR(input);
      CHECK_ERROR(timeScale);
    }
		else
		{
			for (int k = 0; k < order; k++)
				delete diffs[k];
        cout << "Out of range error!!!\n";
        cout << "i:" << i << " pos:" << position << '\n';
		  // give error once I know how
			return timeScale;
		}
    if ((timeScale.getValue(i,0) > input.getValue(position,0) - delta)&&
        (timeScale.getValue(i,0) < input.getValue(position,0) + delta))
    {
      CHECK_ERROR(timeScale);
      CHECK_ERROR(input);
      timeScale.setValue(i,1, input.getValue(position,1));
      CHECK_ERROR(timeScale);
      CHECK_ERROR(input);
      timeScale.setValue(i,3, 0.0);
      CHECK_ERROR(timeScale);
    }
    else if ((timeScale.getValue(i,0) > input.getValue(position+1,0) - delta)&&
             (timeScale.getValue(i,0) < input.getValue(position+1,0) + delta))
    {
      CHECK_ERROR(timeScale);
      CHECK_ERROR(input);
      timeScale.setValue(i,1, input.getValue(position+1,1));
      CHECK_ERROR(timeScale);
      CHECK_ERROR(input);
      timeScale.setValue(i,3, 0.0);
      CHECK_ERROR(timeScale);
    }
    else
    {
      timeScale.setValue(i,3, 1.0);
      CHECK_ERROR(timeScale);
  		for (j = 0; j < order; j++)
  		{
  //      cout << "i:" << i << " pos:" << position << " j:" << j << '\n';
        if (position + order < oldSize)
        {
//        cout << "if 1\n";
          tempValue *= (timeScale.getValue(i,0) - input.getValue(position + j,0));
          CHECK_ERROR(timeScale);
          CHECK_ERROR(input);
          timeScale.setValue(i,1, timeScale.getValue(i,1) + tempValue * diffs[j]->getValue(position,0));
          CHECK_ERROR_P(diffs[j]);
          CHECK_ERROR(input);
          CHECK_ERROR(timeScale);
			  }
        else
        {
//        cout << "if 2\n";
  				tempValue *= (timeScale.getValue(i,0) - input.getValue(oldSize-(order+2) + j,0));
//        cout << "tempValue *= timeScale(" << i << ",0) - input("
//             << oldSize-(order+2)+j << ",0)\n"
//             << tempValue << " = " << timeScale.getValue(i,0) << "- "
//             << input.getValue(oldSize-(order+2) + j,0)<< '\n';

          CHECK_ERROR(timeScale);
          CHECK_ERROR(input);
				  timeScale.setValue(i,1, timeScale.getValue(i,1) + tempValue * diffs[j]->getValue(oldSize-(order+2),0));
//        cout << "TimeScale(" << i << "1) += tempValue * diffs[" << j
//             << "](" << oldSize-(order+2) << ",0)\n"
//             << timeScale.getValue(i,1) << "+="
//             << tempValue << " * " << diffs[j]->getValue(oldSize-(order+2),0) <<'\n';
          CHECK_ERROR_P(diffs[j]);
          CHECK_ERROR(timeScale);
	  		}
      }
		}
	}
	for (int k = 0; k < order; k++)
		delete diffs[k];
	return timeScale;
}

bool cepInterp::inBounds(cepMatrix<double> & input, cepMatrix<double> & timeScale,
								 int & position, int & i, int newSize, int oldSize)
{

	if ((position < oldSize -1) && input.getValue(position+1,0) < timeScale.getValue(i,0))
	{
		if (position+2 >= oldSize)
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
		t.setValue(i-1,1, 1.0);

		// subtraction to eliminate first value in next row
		t.setValue(i,1, t.getValue(i,1) - t.getValue(i-1,2)*t.getValue(i,0));
		s.setValue(i+1,0, s.getValue(i+1,0) - s.getValue(i,0)*t.getValue(i,0));
		t.setValue(i,0, 0.0);
	}
  // for last row
	// row divided by first element
	s.setValue(n-1,0, s.getValue(n-1,0)/t.getValue(n-2,1));
	t.setValue(n-2,1, 1.0);

	// Work back up the matrix zeroing right most values in t
	for (i = n-1; i > 1; i--)
	{
		s.setValue(i-1,0, s.getValue(i-1,0) - s.getValue(i,0)*t.getValue(i-2,2));
		t.setValue(i-2,2, 0.0);
	}
}

void cepInterp::calc_abc(cepMatrix<double> & a,cepMatrix<double> & b,cepMatrix<double> & c,
							cepMatrix<double> & s,cepMatrix<double> & h,cepMatrix<double> & input, int n)
{
	int i;

	for (i = 0; i < n; i++)
	{
		a.setValue(i,0, (s.getValue(i+1,0)-s.getValue(i,0))/(6.0*h.getValue(i,0)));
		b.setValue(i,0, s.getValue(i,0)*0.5);
		c.setValue(i,0, (input.getValue(i+1,1)-input.getValue(i,1))/h.getValue(i,0) -
							(h.getValue(i,0)*(2.0*s.getValue(i,0) + s.getValue(i+1,0)))/6.0);
	}
}

void cepInterp::setColour(cepMatrix<double> & input, cepMatrix<double> & timeScale, int position, int i)
{
  double delta = 0.0001;
  if (((timeScale.getValue(i,0) < input.getValue(position,0)+delta) &&
        (timeScale.getValue(i,0) < input.getValue(position,0)-delta)) ||
        ((timeScale.getValue(i,0) < input.getValue(position+1,0)+delta) &&
        (timeScale.getValue(i,0) < input.getValue(position+1,0)-delta)))
  {
    timeScale.setValue(i,3, 1.0);
  }
  else
  {
    timeScale.setValue(i,3, 2.0);
  }
}

double cepInterp::yearsToJulian(double year)
{
  const int FEBRUARY = 1;
  int i, j;
  int leap_year, iyear, ijd, syr;
  double fract, days1, days2, fract1;

  int month[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
  days1 = 365.0;

  leap_year = (int)year%4; // leap_year = year mod 4
  if (leap_year == 0)
  {
    month[FEBRUARY]++; // increase the number of days in february
    days1 = days1 + 1.0;
  }

  // get decimal portion of year. ie 0.0027
  fract = year - (double)((int)year);
  // get whole number of years. ie 2002
  iyear = (int)year;

  // count how many day equal the required fraction
  days2 = 0.0;
  fract1 = days2/days1;
  // ask peter about the different end loop conditions
  for (i = 0; (i < 12)&&(fract1 < fract); i++)
  {
    for (j = 0; (j < month[i])&&(fract1 < fract);j++)
    {
      days2 = days2 + 1.0;
      fract1 = days2/days1;
    }
  }

  // calculate years since 1900
  syr = iyear - 1900;
  // calculate integer julian day
  ijd = syr*365 + (syr-1)/4 + (int)days2;


  return (double)ijd;
}

double cepInterp::julianToYears(double julian)
{
  int iyear,leap_year;
  double fract;

  // calculate number of whole years since 1900
  iyear = (int)(julian/365.25);
  julian = julian - (double)(iyear*365 + iyear/4);

  // calculate year fraction
  leap_year = iyear%4;
  if (leap_year == 0)
    fract = (julian-0.5)/366.0;
  else
    fract = (julian-0.5)/365.0;

  return 1900.0 + iyear + fract;
}

