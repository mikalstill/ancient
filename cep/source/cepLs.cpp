#include "cepLs.h"
 cepLs::cepLs () 
{
}

cepLs::~cepLs () 
{
}
void
cepLs::cepDoLeastSquares () 
{
  
    //this data stuff wil have to go
    cepMatrix matA, matP, matL, transAP, ans;
  double a[] = { 1.0, 1.0, 2.0, 1.0, 3.0, 1.0 };
  double p[] = { 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 };
  double l[] = { 1.0, 1.0, 1.0 };
  matA.numCols = 2;
  matA.numRows = 3;
  matA.matrix = new double[matA.numCols * matA.numRows];
  matA.matrix = a;
  matP.numCols = 3;
  matP.numRows = 3;
  matP.matrix = new double[matP.numCols * matP.numRows];
  matP.matrix = p;
  matL.numCols = 1;
  matL.numRows = 3;
  matL.matrix = new double[matL.numCols * matL.numRows];;
  matL.matrix = l;
  
    //make sure dim's of matrix are correct etc.
    sanityCheck (matA, matP, matL);
  
    //Do A(T)P
    if (isDiagonal (matP) == true)
    {
      transAP = mulDiag (transpose (matA), matP);
    }
  
  else
    {
    }
}
void
cepLs::sanityCheck (cepMatrix & matA, cepMatrix & matP, cepMatrix & matL) 
{
  
    //matA must be obs x 2
    if (matP.numCols != matP.numRows)
    {
      cout << "Error MatP has wrong dimesions\n";
      exit (1);
    }
  
    //matP must be obs x obs
    if ((matA.numCols != 2) || (matA.numRows != matP.numRows))
    {
      cout << "Error MatA has wrong dimesions\n";
      exit (1);
    }
  
    //matL must be obs x 1
    if ((matL.numRows != matP.numRows) || (matL.numCols != 1))
    {
      cout << "Error MatL has wrong dimesions\n";
      exit (1);
    }
  
    //2nd col of matA alway = 1
    for (int i = 0; i < matA.numRows * matA.numCols; i += matA.numCols)
    {
      if (matA.matrix[1 + i] != 1)
	{
	  cout << "Error Mat A has invalid data\n";
	  exit (1);
	}
    }
}

bool cepLs::isDiagonal (cepMatrix & mat) 
{
  for (int i = 0; i < mat.numRows; i++)
    {
      for (int j = 0; j < mat.numCols; j++)
	{
	  if ((i != j) && (mat.matrix[i + j * mat.numCols] != 0))
	    {
	      return false;
	    }
	}
    }
  return true;
}

cepMatrix cepLs::transpose (cepMatrix & mat) 
{
  cepMatrix transpose;
  transpose.numCols = mat.numRows;
  transpose.numRows = mat.numCols;
  transpose.matrix = new double[transpose.numCols * transpose.numRows];
  int
    x =
    0;
  for (int i = 0; i < mat.numCols; i++)
    {
      for (int j = 0; j < mat.numRows; j++)
	{
	  transpose.matrix[x++] = mat.matrix[i + j * mat.numCols];
	}
    }
  return transpose;
}


//FIX THIS!!
  cepMatrix cepLs::mulDiag (cepMatrix & matA, cepMatrix & matB) 
{
  cepMatrix ans;
  ans.numRows = matA.numRows;
  ans.numCols = matB.numCols;
  ans.matrix = new double[ans.numRows * ans.numCols];
  for (int i = 0; i < matB.numCols; i++)
    {
      ans.matrix[i] = matA.matrix[i] * matB.matrix[i + i * matB.numCols];
      ans.matrix[i + ans.numCols] = matB.matrix[i + i * matA.numCols];
    }
  return ans;
}


//Will this work???
  cepMatrix cepLs::mul (double *row, int rowElements, cepMatrix & mat) 
{
  return mat;
}

cepMatrix cepLs::inverse (cepMatrix & mat) 
{
  cepMatrix ans;
  double
    mul;
  ans.numRows = mat.numCols;
  ans.numCols = mat.numCols;
  ans.matrix = new double[ans.numRows * ans.numCols];
  if ((mat.numCols == 2) && (mat.numRows == 2))
    {
      cout << "error in matrix inverse - invalid matrix size\n";
      exit (1);
    }
  mul = 1 / (mat.matrix[0] * mat.matrix[3] - mat.matrix[1] * mat.matrix[2]);
  ans.matrix[0] = mat.matrix[3] * mul;
  ans.matrix[1] = -mat.matrix[1] * mul;
  ans.matrix[2] = -mat.matrix[2] * mul;
  ans.matrix[3] = mat.matrix[0] * mul;
  return ans;
}
