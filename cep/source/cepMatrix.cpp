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

#include "cepMatrix.h"

cepMatrix::cepMatrix ()
{
  numRows = 0;
  numCols = 0;
  matrix = new double[1];

  if (matrix == NULL)
  {
    cout << "Error! could not allocate memory";
    exit (1);
  }

}

cepMatrix::cepMatrix (int rows, int cols)
{
  numRows = rows;
  numCols = cols;

  matrix = new double[rows * cols];

  if (matrix == NULL)
  {
    cout << "Error! could not allocate memory";
    exit (1);
  }

}

cepMatrix::cepMatrix (const cepMatrix & copyMatrix)
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

cepMatrix::~cepMatrix ()
{
  delete[]matrix;
}

const cepMatrix & cepMatrix::transpose ()
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
const cepMatrix & cepMatrix::operator+= (const cepMatrix & B)
{
  double *tempMatrix = matrix;

  if ((B.numRows != numRows) || (B.numCols != numCols))
  {
    cout << "matrix sizes wrong\n";
    exit (1);
  }
  else
  {
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

const cepMatrix & cepMatrix::operator-= (const cepMatrix & B)
{
  double *tempMatrix = matrix;

  if ((B.numRows != numRows) || (B.numCols != numCols))
  {
    cout << "matrix sizes wrong\n";
    exit (1);
  }
  else
  {
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

const cepMatrix & cepMatrix::operator*= (const cepMatrix & B)
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

const cepMatrix & cepMatrix::operator*= (const double &scalar)
{
  for (int i = 0; i < numRows; i++)
  {
    for (int j = 0; j < numCols; j++)
    {
      matrix[(i * numCols) + j] *= scalar;
    }
  }

  return *this;
}

//*****************copy operator***************************
const cepMatrix & cepMatrix::operator= (const cepMatrix & B)
{
  numRows = B.numRows;
  numCols = B.numCols;

  matrix = new double[numRows * numCols];

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

bool cepMatrix::operator== (const cepMatrix & B)
{
  if ((B.numRows != numRows) || (B.numCols != numCols))
  {
    return false;
  }
  else
  {
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

bool cepMatrix::isDiagonal ()
{
  if (numCols != numRows)
  {
    return false;
  }

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

double cepMatrix::getValue (int row, int col)
{
  if ((row * numCols) + col > (numRows * numCols))
  {
    cout << "Invalid refence\n";
    exit (1);
  }
  
  return matrix[(row * numCols) + col];
}

void cepMatrix::setValue (int row, int col, double value)
{
  if ((row * numCols) + col > (numRows * numCols))
  {
    cout << "Invalid refence\n";
    exit (1);
  }
  
  matrix[(row * numCols) + col] = value;
}

int cepMatrix::getNumRows ()
{
  return numRows;
}

int cepMatrix::getNumCols ()
{
  return numCols;
}
