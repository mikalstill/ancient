
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
  * Copyright (C) Daniel Fernandez - minor changes to some type references
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
cepMatrix


PURPOSE <!-- use cases -->
Provides a matrix class template data container for all mathematical operations.
The matrix object can either be 2D or 3D

SYNOPSIS START
<!-- how to instantiate one? -->
The follwing example show how to create a cepMatrix.

Declaring a NULL matrix:-
cepMatrix<T> matrix()

Declaring a 2D matrix:-
cepMatrix<T> matrix(int numRows, int numCols)

Declaring a 3D matrix:-
cepMatrix<T> matrix(int numRows, int numCols, int numTables)

SYNOPSIS END

DESCRIPTION START
<!-- description goes here -->
<para>An implementation of the <command>cepMatrix</command> template class
data container.
</para>

<para> <!-- per function descriptions -->
<command>const cepMatrix & operator+= (const cepMatrix &B)</command>
calculates A+B where A is the current matrix object and B is another matrix
</para>

<para> <!-- per function descriptions -->
<command>const cepMatrix & operator-= (const cepMatrix &B)</command>
calculates A-B where A is the current matrix object and B is another matrix
</para>

<para> <!-- per function descriptions -->
<command>const cepMatrix & operator*= (const cepMatrix &B)</command>
calculates A*B where A is the current matrix object and B is another matrix
</para>

<para> <!-- per function descriptions -->
<command>const cepMatrix & operator*= (const T &scalar)
calculates c*A where A is the current matrix object and scalar is a scalar value
</para>

<para> <!-- per function descriptions -->
<command>const cepMatrix & operator= (const cepMatrix &B)</command>
copies A to B where A is the current matrix object and B is another matrix
</para>

<para> <!-- per function descriptions -->
<command>bool operator== (const cepMatrix &B)</command>
compares A to B where A is the current matrix object and B is another matrix
returns true if A and B are equal, else returns false
</para>

<para> <!-- per function descriptions -->
<command>bool operator!= (const cepMatrix &B)</command>
compares A to B where A is the current matrix object and B is another matrix
returns true if A and B are equal, else returns false
</para>

<para> <!-- per function descriptions -->
<command>bool isDiagonal ()</command>
determies whether the A, the current matrix object is strictly Diagonal.
returs true if A is diagonal, else returns false
</para>

<para> <!-- per function descriptions -->
<command>const T& getValue (int row, int col) const</command>
gets the value of the matrix at matrix[row,col]
</para>

<para> <!-- per function descriptions -->
<command>void setValue (const int row, const int col, const T & value)</command>
sets the value of the matrix at matrix[row,col] = value
</para>

<para> <!-- per function descriptions -->
<command>const int getNumRows () const</command>
gets the number of rows in the matrix
</para>

<para> <!-- per function descriptions -->
<command>const int getNumCols () const</command>
gets the number of cols in the matrix
</para>

<para> <!-- per function descriptions -->
<command>const T& getValue (int row, int col, int tab) const</command>
gets the value of the matrix at matrix[row,col,inst]
</para>

<para> <!-- per function descriptions -->
<command>void setValue (const int row, const int col, const int tab, const T & value)</command>
sets the value of the matrix at matrix[row,col,inst] = value
</para>

<para> <!-- per function descriptions -->
<command>const int getNumTables() const</command>
gets the number of tables in the matrix
</para>
    
DESCRIPTION END

DOCBOOK END
******************************************************************************/

#ifndef __CEPMATRIX_H
#define __CEPMATRIX_H

#include <iostream.h>
#include <math.h>
#include <stdlib.h>

#include "cepError.h"

#define debug

template <class T>
class cepMatrix
{

public:
  cepMatrix ();
  cepMatrix (int rows, int cols);
  cepMatrix (int rows, int cols, int tab);
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

  //compares A to B where A is the current matrix object and B is another matrix
  //returns true if A and B are equal, else returns false
  bool operator!= (const cepMatrix &B);

  //determies whether the A, the current matrix object is strictly Diagonal.
  //returs true if A is diagonal, else returns false  
  bool isDiagonal ();
  
  // ***************get/set methods**************************
  //gets the value of the matrix at matrix[row,col]
  const T& getValue (int row, int col) const;
  
  //sets the value of the matrix at matrix[row,col] = value
  void setValue (const int row, const int col, const T & value);

  //gets the number of rows in the matrix  
  const int getNumRows () const;

  //gets the number of cols in the matrix
  const int getNumCols () const;

  // ***************get/set methods**************************
  //gets the value of the matrix at matrix[row,col,inst]
  const T& getValue (int row, int col, int tab) const;

  //sets the value of the matrix at matrix[row,col,inst] = value
  void setValue (const int row, const int col, const int tab, const T & value);

  //gets the number of tables in the matrix
  const int getNumTables() const;
  
private:
  T *matrix;          //holds the matrix values 
  T **tables;         //holds the values of a 3D matrix
  int numRows,        //holds the number of rows in the matrix
      numCols,        //holds the number of cols in the matrix
      numTables;      //holds the number if instances of matricies
};

template <class T>
cepMatrix<T>::cepMatrix ()
{
  //initalize a default matrix
  numRows = 0;
  numCols = 0;
  numTables = 0;

  //make both pointers NULL by default
  matrix = NULL;
  tables = NULL;

}

template <class T>
cepMatrix<T>::cepMatrix (int rows, int cols)
{
  //initalize a matrix
  numRows = rows;
  numCols = cols;
  numTables = 1;

  tables = NULL;     //set 3D matrix pointer NULL, using 2D matrix
  matrix = new T[rows * cols];

  if (matrix == NULL)
  {
    #ifdef debug
      cout << "Error! Could not allocate memory for Matrix" << endl;
      exit(1);
    #endif
    
    cepError("Error! Could not allocate memory for Matrix", cepError::sevErrorRecoverable);
  }

}

template <class T>
cepMatrix<T>::cepMatrix (int rows, int cols, int tab)
{
  //initalize a matrix
  numRows = rows;
  numCols = cols;
  numTables = tab;

  matrix = NULL; //setting matrix NULL, using 3D matrix
  tables = new T*[numTables];
  if (tables == NULL)
  {
    #ifdef debug
      cout << "Error! Could not allocate memory for Matrix" << endl;
      exit(1);
    #endif
    
    cepError("Error! Could not allocate memory for Matrix", cepError::sevErrorRecoverable);
  }
  
  for(int i = 0; i < numTables; i++)
  {
    tables[i] = new T[numRows * numCols];
  
    if (tables[i] == NULL)
    {
      #ifdef debug
        cout << "Error! Could not allocate memory for Matrix" << endl;
        exit(1);
      #endif
    
      cepError("Error! Could not allocate memory for Matrix", cepError::sevErrorRecoverable);
    }
  }
}

template <class T>
cepMatrix<T>::cepMatrix (const cepMatrix & copyMatrix)
{

  numRows = copyMatrix.numRows;
  numCols = copyMatrix.numCols;
  numTables = copyMatrix.numTables;

  if (copyMatrix.matrix == NULL)
  {
    tables = new T*[numTables];

    if (tables == NULL)
    {
      #ifdef debug
        cout << "Error! Could not allocate memory for Matrix" << endl;
        exit(1);
      #endif
    
      cepError("Error! Could not allocate memory for Matrix", cepError::sevErrorRecoverable);
    }

    for(int i = 0; i < numTables; i++)
    {
      tables[i] = new T[numRows * numCols];

      if (tables[i] == NULL)
      {
        #ifdef debug
          cout << "Error! Could not allocate memory for Matrix" << endl;
          exit(1);
        #endif
    
        cepError("Error! Could not allocate memory for Matrix", cepError::sevErrorRecoverable);
      }
    }

    for (int i = 0; i < numTables; i++)
    {
      for (int j = 0; j < numRows; j++)
      {
        for(int k = 0; k < numCols; k ++)
        {
          tables[i][(j * numCols) + k] = copyMatrix.tables[i][(j * numCols) + k];
        }
      }
    } 
  }
  else
  {
    matrix = new T[copyMatrix.numRows * copyMatrix.numCols];

    if (matrix == NULL)
    {
      #ifdef debug
        cout << "Error! Could not allocate memory for Matrix" << endl;
        exit(1);
      #endif
    
      cepError("Error! Could not allocate memory for Matrix", cepError::sevErrorRecoverable);
    }

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
  if(matrix == NULL)
  {
    for(int i =0; i < numTables; i ++ )
    {
      delete[]tables[i];
    }
    delete[]tables;
  }
  else
  {
    delete[]matrix;
  }  
}

template <class T>
const cepMatrix<T> & cepMatrix<T>::transpose ()
{
  double *tempMatrix = matrix;
  int temp;

  temp = numRows;
  numRows = numCols;
  numCols = temp;

  if (matrix == NULL)
  {
    #ifdef debug
      cout << "Error! Can not transpose as 3D matrix" << endl;
      exit(1);
    #endif
    
    cepError("Error! Can Not Transpose a 3D matrix", cepError::sevErrorRecoverable);
  }
  
  matrix = new double[numRows * numCols];

  if (matrix == NULL)
  {
    #ifdef debug
      cout << "Error! Could not allocate memory for Matrix" << endl;
      exit(1);
    #endif
    
    cepError("Error! Could not allocate memory for Matrix", cepError::sevErrorRecoverable);
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

// ****************Assignment operators*******************
template <class T>
const cepMatrix<T> & cepMatrix<T>::operator+= (const cepMatrix & B)
{
  double *tempMatrix = matrix;

  if (matrix == NULL)
  {
    cepError("Error! Could not allocate memory for Matrix", cepError::sevErrorRecoverable);
  }
  
  if ((B.numRows != numRows) || (B.numCols != numCols))
  {
    cepError("Error! Matrix sizes are wrong", cepError::sevErrorRecoverable);
  }
  else
  {
    //calucluates A+B for each element is A and B
    for (int i = 0; i < numRows; i++)
    {
      for (int j = 0; j < numCols; j++)
      {
        matrix[(i * numCols) + j] = tempMatrix[(i * numCols) + j] + B.matrix[(i * numCols) + j];
      }
    }
  }

  return *this;
}

template <class T>
const cepMatrix<T> & cepMatrix<T>::operator-= (const cepMatrix & B)
{
  double *tempMatrix = matrix;

  if (matrix == NULL)
  {
    #ifdef debug
      cout << "Error! Can not use the -  operator on a 3D Matrix" << endl;
      exit(1);
    #endif
    
    cepError("Error! Can not use the - operator on a 3D Matrix", cepError::sevErrorRecoverable);
  }
  
  if ((B.numRows != numRows) || (B.numCols != numCols))
  {
    #ifdef debug
      cout << "Error! Matrix sizes are wrong" << endl;
      exit(1);
    #endif
    
    cepError("Error! Matrix sizes are wrong", cepError::sevErrorRecoverable);
  }
  else
  {
    //calucluates A-B for each element is A and B
    for (int i = 0; i < numRows; i++)
    {
      for (int j = 0; j < numCols; j++)
      {
        matrix[(i * numCols) + j] = tempMatrix[(i * numCols) + j] - B.matrix[(i * numCols) + j];
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

  if (matrix == NULL)
  {
    #ifdef debug
      cout << "Error! Can not use the - operator on a 3D Matrix" << endl;
      exit(1);
    #endif

    cepError("Error! Can not use the - operator on a 3D Matrix", cepError::sevErrorRecoverable);
  }
  
  if (numCols != B.numRows)
  {

    #ifdef debug
      cout << "Error! Matrix sizes are wrong" << endl;
      exit(1);
    #endif
    
    cepError("Error! Matrix sizes are wrong", cepError::sevErrorRecoverable);
  }
  else
  {
    matrix = new double[numRows * B.numCols];

    if (matrix == NULL)
    {

      #ifdef debug
        cout << "Error! Can not allocate memory for matrix" << endl;
        exit(1);
      #endif
      
      cepError("Error! Could not allocate memory for Matrix", cepError::sevErrorRecoverable);
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
          matrix[(i * B.numCols) + j] += tempMatrix[k + (i * numCols)] * B.matrix[j + (k * B.numCols)];

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
  if (matrix == NULL)
  {
    #ifdef debug
      cout << "Error! Can not use the * operator on a 3D Matrix" << endl;
      exit(1);
    #endif
    
    cepError("Error! Can not use the * operator on a 3D Matrix", cepError::sevErrorRecoverable);
  }
  
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
// *****************copy operator***************************
const cepMatrix<T> & cepMatrix<T>::operator= (const cepMatrix & B)
{
  numRows = B.numRows;
  numCols = B.numCols;
  numTables = B.numTables;

  if( B.matrix == NULL)
  {
    tables = new T*[numTables];

    if (tables == NULL)
    {
      #ifdef debug
        cout << "Error! Could not allocate memory for matrix" << endl;
        exit(1);
      #endif

      cepError("Error! Could not allocate memory for Matrix", cepError::sevErrorRecoverable);
    }

    for(int i = 0; i < numTables; i++)
    {
      tables[i] = new T[numRows * numCols];

      if (tables[i] == NULL)
      {
        #ifdef debug
          cout << "Error! Could not allocate memory for matrix" << endl;
          exit(1);
        #endif

        cepError("Error! Could not allocate memory for Matrix", cepError::sevErrorRecoverable);
      }
    }
    
    for (int i = 0; i < numTables; i++)
    {
      for (int j = 0; j < numRows; j++)
      {
        for(int k = 0; k < numCols; k ++)
        {
          tables[i][(j * numCols) + k] = B.tables[i][(j * numCols) + k]; 
        }
      }  
    }
  }
  else
  {
    matrix = new T[numRows * numCols]; //just in case this line breaks - daniel changed this from double to T.

    if (matrix == NULL)
    {
      #ifdef debug
        cout << "Error! Could not allocate memory for matrix" << endl;
        exit(1);
      #endif

      cepError("Error! Could not allocate memory for Matrix", cepError::sevErrorRecoverable);
    }

    //copy each element of B to A for 2D matrix
    for (int i = 0; i < numRows; i++)
    {
      for (int j = 0; j < numCols; j++)
      {
        matrix[(i * numCols) + j] = B.matrix[(i * numCols) + j];
      }
    }
  }

  return *this;
}

// ***************query operators****************************

template <class T>
bool cepMatrix<T>::operator== (const cepMatrix & B)
{
  if (matrix == NULL)
  {
    #ifdef debug
      cout << "Error! Could not use the == operate on a 3D matrix" << endl;
      exit(1);
    #endif

    cepError("Error! Can not use the == operator on a 3D Matrix", cepError::sevErrorRecoverable);
  }
  
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
bool cepMatrix<T>::operator!= (const cepMatrix & B)
{
  return !(*this==B);
}
template <class T>
bool cepMatrix<T>::isDiagonal ()
{
  if (matrix == NULL)
  {
    #ifdef debug
      cout << "Error! Can not use the isDiagonal operator on a 3D matrix" << endl;
      exit(1);
    #endif

    cepError("Error! Can not use the isDiagonal operator on a 3D Matrix", cepError::sevErrorRecoverable);
  }
  
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

// ***************get/set methods**************************

template <class T>
const T& cepMatrix<T>::getValue (int row, int col) const
{
  if (matrix == NULL)
  {
    #ifdef debug
      cout << "Error! The matrix contains no values" << endl;
      exit(1);
    #endif

    cepError("Error! The matrix contains no values", cepError::sevErrorRecoverable);
  }
  
  if (row >= numRows)
  {
    #ifdef debug
      cout << "Error! Invalid row number" << endl;
      exit(1);
    #endif

    cepError("Error Invalid Row Number", cepError::sevErrorRecoverable);
  }

  if (col >= numCols)
  {
    #ifdef debug
      cout << "Error! Invalid col number" << endl;
      exit(1);
    #endif

    cepError("Error Invalid Col Number", cepError::sevErrorRecoverable);
  }

  return matrix[(row * numCols) + col];
}

template <class T>
void cepMatrix<T>::setValue (const int row, const int col, const T & value)
{ 
  if (row >= numRows)
  {
    #ifdef debug
      cout << "Error! Invalid row number" << endl;
      exit(1);
    #endif

    cepError("Error Invalid Row Number", cepError::sevErrorRecoverable);
  }

  if (col >= numCols)
  {
    #ifdef debug
      cout << "Error! Invalid col number" << endl;
      exit(1);
    #endif

    cepError("Error Invalid Col Number", cepError::sevErrorRecoverable);
  }

  matrix[(row * numCols) + col] = value;
}

template <class T>
const int cepMatrix<T>::getNumRows () const
{
  return numRows;
}

template <class T>
const int cepMatrix<T>::getNumCols () const
{
  return numCols;
}

// ***************get/set methods 3D matrix**************************

template <class T>
const T& cepMatrix<T>::getValue (int row, int col, int tab) const
{
  if (tables == NULL)
  {
    #ifdef debug
      cout << "Error! The matrix contains no values" << endl;
      exit(1);
    #endif

    cepError("Error! The matrix contains no values", cepError::sevErrorRecoverable);
  }
  
  if (row >= numRows)
  {
    #ifdef debug
      cout << "Error! Invalid row number" << endl;
      exit(1);
    #endif

    cepError("Error Invalid Row Number", cepError::sevErrorRecoverable);
  }

  if (col >= numCols)
  {
    #ifdef debug
      cout << "Error! Invalid col number" << endl;
      exit(1);
    #endif

    cepError("Error Invalid Col Number", cepError::sevErrorRecoverable);
  }
  if (tab >= numTables)
  {
    #ifdef debug
      cout << "Error! Invalid table number" << endl;
      exit(1);
    #endif

    cepError("Error Invalid Table Number", cepError::sevErrorRecoverable);
  }

  return tables[tab][(row * numCols) + col];
}

template <class T>
void cepMatrix<T>::setValue (const int row, const int col, const int tab, const T & value)
{
  if (row >= numRows)
  {
    #ifdef debug
      cout << "Error! Invalid row number" << endl;
      exit(1);
    #endif

    cepError("Error Invalid Row Number", cepError::sevErrorRecoverable);
  }

  if (col >= numCols)
  {
    #ifdef debug
      cout << "Error! Invalid col number" << endl;
      exit(1);
    #endif

    cepError("Error Invalid Col Number", cepError::sevErrorRecoverable);
  }
  if (tab >= numTables)
  {
    #ifdef debug
      cout << "Error! Invalid table number" << endl;
      exit(1);
    #endif

    cepError("Error Invalid Table Number", cepError::sevErrorRecoverable);
  }

  tables[tab][(row * numCols) + col] = value;
 
}

template <class T>
const int cepMatrix<T>::getNumTables () const
{
  return numTables;
}

#endif //end __cepMatrix_H
