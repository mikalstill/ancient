/* 
   CEP matrix
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

#include "cepMatrix.h"
#include "cepMatrixImp.h"
#include "cepVector.h"
#include <stdio.h>
#include <time.h>

cepMatrix::cepMatrix(int x, int y)
{
  m_imp = new cepMatrixImp(x, y);
  m_transposed = false;
}

cepMatrix::cepMatrix(cepMatrixImp *imp, bool trans){
  m_imp = imp;
  m_imp->addRef();

  m_transposed = trans;
}

cepMatrix::~cepMatrix(){
  m_imp->decRef();
}

int cepMatrix::getCell(int x, int y){
  if(m_transposed) return m_imp->getCell(y, x);
  return m_imp->getCell(x, y);
}

cepError cepMatrix::setCell(int x, int y, int value){
  if(m_transposed) return m_imp->setCell(y, x, value);
  return m_imp->setCell(x, y, value);
}

cepMatrix cepMatrix::transpose(){
  return cepMatrix(m_imp, !m_transposed);
}

int cepMatrix::width(){
  if(m_transposed)
    return m_imp->width();
  return m_imp->height();
}

int cepMatrix::height(){
  if(m_transposed)
    return m_imp->height();
  return m_imp->width();
}

cepVector cepMatrix::operator*(cepVector& vec)
{
  cepVector result(vec.size());
  int xcount, ycount;

  for(xcount = 0; xcount < width(); xcount++){
    int val = 0;
    
    for(ycount = 0; ycount < height(); ycount++){
      val += getCell(xcount, ycount) * vec.getCell(ycount);
    }

    result.setCell(xcount, val);
  }

  return result;
}

cepMatrix cepMatrix::operator*(cepMatrix& mat)
{
  cepMatrix result(42, 42);
  return result;
}
