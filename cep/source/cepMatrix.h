#include <iostream.h>
#include <math.h>
#include <stdlib.h>


class cepMatrix
{

public:
  cepMatrix(int rows, int cols);
  ~cepMatrix();

  //Note! these fuctions operate on the object itself!
  const cepMatrix &transpose();
  const cepMatrix &inverse();

  //assingement operators
  const cepMatrix &operator+=(const cepMatrix &);
  const cepMatrix &operator-= (const cepMatrix &);
  const cepMatrix &operator*= (const cepMatrix &); //mat *mat
  const cepMatrix &operator*= (const double &);   //mat * scalar
  
  //asignment
  const cepMatrix &operator= (const cepMatrix &);
  
  //query dataset
  bool operator== (const cepMatrix &); 
  bool isDiagonal();
  
  //get/set methods
  double getValue(int row, int col);
  void  setValue(int row, int col, double value);
  int getNumRows();
  int getNumCols();

private:
  double *matrix;
  int numRows, numCols;
};

