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

float
matrix::getVertical ()
{
  // _ _ _
  // _ _ _
  // _ * _
  debug(dlTrace, string("Matrix vertical is ") + toString(m_matrix[1][2]));
  return m_matrix[1][2];
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
