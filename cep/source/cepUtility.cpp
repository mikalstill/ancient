/* 
   Ulitity functions for CEP
   Copyright (C) Michael Still                    2002
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "core.h"
#include <stdio.h>

string
itoa (int number)
{
  char buffer[10];

  snprintf (buffer, 10, "%d", number);
  return string (buffer);
}

string
ltoa (long number)
{
  char buffer[10];

  snprintf (buffer, 10, "%d", number);
  return string (buffer);
}

string
dtoa (double number)
{
  char buffer[10];

  snprintf (buffer, 10, "%f", number);
  return string (buffer);
}

string
ftoa (float number)
{
  char buffer[10];

  snprintf (buffer, 10, "%f", number);
  return string (buffer);
}

bool
isblank (char c)
{
  if (c == ' ')
    return true;
  if (c == '\t')
    return true;
  if (c == '\r')
    return true;
  if (c == '\n')
    return true;

  return false;
}
