/**********************************************************
cepMatrix.h

Written By:- Kristy Van Der Vlist
Description:- The cepMatrix object contains the matrix data,
  acessor methods an core mathematical fuctions that need
  to be preformed on it.
***********************************************************/
#include <iostream.h>
#include <math.h>
#include <stdlib.h>


class cepMatrix
{

public:
  cepMatrix();
  cepMatrix(int rows, int cols);
  cepMatrix(const cepMatrix &copyMatrix);   //copy constructor

  ~cepMatrix();

  const cepMatrix &transpose();
 
  //***************assingement operators***********************
  const cepMatrix &operator+= (const cepMatrix &);
  const cepMatrix &operator-= (const cepMatrix &);
  const cepMatrix &operator*= (const cepMatrix &); //mat *mat
  const cepMatrix &operator*= (const double &);   //mat * scalar
  
  //***************copy operator*****************************
  const cepMatrix &operator= (const cepMatrix &);
  
  //***************query operators****************************
  bool operator== (const cepMatrix &); 
  bool isDiagonal();
  
  //***************get/set methods**************************
  double getValue(int row, int col);
  void  setValue(int row, int col, double value);
  int getNumRows();
  int getNumCols();

private:
  double *matrix;       
  int numRows, numCols; 
};

