
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

cepWindowChebyshev::cepWindowChebyshev( int size ) : cepWindowAlg( size ) {
  // set a default attenuation of 0.6
  setAttenuation( 0.60 );
}


cepWindowChebyshev::~cepWindowChebyshev(){
  if( coeffs != NULL ) delete coeffs;
}


void cepWindowChebyshev::setAttenuation(double att) {
  A = att;
}


double cepWindowChebyshev::getValue( int offset )
{
  return calcValue( offset );
}

void cepWindowChebyshev::preCalc()
{
    // beta calulation
    gamma = pow(10,(-A/20));
    beta = cosh(1/(size-1) * acosh(1/gamma));
}

double cepWindowChebyshev::calcValue( int n ) {
  if( size == 1 ) {
    return 1;
  } else {
      double x = beta*cos(PI*n/size);
      return computeCheb(x,size-1);
  }
}

double cepWindowChebyshev::computeCheb( double value, long order )
{
  if( value <= 1 ) {
    return cos(order*acos(value));
  } else {
    return cosh(order*acosh(value));
  }
}

const cepMatrix<double> & cepWindowChebyshev::getCoeffs() {
  double *p = new double[size];
  for( int k=0;k<size;k++) {
    p[k] = calcValue(k);
  }
  int M = (size+1)/2;

  // this does not compile as yet .. how do i use fft libs?
//  double *w = real(ifft(p));  // will be of size M
  // simply so it compiles
  double *w = NULL;
  
  if( coeffs == NULL ) {
    // for debugging
    cout << "cheb<getCoeffs> coeffs is null" << endl;
  } else {
    cout << "cheb<getCoeffs> coeffs rows:" << coeffs->getNumRows() << "cols:" << coeffs->getNumCols();
  }

  // take the ifft values and make the window. we need to amke sym data from asym values
  // the resulting window is constructed as follows
  //     p[M-1], .. p[1],1,p[1], .. p[M-1]

  if( w != NULL ) {
    for( int i=1; i<M; ++i ) {
      w[i] = p[M-i]/p[0];
      w[size-i]=p[M-i]/p[0];
    }
    w[M-1] = 1.0;
  }
  return *(new cepMatrix<double>());
  
}




