#include "matrix.h"

matrix::matrix()
{
  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++)
      m_matrix[i][j] = 0.0;
}

void matrix::setIdentity()
{
 for(int i = 0; i < 3; i++)
   for(int j = 0; j < 3; j++){
     if(i == j) m_matrix[i][j] = 1.0;
     else m_matrix[i][j] = 0.0; 
   }
}

void matrix::setValues(float vals[6])
{
  for(int i = 0; i < 2; i++)
   for(int j = 0; j < 3; j++)
     m_matrix[i][j] = vals[i * j]; 
}
