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
  
cepLsIsReweight::cepLsIsReweight():
  wxMessageDialog(NULL, "Do you wish to use the Automatic Re-weighting Fuction?", "Least Squares Regression", wxYES_NO|wxCANCEL)
{
  Centre();

  switch (ShowModal())
  {
    case wxID_YES:    //if yes pressed
      isReweight = 1;
      break;
    case wxID_NO:     //if no pressed
      isReweight = 2;
      break;
    case wxID_CANCEL: //if cancel pressed
      isReweight = -1;
      break;
    default:
      //**************ERROR here!
      isReweight = -1;
      break;
  }  
}

int cepLsIsReweight::getIsReweight()
{
  return isReweight;   
}

BEGIN_EVENT_TABLE (cepLsShowDir, wxDialog)
  EVT_BUTTON(CEPBTN_DIR_SUBMIT, cepLsShowDir::dlgDirOnOK)
  EVT_BUTTON(CEPBTN_DIR_CANCEL, cepLsShowDir::dlgDirOnQuit)
  EVT_CLOSE( cepLsShowDir::dlgDirOnQuit)
END_EVENT_TABLE ()
                          
cepLsShowDir::cepLsShowDir():
  wxDialog((wxDialog *) NULL, -1, "Choose Direction", wxPoint(120,120), wxSize(200, 200))
{
  panel = new wxPanel(this, -1, wxPoint(120,120), wxSize(200,200));

  statBox = new wxStaticBox(panel, -1, "", wxPoint(15, 50), wxSize(170, 100));

  statText1 = new wxStaticText(panel, -1, "Please select the direction(s)", wxPoint(5,5), wxSize(190, 20), wxALIGN_CENTRE);
  statText2 = new wxStaticText(panel, -1, "you wish to preform the", wxPoint(5,19), wxSize(190, 20), wxALIGN_CENTRE);
  statText3 = new wxStaticText(panel, -1, " Least Squares transformation on:", wxPoint(5,33), wxSize(190, 20), wxALIGN_CENTRE);


  cbDirX = new wxCheckBox(panel, -1, "Direction: x (North)", wxPoint(25, 70));
  cbDirY = new wxCheckBox(panel, -1, "Direction: y (East)", wxPoint(25, 90));
  cbDirZ = new wxCheckBox(panel, -1, "Direction: z (Up)", wxPoint(25, 110));

  bSubmit = new wxButton(panel, CEPBTN_DIR_SUBMIT, "SUBMIT", wxPoint(10,160));
  bCancel = new wxButton(panel, CEPBTN_DIR_CANCEL, "CANCEL", wxPoint(110,160));

  Center();
  ShowModal();
}
      
void cepLsShowDir::dlgDirOnQuit(wxCommandEvent& WXUNUSED(event))
{
  //if cancel or quit button pressed set all values to false
  cbDirX->SetValue(false);
  cbDirY->SetValue(false);
  cbDirZ->SetValue(false);
  EndModal(1);
  Destroy();
}

void cepLsShowDir::dlgDirOnOK(wxCommandEvent& WXUNUSED(event))
{
  EndModal(0);
  Destroy();  
}

bool cepLsShowDir::getWhichDir(char dir)
{
  //retuned the selected directions
  switch (dir)
  {
    case 'x':
      return cbDirX->GetValue();
    case 'y':
      return cbDirY->GetValue();
    case 'z':
      return cbDirZ->GetValue();
    default:
      //ERROR here!
      return false;
  }
}

BEGIN_EVENT_TABLE (cepLsReadP, wxDialog)
  EVT_BUTTON(CEPBTN_READ_SUBMIT, cepLsReadP::dlgReadOnOK)
  EVT_BUTTON(CEPBTN_READ_CANCEL, cepLsReadP::dlgReadOnQuit)
  EVT_CLOSE(cepLsReadP::dlgReadOnQuit)
END_EVENT_TABLE ()

cepLsReadP::cepLsReadP(string dir):
  wxDialog((wxDialog *) NULL, -1, "Specify Weighting Matrix", wxPoint(120,120), wxSize(200, 200))
{
  wxString wxDir = "matrix for direction ";
  wxDir += dir.c_str();

  isCancel = 0;
  
  panel = new wxPanel(this, -1, wxPoint(120,120), wxSize(200,200));

  statBox = new wxStaticBox(panel, -1, "", wxPoint(15, 50), wxSize(170, 100));

  statText1 = new wxStaticText(panel, -1, "Do you want to load the weighting", wxPoint(5,5), wxSize(190, 20), wxALIGN_CENTRE);
  statText2 = new wxStaticText(panel, -1, wxDir, wxPoint(5,19), wxSize(190, 20), wxALIGN_CENTRE);
  statText3 = new wxStaticText(panel, -1, "from a file?:", wxPoint(5,33), wxSize(190, 20), wxALIGN_CENTRE);

  rYes = new wxRadioButton(panel, -1, "Yes", wxPoint(25, 70), wxSize(-1, -1), wxRB_GROUP);
  rNo = new wxRadioButton(panel, -1, "No", wxPoint(25, 90));

  bSubmit = new wxButton(panel, CEPBTN_READ_SUBMIT, "SUBMIT", wxPoint(10,160));
  bCancel = new wxButton(panel, CEPBTN_READ_CANCEL, "CANCEL", wxPoint(110,160));

  Center();
  ShowModal();

}

int cepLsReadP::getIsReadP()
{
  //if cancel pressed
  if(isCancel == -1)
  {
    return -1;
  }

  //if read from file selected 
  if(rYes->GetValue() == true)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
void cepLsReadP::dlgReadOnQuit(wxCommandEvent& WXUNUSED(event))
{
  //if cancel or quit button selected
  isCancel = -1;
  EndModal(1);
  Destroy();
}

void cepLsReadP::dlgReadOnOK(wxCommandEvent& WXUNUSED(event))
{
  EndModal(0);
  Destroy(); 
}

cepLsShowFile::cepLsShowFile():
  wxFileDialog(NULL, "Choose Weighted Matrix File", "", "", "*.dat", wxOPEN, wxPoint(-1, -1))
{

  Center();

    switch (ShowModal())
  {
    case wxID_OK:     //if ok pressed
      filename = GetPath().c_str();
      break;
    case wxID_CANCEL: //if cancel pressed
      filename = "";
      break;
    default:
      //ERROR HERE!
      filename = "";
      break;
  }

}

string cepLsShowFile::getFilename()
{
  return filename;
}

cepLsUi::cepLsUi() {}

void cepLsUi::showIsReweight()
{
  cepLsIsReweight sir;

  isReweight = sir.getIsReweight();
}

void cepLsUi::showWhichDir()
{
  cepLsShowDir sd;

  doDirX = sd.getWhichDir('x');
  doDirY = sd.getWhichDir('y');
  doDirZ = sd.getWhichDir('z');
}

void cepLsUi::showIsReadP(string dir)
{
  cepLsReadP rp(dir);

  isReadP = rp.getIsReadP();
}

void cepLsUi::showGetfNameP()
{
  cepLsShowFile sf;

  filename = sf.getFilename();
}

int cepLsUi::getIsReweight()
{
  return isReweight;
}

bool cepLsUi::getWhichDir(char dir)
{
  //return selected directions
  switch (dir)
  {
    case 'x':
      return doDirX;
    case 'y':
      return doDirY;
    case 'z':
      return doDirZ;
    default:
      //ERROR here!
      return false;
  }

}
int cepLsUi::getIsReadP()
{
  return isReadP;
}

string cepLsUi::getfNameP()
{
  return filename;
}
