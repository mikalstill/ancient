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

#include "cepLs.h"

cepLs::cepLs()
{
}

cepLs::~cepLs()
{
}

const cepLs & cepLs::cepDoLeastSquares(cepMatrix &matA, cepMatrix &matP, cepMatrix &matL)
{

  cepMatrix Atrans;
  cepMatrix AtransP, AtPInv;
  
  Atrans = matA;
  Atrans.transpose();
  
  sanityCheck(matA, matP, matL);
  
  //if matrix is diagonal
  if(matP.isDiagonal() == true){
    AtransP = mulDiag(Atrans, matP);
  }
  else{
    AtransP = Amul(Atrans, matP);
  }

  //caluclate (A^TPA)^-1
  AtPInv = mulA(AtransP, matA);
  AtPInv = inverse(AtPInv);

  //calcualte A^TPL
  AtransP *= matL;
  AtPInv *= AtransP;

  matX = AtPInv;

  calcResiduals(matA, matL);

  return *this;
}

double cepLs::getResidual(int row, int col)
{
  return residual.getValue(row, col);
}

double cepLs::getB1()
{
  return matX.getValue(0,0);
}

double cepLs::getB2()
{
  return matX.getValue(1,0);
}

const cepMatrix cepLs::Amul(cepMatrix &matA, cepMatrix &matB)
{
  cepMatrix ans(matA.getNumRows(), matB.getNumCols());

  for(int i = 0; i < ans.getNumCols(); i ++ ){
    for(int j = 0; j < ans.getNumRows(); j ++ ){
      ans.setValue(i, j, 0);
    }
  }
      
  if(matA.getNumCols() != matB.getNumRows()){
    cout << "matrix sizes wrong\n";
    exit(1);
  }

  //do A*B
  for(int i = 0; i < ans.getNumCols(); i ++ ){
    for(int j = 0; j < matB.getNumCols(); j ++ ){
      ans.setValue(0, i, (ans.getValue(0, i) + matA.getValue(0,j) * matB.getValue(i,j)));           
      ans.setValue(1, i, (ans.getValue(1, i) + matB.getValue(i,j)));
    }
  }    
  
  return ans;
}

const cepMatrix cepLs::mulA(cepMatrix &matA, cepMatrix &matB)
{
  cepMatrix ans(matA.getNumRows(), matB.getNumCols());

  for(int i = 0; i < ans.getNumCols(); i ++ ){
    for(int j = 0; j < ans.getNumRows(); j ++ ){
      ans.setValue(i, j, 0);
    }
  }
   
  if(matA.getNumCols() != matB.getNumRows()){
    cout << "matrix sizes wrong\n";
    exit(1);
  }
  
  //do B*A
  for(int i = 0; i < ans.getNumCols(); i ++ ){
    for(int j = 0; j < matB.getNumRows(); j ++ ){
      ans.setValue(i, 0, (ans.getValue(i, 0) + matA.getValue(i,j) * matB.getValue(j,0)));           
      ans.setValue(i, 1, (ans.getValue(i, 1) + matA.getValue(i,j)));
    }
  }    
  
  return ans;
}

const cepMatrix cepLs::mulDiag(cepMatrix &matA, cepMatrix &matB)
{
  cepMatrix ans(matA.getNumRows(), matB.getNumCols());
  
  if(matA.getNumCols() != matB.getNumRows()){
    cout << "matrix sizes wrong\n";
    exit(1);
  }

  //do A*B where B is diagonal
  for(int i = 0; i < ans.getNumRows(); i ++){
    for(int j = 0; j < ans.getNumCols(); j ++){
      ans.setValue(i, j, matA.getValue(i, j) * matB.getValue(j,j));
    }
  }
  
  return ans;
}

const cepMatrix cepLs::inverse(cepMatrix &mat)
{
  cepMatrix ans(mat.getNumRows(), mat.getNumCols());
  ans = mat;

  double mul;
  
  if ( ( mat.getNumCols() != 2 ) || ( mat.getNumRows() != 2 ) ){
    cout << "error in matrix inverse - invalid matrix size\n";
    exit(1);
  }

  //calulate det mat
  mul = ( mat.getValue(0, 0) * mat.getValue(1, 1) - mat.getValue(0, 1) * mat.getValue(1, 0) );

  if(mul == 0){
      cout << "Error - Matrix is not can not be inverted";
      exit(1);
  }

  //calculate matrix inverse
  ans.setValue(0,0, mat.getValue(1,1) * 1/mul);
  ans.setValue(0,1, -mat.getValue(0,1) * 1/mul);
  ans.setValue(1,0, -mat.getValue(1,0) * 1/mul);
  ans.setValue(1,1, mat.getValue(0,0) * 1/mul);
  
  return ans;
}

void cepLs::sanityCheck( cepMatrix &matA, cepMatrix &matP, cepMatrix &matL)
{
  //matA must be obs x 2
  if( matP.getNumCols() != matP.getNumRows() ){
    cout << "Error MatP has wrong dimesions\n";
    exit(1);
  }
  
  //matP must be obs x obs
  if( ( matA.getNumCols() !=2 ) || ( matA.getNumRows() != matP.getNumRows() ) ){
    cout << "Error MatA has wrong dimesions\n";
    exit(1);
  }
  
  //matL must be obs x 1
  if( ( matL.getNumRows() != matP.getNumRows() ) || (matL.getNumCols() != 1 ) ){
    cout << "Error MatL has wrong dimesions\n";
    exit(1);
  }

  //2nd col of matA alway = 1
  for(int i = 0; i < matA.getNumRows(); i ++){
    if (matA.getValue(i, matA.getNumCols()-1) != 1 ){
      cout << "Error Mat A has invalid data\n";
      exit(1);
    }
  }
}

void cepLs::calcResiduals(cepMatrix &matA, cepMatrix &matL)
{
  //calc rediuals= A*X + L
  residual = matA;
  residual *= matX;
  residual += matL;
}
