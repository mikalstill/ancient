/*
 *   Imp for the cepDataWindower.cpp
 *
 *   Copyright (C) Daniel Fernandez                 2002
 *   Copyright (C) Michael Still                    2002
 *   Copyright (C) Blake Swadling                   2002
 *   Copyright (C) Kristy Van Der Vlist             2002
 *   Copyright (C) Nick Wheatstone                  2002
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc., 675
 * Mass Ave, Cambridge, MA 02139, USA.
 */

#include "cepDataWindower.h"
#include "cepWindowRect.h"
#include "cepwindowhamming.h"
#include "cepwindowhanning.h"
#include "cepwindowblackman.h"
#include "cepwindowchebyshev.h"
#include "cepConfiguration.h"
#include "cepDataset.h"

cepWindow::cepWindow (int id, const char *name)
{
  myID = id;
  str = name;
}

bool cepWindow::operator== (const cepWindow & w) const
{
  return (w.id () == myID);
}

bool cepWindow::operator!= (const cepWindow & w) const
{
  return (w.id () != myID);
}
const int
cepWindow::id () const
{
  return myID;
}
const char *
cepWindow::toString () const
{
  return str;
}


cepDataWindower::cepDataWindower ()
{
}

void
cepDataWindower::init ()
{
  windowAlg = NULL;
  algType = WINDOW_UNDEFINED;

  // dont display these things, just log them
  cepError err;
  int id;
  err =
    cepConfiguration::getInstance ().getValue (CONFIG_NAME_TYPE,
					       WINDOW_UNDEFINED.id (), id);
  algType = lookupWindow (id);
  if (err.isReal ())
    err.log ();

  err =
    cepConfiguration::getInstance ().getValue (CONFIG_NAME_SIZE, 128, size);
  if (err.isReal ())
    err.log ();

  err =
    cepConfiguration::getInstance ().getValue (CONFIG_NAME_OVERLAP, 64,
					       overlap);
  if (err.isReal ())
    err.log ();

  double tbw = 0.0;
  err =
    cepConfiguration::getInstance ().
    getValue (cepWindowChebyshev::CONFIG_NAME_CHEB, 0.05, tbw);
  cepWindowChebyshev::setTransitionBandwidth (tbw);
  if (err.isReal ())
    err.log ();

//  cout << "window params loaded" <<endl
//       << "type: " << algType.toString() << endl
//       << "size: " << cepToString( size ) << endl
//       << "overlap: " << cepToString( overlap ) << endl
//       << "bandwidth: " << cepToString( tbw ) << endl;
}


cepDataWindower::~cepDataWindower ()
{
  if (windowAlg != NULL)
    delete windowAlg;
}

const cepError
cepDataWindower::setChebBandwidth (double dw)
{
  return cepWindowChebyshev::setTransitionBandwidth (dw);
}

const cepError
cepDataWindower::setWindowType (const cepWindow & type, const int sz,
				const int ol)
{

  algType = type;
  size = sz;
  overlap = ol;

//  cout << "<setWindowType>: " << type.toString() << ", size="<<size<<" overlap="<<overlap <<endl;

  if (type == WINDOW_RECTANGULAR)
    {
      if (windowAlg != NULL)
	delete windowAlg;
      windowAlg = new cepWindowRect (size);

    }
  else if (type == WINDOW_HAMMING)
    {
      if (windowAlg != NULL)
	delete windowAlg;
      windowAlg = new cepWindowHamming (size);

    }
  else if (type == WINDOW_HANNING)
    {
      if (windowAlg != NULL)
	delete windowAlg;
      windowAlg = new cepWindowHanning (size);

    }
  else if (type == WINDOW_BLACKMAN)
    {
      if (windowAlg != NULL)
	delete windowAlg;
      windowAlg = new cepWindowBlackman (size);

    }
  else if (type == WINDOW_CHEBYSHEV)
    {
      if (windowAlg != NULL)
	delete windowAlg;
      windowAlg = new cepWindowChebyshev (size);

    }
  else
    {
      if (windowAlg != NULL)
	delete windowAlg;
      algType = WINDOW_UNDEFINED;
      // return here and leave the cnfig settings alone
      return cepError ("unknown windowing algorithm. Set type failed",
		       cepError::sevWarning);
    }

  cepConfiguration::getInstance ().setValue (CONFIG_NAME_TYPE.c_str (),
					     type.id ());
  cepConfiguration::getInstance ().setValue (CONFIG_NAME_SIZE.c_str (), size);
  cepConfiguration::getInstance ().setValue (CONFIG_NAME_OVERLAP.c_str (),
					     overlap);

  return cepError ();

}


const int
cepDataWindower::getSize ()
{
  return size;
}
const int
cepDataWindower::getOverlap ()
{
  return overlap;
}

int
cepDataWindower::countWindows (int samples, int winSize, int overlap)
{
  if (overlap == 0)
    return (int) ((double) samples / winSize);
  return (int) ((double) (samples - winSize) / (winSize - overlap) + 1);
}



const cepError
cepDataWindower::window (const cepMatrix < double >&dataIn,
			 cepMatrix < double >&windowedData)
{

  /*
   * TODO BS - if we have no windowing algorithm defined should we simply fail
   * or should we use rectangular?
   */
  if (windowAlg == NULL || algType == WINDOW_UNDEFINED)
    {
      return cepError ("Window algorithm is not defined. Windowing failed",
		       cepError::sevErrorRecoverable);
    }
  int numSamples = const_cast < cepMatrix < double >&>(dataIn).getNumRows ();
  int numWindows = countWindows (numSamples, windowAlg->getSize (), overlap);
  int increment = windowAlg->getSize () - overlap;
//  cout << "windowing: " << algType.toString() << endl
//       << "  numSamples=" << numSamples << endl
//       << "  numWindows=" << numWindows << endl
//       << "  increment=" << increment << endl;

  // sanity checks
  if (windowAlg->getSize () > numSamples)
    {
      return cepError ("invalid window size specified. size (" +
		       cepToString (size) +
		       ") is greater than the sample size (" +
		       cepToString (numSamples) + ")");
    }
  else if (const_cast < cepMatrix < double >&>(dataIn).getNumCols () < 2)
    {
      return
	cepError
	("insufficient data to proceed. require at least 2 cols: date and sample");
    }
  else if (increment < 0)
    {
      return cepError ("invalid overlap - negative value supplied");
    }

  cepMatrix < double >coeffs = windowAlg->getCoeffs ();	// scaling factors
  cepMatrix < double >result (windowAlg->getSize (), 4, numWindows);	// result. numwindows x windowSize x 4

  int color = 0;
  for (int win = 0; win < numWindows; ++win)
    {
      for (int element = 0; element < windowAlg->getSize (); element++)
	{
	  // sanity check
	  int ptr = (win * increment) + element;
	  if (ptr > numSamples)
	    {
	      return
		cepError
		("Exceeded the bounds of the matrix when accessing row" +
		 ptr);
	    }

	  // copy the date
	  result.setValue (element, cepDataset::colDate, win,
			   const_cast < cepMatrix <
			   double >&>(dataIn).getValue (ptr, 0));
	  if (const_cast < cepMatrix <
	      double >&>(dataIn).getError ().isReal ())
	    return const_cast < cepMatrix < double >&>(dataIn).getError ();
	  if (result.getError ().isReal ())
	    return result.getError ();

	  // scale the value
	  result.setValue (element, cepDataset::colSample, win,
			   const_cast < cepMatrix <
			   double >&>(dataIn).getValue (ptr,
							1) *
			   coeffs.getValue (element, 0));
	  if (const_cast < cepMatrix <
	      double >&>(dataIn).getError ().isReal ())
	    return const_cast < cepMatrix < double >&>(dataIn).getError ();
	  if (coeffs.getError ().isReal ())
	    return coeffs.getError ();
	  if (result.getError ().isReal ())
	    return result.getError ();

	  // error always zero
	  result.setValue (element, cepDataset::colError, win, 0);
	  if (result.getError ().isReal ())
	    return result.getError ();

	  // colour not used here
	  result.setValue (element, cepDataset::colColourHint, win, color);
	  if (result.getError ().isReal ())
	    return result.getError ();
	}
    }

  windowedData = result;
  if (windowedData.getError ().isReal ())
    return windowedData.getError ();
  if (result.getError ().isReal ())
    return result.getError ();

  return cepError ();
}

const cepWindow
cepDataWindower::lookupWindow (int id)
{
  switch (id)
    {
    case 0:
      return cepDataWindower::WINDOW_RECTANGULAR;
    case 1:
      return cepDataWindower::WINDOW_HAMMING;
    case 2:
      return cepDataWindower::WINDOW_HANNING;
    case 3:
      return cepDataWindower::WINDOW_BLACKMAN;
    case 4:
      return cepDataWindower::WINDOW_CHEBYSHEV;
    default:
      return cepDataWindower::WINDOW_UNDEFINED;
    }
}

const cepWindow
cepDataWindower::WINDOW_RECTANGULAR (0, "Rectangular");
const cepWindow
cepDataWindower::WINDOW_HAMMING (1, "Hamming");
const cepWindow
cepDataWindower::WINDOW_HANNING (2, "Hanning");
const cepWindow
cepDataWindower::WINDOW_BLACKMAN (3, "Blackman");
const cepWindow
cepDataWindower::WINDOW_CHEBYSHEV (4, "Chebyshev");
const cepWindow
cepDataWindower::WINDOW_UNDEFINED (5, "Undefined");

int
  cepDataWindower::size =
  0;
int
  cepDataWindower::overlap =
  0;
cepWindowAlg *
  cepDataWindower::windowAlg =
  0;
cepWindow cepDataWindower::algType = WINDOW_UNDEFINED;

const
  string
cepDataWindower::CONFIG_NAME_TYPE ("window-type");
const string
cepDataWindower::CONFIG_NAME_SIZE ("window-size");
const string
cepDataWindower::CONFIG_NAME_OVERLAP ("window-overlap");
