 /* 
  * Imp for the least squares implementation
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

#include  "cepMatrix.h"

class cepLs
{
public:
  
  cepLs();
  ~cepLs();

  const cepLs & cepDoLeastSquares(cepMatrix &, cepMatrix &, cepMatrix &);
  //do one iteration of the least squares algoritum
  
  double getResidual(int, int);
  //get the value of the residual at pos (x,y)
  double getB1();
  //get the value B1 in the least squares solution y=B1*x +B2   
  double getB2();
  //get the value B1 in the solution y=B1*x +B2
  
private:

  cepMatrix residual;       //holds the value of the residuals 
  cepMatrix matX;  //holds the value of B1, B2

  void sanityCheck(cepMatrix &, cepMatrix &, cepMatrix &);
  //ensure that all values of the matrix A P and L are consistant with the
  //least squares alogrithum.
  
  void calcResiduals(cepMatrix &, cepMatrix & );
  //calculate the residuals of the least squares tranformation
  const cepMatrix inverse(cepMatrix &);
  //calculate the matrix inverse
  const cepMatrix mulDiag(cepMatrix &, cepMatrix &);
  //calculate A*B where B is a diagonal
  const cepMatrix Amul(cepMatrix &, cepMatrix &);
  //calculates A*B where A is the design matrix in the least squares tranform
  const cepMatrix mulA(cepMatrix &, cepMatrix &);
  //calculates B*A where A is the design matrix in the least squares tranform
};


