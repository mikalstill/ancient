 /* 
  * Imp for the least squares implementation
  * Copyright (C) Kristy Van Der Vlist             2002
  * 
  * This program is free software; you can redistribute it and/or modify it
  * under the terms of the GNU General Public License as published by the Free
  * Software Foundation; either version 2 of the License, or (at your option)
  * any later version.
  * 
  * This program is distributed in the hope that it will be useful, but WITHOUT
  * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  * more details.
  * 
  * You should have received a copy of the GNU General Public License along
  * with this program; if not, write to the Free Software Foundation, Inc., 675
  * Mass Ave, Cambridge, MA 02139, USA. 
*/

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


