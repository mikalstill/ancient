/*
 *   Header for the functions which do the maths
 *
 *   Copyright (C) Michael Still                    2002
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

#include "cepCore.h"
#include "cepLs.h"
#include "cepInterp.h"
#include "cepDataWindower.h"
#include "cepCfft.h"
#include "cepDFT.h"
#include "cepPSD.h"

cepError processInterp (cepDataset * ds, const int iType, string desc,
			double sampleRate, string newcfname);
cepError processInterpLs (cepDataset * ds, double sampleRate,
			  string newcfname);
cepError processWindow (cepDataset * ds, const cepWindow wType, string desc,
			int size, int overlap, string newcfname);
cepError processFFT (cepDataset * ds, string newcfname);
cepError processLsVCV (cepDataset * ds, cepDataset::direction i,
		       cepMatrix<double> & data, cepMatrix<double> & residual,
		       double & b1, double & b2);
cepError processLsVCV (cepDataset * ds, cepDataset::direction i, cepMatrix<double> &reweight,
		       cepMatrix<double> & data, cepMatrix<double> & residual,
		       double & b1, double & b2);
cepError processLsRW (cepDataset * ds);
cepError processDFT (cepDataset * ds, string newcfname);
