
/*
 *   Imp for the cepWindowAlg.cpp
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

#include "cepWindowAlg.h"
#include <cmath>

/** calculate a single global value of PI */
const double cepWindowAlg::PI = 2*asin(1);

cepWindowAlg::cepWindowAlg( int size )
{
  setSize( size );
}

cepWindowAlg::~cepWindowAlg() {
  setSize(0);
}

int cepWindowAlg::getSize() {
  return size;
}


void cepWindowAlg::setSize( int s )
{
  size = s;
}


void cepWindowAlg::init()
{
  // TODO BS - fix this.. it will leak, but it avoids the segfault
  // if( coeffs != NULL ) delete coeffs;
  coeffs = & const_cast<cepMatrix<double>&>(generateCoeffs( getSize() ));
}




const cepMatrix<double> & cepWindowAlg::getCoeffs()
{

  if( coeffs == NULL ) {
    cepMatrix<double> *foo = new cepMatrix<double>(0,0);
    return *foo;
  }
  return *coeffs;
}


const cepMatrix<double> & cepWindowAlg::generateCoeffs( int size ) {
  cepMatrix<double> *foo = new cepMatrix<double>(size, 1);
  for( int i=0; i<size; i++ ) {
    foo->setValue(i,0, getValue( i ));
  }
  return *foo;
}

