
/*
 *   Imp for the cepDataWindower.h
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
cepDataWindower

PURPOSE <!-- use cases -->
allows for the widowing of datasets using a variety of difference windowing algorithms

SYNOPSIS START
<!-- how to instantiate one? -->
default constuctor makes an unititialised windower. If this method is useed then
the wondower must be initialised using the <command>setWindowType<command> method
<command>cepDataWindower windower = new cepDataWindower();</command>

this method instantiates a windower that is ready to go
<command>cepDataWindower windower = new cepDataWindower( int type, int size, int overlap);</command>
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

#ifndef CEPDATAWINDOWER_H
#define CEPDATAWINDOWER_H

#include "cepDataset.h"
#include "cepError.h"
#include "cepMatrix.h"
#include "cepWindowAlg.h"

class cepWindow {
public:
  cepWindow( int id, const char* name );
  bool operator== (const cepWindow &w) const;
  bool operator!= (const cepWindow &w) const;
  const int id() const;
  const char* toString() const;
  
private:
  int myID;
  const char *str;
};

class cepDataWindower {
public:
  static const cepError setWindowType( const cepWindow &type, const int size, const int overlap );
  
  // att is the side lobe attenuation as required for dolph-chebyshev
  static const cepError setChebBandwidth( double dw );

  // windowedData is in/out param. numRows is the number of windows, numCols is the windows size
  // and the tird dimension is 2 (date and value)
  static const cepError window( const cepMatrix<double> & dataIn, cepMatrix<double> & windowedData );

  static void init();

  static const int getSize();
  static const int getOverlap();
  static const cepWindow lookupWindow( int id );

  static const cepWindow WINDOW_RECTANGULAR;
  static const cepWindow WINDOW_HAMMING;
  static const cepWindow WINDOW_HANNING;
  static const cepWindow WINDOW_BLACKMAN;
  static const cepWindow WINDOW_CHEBYSHEV;
  static const cepWindow WINDOW_UNDEFINED;
  
  static const string CONFIG_NAME_TYPE;
  static const string CONFIG_NAME_SIZE;
  static const string CONFIG_NAME_OVERLAP;
 
  static int countWindows( int samples, int winSize, int overlap );

protected:
  cepDataWindower();
  ~cepDataWindower();
  
  static cepWindowAlg *windowAlg;
  static int size;
  static int overlap;
  static cepWindow algType;
  
};


#endif
