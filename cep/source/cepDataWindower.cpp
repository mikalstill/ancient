
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
      
    case WINDOW_TRIANGULAR:    
    case WINDOW_HANNING:
    case WINDOW_KEISER:
    case WINDOW_TAYLOR:
    case WINDOW_CHEBYSHEV:
      return cepError("This type is not yet implemented. Set type failed", cepError::sevWarning);
      
    default:
      // leave the current settings alone and return an error
      return cepError("unknown windowing algorithm. Set type failed", cepError::sevWarning);
  }

  overlap=ol;
  return cepError();
  
}



const cepError cepDataWindower::setWindowGeom( const int s, const int ol ) {
  if( windowAlg == NULL ) {
    return cepError( "Window algorithm is not defined. Set geometry failed", cepError::sevWarning);
  }
  windowAlg->setSize( s );
  overlap = ol;
  return cepError();
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

  // need to cast away constness for this assignment
  int lastWinPtr = const_cast< cepMatrix<double>& >(dataIn).getNumRows() - windowAlg->getSize();
  
  // returns an N*1 matrix of scaling factors
  cepMatrix<double> coeffs = windowAlg->getCoeffs();
  
  for( int winPtr = 0; winPtr < lastWinPtr; winPtr += (windowAlg->getSize()-overlap) ) {
    cepMatrix<double> currWin( windowAlg->getSize(), 2 );
    
    for( int i=0; i<windowAlg->getSize(); i++ ) {
      // sanity check
      if( winPtr + i > const_cast< cepMatrix<double>& >(dataIn).getNumRows() ) {
        return cepError("Exceeded the bounds of the matrix");
      }
      
      // copy the date directly & scale the value
      currWin.setValue(i, 0,
              const_cast< cepMatrix<double>& >(dataIn).getValue( winPtr+i, 0 ));
      currWin.setValue(i, 1,
              const_cast< cepMatrix<double>& >(dataIn).getValue( winPtr+i, 1 )*coeffs.getValue(i,0));
    }

    // TODO BS - how are we to store this?  waiting for 3D MAtrix
    // for now return a single window !! this must be rectified
    windowedData = currWin;

  }

  return cepError();
  
  
}
