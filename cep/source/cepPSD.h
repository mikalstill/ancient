/*
 * cepPSD.h
 *
 * Description:- Prepares frequency domain data for plotting by doing making
 *               a power spectral density data set
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



#ifndef __CEP_PSD_H
#define __CEP_PSD_H


/****
DOCBOOK START
cep Interpolation class

This class

DOCBOOK END
****/
typedef complex<double> ComplexDble;

class cepPSD
{

public:
  cepPSD();

  cepMatrix<double> makePSD(cepMatrix<ComplexDble> & freqSeries);


private:


};

#endif // ! __CEP_PSD_H
