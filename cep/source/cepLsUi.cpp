/*
  * Imp for the least squares wxWindows UI implementation
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

#include "cepLsUi.h"

BEGIN_EVENT_TABLE (cepLsUi, wxFrame)
  EVT_BUTTON (WHICH_DIR_OK, cepLsUi::onShowWhichDirOk)
END_EVENT_TABLE ()
  
cepLsUi::cepLsUi()
{
    doDirX = doDirY = doDirZ = false;
}

cepLsUi::~cepLsUi() {}

int cepLsUi::showIsReweight()
{
  wxMessageDialog  dialog(NULL, "Do you wish to use the Automatic Re-weighting fuction?", "Least Squares Regression", wxYES_NO|wxCANCEL);

  dialog.Centre();
  
  switch (dialog.ShowModal())
  {
    case wxID_YES:
      cout << "button pressed is yes" << endl;
      return 1;
      break;
    case wxID_NO:
      cout << "button pressed is no" << endl;
      return 0;
      break;
    case wxID_CANCEL:
      cout << "button pressed is cancel" << endl;
      return -1;
      break;
    default:
      return -1;
      break;
  }
    
    
}

void cepLsUi::showWhichDir()
{
  wxPanel *panel;
  frame = new wxFrame((wxFrame *) NULL, -1, "Chose Direction", wxPoint(300,300), wxSize(300,300));
  panel = new wxPanel(frame, -1, wxPoint(0,0), wxSize(300,300), wxTAB_TRAVERSAL);
  btnOk = new wxButton(panel, WHICH_DIR_OK, "&OK", wxPoint(200,250));
  frame ->Centre();

  
  frame ->Show(true);
  
  
}

void cepLsUi::onShowWhichDirOk(wxCommandEvent& WXUNUSED (event))
{
  cout << "ok clicked in show which dir OK" << endl;  
}
