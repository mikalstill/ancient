
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

template <class T>
class cepMatrix
{

public:
  cepMatrix ();
  cepMatrix (int rows, int cols);
  cepMatrix (const cepMatrix & copyMatrix);   // copy constructor

   ~cepMatrix ();

  // Caluclates the transpose of the current matrix object 
  const cepMatrix & transpose ();
  
  // ***************assingement operators***********************
  //calculates A+B where A is the current matrix object and B is another matrix
  const cepMatrix & operator+= (const cepMatrix &B);

  //calculates A-B where A is the current matrix object and B is another matrix  
  const cepMatrix & operator-= (const cepMatrix &B);

  //calculates A*B where A is the current matrix object and B is another matrix 
  const cepMatrix & operator*= (const cepMatrix &B);  

  //calculates c*A where A is the current matrix object and scalar is a scalar value  
  const cepMatrix & operator*= (const T &scalar);        // mat * scalar
  
  // ***************copy operator*****************************
  //copies A to B where A is the current matrix object and B is another matrix
  const cepMatrix & operator= (const cepMatrix &B);

  // ***************query operators****************************
  //compares A to B where A is the current matrix object and B is another matrix
  //returns true if A and B are equal, else returns false  
  bool operator== (const cepMatrix &B);

  //determies whether the A, the current matrix object is strictly Diagonal.
  //returs true if A is diagonal, else returns false  
  bool isDiagonal ();
  
  // ***************get/set methods**************************
  //gets the value of the matrix at matrix[row,col]
  T& getValue (int row, int col);
  
  //sets the value of the matrix at matrix[row,col] = value
  void setValue (int row, int col, T value);

  //gets the number of rows in the matrix  
  int getNumRows ();

  //gets the number of cols in the matrix
  int getNumCols ();
  
private:
  double *matrix;     //holds the matrix values
  int numRows,        //holds the number of rows in the matrix
      numCols;        //holds the number of cols in the matrix
};


template <class T>
cepMatrix<T>::cepMatrix ()
{
  //initalize a default matrix
  numRows = 0;
  numCols = 0;
  matrix = new T[1];

  if (matrix == NULL)
  {
    cout << "Error! could not allocate memory";
    exit (1);
  }

}

template <class T>
cepMatrix<T>::cepMatrix (int rows, int cols)
{
  //initalize a matrix
  numRows = rows;
  numCols = cols;

  matrix = new T[rows * cols];

  if (matrix == NULL)
  {
    cout << "Error! could not allocate memory";
    exit (1);
  }

}

template <class T>
cepMatrix<T>::cepMatrix (const cepMatrix & copyMatrix)
{

  numRows = copyMatrix.numRows;
  numCols = copyMatrix.numCols;


  matrix = new double[copyMatrix.numRows * copyMatrix.numCols];

  if (matrix == NULL)
  {
    cout << "Error! could not allocate memory";
    exit (1);
  }
  else
  {
    for (int i = 0; i < numRows; i++)
    {
      for (int j = 0; j < numCols; j++)
      {
        matrix[(i * numCols) + j] = copyMatrix.matrix[(i * numCols) + j];
      }
    }
  }
}

template <class T>
cepMatrix<T>::~cepMatrix ()
{
  delete[]matrix;
}

template <class T>
const cepMatrix<T> & cepMatrix<T>::transpose ()
{
  double *tempMatrix = matrix;
  int temp;

  temp = numRows;
  numRows = numCols;
  numCols = temp;

  matrix = new double[numRows * numCols];

  if (matrix == NULL)
  {
    cout << "Error! could not allocate memory";
    exit (1);
  }

  int x = 0;

  //calculates the matrix transpose
  for (int i = 0; i < numRows; i++)
  {
    for (int j = 0; j < numCols; j++)
    {
      matrix[x++] = tempMatrix[j * numRows + i];
    }
  }

  delete[]tempMatrix;

  return *this;
}

//****************Assignment operators*******************
template <class T>
const cepMatrix<T> & cepMatrix<T>::operator+= (const cepMatrix & B)
{
  double *tempMatrix = matrix;

  if ((B.numRows != numRows) || (B.numCols != numCols))
  {
    cout << "matrix sizes wrong\n";
    exit (1);
  }
  else
  {
    //calucluates A+B for each element is A and B
    for (int i = 0; i < numRows; i++)
    {
      for (int j = 0; j < numCols; j++)
      {
        matrix[(i * numCols) + j] =
          tempMatrix[(i * numCols) + j] + B.matrix[(i * numCols) + j];
      }
    }
  }

  return *this;
}

template <class T>
const cepMatrix<T> & cepMatrix<T>::operator-= (const cepMatrix & B)
{
  double *tempMatrix = matrix;

  if ((B.numRows != numRows) || (B.numCols != numCols))
  {
    cout << "matrix sizes wrong\n";
    exit (1);
  }
  else
  {
    //calucluates A-B for each element is A and B
    for (int i = 0; i < numRows; i++)
    {
      for (int j = 0; j < numCols; j++)
      {
        matrix[(i * numCols) + j] =
          tempMatrix[(i * numCols) + j] - B.matrix[(i * numCols) + j];
      }
    }
  }

  return *this;
}

template <class T>
const cepMatrix<T> & cepMatrix<T>::operator*= (const cepMatrix & B)
{
  double *tempMatrix = matrix;
  int i;

  if (numCols != B.numRows)
  {
    cout << "matrix sizes wrong\n";
    exit (1);
  }
  else
  {
    matrix = new double[numRows * B.numCols];

    if (matrix == NULL)
    {
      cout << "Error! could not allocate memory";
      exit (1);
    }

    for (i = 0; i < numRows * B.numCols; i++)
    {
      matrix[i] = 0;
    }

    //calculates A*B for each row col pair
    // this will be REALLY slow
    for (i = 0; i < numRows; i++)
    {
      for (int j = 0; j < B.numCols; j++)
      {
        for (int k = 0; k < B.numRows; k++)
        {
          matrix[(i * B.numCols) + j] +=
            tempMatrix[k + (i * numCols)] * B.matrix[j + (k * B.numCols)];

        }

      }
    }
  }

  numCols = B.numCols;

  delete[]tempMatrix;
  return *this;
}

template <class T>
const cepMatrix<T> & cepMatrix<T>::operator*= (const T &scalar)
{
  //multiply each element of A with the scalar
  for (int i = 0; i < numRows; i++)
  {
    for (int j = 0; j < numCols; j++)
    {
      matrix[(i * numCols) + j] *= scalar;
    }
  }

  return *this;
}

template <class T>
//*****************copy operator***************************
const cepMatrix<T> & cepMatrix<T>::operator= (const cepMatrix & B)
{
  numRows = B.numRows;
  numCols = B.numCols;

  matrix = new double[numRows * numCols];

  if (matrix == NULL)
  {
    cout << "Error! could not allocate memory";
    exit (1);
  }

  //copy each element of B to A
  for (int i = 0; i < numRows; i++)
  {
    for (int j = 0; j < numCols; j++)
    {
      matrix[(i * numCols) + j] = B.matrix[(i * numCols) + j];
    }
  }

  return *this;
}

//***************query operators****************************

template <class T>
bool cepMatrix<T>::operator== (const cepMatrix & B)
{
  //check if rows and cols of A are equal to rows and cols of B
  if ((B.numRows != numRows) || (B.numCols != numCols))
  {
    return false;
  }
  else
  {
    //check if each element of A and B are equal
    for (int i = 0; i < numRows; i++)
    {
      for (int j = 0; j < numCols; j++)
      {
        if (matrix[(i * numCols) + j] != B.matrix[(i * numCols) + j])
        {
          return false;
        }
      }
    }
  }

  return true;
}

template <class T>
bool cepMatrix<T>::isDiagonal ()
{
  if (numCols != numRows)
  {
    return false;
  }

  //check that each element of A[i,j] = 0 where i != j
  for (int i = 0; i < numRows; i++)
  {
    for (int j = 0; j < numCols; j++)
    {
      if ((i != j) && (matrix[(i * numCols) + j] != 0))
      {
        return false;
      }
    }
  }

  return true;
}

//***************get/set methods**************************

template <class T>
T& cepMatrix<T>::getValue (int row, int col)
{
  if (row >= numRows)
  {
      cout << "Invalid row number";
      exit(1);
  }

  if (col >= numCols)
  {
      cout << "Invalid col number";
      exit(1);
  }
  return matrix[(row * numCols) + col];
}

template <class T>
void cepMatrix<T>::setValue (int row, int col, T value)
{
  if (row >= numRows)
  {
      cout << "Invalid row number";
      exit(1);
  }

  if (col >= numCols)
  {
      cout << "Invalid col number";
      exit(1);
  }

  matrix[(row * numCols) + col] = value;
}

template <class T>
int cepMatrix<T>::getNumRows ()
{
  return numRows;
}

template <class T>
int cepMatrix<T>::getNumCols ()
{
  return numCols;
}

