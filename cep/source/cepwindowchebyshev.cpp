
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
  init();
  G = 1;
  dw = 1;
}

cepWindowChebyshev::~cepWindowChebyshev(){
  if( coeffs != NULL ) delete coeffs;
}

void cepWindowChebyshev::setSize( int size ) {
  cepWindowAlg::setSize( size );
  preCalc();
  
}


void cepWindowChebyshev::setParams( double gain, double trans_bw ) {
  G = gain;
  dw = trans_bw;
}

/*
 * reference site for the dolph-chebyshev windowing algorithm
 * http://www.isip.msstate.edu/cgi/bin/ifc_document.pl?file=$isip/class/algo/Window/win_06.cc&method=generateDolphChebyshev
 *
 */
void cepWindowChebyshev::preCalc() {
  // precalc what we can ...
  N = size;
  M = (N-1)/2;

  PSLL_dB = (2*M * 2.285 * dw + 16.4) / 2.056;
  S = pow (10, (PSLL_dB/20) );

  beta = cosh( acosh(S)/(N-1) );
}



double cepWindowChebyshev::getValue( int offset )
{
  double sum = 0;
  for( int n=0; n<M; ++n ) {
    double nPI_N = n*PI/N;
    double arg1 = beta * cos(nPI_N);
    double arg2 = 2*nPI_N * (offset - (N+1)/2);
    
    double cheb = 0;
    if( arg1 <= 1 ) {
      cheb = cos((N-1)*acos(arg1));
    } else {
      cheb = cosh((N-1)*acosh(arg1));
    }

    sum += cheb*cos(arg2);
  }
  
  double result = 2/N * (S + 2*sum);
  cout << "result=" << result << endl;
  return result;
}




