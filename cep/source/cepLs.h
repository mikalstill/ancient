#include <iostream.h>
#include <math.h>
#include <stdlib.h>

#include  "cepMatrix.h"

class cepLs
{
public:
  
  cepLs();
  ~cepLs();

  const cepMatrix cepDoLeastSquares(cepMatrix &, cepMatrix &, cepMatrix &); 
  
private:

  void sanityCheck(cepMatrix &, cepMatrix &, cepMatrix &);
  bool isDiagonal(cepMatrix &mat);
  
  const cepMatrix inverse(cepMatrix &);
  const cepMatrix mulDiag(cepMatrix &, cepMatrix &);
  const cepMatrix Amul(cepMatrix &, cepMatrix &);
  const cepMatrix mulA(cepMatrix &, cepMatrix &);
};

