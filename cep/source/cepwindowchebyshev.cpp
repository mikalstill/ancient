
/*
 *   Imp for the cepwindowchebyshev.cpp
 *   Copyright (C) Blake Swadling                   2002
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
#include <math.h>
#include <vector>

using namespace std;

cepWindowChebyshev::cepWindowChebyshev( int size ) : cepWindowAlg( size ) {
  // set a default attenuation of 60dB
  setTransitionBandwidth( 0.05 );
  initCoeffs();
}


cepWindowChebyshev::~cepWindowChebyshev(){
  if( coeffs != NULL ) delete coeffs;
}

double cepWindowChebyshev::df = 0.0;

const cepError cepWindowChebyshev::setTransitionBandwidth(double tbw) {
  if( tbw <= 0 ) {
    return cepError("normalised transition bandwidth must be greater than Zero", cepError::sevWarning);
  } else if (tbw >0.499) {
    return cepError("normalised transition bandwidth must be less than 0.499", cepError::sevWarning);
  }
  df = tbw;
  cout << "transistion bandwidth set to " << df << endl;
  return cepError();
}

double cepWindowChebyshev::getTransitionBandwidth() {
  return df;
}

double cepWindowChebyshev::getValue( int offset )
{
  cout << "error: cepWindowChebyshev::getValue() should never be called?" << endl;
  return 0.0;
}

double cepWindowChebyshev::calcValue( int n ) {
  cout << "error: cepWindowChebyshev::calcValue() should never be called?" << endl;
  return 0.0;
}

typedef struct c {
  double real;
  double imag;
} complex;


/** This code is a direct port of an algorithm originally written on FORTRAN
 * Refer to "Programs for Digital Signal Processing", Section 5.2
 * &copy; The Institue of Electrical and Electronics Engineers, INC, 1979
 * edited by Digital Signal Processing Committee (IEEE Acoustics,
 * Speech, and Signal Processing Society)
 */
cepMatrix<double> *cepWindowChebyshev::generateCoeffs( int size ) {

#define PIE PI
#define TWOPI 2.0*PI
#define NF size
#define DF df
  bool even = (size%2==0);

  int XN=NF-1;
  double C0=cos(PIE*DF);
  double C1=XN*acosh(1.0/C0);
  double DP=1.0/(cosh(C1)-1);
  double X0 = (3-cos(TWOPI*DF))/(1+cos(TWOPI*DF));
  int N = 0;
  
  if( even ) {
    N=(NF/2);
  } else {
    N=(NF+1)/2;
  }

  double FNF = NF;
  double ALPHA=(X0+1)/2.0;
  double BETA=(X0-1)/2.0;
  double C2=XN/2.0;
  complex *vals = new complex[NF];

  for( int i=1; i<=NF; ++i ) {
    double XI = i-1;
    double F = (double)XI/FNF;
    double X = ALPHA*cos(TWOPI*F)+BETA;
    double P = 0.0;
    if( fabs(X) <= 1.0 ) {
      P = DP*cos(C2*acos(X));
    } else {
      P = DP*cosh(C2*acosh(X));
    }
    complex c;
    c.real = P;
    c.imag = 0.0;
    if( even ) {
      c.real = P*cos(PIE*F);
      c.imag = -P*sin(PIE*F);
      if( i > (NF/2.0+1) ) {
        c.real *= -1.0;
        c.imag *= -1.0;
      }
    }
    vals[i-1]=c;
  }

  double TWN = TWOPI/FNF;
  double *result = new double[(int)N];
  for( int i=1; i<=N; ++i ) {
    double XI = i-1.0;
    double SUM = 0.0;
    for( int j=1; j<=NF; ++j ) {
      double XJ=j-1.0;
      SUM = SUM + vals[j-1].real*cos(TWN*XJ*XI) + vals[j-1].imag*sin(TWN*XJ*XI);
    }
    if( SUM < 0 ) SUM = 0;
    result[i-1]=SUM;
  }

  //cout << "iFFT output"<<endl;
  C1 = result[0];
  for( int i=0; i<N; ++i ) {
    result[i] = result[i]/C1;
    //cout << i << " " << result[i] << endl;
  }

  cepMatrix<double> *foo = new cepMatrix<double>(size,1);

  // TODO Blake
  // i think there is an issue here.
  // i still get a small dip in the peak and the coeffs appear not to be symmetrical
  for( int i=0; i<N; ++i ) {
    foo->setValue(i,0,result[N-i-1]);
    if( i>=size-i ) cout << "Error! overlap while populating coeff matrix" << endl;
    foo->setValue(size-i-1,0,result[N-i-1]);
  }
  /*
  cout << "Resulting coeffs" << endl;
  for( int i=0; i<foo->getNumRows(); i++ ) {
    cout << i << " " << foo->getValue(i,0) << endl;
  }
  */
  return foo;
}
