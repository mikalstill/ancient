
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
#include "cepConfiguration.h"



cepWindow::cepWindow( int id, const char* name ) {
  myID = id;
  str = name;
}
bool cepWindow::operator== (const cepWindow &w) const {
  return (w.id() == myID);
}

bool cepWindow::operator!=(const cepWindow &w) const {
  return (w.id() != myID) ;
}
const int cepWindow::id() const {
  return myID;
}
const char* cepWindow::toString() const {
  return str;
}


cepDataWindower::cepDataWindower() {
}

void cepDataWindower::init() {
  windowAlg = NULL;
  algType = WINDOW_UNDEFINED;

  // dont display these things, just log them
  cepError err;
  int id;
  err = cepConfiguration::getInstance().getValue( CONFIG_NAME_TYPE, WINDOW_UNDEFINED.id(), id );
  algType = lookupWindow( id );
  if( err.isReal() ) err.log();
  
  err = cepConfiguration::getInstance().getValue(CONFIG_NAME_SIZE, 1024, size);
  if( err.isReal() ) err.log();
  
  err = cepConfiguration::getInstance().getValue(CONFIG_NAME_OVERLAP, 512, overlap);
  if( err.isReal() ) err.log();
  
  double tbw = 0.0;
  err = cepConfiguration::getInstance().getValue(cepWindowChebyshev::CONFIG_NAME_CHEB, 0.05, tbw);
  cepWindowChebyshev::setTransitionBandwidth( tbw );
  if( err.isReal() ) err.log();

//  cout << "window params loaded" <<endl
//       << "type: " << algType.toString() << endl
//       << "size: " << cepToString( size ) << endl
//       << "overlap: " << cepToString( overlap ) << endl
//       << "bandwidth: " << cepToString( tbw ) << endl;
}


cepDataWindower::~cepDataWindower(){
  if( windowAlg != NULL ) delete windowAlg;
}

const cepError cepDataWindower::setChebBandwidth( double dw ) {
  return cepWindowChebyshev::setTransitionBandwidth( dw );
}

const cepError cepDataWindower::setWindowType( const cepWindow &type, const int sz, const int ol ) {
  
  algType = type;
  size = sz;
  overlap=ol;

//  cout << "<setWindowType>: " << type.toString() << ", size="<<size<<" overlap="<<overlap <<endl;
  
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

  cepConfiguration::getInstance().setValue(CONFIG_NAME_TYPE.c_str(), type.id());
  cepConfiguration::getInstance().setValue(CONFIG_NAME_SIZE.c_str(), size);
  cepConfiguration::getInstance().setValue(CONFIG_NAME_OVERLAP.c_str(), overlap);

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
  if( windowAlg == NULL || algType == WINDOW_UNDEFINED ) {
    return cepError( "Window algorithm is not defined. Windowing failed", cepError::sevErrorRecoverable );
  }
  int numSamples = const_cast<cepMatrix<double>&>(dataIn).getNumRows();
  int numWindows = countWindows( numSamples, windowAlg->getSize(), overlap );
  int increment = windowAlg->getSize() - overlap;

  // sanity checks
  if( windowAlg->getSize() > numSamples ) {
    return cepError("invalid window size specified. size must not be greater than the sample size");
  } else if( const_cast<cepMatrix<double>&>(dataIn).getNumCols() < 2 ) {
    return cepError("insufficient data to proceed. require at least 2 cols: date and sample");
  } else if( increment < 0 ) {
    return cepError("invalid overlap specified. cannot be negative");
  }


  cepMatrix<double> coeffs = windowAlg->getCoeffs();                // scaling factors
  cepMatrix<double> result( windowAlg->getSize(), 3, numWindows );  // result. numwindows x windowSize x 2

  int color = 0;
  for( int win = 0; win < numWindows; ++win ) {
    // a number between 1 and 3
    // not used
//    color = 1+win%3;
    for( int element = 0; element < windowAlg->getSize(); ++element ) {

      // sanity check
      int ptr = (win*increment)+element;
      if( ptr > numSamples ) {
        return cepError("Exceeded the bounds of the matrix when accessing row"+ptr);
      }
      
      // copy the date directly & scale the value
      // 0 is date, 1 is value, 2 is the color hint
      result.setValue(element, 0, win, const_cast< cepMatrix<double>& >(dataIn).getValue( ptr, 0 ));
      result.setValue(element, 1, win, const_cast< cepMatrix<double>& >(dataIn).getValue( ptr, 1 )*coeffs.getValue(element,0));
      result.setValue(element, 2, win, color);
      
//      cout << "scale=" << const_cast< cepMatrix<double>& >(dataIn).getValue( ptr, 0 )
//           << "(" << result.getValue(element, 0, win) << ")"
//           << "   value=" << const_cast< cepMatrix<double>& >(dataIn).getValue( ptr, 1 )*coeffs.getValue(element,0)
//           << "(" << result.getValue(element, 1, win) << ")"
//           << "   color=" << color
//           << "(" << result.getValue(element, 2, win) << ")"
//           << endl;
    }
  }

  windowedData = result;

  return cepError();
}

const cepWindow cepDataWindower::lookupWindow( int id ) {
  switch( id ) {
    case 0:
      return cepDataWindower::WINDOW_RECTANGULAR;
    case 1:
      return cepDataWindower::WINDOW_HAMMING;
    case 2:
      return cepDataWindower::WINDOW_BLACKMAN;
    case 3:
      return cepDataWindower::WINDOW_CHEBYSHEV;
    default:
      return cepDataWindower::WINDOW_UNDEFINED;
  }
}

const cepWindow cepDataWindower::WINDOW_RECTANGULAR(0, "Rectangular");
const cepWindow cepDataWindower::WINDOW_HAMMING(1, "Hamming");
const cepWindow cepDataWindower::WINDOW_BLACKMAN(2, "Blackman");
const cepWindow cepDataWindower::WINDOW_CHEBYSHEV(3, "Chebyshev");
const cepWindow cepDataWindower::WINDOW_UNDEFINED(4, "Undefined");

int cepDataWindower::size = 0;
int cepDataWindower::overlap = 0;
cepWindowAlg* cepDataWindower::windowAlg = 0;
cepWindow cepDataWindower::algType = WINDOW_UNDEFINED;

const string cepDataWindower::CONFIG_NAME_TYPE("window-type");
const string cepDataWindower::CONFIG_NAME_SIZE("window-size");
const string cepDataWindower::CONFIG_NAME_OVERLAP("window-overlap");

