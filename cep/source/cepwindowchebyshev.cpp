
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
  // set a default attenuation of 60dB
  setAttenuation( 0.5 );
  initCoeffs();
}


cepWindowChebyshev::~cepWindowChebyshev(){
  if( coeffs != NULL ) delete coeffs;
}


void cepWindowChebyshev::setAttenuation(double att) {
  dw = att;
}

double cepWindowChebyshev::getValue( int offset )
{ 
  return calcValue( offset );
}

double cepWindowChebyshev::calcValue( int n ) {
  cout << "error: why is this being called?" << endl;
  return 0.0;
}

double cepWindowChebyshev::computeCheb( double value, long order )
{
  if( value <= 1 ) {
    return cos((double)order*acos(value));
  } else {
    return cosh((double)order*acosh(value));
  }
}

// method: generateDolphChebyshev
//
// arguments:
//  VectorFloat& output: (output) the window function
//  const VectorFloat& params: (input) window function parameters
//  long num_points: (input) number of points in the window
//
// return: a boolean value indicating status
//
// this method generates a Dolph-Chebyshev window function:
//
//  w(n) = { G/(2M+1) * [1/gamma +
//         {             2 * sum Cheb(beta*k*PI/(2M+1)) *
//         {             cos(2*n*k*PI/(2M+1))]                  n <= |M|
//         { 0                                                  elsewhere
//
//  there are a couple of important auxiliary equations:
//
//   alpha_s = (2*M * 2.285 * (double)dw + 16.4) / 2.056
//
//  where dw is the transition bandwidth in normalized frequency.
//  using alpha_s, we compute gamma and beta (needed above):
//
//   gamma = 10 ** (-alpha_s / 20.0)
//   beta = cosh((1/2M) * acosh(1/gamma))
//
// a good reference for this window can be found at:
//
//  S.K. Mitra, Digital Signal Processing,
//  McGraw-Hill, Boston, Massachusetts, USA, 2001, pp. 456.
//
// there are two parameters allowed for this window:
//
//  params(0): G, the gain of the window
//  params(1): dw, the normalized transition bandwidth
//
// for an odd number of points, the window is defined as above.
// for an even number of points, an interpolation technique is used.
//
cepMatrix<double> *cepWindowChebyshev::generateCoeffs( int size ) {

  // compute some constants related to the length of the window
  //
  long N = size;
  long Nm1 = N - 1;
  long Nm2 = Nm1 - 1;
  double scale = (double)Nm2/Nm1;

  // compute the half-window length
  //
  long M = (N - 1) / 2;

  // create output space
  //
  cepMatrix<double> *output_a = new cepMatrix<double>(size,1);

  // in this approach, the constant represents the transition bandwidth.
  // the window length is supplied by the user. we compute the appropriate
  // constants to satisfy this relationship.
  //
  double A = (2*M * 2.285 * dw + 16.4) / 2.056;
  double gamma = pow(10.0, -A/20.0);
  double gamma_inv = 1.0 / gamma;

  // from gamma and M, we compute beta
  //
  double beta = cosh(acosh(gamma)/(double)(2*M));


  // for debugger visibility
  double arg1 = 0.0;
  double arg2 = 0.0;
  double val = 0.0;
  double sum = 0.0;
  
  // case 1: an odd number of points
  //
  if ( (N%2)!= 0) {
    for (int n = -M; n <= M; n++) {
      sum = 0.0;
      // compute the inner summation
      //
      for (int k = 1; k < M; k++) {
        arg1 = beta*(double)k * PI / (double)(N);
        arg2 = (2*N*(double)k * PI) / (M+1);
        val = computeCheb( arg1*cos(arg2), k);
        sum += val;
      }

      // save the output value
      //
      output_a->setValue( n+M, 0, ( (1.0/N)*(gamma_inv + 2*sum)));
    }
  }

  // case 2: an even number of points
  //  use a resampling strategy
  //
  else {
    for (long n = 0; n < N; n++) {
      double scaled_index = -M + scale * n;

      // compute the inner summation
      //
      double sum = 0;
      for (long k = 1; k < M; k++) {
        double arg1 = (double)k * PI / (double)(Nm1);
        double arg2 = 2 * scaled_index * arg1;
        double val = computeCheb(beta * cos(arg1), k);
        sum += val * cos(arg2);
      }

      // save the output value
      //
      output_a->setValue(n,0, (1.0 / (double)(Nm1) * (gamma_inv + 2 * sum)));
    }
  }

  // exit gracefully
  //
  return output_a;
}

