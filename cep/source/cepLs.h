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

#ifndef __CEPLS_H
#define __CEPLS_H

#include <iostream.h>
#include <math.h>
#include <stdlib.h>

#include  "cepMatrix.h"

class cepLs
{
public:
  
  cepLs();
  ~cepLs();

  //do one iteration of the least squares algoritum
  const cepLs & cepDoVCV(cepMatrix<double> &data, cepMatrix<double> &matP);

  //do one iteration of the least squares algoritum
  const cepLs & cepDoRW(cepMatrix<double> &matA, cepMatrix<double> &matP);

  //get the value of the residual at pos (x,y)    
  const cepMatrix<double> &getResidual();
  
  //get the value B1 in the least squares solution y=B1*x +B2  
  double getB1();

  //get the value B2 in the solution y=B1*x +B2  
  double getB2();
  
private:

  cepMatrix<double> residual;       //holds the value of the residuals 
  cepMatrix<double> matX;           //holds the value of B1, B2

  //ensure that all values of the matrix A P and L are consistant with the
  //least squares alogrithum.
  void sanityCheck(cepMatrix<double> &matA, cepMatrix<double> &matP);
  
  //calculate the residuals of the least squares tranformation
  void calcResiduals(cepMatrix<double> &matA, cepMatrix<double> &matL );

  //make the A matrix
  const cepMatrix<double> makeA(cepMatrix<double> &data);

  //make the L matrix
  const cepMatrix<double> makeL(cepMatrix<double> &data);

  //calculate the matrix inverse
  const cepMatrix<double> inverse(cepMatrix<double> &mat);

  //calculate A*B where B is a diagonal
  const cepMatrix<double> mulDiag(cepMatrix<double> &matA, cepMatrix<double> &matB);

  //calculates A*B where A is the design matrix in the least squares tranform
  const cepMatrix<double> Amul(cepMatrix<double> &matA, cepMatrix<double> &matB);

  //calculates B*A where A is the design matrix in the least squares tranform
  const cepMatrix<double> mulA(cepMatrix<double> &matA, cepMatrix<double> &matA);
};

#endif //end __CEPLS_H
