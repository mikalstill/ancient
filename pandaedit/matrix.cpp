#include "matrix.h"
#include <stdio.h>

matrix::matrix()
{
  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++)
      m_matrix[i][j] = 0.0;
}

void matrix::setIdentity()
{
  printf("DEBUG: Setting matrix to identity\n");
  dumpMatrix();
  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++){
      if(i == j) m_matrix[i][j] = 1.0;
      else m_matrix[i][j] = 0.0; 
    }
  dumpMatrix();
}

void matrix::setValues(float vals[6])
{
  printf("DEBUG: Setting matrix to %f %f | %f %f | %f %f\n",
	 vals[0], vals[1], vals[2], vals[3], vals[4], vals[5], vals[6]);

  dumpMatrix();
  int count = 0;
  m_matrix[0][0] = vals[count++];
  m_matrix[1][0] = vals[count++];
  m_matrix[0][1] = vals[count++];
  m_matrix[1][1] = vals[count++];
  m_matrix[0][2] = vals[count++];
  m_matrix[1][2] = vals[count++];
  dumpMatrix();
}

float matrix::getHorizontal()
{
  // _ _ _
  // _ _ _
  // * _ _
  printf("DEBUG: Matrix horizontal is %f\n", m_matrix[0][2]);
  return m_matrix[0][2];
}

float matrix::getVertical()
{
  // _ _ _
  // _ _ _
  // _ * _
  printf("DEBUG: Matrix vertical is %f\n", m_matrix[1][2]);
  return m_matrix[1][2];
}

float matrix::getAngle()
{
  // todo_mikal
  return 0.0;
}

void matrix::dumpMatrix()
{
  printf("DEBUG: Matrix dump...\n");
  for(int j = 0; j < 3; j++){
    for(int i = 0; i < 3; i++){
      printf("%f ", m_matrix[i][j]);
    }
    printf("\n");
  }
}
