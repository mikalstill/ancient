/*
 *   Imp for the cepwindowhanning.cpp
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

#include "cepwindowhanning.h"

cepWindowHanning::cepWindowHanning (int size):
cepWindowAlg (size)
{
  initCoeffs ();
}

cepWindowHanning::~cepWindowHanning ()
{
  if (coeffs != NULL)
    delete coeffs;
}


double
cepWindowHanning::getValue (int offset)
{
  if (size == 1)
    return 1;
  double val = (double) offset / (size - 1);
  double result = 0.50 - 0.50 * cos (2 * PI * val);
  return result;
}
