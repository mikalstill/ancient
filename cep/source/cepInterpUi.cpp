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

#include "cepInterpUi.h"
  
BEGIN_EVENT_TABLE (cepInterpShowDir, wxDialog)
  EVT_BUTTON(CEPBTN_DIR_SUBMIT, cepInterpShowDir::dlgDirOnOK)
  EVT_BUTTON(CEPBTN_DIR_CANCEL, cepInterpShowDir::dlgDirOnQuit)
  EVT_CLOSE( cepInterpShowDir::dlgDirOnQuit)
END_EVENT_TABLE ()
                          
cepInterpShowDir::cepInterpShowDir():
  wxDialog((wxDialog *) NULL, -1, "Choose Direction", wxPoint(120,120), wxSize(200, 200))
{
  m_panel = new wxPanel(this, -1, wxPoint(120,120), wxSize(200,200));

  m_statBox = new wxStaticBox(m_panel, -1, "", wxPoint(15, 50), wxSize(170, 100));

  m_statText1 = new wxStaticText(m_panel, -1, "Please select the direction(s)", wxPoint(5,5), wxSize(190, 20), wxALIGN_CENTRE);
  m_statText2 = new wxStaticText(m_panel, -1, "you wish to preform the", wxPoint(5,19), wxSize(190, 20), wxALIGN_CENTRE);
  m_statText3 = new wxStaticText(m_panel, -1, " Least Squares transformation on:", wxPoint(5,33), wxSize(190, 20), wxALIGN_CENTRE);


  m_cbDirX = new wxCheckBox(m_panel, -1, "Direction: x (North)", wxPoint(25, 65));
  m_cbDirY = new wxCheckBox(m_panel, -1, "Direction: y (East)", wxPoint(25, 90));
  m_cbDirZ = new wxCheckBox(m_panel, -1, "Direction: z (Up)", wxPoint(25, 115));

  m_bSubmit = new wxButton(m_panel, CEPBTN_DIR_SUBMIT, "Ok", wxPoint(10,160));
  m_bCancel = new wxButton(m_panel, CEPBTN_DIR_CANCEL, "Cancel", wxPoint(110,160));

  Center();
  ShowModal();
}
      
void cepInterpShowDir::dlgDirOnQuit(wxCommandEvent& WXUNUSED(event))
{
  //if cancel or quit button pressed set all values to false
  m_cbDirX->SetValue(false);
  m_cbDirY->SetValue(false);
  m_cbDirZ->SetValue(false);
  EndModal(1);
  Destroy();
}

void cepInterpShowDir::dlgDirOnOK(wxCommandEvent& WXUNUSED(event))
{
  EndModal(0);
  Destroy();  
}

bool cepInterpShowDir::getWhichDir(char dir)
{
  //retuned the selected directions
  switch (dir)
  {
    case 'x':
      return m_cbDirX->GetValue();
    case 'y':
      return m_cbDirY->GetValue();
    case 'z':
      return m_cbDirZ->GetValue();
    default:
      //ERROR here!
      return false;
  }
}

cepInterpReadP::cepInterpReadP(string dir):
  wxMessageDialog(NULL, wxString( "Do you wish to load the weighting matrix for direction ") + wxString(dir.c_str()) + wxString(" from a file?:" ), "Specify a Weighting Matrix", wxYES_NO|wxCANCEL)
{
  Centre();

  switch (ShowModal())
  {
    case wxID_YES:    //if yes pressed
      m_isRead = 1;
      break;
    case wxID_NO:     //if no pressed
      m_isRead = 0;
      break;
    case wxID_CANCEL: //if cancel pressed
      m_isRead = -1;
      break;
    default:
      //**************ERROR here!
      m_isRead = -1;
      break;
  }
}

int cepInterpReadP::getIsReadP()
{
  return m_isRead;
}

cepInterpShowFile::cepInterpShowFile():
  wxFileDialog(NULL, "Choose Weighted Matrix File", "", "", "*.dat", wxOPEN, wxPoint(-1, -1))
{

  Center();

    switch (ShowModal())
  {
    case wxID_OK:     //if ok pressed
      m_filename = GetPath().c_str();
      break;
    case wxID_CANCEL: //if cancel pressed
      m_filename = "";
      break;
    default:
      //ERROR HERE!
      m_filename = "";
      break;
  }

}

string cepInterpShowFile::getFilename()
{
  return m_filename;
}

cepInterpUi::cepInterpUi() {}

void cepInterpUi::showWhichDir()
{
  cepInterpShowDir sd;

  m_doDirX = sd.getWhichDir('x');
  m_doDirY = sd.getWhichDir('y');
  m_doDirZ = sd.getWhichDir('z');
}

void cepInterpUi::showIsReadP(string dir)
{
  cepInterpReadP rp(dir);

  m_isReadP = rp.getIsReadP();
}

void cepInterpUi::showGetfNameP()
{
  cepInterpShowFile sf;

  m_filename = sf.getFilename();
}

int cepInterpUi::getIsReweight()
{
  return m_isReweight;
}

bool cepInterpUi::getWhichDir(char dir)
{
  //return selected directions
  switch (dir)
  {
    case 'x':
      return m_doDirX;
    case 'y':
      return m_doDirY;
    case 'z':
      return m_doDirZ;
    default:
      //ERROR here!
      return false;
  }

}
int cepInterpUi::getIsReadP()
{
  return m_isReadP;
}

string cepInterpUi::getfNameP()
{
  return m_filename;
}

