
/**********************************************************
cepMatrix.h

Copyright:- Kristy Van Der Vlist
Description:- The cepMatrix object contains the matrix data,
  acessor methods an core mathematical fuctions that need
  to be preformed on it.
***********************************************************/
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
  // Caluclates the transpose of the current matrix object
  
  // ***************assingement operators***********************
  const cepMatrix & operator+= (const cepMatrix &);
  //calculates A+B where A is the current matrix object and B is another matrix
  
  const cepMatrix & operator-= (const cepMatrix &);
  //calculates A-B where A is the current matrix object and B is another matrix
  
  const cepMatrix & operator*= (const cepMatrix &);  
  //calculates A*B where A is the current matrix object and B is another matrix
  
  const cepMatrix & operator*= (const double &);        // mat * scalar
  //calculates c*A where A is the current matrix object and c is a scalar value
  
  // ***************copy operator*****************************
  const cepMatrix & operator= (const cepMatrix &);
  //copies A to B where A is the current matrix object and B is another matrix

  // ***************query operators****************************
  bool operator== (const cepMatrix &);
  //compares A to B where A is the current matrix object and B is another matrix
  //returns true if A and B are equal, else returns false
  
  bool isDiagonal ();
  //determies whether the A, the current matrix object is strictly Diagonal.
  //returs true if A is diagonal, else returns false
  
  // ***************get/set methods**************************
  double getValue (int row, int col);
  //gets the value of the matrix at matrix[row,col]
  
  void setValue (int row, int col, double value);
  //sets the value of the matrix at matrix[row,col] = value
  
  int getNumRows ();
  //gets the number of rows in the matrix
  
  int getNumCols ();
  //gets the number of cols in the matrix
  
private:
  double *matrix;     //holds the matrix values
  int numRows,        //holds the number of rows in the matrix
      numCols;        //holds the number of cols in the matrix
};
