
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



Window::Window( int id, const char* name ) {
  myID = id;
  str = name;
}
bool Window::operator== (const Window &w) const {
  return (w.id() == myID);
}

bool Window::operator!=(const Window &w) const {
  return (w.id() != myID);
}
const int Window::id() const {
  return myID;
}
const char* Window::toString() const {
  return str;
}


cepDataWindower::cepDataWindower() {
  windowAlg = NULL;
  algType = WINDOW_UNDEFINED;
}


cepDataWindower::~cepDataWindower(){
  if( windowAlg != NULL ) delete windowAlg;
}

const cepError cepDataWindower::setChebBandwidth( double dw ) {
  if( windowAlg == NULL || algType != WINDOW_CHEBYSHEV ) {
    return cepError("Not using chebyshev algorithm. cannot set parameters", cepError::sevWarning);
  }
  ((cepWindowChebyshev *)windowAlg)->setTransitionBandwidth( dw );
  
  return cepError();
}



const cepError cepDataWindower::setWindowType( const Window &type, const int sz, const int ol ) {

  
  algType = type;
  size = sz;
  overlap=ol;

  cout << "<setWindowType>: " << type.toString() << ", size="<<size<<" overlap="<<overlap <<endl;
  
  if( type == WINDOW_RECTANGULAR ) {
      delete windowAlg;
      windowAlg = new cepWindowRect( size );
      
  } else if( type == WINDOW_HAMMING ) {
      delete windowAlg;
      windowAlg = new cepWindowHamming( size );

  } else if( type == WINDOW_BLACKMAN ) {
      delete windowAlg;
      windowAlg = new cepWindowBlackman( size );

  } else if( type == WINDOW_CHEBYSHEV ) {
      delete windowAlg;
      windowAlg = new cepWindowChebyshev( size );
      
  } else {
      // leave the current settings alone and return an error
      algType = WINDOW_UNDEFINED;
      return cepError("unknown windowing algorithm. Set type failed", cepError::sevWarning);
  }

  return cepError();
  
}


const int cepDataWindower::getSize()
{
  return size;
}
const int cepDataWindower::getOverlap()
{
  return overlap;
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
  cepMatrix<double> result( numWindows, windowAlg->getSize(), 3 );  // result. numwindows x windowSize x 2
  
  for( int win = 0, color=0; win < numWindows; ++win, color=(color++)%3 ) {
    for( int element = 0; element < windowAlg->getSize(); ++element ) {

      // sanity check
      int ptr = (win*increment)+element;
      if( ptr > numSamples ) {
        return cepError("Exceeded the bounds of the matrix when accessing row"+ptr);
      }
      
      // copy the date directly & scale the value
      // 0 is date, 1 is value
      result.setValue(win, element, 0, const_cast< cepMatrix<double>& >(dataIn).getValue( ptr, 0 ));
      result.setValue(win, element, 1, const_cast< cepMatrix<double>& >(dataIn).getValue( ptr, 1 )*coeffs.getValue(element,0));
      result.setValue(win, element, 0, const_cast< cepMatrix<double>& >(dataIn).getValue( ptr, 2 ));
    }
  }

  windowedData = result;

  return cepError();
}

const Window cepDataWindower::WINDOW_RECTANGULAR(1, "Rectangular");
const Window cepDataWindower::WINDOW_HAMMING(2, "Hamming");
const Window cepDataWindower::WINDOW_BLACKMAN(3, "Blackman");
const Window cepDataWindower::WINDOW_CHEBYSHEV(4, "Chebyshev");
const Window cepDataWindower::WINDOW_UNDEFINED(5, "Undefined");

int cepDataWindower::size = 0;
int cepDataWindower::overlap = 0;
cepWindowAlg* cepDataWindower::windowAlg = 0;
Window cepDataWindower::algType = WINDOW_UNDEFINED;

