
/* 
 *   A wrapper for the CEP data presentation
 *
 *   Copyright (C) Daniel Fernandez                 2002
 *   Copyright (C) Michael Still                    2002
 *   Copyright (C) Blake Swadling                   2002
 *   Copyright (C) Kristy Van Der Vlist             2002
 *   Copyright (C) Nick Wheatstone                  2002
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

#include "cepDataset.h"

#ifndef CEP_PLOT_HEADER
#define CEP_PLOT_HEADER

class cepPlot
{
public:
  cepPlot (cepDataset * theDataset, cepDataset::direction dir,
	   string cfname, long x, long y, float &vertScale, float &horizScale,
	   long &xminval, long &yminval, long &yrange, bool haveLs,
	   bool freqDomain, float energy, int windowTarget);

  bool getFailed ();

private:
    bool m_plotfailed;
};

#endif
