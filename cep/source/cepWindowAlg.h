
/*
 *   Imp for the cepWindowAlg.h
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

#ifndef CEPWINDOWALG_H
#define CEPWINDOWALG_H

#include "cepMatrix.h"


/**
  *@author Blake Swadling
  */

class cepWindowAlg {
public:
  /** constructs a new <code>cepWindowAlg</code>
   * @param size the window size to use
   */
	cepWindowAlg( int size );
  
  /** default destructor */
	virtual ~cepWindowAlg();
  
  /** modifies the size of the window. this will regenerate all coefficients
   * @param size the new window size
   */
  void setSize( int size );
  
  /** queries the current window size
   * @return the current window size
   */
  int getSize();
  
  /** queries the current coefficients
   * @return an N*1 matrix of the current coefficients
   */  
  const cepMatrix<double> & getCoeffs();

  
protected:
  /** the current window size */
  int size;
  
  /** a single global value of PI to be used in the windowing algorithm */
  static const double PI;
  
  /** the current coefficient matrix */
  cepMatrix<double> *coeffs;

  /** generates the coefficient matrix. calls upn implementation specific getvalue
   * @param size the desired window size
   * @return an N*1 matrix of the current coefficients
   */
  const cepMatrix<double> & generateCoeffs( int size );

  /** generates a coefficient at a specific offset into the coefficient matrix
   * override this to genetate specific coefficient sets
   */
  virtual double getValue( int offset ) = 0;
};

#endif
