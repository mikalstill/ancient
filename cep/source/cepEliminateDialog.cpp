
/* 
 *   Imp for the CEP data presentation
 *   Copyright (C) Michael Still                    2002
 *   
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *   
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *   
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "cepCore.h"
#include "cepEliminateDialog.h"

// todo_mikal: this is not completely implemented

// We define simple things like the size of the dialog here, the dialog
// is actually displayed using display() below...
cepEliminateDialog::cepEliminateDialog():
  m_position(0, 0),
  m_size(200, 200),
  m_posok(10, 10),
  m_dialog(NULL, -1, "Eliinate outliers", m_position, m_size, 
	   wxDIALOG_MODAL, "eliminateOutliers")
  //  m_ok(m_dialog, wxID_OK, "Ok", m_posok)
{
}

void cepEliminateDialog::display()
{
  m_dialog.Centre();
  m_dialog.ShowModal();
  
}
