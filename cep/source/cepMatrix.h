 /* 
  * Imp for the matrix class
  * Copyright (C) Kristy Van Der Vlist             2002
  * 
  * This program is free software; you can redistribute it and/or modify it
  * under the terms of the GNU General Public License as published by the Free
  * Software Foundation; either version 2 of the License, or (at your option)
  * any later version.
  * 
  * This program is distributed in the hope that it will be useful, but WITHOUT
  * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  * more details.
  * 
  * You should have received a copy of the GNU General Public License along
  * with this program; if not, write to the Free Software Foundation, Inc., 675
  * Mass Ave, Cambridge, MA 02139, USA. 
*/

#include <iostream.h>
#include <math.h>
#include <stdlib.h>

class cepMatrix
{

public:
  cepMatrix ();
  cepMatrix (int rows, int cols);
    cepMatrix (const cepMatrix & copyMatrix);   // copy constructor

   ~cepMatrix ();

  const cepMatrix & transpose ();

  // ***************assingement operators***********************
  const cepMatrix & operator+= (const cepMatrix &);
  const cepMatrix & operator-= (const cepMatrix &);
  const cepMatrix & operator*= (const cepMatrix &);     // mat *mat
  const cepMatrix & operator*= (const double &);        // mat * scalar

  // ***************copy operator*****************************
  const cepMatrix & operator= (const cepMatrix &);

  // ***************query operators****************************
  bool operator== (const cepMatrix &);
  bool isDiagonal ();

  // ***************get/set methods**************************
  double getValue (int row, int col);
  void setValue (int row, int col, double value);
  int getNumRows ();
  int getNumCols ();

private:
  double *matrix;
  int numRows, numCols;
};
