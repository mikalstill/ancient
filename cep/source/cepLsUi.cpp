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
#include <bits/nan.h>

BEGIN_EVENT_TABLE (cepLsShowDir, wxDialog)
  EVT_BUTTON(CEPBTN_DIR_SUBMIT, cepLsShowDir::dlgDirOnOK)
  EVT_BUTTON(CEPBTN_DIR_CANCEL, cepLsShowDir::dlgDirOnQuit)
  EVT_CLOSE( cepLsShowDir::dlgDirOnQuit)
END_EVENT_TABLE ()

cepLsShowDir::cepLsShowDir():
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

void cepLsShowDir::dlgDirOnQuit(wxCommandEvent& WXUNUSED(event))
{
  //if cancel or quit button pressed set all values to false
  m_cbDirX->SetValue(false);
  m_cbDirY->SetValue(false);
  m_cbDirZ->SetValue(false);
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

BEGIN_EVENT_TABLE (cepLsWeight, wxDialog)
  EVT_BUTTON(CEPBTN_WEIGHT_SUBMIT, cepLsWeight::dlgWeightOnOK)
  EVT_BUTTON(CEPBTN_WEIGHT_CANCEL, cepLsWeight::dlgWeightOnQuit)
  EVT_BUTTON(CEPBTN_WEIGHT_GO, cepLsWeight::dlgWeightOnGo)
  EVT_CLOSE( cepLsWeight::dlgWeightOnQuit)
END_EVENT_TABLE ()

cepLsWeight::cepLsWeight(double &startDate, double &endDate, double val):
  wxDialog((wxDialog *) NULL, -1, "Weight Data", wxPoint(120,120), wxSize(300, 200))
{
  cepDate fromDate(startDate), toDate(endDate);
  m_go = false;
  
  m_panel = new wxPanel(this, -1, wxPoint(120,200), wxSize(300,200));

  m_statBox = new wxStaticBox(m_panel, -1, "", wxPoint(15, 30), wxSize(270, 120));

  m_statText1 = new wxStaticText(m_panel, -1, "Please specify the data range and the value", wxPoint(5,5), wxSize(290, 20), wxALIGN_CENTRE);
  m_statText2 = new wxStaticText(m_panel, -1, "for the weighting matrix:", wxPoint(5,19), wxSize(290, 20), wxALIGN_CENTRE);

  m_statText3 = new wxStaticText(m_panel, -1, "From:", wxPoint(25,50), wxSize(30, 20), wxALIGN_LEFT);
  m_cbFromDay = new wxComboBox(m_panel, -1, "", wxPoint(65, 50), wxSize(43, 20), 31, LS_DAYS, wxCB_READONLY);
  m_cbFromMonth = new wxComboBox(m_panel, -1, "", wxPoint(113, 50), wxSize(100, 20), 12, LS_MONTHS, wxCB_READONLY);
  m_tbFromYear = new wxTextCtrl(m_panel, - 1, "2000", wxPoint(218, 50), wxSize(60, 20));
  
  m_cbFromDay->SetValue(fromDate.getDay().c_str());
  m_cbFromMonth->SetValue(fromDate.getMonthName().c_str());
  m_tbFromYear->SetValue(fromDate.getYear().c_str());

  m_statText4 = new wxStaticText(m_panel, -1, "To:", wxPoint(25,80), wxSize(30, 20), wxALIGN_LEFT);
  m_cbToDay = new wxComboBox(m_panel, -1, "", wxPoint(65, 80), wxSize(43, 20), 31, LS_DAYS, wxCB_READONLY);
  m_cbToMonth = new wxComboBox(m_panel, -1, "", wxPoint(113, 80), wxSize(100, 20), 12, LS_MONTHS, wxCB_READONLY);
  m_tbToYear = new wxTextCtrl(m_panel, - 1, "2000", wxPoint(218, 80), wxSize(60, 20));

  m_cbToDay->SetValue(toDate.getDay().c_str());
  m_cbToMonth->SetValue(toDate.getMonthName().c_str());
  m_tbToYear->SetValue(toDate.getYear().c_str());

  m_statText5 = new wxStaticText(m_panel, -1, "Value: ", wxPoint(25, 110), wxSize(35,20), wxALIGN_LEFT);
  m_tbVal = new wxTextCtrl(m_panel, -1, cepToString(val).c_str(), wxPoint(65, 110), wxSize(50,20));

  m_bSubmit = new wxButton(m_panel, CEPBTN_WEIGHT_SUBMIT, "Ok", wxPoint(15,160));
  m_bCancel = new wxButton(m_panel, CEPBTN_WEIGHT_CANCEL, "Cancel", wxPoint(110,160));
  m_bGo = new wxButton(m_panel, CEPBTN_WEIGHT_GO, "Do VCV", wxPoint(205, 160));

  Center();
  ShowModal();
}

void cepLsWeight::dlgWeightOnQuit(wxCommandEvent& WXUNUSED(event))
{
  m_val = "-2.0";
  m_toDate = -2.0;
  m_fromDate = -2.0;
  
  EndModal(1);
  Destroy();
}

void cepLsWeight::dlgWeightOnGo(wxCommandEvent& WXUNUSED(event))
{
  m_go = true;

  m_val = m_tbVal->GetValue();

  m_toDate = cepDate(atoi(m_cbToDay->GetValue().c_str()), m_cbToMonth->GetValue().c_str(), atoi(m_tbToYear->GetValue().c_str())).getDecimalDate();
  m_fromDate = cepDate(atoi(m_cbFromDay->GetValue().c_str()), m_cbFromMonth->GetValue().c_str(), atoi(m_tbFromYear->GetValue().c_str())).getDecimalDate();

  cout << endl << "#####ON GO#######" << endl;
  cout << "###########VALUE " << m_go << endl;
  cout << "###########VALUE " << m_val << endl;
  cout << "########fromDate " << m_fromDate << endl;
  cout << "########toDate " << m_toDate << endl;

  EndModal(2);
  Destroy();
}
void cepLsWeight::dlgWeightOnOK(wxCommandEvent& WXUNUSED(event))
{
  m_val = m_tbVal->GetValue();
  
  m_toDate = cepDate(atoi(m_cbToDay->GetValue().c_str()), m_cbToMonth->GetValue().c_str(), atoi(m_tbToYear->GetValue().c_str())).getDecimalDate();
  m_fromDate = cepDate(atoi(m_cbFromDay->GetValue().c_str()), m_cbFromMonth->GetValue().c_str(), atoi(m_tbFromYear->GetValue().c_str())).getDecimalDate();

  cout << endl << "#####ON OK#######" << endl;
  cout << "###########VALUE " << m_val << endl;
  cout << "########fromDate " << m_fromDate << endl;
  cout << "########toDate " << m_toDate << endl;

  EndModal(0);
  Destroy();
}

double cepLsWeight::getWeight()
{
  for(size_t i = 0; i < m_val.Length(); i ++)
  {
    if(cepIsNumeric(m_val.GetChar(i)) == false)
    {
      return NAN;
    }
  }
  return (atof(m_val.c_str()));
}

double cepLsWeight::getFromDate()
{

  return m_fromDate;
}

double cepLsWeight::getToDate()
{

  return m_toDate;
}

bool cepLsWeight::getDoVCV()
{

  return m_go;
}

cepLsUi::cepLsUi() {}

void cepLsUi::showIsReweight()
{
  wxMessageDialog dlg(NULL, "Do you wish to use the Automatic Re-weighting Fuction?", "Least Squares Regression", wxYES_NO|wxCANCEL);

  dlg.Center();
  
  switch (dlg.ShowModal())
  {
    case wxID_YES:    //if yes pressed
      m_isReweight = 1;
      break;
    case wxID_NO:     //if no pressed
      m_isReweight = 0;
      break;
    case wxID_CANCEL: //if cancel pressed
      m_isReweight = -1;
      break;
    default:
      //**************ERROR here!
      m_isReweight = -1;
      break;
  }  
}

void cepLsUi::showWhichDir()
{
  cepLsShowDir sd;

  m_doDirX = sd.getWhichDir('x');
  m_doDirY = sd.getWhichDir('y');
  m_doDirZ = sd.getWhichDir('z');
}

void cepLsUi::showIsReadP(string dir)
{
  wxMessageDialog dlg(NULL, wxString( "Do you wish to load the weighting matrix for direction ") + wxString(dir.c_str()) + wxString(" from a file?:" ), "Specify a Weighting Matrix", wxYES_NO|wxCANCEL);

  dlg.Centre();

   switch (dlg.ShowModal())
  {
    case wxID_YES:    //if yes pressed
      m_isReadP = 1;
      break;
    case wxID_NO:     //if no pressed
      m_isReadP = 0;
      break;
    case wxID_CANCEL: //if cancel pressed
      m_isReadP = -1;
      break;
    default:
      //**************ERROR here!
      m_isReadP = -1;
      break;
  }
}

void cepLsUi::showGetfNameP()
{
  wxFileDialog dlg(NULL, "Choose Weighted Matrix File", "", "", "*.dat", wxOPEN, wxPoint(-1, -1));

  dlg.Centre();

  switch (dlg.ShowModal())
  {
    case wxID_OK:     //if ok pressed
      m_filename = dlg.GetPath().c_str();
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

void cepLsUi::showWeight(double startDate, double endDate, double val)
{
  cepLsWeight weight(startDate, endDate, val);

  m_weight = weight.getWeight();
  m_toDate = weight.getToDate();
  m_fromDate = weight.getFromDate();
  m_go = weight.getDoVCV();
  
}
int cepLsUi::getIsReweight()
{
  return m_isReweight;
}

bool cepLsUi::getWhichDir(char dir)
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
int cepLsUi::getIsReadP()
{
  return m_isReadP;
}

string cepLsUi::getfNameP()
{
  return m_filename;
}

double cepLsUi::getWeight()
{
  return m_weight;
}

double cepLsUi::getFromDate()
{
  return m_fromDate;
}

double cepLsUi::getToDate()
{
  return m_toDate;
}

bool cepLsUi::getDoVCV()
{
  return m_go;
}
