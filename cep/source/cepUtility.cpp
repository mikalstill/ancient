
/* 
 *   Utility functions for CEP
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
#include <stdio.h>

string
cepToString (int number)
{
  char buffer[10];

  snprintf (buffer, 10, "%d", number);
  return string (buffer);
}

string
cepToString (long number)
{
  char buffer[20];

  snprintf (buffer, 20, "%ld", number);
  return string (buffer);
}

string
cepToString (double number)
{
  char buffer[20];

  snprintf (buffer, 20, "%f", number);
  return string (buffer);
}

string
cepToString (float number)
{
  char buffer[10];

  snprintf (buffer, 10, "%f", number);
  return string (buffer);
}

string
cepToString (bool val)
{
  if(val) return "true";
  return "false";
}

string
cepToString (char val)
{
  char buffer[2];

  snprintf (buffer, 2, "%c", val);
  return string(buffer);
}

string
cepToString (size_t val)
{
  char buffer[10];

  snprintf (buffer, 10, "%d", val);
  return string(buffer);
}

bool
cepIsBlank (char c)
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

bool cepIsNumeric(char c)
{
  // This line of code:
  //
  // if(((c < 48) || (c > 57)) && (cepIsBlank(c) == false) && (c != '.') 
  //    && (c != '-'))
  //
  // Became:

  if(isdigit(c))
    {
      return true;
    }
  if(c == '.')
    {
      return true;
    }
  if(c == '-')
    {
    return true;
    }
  
  return false;
}

int
cepMax (int a, int b)
{
  if (a > b)
    return a;
  return b;
}

int
cepMin (int a, int b)
{
  if (a < b)
    return a;
  return b;
}

int
cepAbs (int a)
{
  if (a < 0)
    return -a;
  return a;
}

string cepToLower(string in)
{
  string out;

  for(unsigned int count = 0; count < in.length(); count++)
    out += tolower(in.c_str()[count]);
  
  return out;
}

string cepToUpper(string in)
{
  string out;

  for(unsigned int count = 0; count < in.length(); count++)
    out += toupper(in.c_str()[count]);
  
  return out;
}
