/* 
   Test for the matrix implementation
   Copyright (C) Michael Still                    2002
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "cepMatrixImp.h"
#include "cepMatrix.h"
#include <stdio.h>
#include <time.h>

void cepMatrixImpTestFunction(void);

void main(int argc, char *argv[]){
  time_t timer;

  // cepMatrixImp
  printf("Building the matrix implementation\n");
  timer = time(NULL);
  cepMatrixImp myimp(4000, 4000);
  printf("Initialization took %d seconds\n", time(NULL) - timer);

  timer = time(NULL);
  myimp.test();
  printf("cepMatrixImp::test() took %d seconds\n", time(NULL) - timer);

  // cepMatrix
  printf("Building the matrix\n");
  cepMatrix mymatrix1(4000, 4000);
  
  printf("  Populating it with test values\n");
  int xcounter, ycounter;
  for(xcounter = 0; xcounter < 4000; xcounter++){
    for(ycounter = 0; ycounter < 4000; ycounter++){
      mymatrix1.setCell(xcounter, ycounter, (xcounter * ycounter) % 32000);
    }
  }

  printf("  Transpose the matrix\n");
  timer = time(NULL);
  cepMatrix mymatrix2 = mymatrix1.transpose();
  printf("  Transposition took %d seconds\n", time(NULL) - timer);

  printf("  Check to see if we got the expected values\n");
  bool manypassed = true;
  for(xcounter = 0; xcounter < 4000; xcounter++){
    for(ycounter = 0; ycounter < 4000; ycounter++){
      if(mymatrix2.getCell(ycounter, xcounter) != 
	 (xcounter * ycounter) % 32000){
        manypassed = false;
      }
    }
  }  

  if(!manypassed){
    printf("    Transpose failed\n");
    return false;
  }
  else{
    printf("    Transpose passed\n");
  }

  // Multiply a vector with a matrix
  cepMatrix mymultmat(3, 2);
  mymultmat.setCell(0, 0, 1);
  mymultmat.setCell(0, 1, 0);
  mymultmat.setCell(1, 0, 2);
  mymultmat.setCell(1, 1, -5);
  mymultmat.setCell(2, 0, -1);
  mymultmat.setCell(2, 1, 3);

  cepVector mymultvec(3);
  mymultvec.setCell(0, 4);
  mymultvec.setCell(1, 3);
  mymultvec.setCell(2, 7);

  cepVector mult = mymultmat * mymultvec;

  
}

