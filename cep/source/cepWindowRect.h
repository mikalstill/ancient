
/*
 *   Imp for the cepWindowRect.h
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

/******************************************************************************
DOCBOOK START

FUNCTION <!-- class name -->
cepWindowRect

PURPOSE <!-- use cases -->
generates the coefficient array representing a rectangular window of a specified size

SYNOPSIS START <!-- how to get one -->
This is utilised by cepDataWindower and need not be instantiated directly.

to instantiate a rectangular window
cepWindowRect( int size );

SYNOPSIS END

DESCRIPTION START
<!-- description goes here -->
<para>description</para>

<para> <!-- per function descriptions -->
<command>cepWindowRect( int size )</command>
Makes a new cepWindowRect of a specific size.
</para>

<para> <!-- per function descriptions -->
<command>cepWindowRect( int size )</command>
Makes a new cepWindowRect of a specific size.
</para>

<para> <!-- per function descriptions -->
<command>getValue( int offset )</command>
gets the coefficient at a specified offset from the front of the window. This will always be 1.
</para>

DESCRIPTION END

DOCBOOK END
******************************************************************************/

#ifndef CEPWINDOWRECT_H
#define CEPWINDOWRECT_H

#include "cepWindowAlg.h"

/**
  *@author Blake Swadling
  */

class cepWindowRect : public cepWindowAlg  {  
public:
  cepWindowRect( int size );
  ~cepWindowRect();
  
protected:
  double getValue( int offset );
};

#endif
