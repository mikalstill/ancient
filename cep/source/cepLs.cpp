#include "cepLs.h"

cepLs::cepLs()
{
}

cepLs::~cepLs()
{
}

const cepMatrix cepLs::cepDoLeastSquares(cepMatrix &matA, cepMatrix &matP, cepMatrix &matL)
{

  cepMatrix Atrans;
  cepMatrix AtransP, AtPInv;
  
  Atrans = matA;
  Atrans.transpose();
  
  sanityCheck(matA, matP, matL);
  

  if(matP.isDiagonal() == true){
    AtransP = mulDiag(Atrans, matP);
  }
  else{
    AtransP = Amul(Atrans, matP);
  }
  
  AtPInv = mulA(AtransP, matA);
  AtPInv = inverse(AtPInv);
  
  AtransP *= matL;
  AtPInv *= AtransP;
  return AtPInv;
}

const cepMatrix cepLs::Amul(cepMatrix &matA, cepMatrix &matB)
{
  cepMatrix ans(matA.getNumRows(), matB.getNumCols());
  
  for(int i = 0; i < ans.getNumRows(); i ++){
    for(int j = 0; j < ans.getNumCols(); j ++){
      ans.setValue(i, j, 0);
    }
  }
  
  if(matA.getNumCols() != matB.getNumRows()){
    cout << "matrix sizes wrong\n";
    exit(1);
  }
  
  for(i = 0; i < ans.getNumCols(); i ++ ){
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
  
  for(int i = 0; i < ans.getNumRows(); i ++){
    for(int j = 0; j < ans.getNumCols(); j ++){
      ans.setValue(i, j, 0);
    }
  }
  
  if(matA.getNumCols() != matB.getNumRows()){
    cout << "matrix sizes wrong\n";
    exit(1);
  }
  
  for(i = 0; i < ans.getNumCols(); i ++ ){
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
  
  mul = 1/( mat.getValue(0, 0) * mat.getValue(1, 1) - mat.getValue(0, 1) * mat.getValue(1, 0) );
  
  ans.setValue(0,0, mat.getValue(1,1) * mul);
  ans.setValue(0,1, -mat.getValue(0,1) * mul);
  ans.setValue(1,0, -mat.getValue(1,0) * mul);
  ans.setValue(1,1, mat.getValue(0,0) * mul);
  
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


