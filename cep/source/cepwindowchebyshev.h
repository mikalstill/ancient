
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

<para> <!-- per function descriptions -->
<command>cepConfiguration.initialise(const string&amp; filename);</command>
initialises the configuration.<command>filename</command> defines the location
of the configuration file. If the file does not exist it will be created
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
  const cepError setTransitionBandwidth(double tbw);
  
protected:
  /**
   * overload this as we need a specialised mechanism to make the coeffs
   */
  cepMatrix<double> *generateCoeffs( int size );
  double getValue( int offset );
  double calcValue( int n );

  

private:
  double df;



};

#endif
