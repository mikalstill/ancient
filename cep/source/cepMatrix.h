
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

/******************************************************************************
DOCBOOK START

FUNCTION cepMatrix


PURPOSE Provides a matrix class template data container for all mathematical operations.
The matrix object can either be 2D or 3D

SYNOPSIS START
The follwing example show how to create a cepMatrix.

Declaring a NULL matrix:-
cepMatrix<T> matrix()

Declaring a 2D matrix:-
cepMatrix<T> matrix(int rows, int cols)

<para><itemizedlist>
  <listitem><para>rows:- the number of rows in the matrix</para></listitem>
  <listitem><para>cols:- the number of colums in the matrix</para></listitem>
</itemizedlist></para>


Declaring a 3D matrix:-
cepMatrix<T> matrix(int rows, int cols, int tabs)

<para><itemizedlist>
  <listitem><para>rows:- the number of rows in the matrix</para></listitem>
  <listitem><para>cols:- the number of colums in the matrix</para></listitem>
  <listitem><para>tabs:- the number of tables in the matrix</para></listitem>
</itemizedlist></para>

SYNOPSIS END

DESCRIPTION START
An implementation of the <command>cepMatrix</command> template class
data container.
</para>

<para> 
<command>const cepMatrix & operator+= (const cepMatrix &B)</command>
Calculates A+B.
This function can not be used with 3D matricies.
<para><itemizedlist>
  <listitem><para>B:- the matrix to be added to the current matrix object</para></listitem>
</itemizedlist></para>

</para>

<para> 
<command>const cepMatrix & operator-= (const cepMatrix &B)</command>
Calculates A-B.
This function can not be used with 3D matricies.
<para><itemizedlist>
  <listitem><para>B:- the matrix to be subtracted to the current matrix object</para></listitem>
</itemizedlist></para>
</para>

<para> 
<command>const cepMatrix & operator*= (const cepMatrix &B)</command>
Calculates A*B.
<para><itemizedlist>
  <listitem><para>B:- the matrix to be multiplied with the current matrix object</para></listitem>
</itemizedlist></para>
</para>

<para> 
<command>const cepMatrix & operator*= (const T &scalar)
Calculates c*A.
This function can not be used with 3D matricies.
<para><itemizedlist>
  <listitem><para>c:- the scalar value that the current matrix object is to be multiplied by</para></listitem>
</itemizedlist></para>
</para>

<para> 
<command>const cepMatrix & operator= (const cepMatrix &B)</command>
Sets A=B.
<para><itemizedlist>
  <listitem><para>B:- the matrix that the current matrix object is to be set to</para></listitem>
</itemizedlist></para>
</para>

<para> 
<command>bool operator== (const cepMatrix &B)</command>
Compares A to B and returns true if A is equal to B, otherwise false is returned.
This function can not be used with 3D matricies.
<para><itemizedlist>
  <listitem><para>B:- the matrix to be compared to the current matrix object</para></listitem>
</itemizedlist></para>
</para>

<para> 
<command>bool operator!= (const cepMatrix &B)</command>
Compares A to B and returns true if A is not equal B, otherwise false is returned.
This function can not be used with 3D matricies.
<para><itemizedlist>
  <listitem><para>B:- the matrix to be compared to the current matrix object</para></listitem>
</itemizedlist></para>

</para>

<para> 
<command>bool isDiagonal ()</command>
Determies whether this matrix object is a strictly Diagonal matrix.
This fuction returns true if the matrix is diagonal, else false is returned.
This function can not be used with 3D matricies.
</para>

<para> 
<command>const T& getValue (int row, int col) const</command>
Returns the value of the matrix the specified point.
This function can not be used with 3D matricies.
<para><itemizedlist>
  <listitem><para>row:- the row number of the value</para></listitem>
  <listitem><para>col:- the colum number of the value</para></listitem>
</itemizedlist></para>
</para>

<para> 
<command>void setValue (const int row, const int col, const T & value)</command>
Sets the value of the matrix at the specified point
This function can not be used with 3D matricies.
<para><itemizedlist>
  <listitem><para>row:- the row number where the value is to be inserted</para></listitem>
  <listitem><para>col:- the colum number where the value is to be inserted</para></listitem>
  <listitem><para>value:- the value of the given matrix point</para></listitem>
</itemizedlist></para>

</para>

<para> 
<command>const int getNumRows () const</command>
Gets the number of rows in the matrix.
</para>

<para> 
<command>const int getNumCols () const</command>
Gets the number of colums in the matrix.
</para>

<para> 
<command>const T& getValue (int row, int col, int tab) const</command>
Gets the value of a 3D matrix at a specfied point
This function can not be used with 2D matricies.
<para><itemizedlist>
  <listitem><para>row:- the row number of the value</para></listitem>
  <listitem><para>col:- the colum number of the value</para></listitem>
  <listitem><para>tab:- the table number of the value</para></listitem>
</itemizedlist></para>
</para>

<para> 
<command>void setValue (const int row, const int col, const int tab, const T & value)</command>
Sets the value of the 3D matrix at the specified point.
This function can not be used with 2D matricies.
<para><itemizedlist>
  <listitem><para>row:- the row number where the value is to be inserted</para></listitem>
  <listitem><para>col:- the colum number where the value is to be inserted</para></listitem>
  <listitem><para>tab:- the table number where the value is to be inserted</para></listitem>
  <listitem><para>value:- the value of the given matrix point</para></listitem>
</itemizedlist></para>
</para>

<para> 
<command>const int getNumTables() const</command>
Gets the number of tables in the matrix.
This function can not be used with 2D matricies.
    
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
  cepMatrix (const cepMatrix &B);   // copy constructor

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
  T *m_matrix;          //holds the matrix values 
  T **m_tables;         //holds the values of a 3D matrix
  int m_numRows,        //holds the number of rows in the matrix
      m_numCols,        //holds the number of cols in the matrix
      m_numTables;      //holds the number if instances of matricies
};

template <class T>
cepMatrix<T>::cepMatrix ()
{
  //initalize a default matrix
  m_numRows = 0;
  m_numCols = 0;
  m_numTables = 0;

  //make both pointers NULL by default
  m_matrix = NULL;
  m_tables = NULL;

}

template <class T>
cepMatrix<T>::cepMatrix (int rows, int cols)
{
  //initalize a matrix
  m_numRows = rows;
  m_numCols = cols;
  m_numTables = 1;

  m_tables = NULL;     //set 3D matrix pointer NULL, using 2D matrix
  m_matrix = new T[rows * cols];

  if (m_matrix == NULL)
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
  m_numRows = rows;
  m_numCols = cols;
  m_numTables = tab;

  m_matrix = NULL; //setting matrix NULL, using 3D matrix
  m_tables = new T*[m_numTables];
  if (m_tables == NULL)
  {
    #ifdef debug
      cout << "Error! Could not allocate memory for Matrix" << endl;
      exit(1);
    #endif
    
    cepError("Error! Could not allocate memory for Matrix", cepError::sevErrorRecoverable);
  }
  
  for(int i = 0; i < m_numTables; i++)
  {
    m_tables[i] = new T[m_numRows * m_numCols];
  
    if (m_tables[i] == NULL)
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
cepMatrix<T>::cepMatrix (const cepMatrix & B)
{
  m_numRows = B.m_numRows;
  m_numCols = B.m_numCols;
  m_numTables = B.m_numTables;

  if( B.m_tables != NULL)
  {
    m_tables = new T*[m_numTables];

    if (m_tables == NULL)
    {
      #ifdef debug
        cout << "Error! Could not allocate memory for matrix" << endl;
        exit(1);
      #endif

      cepError("Error! Could not allocate memory for Matrix", cepError::sevErrorRecoverable);
    }

    for(int i = 0; i < m_numTables; i++)
    {
      m_tables[i] = new T[m_numRows * m_numCols];

      if (m_tables[i] == NULL)
      {
        #ifdef debug
          cout << "Error! Could not allocate memory for matrix" << endl;
          exit(1);
        #endif

        cepError("Error! Could not allocate memory for Matrix", cepError::sevErrorRecoverable);
      }
    }

    for (int i = 0; i < m_numTables; i++)
    {
      for (int j = 0; j < m_numRows; j++)
      {
        for(int k = 0; k < m_numCols; k ++)
        {
          m_tables[i][(j * m_numCols) + k] = B.m_tables[i][(j * m_numCols) + k];
        }
      }
    }
    m_matrix = NULL;
  }
  else
  {
    m_matrix = new T[m_numRows * m_numCols];

    if (m_matrix == NULL)
    {
      #ifdef debug
        cout << "Error! Could not allocate memory for matrix" << endl;
        exit(1);
      #endif

      cepError("Error! Could not allocate memory for Matrix", cepError::sevErrorRecoverable);
    }

    //copy each element of B to A for 2D m_matrix
    for (int i = 0; i < m_numRows; i++)
    {
      for (int j = 0; j < m_numCols; j++)
      {
        m_matrix[(i * m_numCols) + j] = B.m_matrix[(i * m_numCols) + j];
      }
    }
    m_tables = NULL;    
  }
}

template <class T>
cepMatrix<T>::~cepMatrix ()
{
   if(m_tables != NULL)
  {
    for(int i =0; i < m_numTables; i ++ )
    {
      delete[]m_tables[i];
    }
    delete[]m_tables;
  }

  if(m_matrix != NULL)
  {
    delete[]m_matrix;
  }  
}

template <class T>
const cepMatrix<T> & cepMatrix<T>::transpose ()
{
  T *tempMatrix = m_matrix;
  int temp;

  temp = m_numRows;
  m_numRows = m_numCols;
  m_numCols = temp;

  if (m_matrix == NULL)
  {
    #ifdef debug
      cout << "Error! Can not transpose as 3D matrix" << endl;
      exit(1);
    #endif
    
    cepError("Error! Can Not Transpose a 3D matrix", cepError::sevErrorRecoverable);
  }
  
  m_matrix = new T[m_numRows * m_numCols];

  if (m_matrix == NULL)
  {
    #ifdef debug
      cout << "Error! Could not allocate memory for Matrix" << endl;
      exit(1);
    #endif
    
    cepError("Error! Could not allocate memory for Matrix", cepError::sevErrorRecoverable);
  }

  int x = 0;

  //calculates the matrix transpose
  for (int i = 0; i < m_numRows; i++)
  {
    for (int j = 0; j < m_numCols; j++)
    {
      m_matrix[x++] = tempMatrix[j * m_numRows + i];
    }
  }

  delete[]tempMatrix;

  return *this;
}

//****************Assignment operators*******************
template <class T>
const cepMatrix<T> & cepMatrix<T>::operator+= (const cepMatrix & B)
{
  T *tempMatrix = m_matrix;

  if (m_matrix == NULL)
  {
    cepError("Error! Could not allocate memory for Matrix", cepError::sevErrorRecoverable);
  }
  
  if ((B.m_numRows != m_numRows) || (B.m_numCols != m_numCols))
  {
    cepError("Error! Matrix sizes are wrong", cepError::sevErrorRecoverable);
  }
  else
  {
    //calucluates A+B for each element is A and B
    for (int i = 0; i < m_numRows; i++)
    {
      for (int j = 0; j < m_numCols; j++)
      {
        m_matrix[(i * m_numCols) + j] = tempMatrix[(i * m_numCols) + j] + B.m_matrix[(i * m_numCols) + j];
      }
    }
  }

  return *this;
}

template <class T>
const cepMatrix<T> & cepMatrix<T>::operator-= (const cepMatrix & B)
{
  T *tempMatrix = m_matrix;

  if (m_matrix == NULL)
  {
    #ifdef debug
      cout << "Error! Can not use the -  operator on a 3D Matrix" << endl;
      exit(1);
    #endif
    
    cepError("Error! Can not use the - operator on a 3D Matrix", cepError::sevErrorRecoverable);
  }
  
  if ((B.m_numRows != m_numRows) || (B.m_numCols != m_numCols))
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
    for (int i = 0; i < m_numRows; i++)
    {
      for (int j = 0; j < m_numCols; j++)
      {
        m_matrix[(i * m_numCols) + j] = tempMatrix[(i * m_numCols) + j] - B.m_matrix[(i * m_numCols) + j];
      }
    }
  }

  return *this;
}

template <class T>
const cepMatrix<T> & cepMatrix<T>::operator*= (const cepMatrix & B)
{
  T *tempMatrix = m_matrix;
  int i;

  if (m_matrix == NULL)
  {
    #ifdef debug
      cout << "Error! Can not use the - operator on a 3D Matrix" << endl;
      exit(1);
    #endif

    cepError("Error! Can not use the - operator on a 3D Matrix", cepError::sevErrorRecoverable);
  }
  
  if (m_numCols != B.m_numRows)
  {

    #ifdef debug
      cout << "Error! Matrix sizes are wrong" << endl;
      exit(1);
    #endif
    
    cepError("Error! Matrix sizes are wrong", cepError::sevErrorRecoverable);
  }
  else
  {
    m_matrix = new T[m_numRows * B.m_numCols];

    if (m_matrix == NULL)
    {

      #ifdef debug
        cout << "Error! Can not allocate memory for matrix" << endl;
        exit(1);
      #endif
      
      cepError("Error! Could not allocate memory for Matrix", cepError::sevErrorRecoverable);
    }

    for (i = 0; i < m_numRows * B.m_numCols; i++)
    {
      m_matrix[i] = 0;
    }

    //calculates A*B for each row col pair
    // this will be REALLY slow
    for (i = 0; i < m_numRows; i++)
    {
      for (int j = 0; j < B.m_numCols; j++)
      {
        for (int k = 0; k < B.m_numRows; k++)
        {
          m_matrix[(i * B.m_numCols) + j] += tempMatrix[k + (i * m_numCols)] * B.m_matrix[j + (k * B.m_numCols)];

        }

      }
    }
  }

  m_numCols = B.m_numCols;

  delete[]tempMatrix;
  return *this;
}

template <class T>
const cepMatrix<T> & cepMatrix<T>::operator*= (const T &scalar)
{
  if (m_matrix == NULL)
  {
    #ifdef debug
      cout << "Error! Can not use the * operator on a 3D Matrix" << endl;
      exit(1);
    #endif
    
    cepError("Error! Can not use the * operator on a 3D Matrix", cepError::sevErrorRecoverable);
  }
  
  //multiply each element of A with the scalar
  for (int i = 0; i < m_numRows; i++)
  {
    for (int j = 0; j < m_numCols; j++)
    {
      m_matrix[(i * m_numCols) + j] *= scalar;
    }
  }

  return *this;
}

template <class T>
//*****************copy operator***************************
const cepMatrix<T> & cepMatrix<T>::operator= (const cepMatrix & B)
{
  m_numRows = B.m_numRows;
  m_numCols = B.m_numCols;
  m_numTables = B.m_numTables;

  if( B.m_matrix == NULL)
  {
    m_tables = new T*[m_numTables];

    if (m_tables == NULL)
    {
      #ifdef debug
        cout << "Error! Could not allocate memory for matrix" << endl;
        exit(1);
      #endif

      cepError("Error! Could not allocate memory for Matrix", cepError::sevErrorRecoverable);
    }

    for(int i = 0; i < m_numTables; i++)
    {
      m_tables[i] = new T[m_numRows * m_numCols];

      if (m_tables[i] == NULL)
      {
        #ifdef debug
          cout << "Error! Could not allocate memory for matrix" << endl;
          exit(1);
        #endif

        cepError("Error! Could not allocate memory for Matrix", cepError::sevErrorRecoverable);
      }
    }
    
    for (int i = 0; i < m_numTables; i++)
    {
      for (int j = 0; j < m_numRows; j++)
      {
        for(int k = 0; k < m_numCols; k ++)
        {
          m_tables[i][(j * m_numCols) + k] = B.m_tables[i][(j * m_numCols) + k]; 
        }
      }  
    }
  }
  else
  {
    m_matrix = new T[m_numRows * m_numCols];

    if (m_matrix == NULL)
    {
      #ifdef debug
        cout << "Error! Could not allocate memory for matrix" << endl;
        exit(1);
      #endif

      cepError("Error! Could not allocate memory for Matrix", cepError::sevErrorRecoverable);
    }

    //copy each element of B to A for 2D matrix
    for (int i = 0; i < m_numRows; i++)
    {
      for (int j = 0; j < m_numCols; j++)
      {
        m_matrix[(i * m_numCols) + j] = B.m_matrix[(i * m_numCols) + j];
      }
    }
  }

  return *this;
}

//***************query operators****************************

template <class T>
bool cepMatrix<T>::operator== (const cepMatrix & B)
{
  if (m_matrix == NULL)
  {
    #ifdef debug
      cout << "Error! Could not use the == operate on a 3D matrix" << endl;
      exit(1);
    #endif

    cepError("Error! Can not use the == operator on a 3D Matrix", cepError::sevErrorRecoverable);
  }
  
  //check if rows and cols of A are equal to rows and cols of B
  if ((B.m_numRows != m_numRows) || (B.m_numCols != m_numCols))
  {
    return false;
  }
  else
  {
    //check if each element of A and B are equal
    for (int i = 0; i < m_numRows; i++)
    {
      for (int j = 0; j < m_numCols; j++)
      {
        if (m_matrix[(i * m_numCols) + j] != B.m_matrix[(i * m_numCols) + j])
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
  if (m_matrix == NULL)
  {
    #ifdef debug
      cout << "Error! Can not use the isDiagonal operator on a 3D matrix" << endl;
      exit(1);
    #endif

    cepError("Error! Can not use the isDiagonal operator on a 3D Matrix", cepError::sevErrorRecoverable);
  }
  
  if (m_numCols != m_numRows)
  {
    return false;
  }

  //check that each element of A[i,j] = 0 where i != j
  for (int i = 0; i < m_numRows; i++)
  {
    for (int j = 0; j < m_numCols; j++)
    {
      if ((i != j) && (m_matrix[(i * m_numCols) + j] != 0))
      {
        return false;
      }
    }
  }

  return true;
}

//***************get/set methods**************************

template <class T>
const T& cepMatrix<T>::getValue (int row, int col) const
{
  if (m_matrix == NULL)
  {
    #ifdef debug
      cout << "Error! The matrix contains no values" << endl;
      exit(1);
    #endif

    cepError("Error! The matrix contains no values", cepError::sevErrorRecoverable);
  }
  
  if (row >= m_numRows)
  {
    #ifdef debug
      cout << "Error! Invalid row number" << endl;
      exit(1);
    #endif

    cepError("Error Invalid Row Number", cepError::sevErrorRecoverable);
  }

  if (col >= m_numCols)
  {
    #ifdef debug
      cout << "Error! Invalid col number" << endl;
      exit(1);
    #endif

    cepError("Error Invalid Col Number", cepError::sevErrorRecoverable);
  }

  return m_matrix[(row * m_numCols) + col];
}

template <class T>
void cepMatrix<T>::setValue (const int row, const int col, const T & value)
{ 
  if (row >= m_numRows)
  {
    #ifdef debug
      cout << "Error! Invalid row number" << endl;
      exit(1);
    #endif

    cepError("Error Invalid Row Number", cepError::sevErrorRecoverable);
  }

  if (col >= m_numCols)
  {
    #ifdef debug
      cout << "Error! Invalid col number" << endl;
      exit(1);
    #endif

    cepError("Error Invalid Col Number", cepError::sevErrorRecoverable);
  }

  m_matrix[(row * m_numCols) + col] = value;
}

template <class T>
const int cepMatrix<T>::getNumRows () const
{
  return m_numRows;
}

template <class T>
const int cepMatrix<T>::getNumCols () const
{
  return m_numCols;
}

//***************get/set methods 3D matrix**************************

template <class T>
const T& cepMatrix<T>::getValue (int row, int col, int tab) const
{
  if (m_tables == NULL)
  {
    #ifdef debug
      cout << "Error! The matrix contains no values" << endl;
      exit(1);
    #endif

    cepError("Error! The matrix contains no values", cepError::sevErrorRecoverable);
  }
  
  if (row >= m_numRows)
  {
    #ifdef debug
      cout << "Error! Invalid row number" << endl;
      exit(1);
    #endif

    cepError("Error Invalid Row Number", cepError::sevErrorRecoverable);
  }

  if (col >= m_numCols)
  {
    #ifdef debug
      cout << "Error! Invalid col number" << endl;
      exit(1);
    #endif

    cepError("Error Invalid Col Number", cepError::sevErrorRecoverable);
  }
  if (tab >= m_numTables)
  {
    #ifdef debug
      cout << "Error! Invalid table number" << endl;
      exit(1);
    #endif

    cepError("Error Invalid Table Number", cepError::sevErrorRecoverable);
  }

  return m_tables[tab][(row * m_numCols) + col];
}

template <class T>
void cepMatrix<T>::setValue (const int row, const int col, const int tab, const T & value)
{
  if (row >= m_numRows)
  {
    #ifdef debug
      cout << "Error! Invalid row number" << endl;
      exit(1);
    #endif

    cepError("Error Invalid Row Number", cepError::sevErrorRecoverable);
  }

  if (col >= m_numCols)
  {
    #ifdef debug
      cout << "Error! Invalid col number" << endl;
      exit(1);
    #endif

    cepError("Error Invalid Col Number", cepError::sevErrorRecoverable);
  }
  if (tab >= m_numTables)
  {
    #ifdef debug
      cout << "Error! Invalid table number" << endl;
      exit(1);
    #endif

    cepError("Error Invalid Table Number", cepError::sevErrorRecoverable);
  }

  m_tables[tab][(row * m_numCols) + col] = value;
 
}

template <class T>
const int cepMatrix<T>::getNumTables () const
{
  return m_numTables;
}

#endif //end __cepMatrix_H
