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
  
cepLsUi::cepLsUi()
{
  dirDlg = NULL;
}

cepLsUi::~cepLsUi()
{
  if(dirDlg != NULL)
  {
    delete dirDlg;
  }
}

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
  dirDlg = new cepShowDir("Choose Direction", 120, 120,200,200);

  dirDlg->Center();
  dirDlg->ShowModal();
}

bool cepLsUi::getWhichDir(char dir)
{
  switch (dir)
  {
    case 'x':
      return dirDlg->getDoDir('x');
    case 'y':
      return dirDlg->getDoDir('y');
    case 'z':
      return dirDlg->getDoDir('z');
    default:
      return false;
  }
}

BEGIN_EVENT_TABLE (cepShowDir, wxDialog)
  EVT_BUTTON(CEPBTN_DIR_SUBMIT, cepShowDir::dlgDirOnOK)
  EVT_BUTTON(CEPBTN_DIR_CANCEL, cepShowDir::dlgDirOnQuit)
  EVT_CLOSE( cepShowDir::dlgDirOnQuit)
END_EVENT_TABLE ()

cepShowDir::cepShowDir(const wxString &title, int x, int y, int w, int h):
  wxDialog((wxDialog *) NULL, -1, title, wxPoint(x,y), wxSize(w, h))
{
  panel = new wxPanel(this, -1, wxPoint(x,y), wxSize(w,h));

  statBox = new wxStaticBox(panel, -1, "", wxPoint(15, 50), wxSize(170, 100));

  statText1 = new wxStaticText(panel, -1, "Please select the direction(s)", wxPoint(5,5), wxSize(190, 20), wxALIGN_CENTRE);
  statText2 = new wxStaticText(panel, -1, "you wish to preform the", wxPoint(5,19), wxSize(190, 20), wxALIGN_CENTRE);
  statText3 = new wxStaticText(panel, -1, " Least Squares transformation on:", wxPoint(5,33), wxSize(190, 20), wxALIGN_CENTRE);


  cbDirX = new wxCheckBox(panel, -1, "Direction: x (North)", wxPoint(25, 70));
  cbDirY = new wxCheckBox(panel, -1, "Direction: y (East)", wxPoint(25, 90));
  cbDirZ = new wxCheckBox(panel, -1, "Direction: z (Up)", wxPoint(25, 110));
  
  bSubmit = new wxButton(panel, CEPBTN_DIR_SUBMIT, "SUBMIT", wxPoint(10,160));
  bCancel = new wxButton(panel, CEPBTN_DIR_CANCEL, "CANCEL", wxPoint(110,160));
}

cepShowDir::~cepShowDir()
{
/*  if(panel != NULL)
  {
    delete panel;
  }

  if(statBox != NULL)
  {
    delete statBox;
  }

  if(statText1 != NULL)
  {
    delete statText1;
  }

  if(statText2 != NULL)
  {
    delete statText2;
  }

  if(statText3 != NULL)
  {
    delete statText3;
  }
       
  if(cbDirX != NULL)
  {
    delete cbDirX;
  }

  if(cbDirY != NULL)
  {
    delete cbDirY;
  }

  if(cbDirZ != NULL)
  {
    delete cbDirZ;
  }

  if(bSubmit != NULL)
  {           
    delete bSubmit;
  }

  if(bCancel != NULL)
  {
    delete bCancel;
  }
*/
}
      
void cepShowDir::dlgDirOnQuit(wxCommandEvent& WXUNUSED(event))
{
  cbDirX->SetValue(false);
  cbDirY->SetValue(false);
  cbDirZ->SetValue(false);
  gotValue = true;
  EndModal(1);
  Destroy();
}

void cepShowDir::dlgDirOnOK(wxCommandEvent& WXUNUSED(event))
{
  gotValue = true;
  EndModal(0);
  Destroy();  
}

bool cepShowDir::getDoDir(char dir)
{
  switch (dir)
  {
    case 'x':
      return cbDirX->GetValue();
    case 'y':
      return cbDirY->GetValue();
    case 'z':
      return cbDirZ->GetValue();
    default:
      return false;
  }
}
  