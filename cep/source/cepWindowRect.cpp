/*
 *   Imp for the cepWindowRect.cpp
 *
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

#include "cepWindowRect.h"

cepWindowRect::cepWindowRect (int s):
cepWindowAlg (s)
{
  initCoeffs ();
}

cepWindowRect::~cepWindowRect ()
{
  // in case we had another type of window previously. coeffs is not used here
  if (coeffs != NULL)
    delete coeffs;
}


double
cepWindowRect::getValue (int offset)
{
  // always return a scaling factor of 1
  return 1;
}
