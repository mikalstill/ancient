/* 
 *   Imp for the CEP string vector class
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

#include "stringArray.h"
#include "utility.h"
#include "verbosity.h"

stringArray::stringArray (string input, string delim):
m_broken (0, string ("")), m_unbroken (input)
{
  char *temp, *p;

  debug(dlTrace, string("String array input length is ") + 
	toString(input.length()));
  if(input.length() > 0)
    {
      debug(dlTrace, "String array non empty input");

      // Break the string by the delimiter
      temp = strdup (input.c_str ());
      p = strtok (temp, delim.c_str ());
      
      while (p != NULL)
	{
	  m_broken.resize (m_broken.size () + 1);
	  m_broken[m_broken.size () - 1] = p;
	  p = strtok (NULL, delim.c_str ());
	}
      
      // Cleanup
      free (temp);
    }
}

size_t
stringArray::size ()
{
  return m_broken.size ();
}

string
stringArray::operator[] (size_t index)
{
  if (m_unbroken.length () == 0)
    return "";
  if (index < m_broken.size ())
    return m_broken[index];
  else
    return "";
}
