#include <string>
#include "matrix.h"
#include "verbosity.h"
#include "utility.h"

using namespace std;

matrix::matrix ()
{
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      m_matrix[i][j] = 0.0;
}

matrix matrix::operator*(const matrix& other)
{
  matrix retval;

  for(int x = 0; x < 3; x++){
    for(int y = 0; y < 3; y++){
      retval.m_matrix[x][y] = vectorMultiply(getRowVector(y), 
					     ((matrix)other).getColVector(x));
    }
  }

  return retval;
}

vector<float> matrix::getRowVector(int row)
{
  vector<float> retval;
  retval.push_back(m_matrix[row][0]);
  retval.push_back(m_matrix[row][1]);
  retval.push_back(m_matrix[row][2]);
  return retval;
}

vector<float> matrix::getColVector(int col)
{
  vector<float> retval;
  retval.push_back(m_matrix[0][col]);
  retval.push_back(m_matrix[1][col]);
  retval.push_back(m_matrix[2][col]);
  return retval;
}

float matrix::vectorMultiply(vector<float> a, vector<float> b)
{
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void
matrix::setIdentity ()
{
  debug(dlTrace, "Setting matrix to identity\n");
  dumpMatrix ();
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      {
	if (i == j)
	  m_matrix[i][j] = 1.0;
	else
	  m_matrix[i][j] = 0.0;
      }
  dumpMatrix ();
}

void
matrix::setValues (float vals[6])
{
  debug(dlTrace, string("Setting matrix to ") + 
	toString(vals[0]) + string(", ") +
	toString(vals[1]) + string(", ") + 
	toString(vals[2]) + string(", ") +
	toString(vals[3]) + string(", ") + 
	toString(vals[4]) + string(", ") +
	toString(vals[5]) + string(", ") + 
	toString(vals[6]));

  dumpMatrix ();
  int count = 0;
  m_matrix[0][0] = vals[count++];
  m_matrix[1][0] = vals[count++];
  m_matrix[0][1] = vals[count++];
  m_matrix[1][1] = vals[count++];
  m_matrix[0][2] = vals[count++];
  m_matrix[1][2] = vals[count++];
  dumpMatrix ();
}

float
matrix::getHorizontal ()
{
  // _ _ _
  // _ _ _
  // * _ _
  debug(dlTrace, string("Matrix horizontal is ") + toString(m_matrix[0][2]));
  return m_matrix[0][2];
}

void
matrix::setHorizontal (float newval)
{
  // _ _ _
  // _ _ _
  // * _ _
  debug(dlTrace, string("Matrix horizontal is now ") + toString(newval));
  m_matrix[0][2] = newval;
}

float
matrix::getVertical ()
{
  // _ _ _
  // _ _ _
  // _ * _
  debug(dlTrace, string("Matrix vertical is ") + toString(m_matrix[1][2]));
  return m_matrix[1][2];
}

void
matrix::setVertical (float newval)
{
  // _ _ _
  // _ _ _
  // _ * _
  debug(dlTrace, string("Matrix vertical is now ") + toString(newval));
  m_matrix[1][2] = newval;
}

float
matrix::getAngle ()
{
  // todo_mikal
  return 0.0;
}

void
matrix::dumpMatrix ()
{
  debug(dlTrace, "DEBUG: Matrix dump...");
  for (int j = 0; j < 3; j++)
    {
      for (int i = 0; i < 3; i++)
	{
	  debug(dlTrace, toString(m_matrix[i][j]));
	}
    }
}
