#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
using namespace std;

class matrix
{
public:
  matrix ();

  matrix operator*(const matrix& other);
  bool operator==(const matrix& other);
  bool operator!=(const matrix& other);

  void setIdentity ();
  void setValues (float vals[6]);

  float getHorizontal ();
  void setHorizontal (float);
  float getVertical ();
  void setVertical (float);
  float getAngle ();

  void dumpMatrix ();
  float getRawItem(int x, int y);

private:
  vector<float> getRowVector(int row);
  vector<float> getColVector(int col);
  float vectorMultiply(vector<float> a, vector<float> b);

  float m_matrix[3][3];
};

#endif
