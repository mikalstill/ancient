
/*
 *   Imp for the cepDataWindower.cpp
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

#include "cepDataWindower.h"
#include "cepWindowRect.h"
#include "cepwindowhamming.h"
#include "cepwindowblackman.h"
#include "cepwindowchebyshev.h"



cepDataWindower::cepDataWindower() {
  windowAlg = NULL;
}



cepDataWindower::cepDataWindower( const windowType t, const int s, const int ol ){
  windowAlg = NULL;     // initialise in case the user supplies a bogus algorithm
  setWindowType( t, s, ol );
}



cepDataWindower::~cepDataWindower(){
  if( windowAlg != NULL ) delete windowAlg;
}



const cepError cepDataWindower::setWindowType( const windowType type, const int size, const int ol ) {
  
  switch( type ) {
    
    case WINDOW_RECTANGULAR:
      windowAlg = new cepWindowRect( size );
      break;
      
    case WINDOW_HAMMING:
      windowAlg = new cepWindowHamming( size );
      break;

    case WINDOW_BLACKMAN:
      windowAlg = new cepWindowBlackman( size );
      break;

    case WINDOW_CHEBYSHEV:
      windowAlg = new cepWindowChebyshev( size );
      break;
      
    case WINDOW_TRIANGULAR:    
    case WINDOW_HANNING:
    case WINDOW_KEISER:
    case WINDOW_TAYLOR:
      return cepError("This type is not yet implemented. Set type failed", cepError::sevWarning);
      
    default:
      // leave the current settings alone and return an error
      return cepError("unknown windowing algorithm. Set type failed", cepError::sevWarning);
  }

  overlap=ol;
  return cepError();
  
}



const cepError cepDataWindower::setWindowGeom( const int sz, const int ovrlap ) {
  if( windowAlg == NULL ) {
    return cepError( "Window algorithm is not defined. Set geometry failed", cepError::sevWarning);
  }
  windowAlg->setSize( sz );
  overlap = ovrlap;
  return cepError();
}



int cepDataWindower::countWindows( int samples, int winSize, int overlap )
{
  if( overlap == 0 ) return (int)((double)samples/winSize);
  return (int)((double)(samples-winSize)/(winSize - overlap) + 1);
}



const cepError cepDataWindower::window( const cepMatrix<double> & dataIn,
                                        cepMatrix<double> &windowedData ) {
  
  /*
   * TODO BS - if we have no windowing algorithm defined should we simply fail
   * or should we use rectangular?
   */
  if( windowAlg == NULL ) {
    return cepError( "Window algorithm is not defined. Windowing failed" );
  }
  int numSamples = const_cast<cepMatrix<double>&>(dataIn).getNumRows();
  int numWindows = countWindows( numSamples, windowAlg->getSize(), overlap );
  int increment = windowAlg->getSize() - overlap;

  // sanity checks
  if( windowAlg->getSize() > numSamples ) {
    return cepError("invalid window size specified. size must not be greater than the sample size");
  } else if( const_cast<cepMatrix<double>&>(dataIn).getNumCols() < 2 ) {
    return cepError("insufficient data to proceed. require at least 2 cols: date and sample");
  } else if( increment <= 0 ) {
    return cepError("invalid overlap specified. overlap must be less than the window size");
  }


  cepMatrix<double> coeffs = windowAlg->getCoeffs();                // scaling factors
  cepMatrix<double> result( numWindows, windowAlg->getSize(), 2 );  // result. numwindows x windowSize x 2
  
  for( int win = 0; win < numWindows; ++win ) {
    for( int element = 0; element < windowAlg->getSize(); ++element ) {

      // sanity check
      int ptr = (win*increment)+element;
      if( ptr > numSamples ) {
        return cepError("Exceeded the bounds of the matrix when accessing row"+ptr);
      }
      
      // copy the date directly & scale the value
      result.setValue(win, element, 0,
              const_cast< cepMatrix<double>& >(dataIn).getValue( ptr, 0 ));
      result.setValue(win, element, 1,
              const_cast< cepMatrix<double>& >(dataIn).getValue( ptr, 1 )*coeffs.getValue(element,0));
    }
  }

  windowedData = result;

  return cepError();
}
