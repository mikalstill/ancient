/*
 * cepDFT.h
 *
 * Description:- Discrete forier transform algorithm
 *
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

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <complex>
#include "cepMatrix.h"
#include "cepError.h"



#ifndef __CEP_DFT_H
#define __CEP_DFT_H


/****
DOCBOOK START
cep Interpolation class

This class

DOCBOOK END
****/
typedef complex<double> ComplexDble;

class cepDFT
{

public:
  cepDFT();

  cepMatrix<ComplexDble > DFT(cepMatrix<double> &timeSeries);
  cepMatrix<double> iDFT(cepMatrix<ComplexDble > &freqSeries);

private:
  double firstDate;
  void initialiseFreq(cepMatrix <double> & timeSeries, cepMatrix<ComplexDble > & freq);
  void initialiseTimeSeries(cepMatrix <ComplexDble > & freqSeries, cepMatrix<double> & timeSeries);
  double getSampleFreq(cepMatrix<double> & matrix);
  double getSampleRate(cepMatrix<ComplexDble > & matrix);


};

#endif // ! __CEP_DFT_H
