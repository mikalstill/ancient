
/* 
   Imp for the CEP data presentation
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

#include "cepCore.h"

// This number needs to be positive, and preferably greater than 2...
// Having big numbers is more efficient, but wastes more RAM
const long CHUNKALLOC = 10;

cepPresentation::cepPresentation (long width, long height):m_dataValid (1,
                                                                        bool
                                                                        (false))
{
  m_width = width;
  m_height = height;
  m_xTitle = "Undefined Axis Title";
  m_yTitle = "Undefined Axis Title";
  m_xUnit = -1;
  m_yUnit = -1;
}

void cepPresentation::xAxisTitle (const string & title)
{
  m_xTitle = title;
}

void cepPresentation::yAxisTitle (const string & title)
{
  m_yTitle = title;
}

void cepPresentation::xAxisScale (int units)
{
  m_xUnit = units;
}

void cepPresentation::yAxisScale (int units)
{
  m_yUnit = units;
}

void cepPresentation::addDataPoint (long x, long y)
{
  // Values
  if (x >= (long)m_data.size ())
  {
    m_data.resize (x + CHUNKALLOC);
    cepDebugPrint ("Resize presentation data to " + cepLtoa (x + CHUNKALLOC));
  }
  m_data[x] = y;
  cepDebugPrint ("Added (" + cepLtoa (x) + ", " + cepLtoa (y) + ")");

  // Validity
  if (x >= (long)m_dataValid.size ())
  {
    m_dataValid.resize (x + CHUNKALLOC, bool (false));
  }
  m_dataValid[x] = true;
  cepDebugPrint ("Made valid " + cepLtoa (x));
}

// There are some output formats which need interpolation, but not all of them
// for those, we have this somewhat dodgy method...
void cepPresentation::interpolate ()
{
  long prevX = 0, prevY = 0;
  bool prevValid (false);

  // todo_mikal: for now we just dump the data points back out again
  for (size_t i = 0; i < m_data.size (); i++)
  {
    if (m_dataValid[i])
    {
      if (prevValid)
      {
        cepDebugPrint ("Iterpolate the line from (" +
                       cepLtoa (prevX) + ", " + cepLtoa (prevY)
                       + ") to (" + cepLtoa (i) + ", " +
                       cepLtoa (m_data[i]) + ")");

        // This pixel toggling is based on the premise that the function
        // for a given straight line can be defined by y = mx + b...
        // 
        // I assume that b is 0, because the 'origin' of this line
        // segment will be the first point at the start of the line.
        // Therefore, the function for the line is something along the
        // lines of:

        double m = ((double)(m_data[i] - prevY) / (double)(i - prevX));

        for (long x = 1; x < (long)(i - prevX); x++)
        {
          long y = prevY + (long)(m * x);

          cepDebugPrint ("Interpolation added the point (" +
                         cepLtoa (x + prevX) + ", " + cepLtoa (y) +
                         "). The gradient is " + cepDtoa (m));
        }
      }

      prevX = i;
      prevY = m_data[i];
      prevValid = true;
    }
  }
}

cepError cepPresentation::createPDF (const string & filename)
{
  return cepError ();
}

cepError cepPresentation::createBitmap (const string & filename)
{
  interpolate ();
  return cepError ();
}
