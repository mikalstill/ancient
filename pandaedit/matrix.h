#ifndef MATRIX_H
#define MATRIX_H

class matrix
{
 public:
  matrix();

  void setIdentity();
  void setValues(float vals[6]);

 private:
  float m_matrix[3][3];
};

#endif
