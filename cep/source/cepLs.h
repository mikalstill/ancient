#include <iostream.h>
#include <math.h>
#include <stdlib.h>
typedef struct cepMatrix
{
  double *matrix;
  int numRows;
  int numCols;
}
blah;
class cepLs
{
public:cepLs ();
  ~cepLs ();
  void cepDoLeastSquares ();
private:  cepMatrix matA, matP, matL;
  void sanityCheck (cepMatrix & matA, cepMatrix & matP, cepMatrix & matL);
  bool isDiagonal (cepMatrix & mat);
  cepMatrix inverse (cepMatrix & mat);
  cepMatrix transpose (cepMatrix & mat);
  cepMatrix mulDiag (cepMatrix & matA, cepMatrix & matB);
  cepMatrix mul (double *row, int rowElements, cepMatrix & mat);
};
