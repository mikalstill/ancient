#include "cepMatrix.h"

cepMatrix::cepMatrix(int rows, int cols)
{
  numRows = rows;
  numCols = cols;

  matrix = new double[rows * cols];
}

cepMatrix::~cepMatrix()
{
  delete [] matrix;
}

void cepMatrix::setValue(int row, int col, double value)
{
  if((row * numCols) + col > (numRows * numCols)){
    cout << "Invalid refence\n";
    exit(1);
  }
  else{
    matrix[(row * numCols) + col] = value;
  }
}

double cepMatrix::getValue(int row, int col)
{
  if((row * numCols) + col > (numRows * numCols)){
    cout << "Invalid refence\n";
    exit(1);
  }
  else{
    return matrix[(row * numCols) + col];

  }
}

int cepMatrix::getNumRows()
{
  return numRows;
}

int cepMatrix::getNumCols()
{
  return numCols;
}

const cepMatrix &cepMatrix::operator+=(const cepMatrix &B)
{
  double *tempMatrix = matrix;
  
  if((B.numRows != numRows) || (B.numCols != numCols)){
    cout << "matrix sizes wrong\n";
    exit(1);
  }
  else{
    for (int i= 0; i < numRows; i ++ ){
      for (int j=0; j < numCols; j ++ ){
        matrix[(i * numCols) + j ] = tempMatrix[(i * numCols) + j ] + B.matrix[(i * numCols) + j ];
      }
    }
  }
  
  return *this;
}

const cepMatrix &cepMatrix::operator-=(const cepMatrix &B)
{
  double *tempMatrix = matrix;
  
  if((B.numRows != numRows) || (B.numCols != numCols)){
    cout << "matrix sizes wrong\n";
    exit(1);
  }
  else{
    for (int i= 0; i < numRows; i ++ ){
      for (int j=0; j < numCols; j ++ ){
        matrix[(i * numCols) + j ] = tempMatrix[(i * numCols) + j ] - B.matrix[(i * numCols) + j ];
      }
    }
  }

  return *this;
}


const cepMatrix &cepMatrix::operator*=(const cepMatrix &B)
{
  double *tempMatrix = matrix;
  int i;
  
  if(numCols != B.numRows){
    cout << "matrix sizes wrong\n";
    exit(1);
  }
  else{
    matrix = new double [numRows * B.numCols];
    
    for(i = 0; i < numRows * B.numCols; i ++){
      matrix[i] = 0;
    }
    
	//this will be REALLY slow
    for(i = 0; i < numRows; i ++ ){
      for(int j = 0; j < B.numCols; j ++ ){
        for(int k = 0; k < B.numRows; k ++){
          matrix[(i * B.numCols) + j] += tempMatrix[k + (i * numCols)] * B.matrix[ j + (k * B.numCols) ];        
          
        }

      }
    }    
  }

  numCols = B.numCols;
  
  delete [] tempMatrix;
  return *this;
}

const cepMatrix &cepMatrix::operator*=(const double &scalar)
{
  for (int i= 0; i < numRows; i ++ ){
    for (int j=0; j < numCols; j ++ ){
      matrix[(i * numCols) + j ] *= scalar;
    }
  }

  return *this;
}

const cepMatrix &cepMatrix::transpose()
{
  double *tempMatrix = matrix;
  int temp;
  temp = numRows;
  numRows = numCols;
  numCols = temp;

  matrix = new double[numRows * numCols];
  
  int x = 0;
  for( int i = 0; i < numRows; i++ ) {
    for( int j = 0; j < numCols; j ++ ) {
        matrix[x++] = tempMatrix[ j * numRows + i ];
    }
  }

  delete [] tempMatrix;

  return *this;
}

const cepMatrix &cepMatrix::inverse()
{
	return * this;
}

const cepMatrix &cepMatrix::operator=(const cepMatrix &B)
{
  if((B.numRows != numRows) || (B.numCols != numCols)){
    cout << "matrix sizes wrong\n";
    exit(1);
  }
  else{
    for (int i= 0; i < numRows; i ++ ){
      for (int j=0; j < numCols; j ++ ){
        matrix[(i * numCols) + j ] = B.matrix[(i * numCols) + j ];
      }
    }
  }

  return *this;
}

bool cepMatrix::operator==(const cepMatrix &B)
{
  if((B.numRows != numRows) || (B.numCols != numCols)){
	return false;
  }
  else{
    for (int i= 0; i < numRows; i ++ ){
      for (int j=0; j < numCols; j ++ ){
		if (matrix[(i * numCols) + j ] != B.matrix[(i * numCols) + j ]){
	      return false;
		}
      }
    }
  }

  return true;
}

bool cepMatrix::isDiagonal()
{
 if(numCols != numRows){
	return false;
  }
 
	for( int i = 0; i < numRows; i++ ) {
	for( int j = 0; j < numCols; j ++ ) {
      if ( ( i != j ) && ( matrix[(i * numCols) + j] != 0 ) ){
        return false;
      }
    }
  }
  
  return true;
}
