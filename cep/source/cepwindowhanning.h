/*
 *   Imp for the cepwindowhanning.h
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

/******************************************************************************
DOCBOOK START

FUNCTION <!-- class name -->
cepConfiguration

PURPOSE <!-- use cases -->
provides storage of name/value pairs

SYNOPSIS START
<!-- how to instantiate one? -->
cepConfiguration::getInstance();

SYNOPSIS END

DESCRIPTION START
<!-- description goes here -->
<para>description</para>
A Hanning window. The coefficients are generated according to the followng equation

<para> <!-- per function descriptions -->
<command>cepWindowHanning( int size )</command>
Makes a new cepWindowHanning of a specific size.
</para>

<para> <!-- per function descriptions -->
<command>cepWindowHanning( int size )</command>
Makes a new cepWindowHanning of a specific size.
</para>

<para> <!-- per function descriptions -->
<command>getValue( int offset )</command>
gets the coefficient at a specified offset from the front of the window. This
will calculated according to the following equation.
If the window size is 1 then the coefficient will be 1, otherwise
<command>double result = 0.50 - 0.50*cos( 2*PI* offset/(size-1) );<command>
</para>

DESCRIPTION END

DOCBOOK END
******************************************************************************/

#ifndef CEPWINDOWHANNING_H
#define CEPWINDOWHANNING_H

#include "cepWindowAlg.h"

/**
  *@author Blake Swadling
  */

class cepWindowHanning:public cepWindowAlg
{
public:
  cepWindowHanning (int size);
   ~cepWindowHanning ();

protected:
  double getValue (int offset);
};

#endif
