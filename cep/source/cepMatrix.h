 /* 
  *  The cepMatrix object contains the matrix data,
  *  accessor methods and core mathematical fuctions that need
  *  to be performed on it.
  *
  *   Copyright (C) Daniel Fernandez                 2002
  *   Copyright (C) Michael Still                    2002
  *   Copyright (C) Blake Swadling                   2002
  *   Copyright (C) Kristy Van Der Vlist             2002
  *   Copyright (C) Nick Wheatstone                  2002
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
cepMatrix<T> matrix(const int & rows, const int & cols)

<para><itemizedlist>
  <listitem><para>rows:- the number of rows in the matrix</para></listitem>
  <listitem><para>cols:- the number of colums in the matrix</para></listitem>
</itemizedlist></para>


Declaring a 3D matrix:-
cepMatrix<T> matrix(const int & rows, const int & cols, const int & tabs)

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
<command>const cepMatrix<T> & operator+= (const cepMatrix &B)</command>
Calculates A+B.
This function can not be used with 3D matricies.
<para><itemizedlist>
  <listitem><para>B:- the matrix to be added to the current matrix object</para></listitem>
</itemizedlist></para>
</para>

<para> 
<command>const cepMatrix<T> & operator-= (const cepMatrix &B)</command>
Calculates A-B.
This function can not be used with 3D matricies.
<para><itemizedlist>
  <listitem><para>B:- the matrix to be subtracted to the current matrix object</para></listitem>
</itemizedlist></para>
</para>

<para> 
<command>const cepMatrix<T> & operator*= (const cepMatrix &B)</command>
Calculates A*B.
<para><itemizedlist>
  <listitem><para>B:- the matrix to be multiplied with the current matrix object</para></listitem>
</itemizedlist></para>
</para>

<para> 
<command>const cepMatrix<T> & operator*= (const T &scalar)
Calculates c*A.
This function can not be used with 3D matricies.
<para><itemizedlist>
  <listitem><para>c:- the scalar value that the current matrix object is to be multiplied by</para></listitem>
</itemizedlist></para>
</para>

<para> 
<command>const cepMatrix<T> & operator= (const cepMatrix &B)</command>
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
<command>const T getMaxValue(const int & col)</command>
Returns the largest value in a given  coloum.
<para><itemizedlist>
  <listitem><para>col:- the coloum to be queried</para></listitem>
</itemizedlist></para>
</para>

<para>
<command>const T getMinValue(const int & col)</command>
Returns the smallest value in a given coloum
<para><itemizedlist>
  <listitem><para>col:- the coloum to be queried</para></listitem>
</itemizedlist></para>
</para>

<para>
<command>const cepMatrix<T> & resize(const int & newRows)</command>
Returns the current matrix which is resized by newRows number of rows. This
function can only make the number of rows in the matrix larger and can not be
used on 3D matricies
<para><itemizedlist>
  <listitem><para>newRows:- the new number of rows the resized matrix is to have</para></listitem>
</itemizedlist></para>
</para>

<para> 
<command>const T & getValue (const int & row, const int & col)</command>
Returns the value of the matrix the specified point.
This function can not be used with 3D matricies.
<para><itemizedlist>
  <listitem><para>row:- the row number of the value</para></listitem>
  <listitem><para>col:- the colum number of the value</para></listitem>
</itemizedlist></para>
</para>

<para> 
<command>void setValue (const int & row, const int & col, const T & value)</command>
Sets the value of the matrix at the specified point
This function can not be used with 3D matricies.
<para><itemizedlist>
  <listitem><para>row:- the row number where the value is to be inserted</para></listitem>
  <listitem><para>col:- the colum number where the value is to be inserted</para></listitem>
  <listitem><para>value:- the value of the given matrix point</para></listitem>
</itemizedlist></para>

</para>

<para>
<command>const cepMatrix<T> & resize(int & newRows)</command>
Returns the current matrix which is resized by newRows number of rows. This function
can only make the number of rows in the matrix larger.
<para><itemizedlist>
  <listitem><para>newRow:- The new number of rows in the matrix</para></listitem>
</itemizedlist></para>
</para>

<para> 
<command>const int & getNumRows ()</command>
Gets the number of rows in the matrix.
</para>

<para> 
<command>const int & getNumCols ()</command>
Gets the number of colums in the matrix.
</para>

<para> 
<command>const T& getValue (const int & row, const int & col, const int & tab)/command>
Gets the value of a 3D matrix at a specfied point
This function can not be used with 2D matricies.
<para><itemizedlist>
  <listitem><para>row:- the row number of the value</para></listitem>
  <listitem><para>col:- the colum number of the value</para></listitem>
  <listitem><para>tab:- the table number of the value</para></listitem>
</itemizedlist></para>
</para>

<para> 
<command>void setValue (const int & row, const int & col, const int & tab, const T & value)</command>
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
<command>const int & getNumTables()</command>
Gets the number of tables in the matrix.
This function can not be used with 2D matricies.
</para>

<para>
<command>const bool is3D()</command>
Returns true if the matrix is a 3D matrix, else returns false.
</para>

<para>
<command>cepError getError()</command>
Returns any error that may have occored whilst carrying out an of the operations
in this class. The error is returned as a cepError object.

SEEALSO cepError
   
DESCRIPTION END

DOCBOOK END
******************************************************************************/

#ifndef __CEPMATRIX_H
#define __CEPMATRIX_H

#include <iostream>
#include <math.h>
#include <stdlib.h>

#include "cepError.h"

//#define debug

template < class T > class cepMatrix
{

public:
  cepMatrix ();
  cepMatrix (const int &rows, const int &cols);
  cepMatrix (const int &rows, const int &cols, const int &tab);
  cepMatrix (const cepMatrix & B);	// copy constructor

  ~cepMatrix ();

  // Caluclates the transpose of the current matrix object 
  const cepMatrix < T > &transpose ();

  // ***************assingement operators***********************
  //calculates A+B where A is the current matrix object and B is another matrix
  const cepMatrix < T > &operator+= (const cepMatrix & B);

  //calculates A-B where A is the current matrix object and B is another matrix  
  const cepMatrix < T > &operator-= (const cepMatrix & B);

  //calculates A*B where A is the current matrix object and B is another matrix
  const cepMatrix < T > &operator*= (const cepMatrix & B);

  //calculates c*A where A is the current matrix object and scalar is a scalar value
  const cepMatrix < T > &operator*= (const T & scalar);	// mat * scalar

  // ***************copy operator*****************************
  //copies A to B where A is the current matrix object and B is another matrix
  const cepMatrix < T > &operator= (const cepMatrix & B);

  // ***************query operators****************************
  //compares A to B where A is the current matrix object and B is another matrix
  //returns true if A and B are equal, else returns false  
  const bool operator== (const cepMatrix & B);

  //compares A to B where A is the current matrix object and B is another matrix
  //returns true if A and B are equal, else returns false
  const bool operator!= (const cepMatrix & B);

  //determies whether the A, the current matrix object is strictly Diagonal.
  //returs true if A is diagonal, else returns false  
  const bool isDiagonal ();

  //returns the largest value in a given  coloum
  const T getMaxValue (const int &col);

  //returns the smallest value in a given coloum
  const T getMinValue (const int &col);
  //****************Resize fuction **************************
  //Returns the current matrix which is resized by newRows number of rows. This
  //function can only make the number of rows in the matrix larger.
  const cepMatrix < T > &resize (const int &newRows);

  // ***************get/set methods**************************
  //gets the value of the matrix at matrix[row,col]
  const T getValue (const int &row, const int &col);

  //sets the value of the matrix at matrix[row,col] = value
  void setValue (const int &row, const int &col, const T & value);

  //gets the number of rows in the matrix  
  const int &getNumRows ();

  //gets the number of cols in the matrix
  const int &getNumCols ();

  // ***************get/set methods**************************
  //gets the value of the matrix at matrix[row,col,inst]
  const T getValue (const int &row, const int &col, const int &tab);

  //sets the value of the matrix at matrix[row,col,inst] = value
  void setValue (const int &row, const int &col, const int &tab,
		 const T & value);

  //gets the number of tables in the matrix
  const int &getNumTables ();

  //returns true if matrix is 3D
  const bool is3D ();
  //*******************Error method**************************
  //returns any error that may have occoured
  cepError getError ();

private:
  T * m_matrix;			//holds the matrix values 
  T **m_tables;			//holds the values of a 3D matrix
  int m_numRows,		//holds the number of rows in the matrix
    m_numCols,			//holds the number of cols in the matrix
    m_numTables;		//holds the number if instances of matricies
  cepError m_error;		//holds any error returned from a fuction

};

template < class T > cepMatrix < T >::cepMatrix ()
{
  //initalize a default matrix
  m_numRows = 0;
  m_numCols = 0;
  m_numTables = 0;

  //make both pointers NULL by default
  m_matrix = NULL;
  m_tables = NULL;
}

template < class T >
  cepMatrix < T >::cepMatrix (const int &rows, const int &cols)
{
  //initalize a matrix
  m_numRows = rows;
  m_numCols = cols;
  m_numTables = 1;

  m_tables = NULL;		//set 3D matrix pointer NULL, using 2D matrix
  m_matrix = new T[rows * cols];

  if (m_matrix == NULL)
    {
      m_error =
	cepError ("Could not allocate memory for Matrix",
		  cepError::sevErrorRecoverable);
      m_error.log ();
    }
}

template < class T >
  cepMatrix < T >::cepMatrix (const int &rows, const int &cols,
			      const int &tab)
{
  //initalize a matrix
  m_numRows = rows;
  m_numCols = cols;
  m_numTables = tab;

  m_matrix = NULL;		//setting matrix NULL, using 3D matrix
  m_tables = new T *[m_numTables];
  if (m_tables == NULL)
    {
      m_error =
	cepError ("Could not allocate memory for Matrix",
		  cepError::sevErrorRecoverable);
      m_error.log ();
      return;
    }

  for (int i = 0; i < m_numTables; i++)
    {
      m_tables[i] = new T[m_numRows * m_numCols];

      if (m_tables[i] == NULL)
	{
	  m_error =
	    cepError ("Could not allocate memory for Matrix",
		      cepError::sevErrorRecoverable);
	  m_error.log ();
	  return;
	}
    }
}

template < class T > cepMatrix < T >::cepMatrix (const cepMatrix & B)
{
  operator= (B);
}

template < class T > cepMatrix < T >::~cepMatrix ()
{
  if (m_tables != NULL)
    {
      for (int i = 0; i < m_numTables; i++)
	{
	  if (m_tables[i] != NULL)
	    delete[]m_tables[i];
	  else
	    cepDebugPrint ("Table " + cepToString (i) + " was NULL");
	}
      if (m_tables != NULL)
	delete[]m_tables;
    }

  if (m_matrix != NULL)
    {
      delete[]m_matrix;
    }
}

template < class T > const cepMatrix < T > &cepMatrix < T >::transpose ()
{
  T *tempMatrix = m_matrix;
  int temp;
  int x = 0;

  temp = m_numRows;
  m_numRows = m_numCols;
  m_numCols = temp;

  if (m_matrix == NULL)
    {
      m_error =
	cepError ("Can Not Transpose a 3D matrix",
		  cepError::sevErrorRecoverable);
      m_error.log ();
      if (tempMatrix != NULL)
	{
	  delete[]tempMatrix;
	}
      return *this;
    }

  m_matrix = new T[m_numRows * m_numCols];

  if (m_matrix == NULL)
    {
      m_error =
	cepError ("Could not allocate memory for Matri",
		  cepError::sevErrorRecoverable);
      m_error.log ();
      if (tempMatrix != NULL)
	{
	  delete[]tempMatrix;
	}
      return *this;
    }

  //calculates the matrix transpose
  for (int i = 0; i < m_numRows; i++)
    {
      for (int j = 0; j < m_numCols; j++)
	{
	  m_matrix[x++] = tempMatrix[j * m_numRows + i];
	}
    }

  if (tempMatrix != NULL)
    {
      delete[]tempMatrix;
    }

  return *this;
}

//****************Assignment operators*******************
template < class T >
  const cepMatrix < T > &cepMatrix < T >::operator+= (const cepMatrix & B)
{
  T *tempMatrix = m_matrix;

  if (m_matrix == NULL)
    {
      m_error =
	cepError (" Could not allocate memory for Matrix",
		  cepError::sevErrorRecoverable);
      m_error.log ();
      if (tempMatrix != NULL)
	{
	  delete[]tempMatrix;
	}
      return *this;
    }

  m_matrix = new T[m_numRows * m_numCols];

  if (m_matrix == NULL)
    {
      m_error =
	cepError ("Could not allocate memory for Matrix",
		  cepError::sevErrorRecoverable);
      m_error.log ();
      if (tempMatrix != NULL)
	{
	  delete[]tempMatrix;
	}
      return *this;
    }

  if ((B.m_numRows != m_numRows) || (B.m_numCols != m_numCols))
    {
      m_error =
	cepError ("Matrix sizes are wrong", cepError::sevErrorRecoverable);
      m_error.log ();
      if (tempMatrix != NULL)
	{
	  delete[]tempMatrix;
	}
      return *this;
    }
  else
    {
      //calucluates A+B for each element is A and B
      for (int i = 0; i < m_numRows; i++)
	{
	  for (int j = 0; j < m_numCols; j++)
	    {
	      m_matrix[(i * m_numCols) + j] =
		tempMatrix[(i * m_numCols) + j] + B.m_matrix[(i * m_numCols) +
							     j];
	    }
	}
    }

  if (tempMatrix != NULL)
    {
      delete[]tempMatrix;
    }

  return *this;
}

template < class T >
  const cepMatrix < T > &cepMatrix < T >::operator-= (const cepMatrix & B)
{
  T *tempMatrix = m_matrix;

  if (m_matrix == NULL)
    {
      m_error =
	cepError (" Can not use the - operator on a 3D Matrix",
		  cepError::sevErrorRecoverable);
      m_error.log ();
      if (tempMatrix != NULL)
	{
	  delete[]tempMatrix;
	}
      return *this;
    }

  m_matrix = new T[m_numRows * m_numCols];

  if (m_matrix == NULL)
    {
      m_error =
	cepError ("Could not allocate memory for Matrix",
		  cepError::sevErrorRecoverable);
      m_error.log ();
      if (tempMatrix != NULL)
	{
	  delete[]tempMatrix;
	}
      return *this;
    }

  if ((B.m_numRows != m_numRows) || (B.m_numCols != m_numCols))
    {
      m_error =
	cepError (" Matrix sizes are wrong", cepError::sevErrorRecoverable);
      m_error.log ();
      if (tempMatrix != NULL)
	{
	  delete[]tempMatrix;
	}
      return *this;
    }
  else
    {
      //calucluates A-B for each element is A and B
      for (int i = 0; i < m_numRows; i++)
	{
	  for (int j = 0; j < m_numCols; j++)
	    {
	      m_matrix[(i * m_numCols) + j] =
		tempMatrix[(i * m_numCols) + j] - B.m_matrix[(i * m_numCols) +
							     j];
	    }
	}
    }

  if (tempMatrix != NULL)
    {
      delete[]tempMatrix;
    }

  return *this;
}

template < class T >
  const cepMatrix < T > &cepMatrix < T >::operator*= (const cepMatrix & B)
{
  T *tempMatrix = m_matrix;
  int i;

  if (m_matrix == NULL)
    {
      m_error =
	cepError (" Can not use the - operator on a 3D Matrix",
		  cepError::sevErrorRecoverable);
      m_error.log ();
      if (tempMatrix != NULL)
	{
	  delete[]tempMatrix;
	}
      return *this;
    }

  if (m_numCols != B.m_numRows)
    {
      m_error =
	cepError (" Matrix sizes are wrong", cepError::sevErrorRecoverable);
      m_error.log ();
      if (tempMatrix != NULL)
	{
	  delete[]tempMatrix;
	}
      return *this;
    }
  else
    {
      m_matrix = new T[m_numRows * B.m_numCols];

      if (m_matrix == NULL)
	{
	  m_error =
	    cepError (" Could not allocate memory for Matrix",
		      cepError::sevErrorRecoverable);
	  m_error.log ();
	  if (tempMatrix != NULL)
	    {
	      delete[]tempMatrix;
	    }
	  return *this;
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
		  m_matrix[(i * B.m_numCols) + j] +=
		    tempMatrix[k + (i * m_numCols)] * B.m_matrix[j +
								 (k *
								  B.
								  m_numCols)];

		}

	    }
	}
    }

  m_numCols = B.m_numCols;

  if (tempMatrix != NULL)
    {
      delete[]tempMatrix;
    }

  return *this;
}

template < class T >
  const cepMatrix < T > &cepMatrix < T >::operator*= (const T & scalar)
{
  if (m_matrix == NULL)
    {
      m_error =
	cepError (" Can not use the * operator on a 3D Matrix",
		  cepError::sevErrorRecoverable);
      m_error.log ();
      return *this;
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

//*****************copy operator***************************
template < class T >
  const cepMatrix < T > &cepMatrix < T >::operator= (const cepMatrix & B)
{
  m_numRows = B.m_numRows;
  m_numCols = B.m_numCols;
  m_numTables = B.m_numTables;

  if (B.m_tables != NULL)
    {
      m_tables = new T *[m_numTables];

      if (m_tables == NULL)
	{
	  m_error =
	    cepError (" Could not allocate memory for Matrix",
		      cepError::sevErrorRecoverable);
	  m_error.log ();
	  return *this;
	}

      for (int i = 0; i < m_numTables; i++)
	{
	  m_tables[i] = new T[m_numRows * m_numCols];

	  if (m_tables[i] == NULL)
	    {
	      m_error =
		cepError (" Could not allocate memory for Matrix",
			  cepError::sevErrorRecoverable);
	      m_error.log ();
	      return *this;
	    }
	}

      for (int i = 0; i < m_numTables; i++)
	{
	  for (int j = 0; j < m_numRows; j++)
	    {
	      for (int k = 0; k < m_numCols; k++)
		{
		  m_tables[i][(j * m_numCols) + k] =
		    B.m_tables[i][(j * m_numCols) + k];
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
	  m_error =
	    cepError (" Could not allocate memory for Matrix",
		      cepError::sevErrorRecoverable);
	  m_error.log ();
	  return *this;
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
  return *this;
}

//***************query operators****************************

template < class T >
  const bool cepMatrix < T >::operator== (const cepMatrix & B)
{
  if (m_matrix == NULL)
    {
      m_error =
	cepError (" Can not use the == operator on a 3D Matrix",
		  cepError::sevErrorRecoverable);
      m_error.log ();
      return false;
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
	      if (m_matrix[(i * m_numCols) + j] !=
		  B.m_matrix[(i * m_numCols) + j])
		{
		  return false;
		}
	    }
	}
    }

  return true;
}

template < class T >
  const bool cepMatrix < T >::operator!= (const cepMatrix & B)
{
  return !(operator== (B));
}
template < class T > const bool cepMatrix < T >::isDiagonal ()
{
  if (m_matrix == NULL)
    {
      m_error =
	cepError (" Can not use the isDiagonal operator on a 3D Matrix",
		  cepError::sevErrorRecoverable);
      m_error.log ();
      return false;
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

template < class T > const T cepMatrix < T >::getMaxValue (const int &col)
{
  if (col >= m_numCols)
    {
      m_error =
	cepError ("Invalid Col Number: " + cepToString (col),
		  cepError::sevErrorRecoverable);
      m_error.log ();
      return (T) 0;
    }

  // This is a 2D matrix
  if (m_matrix != NULL)
    {
      T maxVal;
      maxVal = m_matrix[col];

      for (int i = 0; i < m_numRows; i++)
	{
	  if (maxVal < m_matrix[(i * m_numCols) + col])
	    {
	      maxVal = m_matrix[(i * m_numCols) + col];
	    }
	}

      cepDebugPrint ("2D matrix max value (col " + cepToString (col) + "): " +
		     cepToString (maxVal));
      return maxVal;
    }

  // This is a 3D matrix
  else
    {
      T maxVal;
      maxVal = m_tables[0][col];

      for (int tno = 0; tno < m_numTables; tno++)
	{
	  for (int i = 0; i < m_numRows; i++)
	    {
	      cepDebugPrint ("Checking: " +
			     cepToString (m_tables[tno]
					  [(i * m_numCols) + col]) + " " +
			     cepToString (tno) + " " + cepToString (i) + " " +
			     cepToString (col));
	      if (maxVal < m_tables[tno][(i * m_numCols) + col])
		{
		  maxVal = m_tables[tno][(i * m_numCols) + col];
		}
	    }
	}

      cepDebugPrint ("3D matrix max value (col " + cepToString (col) + "): " +
		     cepToString (maxVal));
      return maxVal;
    }
}

template < class T > const T cepMatrix < T >::getMinValue (const int &col)
{
  if (col >= m_numCols)
    {
      m_error =
	cepError ("Invalid Col Number: " + cepToString (col),
		  cepError::sevErrorRecoverable);
      m_error.log ();
      return (T) 0;
    }

  // This is a 2D matrix
  if (m_matrix != NULL)
    {
      T minVal;
      minVal = m_matrix[col];

      for (int i = 0; i < m_numRows; i++)
	{
	  if (minVal > m_matrix[(i * m_numCols) + col])
	    {
	      minVal = m_matrix[(i * m_numCols) + col];
	    }
	}

      cepDebugPrint ("2D matrix min value (col " + cepToString (col) + "): " +
		     cepToString (minVal));
      return minVal;
    }

  // This is a 3D matrix
  else
    {
      T minVal;
      minVal = m_tables[0][col];

      for (int tno = 0; tno < m_numTables; tno++)
	{
	  for (int i = 0; i < m_numRows; i++)
	    {
	      if (minVal > m_tables[tno][(i * m_numCols) + col])
		{
		  minVal = m_tables[tno][(i * m_numCols) + col];
		}
	    }
	}

      cepDebugPrint ("3D matrix min value (col " + cepToString (col) + "): " +
		     cepToString (minVal));
      return minVal;
    }
}

//****************Resize fuction *************************
template < class T >
  const cepMatrix < T > &cepMatrix < T >::resize (const int &newRows)
{
  T *tempMatrix = NULL;

  if (m_matrix == NULL)
    {
      m_error =
	cepError (" Can not resize a 3D Matrix",
		  cepError::sevErrorRecoverable);
      m_error.log ();
      if (tempMatrix != NULL)
	{
	  delete[]tempMatrix;
	}
      return *this;
    }

  if (m_numRows <= newRows)
    {
      tempMatrix = new T[m_numRows * m_numCols];

      if (tempMatrix == NULL)
	{
	  m_error =
	    cepError (" Could not allocate memory for Matrix",
		      cepError::sevErrorRecoverable);
	  m_error.log ();
	  if (tempMatrix != NULL)
	    {
	      delete[]tempMatrix;
	    }
	  return *this;
	}

      for (int i = 0; i < (m_numRows * m_numCols); i++)
	{
	  tempMatrix[i] = m_matrix[i];
	}

      m_matrix = new T[newRows * m_numCols];
      if (m_matrix == NULL)
	{
	  m_error =
	    cepError (" Could not allocate memory for Matrix",
		      cepError::sevErrorRecoverable);
	  m_error.log ();
	  if (tempMatrix != NULL)
	    {
	      delete[]tempMatrix;
	    }
	  return *this;
	}

      for (int i = 0; i < (m_numRows * m_numCols); i++)
	{
	  m_matrix[i] = tempMatrix[i];
	}

      m_numRows = newRows;
    }
  else
    {
      m_error =
	cepError ("Can not make a matrix smaller",
		  cepError::sevErrorRecoverable);
      m_error.log ();
      if (tempMatrix != NULL)
	{
	  delete[]tempMatrix;
	}
      return *this;
    }

  if (tempMatrix != NULL)
    {
      delete[]tempMatrix;
    }

  return *this;
}

//***************get/set methods**************************

template < class T >
  const T cepMatrix < T >::getValue (const int &row, const int &col)
{
  if (m_matrix == NULL)
    {
      m_error =
	cepError ("The matrix contains no values",
		  cepError::sevErrorRecoverable);
      m_error.log ();
      return (T) 0;
    }

  if (row >= m_numRows)
    {
      m_error =
	cepError ("Invalid Row Number: " + cepToString (row) + " with col: " +
		  cepToString (col), cepError::sevErrorRecoverable);
      m_error.log ();
      return (T) 0;
    }

  if (col >= m_numCols)
    {
      m_error =
	cepError ("Invalid Col Number: " + cepToString (col),
		  cepError::sevErrorRecoverable);
      m_error.log ();
      return (T) 0;
    }
  return m_matrix[(row * m_numCols) + col];
}

template < class T >
  void cepMatrix < T >::setValue (const int &row, const int &col,
				  const T & value)
{
  if (row >= m_numRows)
    {
      m_error =
	cepError ("Invalid Row Number: " + cepToString (row) + " with col: " +
		  cepToString (col), cepError::sevErrorRecoverable);
      m_error.log ();
      return;
    }

  if (col >= m_numCols)
    {
      m_error =
	cepError ("Invalid Col Number: " + cepToString (col),
		  cepError::sevErrorRecoverable);
      m_error.log ();
      return;
    }

  m_matrix[(row * m_numCols) + col] = value;
}

template < class T > const int &cepMatrix < T >::getNumRows ()
{
  return m_numRows;
}

template < class T > const int &cepMatrix < T >::getNumCols ()
{
  return m_numCols;
}

//***************get/set methods 3D matrix**************************

template < class T >
  const T cepMatrix < T >::getValue (const int &row, const int &col,
				     const int &tab)
{
  if ((m_tables == NULL) && (tab == 0))
    {
      return getValue (row, col);
    }

  if (m_tables == NULL)
    {
      m_error =
	cepError ("The matrix contains no values",
		  cepError::sevErrorRecoverable);
      m_error.log ();
      return (T) 0;
    }

  if (row >= m_numRows)
    {
      m_error =
	cepError ("Invalid Row Number: " + cepToString (row) + " with col: " +
		  cepToString (col), cepError::sevErrorRecoverable);
      m_error.log ();
      return (T) 0;
    }

  if (col >= m_numCols)
    {
      m_error =
	cepError ("Invalid Col Number: " + cepToString (col),
		  cepError::sevErrorRecoverable);
      m_error.log ();
      return (T) 0;
    }
  if (tab >= m_numTables)
    {
      m_error =
	cepError ("Invalid Table Number: " + cepToString (tab),
		  cepError::sevErrorRecoverable);
      m_error.log ();
      return (T) 0;
    }

  return m_tables[tab][(row * m_numCols) + col];
}

template < class T >
  void cepMatrix < T >::setValue (const int &row, const int &col,
				  const int &tab, const T & value)
{
  if ((m_tables == NULL) && (tab == 0))
    {
      setValue (row, col, value);
    }
  else
    {
      if (row >= m_numRows)
	{
	  m_error =
	    cepError ("Invalid Row Number: " + cepToString (row) +
		      " with col: " + cepToString (col),
		      cepError::sevErrorRecoverable);
	  m_error.log ();
	  return;
	}

      if (col >= m_numCols)
	{
	  m_error =
	    cepError ("Invalid Col Number: " + cepToString (col),
		      cepError::sevErrorRecoverable);
	  m_error.log ();
	  return;
	}
      if (tab >= m_numTables)
	{
	  m_error =
	    cepError ("Invalid Table Number: " + cepToString (tab),
		      cepError::sevErrorRecoverable);
	  m_error.log ();
	  return;
	}

      m_tables[tab][(row * m_numCols) + col] = value;
    }
}

template < class T > const int &cepMatrix < T >::getNumTables ()
{
  return m_numTables;
}

template < class T > const bool cepMatrix < T >::is3D ()
{
  return (m_tables != NULL);
}

template < class T > cepError cepMatrix < T >::getError ()
{
  return m_error;
}


#endif //end __cepMatrix_H
