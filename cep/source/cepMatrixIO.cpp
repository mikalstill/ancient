
/* 
 *   Imp for the CEP matrix file IO
 *   Copyright (C) Michael Still                    2002
 *   Copyright (C) Kristy Van Der Vlist             2002
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

#include "cepMatrixIO.h"

cepMatrix < double >
cepReadMatrix (const string & filename)
{
  fstream file;
  int rows = 0, cols = 0;
  double val = 0.0;

  file.open (filename.c_str (), ios::in);

  if (!file.is_open ())
    {
      cepError oor ("Error! Can not open file:" + filename,
		    cepError::sevErrorFatal);
      oor.display ();
    }

  file >> rows;
  file >> cols;

  // Check the matrix is square
  if (rows != cols)
    {
      cepError oor ("Error! The file: " + filename +
		    " does not contain a square matrix",
		    cepError::sevErrorFatal);
      oor.display ();
    }

  cepMatrix < double >matP (rows, cols);

  for (int i = 0; i < rows; i++)
    {
      for (int j = 0; j < cols; j++)
	{
	  file >> val;

	  // Check to see if we have hit the eof to early
	  if (file.eof ())
	    {
	      cepError oor ("Error! File:" + filename +
			    " contain to few values",
			    cepError::sevErrorFatal);
	      oor.display ();
	    }
	  matP.setValue (i, j, val);
	}
    }

  file >> val;

  // Check to see there are no more values in the file
  if (!file.eof ())
    {
      cepError oor ("Error! File: " + filename + " contains too many values",
		    cepError::sevErrorFatal);
      oor.display ();
    }

  return matP;
}
