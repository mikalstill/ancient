/* 
   CEP vector
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

#include "cepVector.h"
#include "cepMatrix.h"
#include <stdio.h>

cepVector::cepVector(int x)
{
  m_cells = new int[x];
  m_x = x;
}

cepVector::~cepVector(){
  delete m_cells;
}

int cepVector::getCell(int x){
  return m_cells[x];
}

cepError cepVector::setCell(int x, int value){
  if(x > m_x)
    return cepError("Vector cell out of range");

  m_cells[x] = value;
}

int cepVector::size()
{
  return m_x;
}

// todo_mikal
//cepVector cepVector::operator*(cepMatrix& mat)
//{
//  return mat.operator*(this);
//}
