
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


const cepError cepWindowChebyshev::setTransitionBandwidth(double tbw) {
  if( tbw <= 0 ) {
    return cepError("normalised transition bandwidth must be greater than Zero");
  } else if (tbw >0.499) {
    return cepError("normalised transition bandwidth must be less than 0.499");
  }
  df = tbw;
  return cepError();
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

cepMatrix<double> *cepWindowChebyshev::generateCoeffs( int size ) {

#define PIE PI
#define TWOPI 2.0*PI
#define NF size
#define DF df
  bool even = (size%2==1);

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

  C1 = result[0];
  for( int i=0; i<N; ++i ) {
    result[i] = result[i]/C1;
  }

  cepMatrix<double> *foo = new cepMatrix<double>(size,1);

  for( int i=0; i<N; ++i ) {
    foo->setValue(i,0,result[N-i]);
    foo->setValue(size-i,0,result[N-i]);
  }

  for( int i=0; i<foo->getNumRows(); i++ ) {
    cout << i << " " << foo->getValue(i,0) << endl;
  }
  return foo;
}











/*
cepMatrix<double> *cepWindowChebyshev::generateCoeffs( int size ) {

  int M = (int)floor((double)size/2); // the half window size
  bool odd = (size%2 == 1);

  // calc the ripple
  double C0=cos(PI*df);
  double C1=(size-1.0)*acosh(1.0/C0);
  double dp = 1.0/(cosh(C1)-1.0);                         // the ripple in decibels
  double X0 = (3.0*cos(2.0*PI*df))/(1.0+cos(2.0*PI*df));  // the chebyshev window constant

  double alpha = (X0-1.0)/2.0;
  double beta  = (X0-1.0)/2.0;

  complex<double> *cplx = new complex<double>[size];

  for( int i=0; i<size; ++i ) {
    double f = (double)i/size;
    double x = alpha*cos(2.0*PI*(double)i)/(double)size + beta;
    double p = 0.0;
    if( abs(x) <= 1 ) {
      p = dp*cos(M*acos(x));
    } else {
      p = dp*cosh(M*acosh(x));
    }
    if( odd ) {
      cplx[i] = complex<double>( p, 0.0 );
    } else {
      cplx[i] = complex<double>( p*cos(PI*f), -p*sin(PI*f) );
      if( i > ((double)size/2+1) ) {
        cplx[i] = -1.0*cplx[i];
      }
    }
  }

  // the coefficient matrix
  cepMatrix<double> *foo = new cepMatrix<double>(size,1);

  // the ifft
  double sum = 0.0;
  double step = 2.0*PI/size;
  double *value = new double[M];
  for( int i=0; i<M; ++i ) {
    for( int j=0; j<size; ++j ) {
      sum += cplx[j].real()*cos(step*(double)j*(double)i) +
             cplx[j].imag()*sin(step*(double)j*(double)i);
    }
    // sum = (sum<0)?0.0:sum;
    value[i] = sum;
  }

  // clean up resources
  delete[] cplx;
  delete[] value;

  // normalise the values
  cout << "normalising" << endl;
  for( int i=0; i<M; i++ ) {
    value[i] = value[i]/value[0];
    cout << i << " " << value[i] << endl;
  }
  cout << endl << endl;

  for( int i=0; i<M; ++i ) {
    foo->setValue(i,1,value[i]);
    if( i >= size-i ) {
      cout << "passing the centre, aborting" << endl;
    }
  }

  return foo;
  
}
*/
