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

#include "wx/docmdi.h"
#include <wx/image.h>
#include <wx/dialog.h>
#include <wx/button.h>
#include "cepCore.h"

class cepEliminateDialog{
 public:
  cepEliminateDialog();
  void display();

 private:
  wxPoint m_position, mposok;
  wxSize m_size;
  wxDialog m_dialog;
  wxButton m_ok;
};
