
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

#include <cepWindowAlg.h>

/**
  *@author Blake Swadling
  */

class cepWindowChebyshev : public cepWindowAlg  {
public: 
	cepWindowChebyshev( int size );
	~cepWindowChebyshev();
  
  void setParams( double gain, double trans_bw );
  void setSize( int size );
  
protected:
  double getValue( int offset );
  void preCalc();

private:
  double G;       // the gain
  double dw;      // normalised transition bandwidth;

  double S;
  double beta;
  
  long N;         // window size
  long M;         // the half window size

  double PSLL_dB; // the side lobe attenuation

};

#endif
