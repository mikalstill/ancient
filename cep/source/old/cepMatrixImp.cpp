/* 
   Imp for the CEP matrix
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
#include <stdio.h>
#include <time.h>

cepMatrixImp::cepMatrixImp(int x, int y){
   m_cells = new int[x * y];
   m_x = x;
   m_y = y;
   m_refs = 1;
 }

int cepMatrixImp::getCell(int x, int y){
  if(x > m_x){
    return 0;
  }
  if(y > m_y){
    return 0;
  }

  return m_cells[y * m_x + x];
}

cepError cepMatrixImp::setCell(int x, int y, int value){
  if(m_refs > 1){
    printf("ERROR: Copy on write is not yet implemented, and more than one matrix refers to this cepMatrixImp\n");
  }

  if(x > m_x){
    return cepError("Matrix is not that wide");
  }
  if(y > m_y){
    return cepError("Matrix is not that tall");
  }

  m_cells[y * m_x + x] = value;
}

bool cepMatrixImp::test(){
  time_t timer;
  printf("Testing the matrix implementation\n"); 

  // ---------- Test one: get / set a single value
  printf("  Attempting to set the value of a cell\n");
  timer = time(NULL);
  setCell(1, 1, 42);
  
  printf("  Attempting to get the value of a cell\n");
  if(getCell(1, 1) != 42){
    printf("    Set / Get failure\n");
    return false;
  }
  else{
    printf("    Set / Get pass\n");
  }
  printf("Time to get / set a single value is %d seconds\n", 
	 time(NULL) - timer);

  // ---------- Test two: get / set many values
  printf("  Attempting to set the value of many cells\n");
  int xcounter, ycounter;
  timer = time(NULL);
  for(xcounter = 0; xcounter < m_x; xcounter++){
    for(ycounter = 0; ycounter < m_x; ycounter++){
      setCell(xcounter, ycounter, (xcounter * ycounter) % 32000);
    }
  }
  
  printf("  Attempting to get the value of many cells\n");
  bool manypassed = true;
  for(xcounter = 0; xcounter < m_x; xcounter++){
    for(ycounter = 0; ycounter < m_y; ycounter++){
      if(getCell(xcounter, ycounter) != (xcounter * ycounter) % 32000){
	manypassed = false;
      }
    }
  }  

  if(!manypassed){
    printf("    Set / Get many failure\n");
    return false;
  }
  else{
    printf("    Set / Get many pass\n");
  }
  printf("Getting and setting %d values took %d seconds\n",
	 m_x * m_y, time(NULL) - timer);

  // Otherwise we passed them all
  return true;
}

void cepMatrixImp::addRef(){
  m_refs++;
}

void cepMatrixImp::decRef(){
  m_refs--;
}

int cepMatrixImp::width(){
  return m_x;
}

int cepMatrixImp::height(){
  return m_y;
}
