
/*
 *   Imp for the cepwindowchebyshev.h
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

/******************************************************************************
DOCBOOK START

FUNCTION <!-- class name -->
cepConfiguration

PURPOSE <!-- use cases -->
provides storage of name/value pairs

SYNOPSIS START
<!-- how to instantiate one? -->
cepConfiguration::getInstance();

SYNOPSIS END

DESCRIPTION START
<!-- description goes here -->
<para>description</para>
A dolph-chebyshev window. This window is very similar to hamming window,
except it provides better attenuation of high frequence noise

<para> <!-- per function descriptions -->
<command>cepWindowHamming( int size )</command>
Makes a new cepWindowHamming of a specific size.
</para>

<para> <!-- per function descriptions -->
<command>cepWindowHamming( int size )</command>
Makes a new cepWindowHamming of a specific size.
</para>

<para> <!-- per function descriptions -->
<command>getValue( int offset )</command>
gets the coefficient at a specified offset from the front of the window. This
will calculated according to the following equation.
If the window size is 1 then the coefficient will be 1, otherwise
<command>double result = 0.54 - 0.46*cos( 2*PI* offset/(size-1) );<command>
</para>

DESCRIPTION END

DOCBOOK END
******************************************************************************/

#ifndef CEPWINDOWCHEBYSHEV_H
#define CEPWINDOWCHEBYSHEV_H

#include "cepWindowAlg.h"
#include "cepMatrix.h"

/**
  * constructs an n point dolph-chebyshev filter with a specified attenuation in
  * the stop band of the corresponding fourier transform.
  * @author Blake Swadling
  */


class cepWindowChebyshev : public cepWindowAlg  {
public:
  cepWindowChebyshev( int size );  
  ~cepWindowChebyshev();
  
  static const cepError setTransitionBandwidth(double tbw);
  static const double getTransitionBandwidth();
  
  static const cepError setBandwidthValue(double tbw);
  static void saveBandwidthValue();

  static const string CONFIG_NAME_CHEB;
  
protected:
  /**
   * overload this as we need a specialised mechanism to make the coeffs
   */
  cepMatrix<double> *generateCoeffs( int size );
  double getValue( int offset );
  double calcValue( int n );

  

private:
  static double df;



};

#endif
