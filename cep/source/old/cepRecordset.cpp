 /* 
   Imp for the CEP recordset
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

#include "core.h"

void cepRecordrow::putCell(int col, string data){
   m_cells[col] = data;
 }
string cepRecordrow::getCell(int col){
  return m_cells[col];
}

cepRecordset::cepRecordset():
  m_currentRow(0)
{
}

void cepRecordset::putRowCell(int row, int col, string value){
  m_rows[row].putCell(col, value);
}
