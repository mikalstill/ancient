/*
 *   Imp for the cepwindowchebyshev.cpp
 *
 *   Copyright (C) Daniel Fernandez                 2002
 *   Copyright (C) Michael Still                    2002
 *   Copyright (C) Blake Swadling                   2002
 *   Copyright (C) Kristy Van Der Vlist             2002
 *   Copyright (C) Nick Wheatstone                  2002
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

#include "cepwindowchebyshev.h"
#include "cepConfiguration.h"
#include <math.h>
#include <vector>
#include <iomanip>

using namespace std;

cepWindowChebyshev::cepWindowChebyshev (int size):
cepWindowAlg (size)
{
  // set a default attenuation of 60dB
  initCoeffs ();
}


cepWindowChebyshev::~cepWindowChebyshev ()
{
  if (coeffs != NULL)
    delete coeffs;
}

double
  cepWindowChebyshev::df =
  0.0;

const
  cepError
cepWindowChebyshev::setTransitionBandwidth (double tbw)
{
  cepError err = setBandwidthValue (tbw);
  saveBandwidthValue ();
  return err;
}


void
cepWindowChebyshev::saveBandwidthValue ()
{
  cepDebugPrint ("chebyshev transition bandwidth is set to " +
		 cepToString (df));
  cepConfiguration::getInstance ().setValue (CONFIG_NAME_CHEB, df);
}

const cepError
cepWindowChebyshev::setBandwidthValue (double tbw)
{
  cepError err;
  if (tbw < 0.02)
    {
      cepDebugPrint ("rounding chebyshev transition bandwidth to 0.02");
      err =
	cepError ("rounding chebyshev transition bandwidth to 0.02",
		  cepError::sevWarning);
      tbw = 0.02;
    }
  else if (tbw > 0.499)
    {
      cepDebugPrint ("rounding chebyshev transition bandwidth to 0.499");
      err =
	cepError ("rounding chebyshev transition bandwidth to 0.02",
		  cepError::sevWarning);
      tbw = 0.499;
    }
  df = tbw;
  return err;
}


const double
cepWindowChebyshev::getTransitionBandwidth ()
{
  return df;
}

double
cepWindowChebyshev::getValue (int offset)
{
//  cout << "error: cepWindowChebyshev::getValue() should never be called?" << endl;
  return 0.0;
}

double
cepWindowChebyshev::calcValue (int n)
{
//  cout << "error: cepWindowChebyshev::calcValue() should never be called?" << endl;
  return 0.0;
}

typedef struct c
{
  double real;
  double imag;
}
complex;


/** This code is a direct port of an algorithm originally written on FORTRAN
 * Refer to "Programs for Digital Signal Processing", Section 5.2
 * &copy; The Institue of Electrical and Electronics Engineers, INC, 1979
 * edited by Digital Signal Processing Committee (IEEE Acoustics,
 * Speech, and Signal Processing Society)
 */
cepMatrix < double >*
cepWindowChebyshev::generateCoeffs (int size)
{

#define PIE 4*atan(1.0)
#define TWOPI 2.0*PIE
#define NF size
#define DF df
  bool even = (size % 2 == 0);

  int XN = NF - 1;
  double C0 = cos (PIE * DF);
  double C1 = XN * acosh (1.0 / C0);
  double DP = 1.0 / (cosh (C1) - 1);
  double X0 = (3 - cos (TWOPI * DF)) / (1 + cos (TWOPI * DF));
  int N = 0;

  if (even)
    {
      N = (NF / 2);
    }
  else
    {
      N = (NF + 1) / 2;
    }

  double FNF = NF;
  double ALPHA = (X0 + 1) / 2.0;
  double BETA = (X0 - 1) / 2.0;
  double C2 = XN / 2.0;
  complex *vals = new complex[NF];

  for (int i = 1; i <= NF; ++i)
    {
      double XI = i - 1;
      double F = (double) XI / FNF;
      double X = ALPHA * cos (TWOPI * F) + BETA;
      double P = 0.0;
      bool usedCosh = false;

      if (-1.0 <= X && X <= 1.0)
	{
	  P = DP * cos (C2 * acos (X));
//      cout << "  X=" << X << endl;
	  // a hack to see if this will fix it
	  if (isnan (P))
	    {
	      P = DP * cosh (C2 * acosh (X));
	    }
	}
      else
	{
	  P = DP * cosh (C2 * acosh (X));
	  usedCosh = true;
//      cout << "* X=" << X << endl;
	  // a hack to see if this will fix it
	  if (isnan (P))
	    {
	      P = DP * cos (C2 * acos (X));
	    }
	}
      complex c;
      c.real = P;
      c.imag = 0.0;
      if (even)
	{
	  c.real = P * cos (PIE * F);
	  c.imag = -P * sin (PIE * F);
	  if (i > (NF / 2.0 + 1))
	    {
	      c.real *= -1.0;
	      c.imag *= -1.0;
	    }
	}
      vals[i - 1] = c;
    }

  double TWN = TWOPI / FNF;
  double *result = new double[(int) N];
  for (int i = 1; i <= N; ++i)
    {
      double XI = i - 1.0;
      double SUM = 0.0;
      for (int j = 1; j <= NF; ++j)
	{
	  double XJ = j - 1.0;
	  SUM =
	    SUM + vals[j - 1].real * cos (TWN * XJ * XI) + vals[j -
								1].imag *
	    sin (TWN * XJ * XI);
	}
      if (SUM < 0)
	SUM = 0;
      result[i - 1] = SUM;
    }

  C1 = result[0];

  for (int i = 0; i < N; ++i)
    {
      result[i] = result[i] / C1;
    }

  cepMatrix < double >*foo = new cepMatrix < double >(size, 1);

  // TODO Blake
  // i think there is an issue here.
  // i still get a small dip in the peak and the coeffs appear not to be symmetrical
  for (int i = 0; i < N; ++i)
    {
      foo->setValue (i, 0, result[N - i - 1]);
      if (i >= size - i)
	cout << "Error! overlap while populating coeff matrix" << endl;
      foo->setValue (size - i - 1, 0, result[N - i - 1]);
    }

  return foo;
}

const string
cepWindowChebyshev::CONFIG_NAME_CHEB ("chebyshev-bandwidth");
