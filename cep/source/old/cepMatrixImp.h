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

#include "cepError.h"

#ifndef CEP_MATRIXIMP_HEADER
#define CEP_MATRIXIMP_HEADER

class cepMatrixImp{
 public:
   cepMatrixImp(int x, int y);

   int getCell(int x, int y);
   cepError setCell(int x, int y, int value);

   void addRef();
   void decRef();

   int width();
   int height();

   bool test();

 private:
   int *m_cells;
   int m_x;
   int m_y;
   int m_refs;
 };

#endif
