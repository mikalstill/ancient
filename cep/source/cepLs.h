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

/******************************************************************************
DOCBOOK START

FUNCTION <!-- class name -->
cepLs


PURPOSE <!-- use cases -->
Cacluates a Linear Least Squares transformation on a given dataset. It offers three
types of Least Squares transfomations:-

Variance Co-Variance
Variance Co-Variance (with automated data re-weighting)
Random Walk

SYNOPSIS START
<!-- how to instantiate one? -->
The follwing is an example of how to create a least squares object.

cepLs ls;

SYNOPSIS END

DESCRIPTION START
<!-- description goes here -->
<para>An implementation of the <command>cepLs</command> class
which calculates a Linear Least Squares transformation.
</para>

<para> <!-- per function descriptions -->
<command>const cepLs & cepDoVCV(cepMatrix<double> &data, cepMatrix<double> &matP)</command>
do one iteration of the VCV least squares algoritum
</para>

<para> <!-- per function descriptions -->
<command>const cepLs & cepDoVCV(cepMatrix<double> &data)</command>
iterate least squares algoritum until stable
</para>

<para> <!-- per function descriptions -->
<command>const cepLs & cepDoRW(cepMatrix<double> &matA, cepMatrix<double> &matP)</command>
do one iteration of the least squares algoritum
</para>

<para> <!-- per function descriptions -->
<command>const cepMatrix<double> &getResidual()</command>
get the value of the residual at pos (x,y)
</para>

<para> <!-- per function descriptions -->
<command>double getB1()</command>
get the value B1 in the least squares solution y=B1*x +B2
</para>

<para> <!-- per function descriptions -->
<command>double getB2()</command>
get the value B2 in the solution y=B1*x +B2
</para>

<para> <!-- per function descriptions -->
<command>void sanityCheck(cepMatrix<double> &matA, cepMatrix<double> &matP)</command>
ensure that all values of the matrix A P and L are consistant with the
least squares alogrithum.
</para>

<para> <!-- per function descriptions -->
<command>void calcResiduals(cepMatrix<double> &matA, cepMatrix<double> &matL )</command>
calculate the residuals of the least squares tranformation
</para>

<para> <!-- per function descriptions -->
<command>const cepMatrix<double> initResiduals(cepMatrix<double> &data)</command>
make the intial P matrix
</para>

<para> <!-- per function descriptions -->
<command>const cepMatrix<double> makeP(cepMatrix<double> &data)</command>
make the intial P matrix
</para>

<para> <!-- per function descriptions -->
<command>const cepMatrix<double> makeA(cepMatrix<double> &data)</command>
make the A matrix
</para>

<para> <!-- per function descriptions -->
<command>const cepMatrix<double> makeL(cepMatrix<double> &data)</command>
make the L matrix
</para>

<para> <!-- per function descriptions -->
<command>void calcVCV(cepMatrix<double> &matA, cepMatrix<double> &matP, cepMatrix<double> &matL)</command>
calc least square for a VCV matrix
</para>

<para> <!-- per function descriptions -->
<command>void calcRW(cepMatrix<double> &matA, cepMatrix<double> &matP, cepMatrix<double> &matL)</command>
calc least squares for a RW matrix
</para>

<para> <!-- per function descriptions -->
<command>const cepMatrix<double> reweightVCV()</command>
re-caluclates the P weighting matrix
</para>

<para> <!-- per function descriptions -->
<command>const cepMatrix<double> inverse(cepMatrix<double> &mat)</command>
calculate a 2x2 matrix inverse
</para>

<para> <!-- per function descriptions -->
<command>const cepMatrix<double> mulDiag(cepMatrix<double> &matA, cepMatrix<double> &matB)</command>
calculate A*B where B is a diagonal
</para>

<para> <!-- per function descriptions -->
<command>const cepMatrix<double> Amul(cepMatrix<double> &matA, cepMatrix<double> &matB)</command>
calculates A*B where A is the design matrix in the least squares tranform
</para>

<para> <!-- per function descriptions -->
<command>const cepMatrix<double> mulA(cepMatrix<double> &matA, cepMatrix<double> &matA)</command>
calculates B*A where A is the design matrix in the least squares tranform
</para>

DESCRIPTION END

DOCBOOK END
******************************************************************************/

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

  //iterate least squares algoritum until stable
  const cepLs & cepDoVCV(cepMatrix<double> &data);

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

  //make the intial P matrix
  const cepMatrix<double> initResiduals(cepMatrix<double> &data);

  //make the intial P matrix
  const cepMatrix<double> makeP(cepMatrix<double> &data);

  //make the A matrix
  const cepMatrix<double> makeA(cepMatrix<double> &data);

  //make the L matrix
  const cepMatrix<double> makeL(cepMatrix<double> &data);

  //calc least square for a VCV matrix
  void calcVCV(cepMatrix<double> &matA, cepMatrix<double> &matP, cepMatrix<double> &matL);

  //calc least squares for a RW matrix
  void calcRW(cepMatrix<double> &matA, cepMatrix<double> &matP, cepMatrix<double> &matL);

  //re-caluclates the P weighting matrix
  const cepMatrix<double> reweightVCV();
  
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
