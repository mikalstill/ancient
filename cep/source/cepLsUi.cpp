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
  m_bSubmit->SetDefault();
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

cepLsWeight::cepLsWeight(wxString fromDay, wxString fromMonth, wxString fromYear,
                         wxString toDay, wxString toMonth, wxString toYear,
                         wxString val):
  wxDialog((wxDialog *) NULL, -1, "Weight Data", wxPoint(120,120), wxSize(300, 200))
{
  m_go = false;
  
  m_panel = new wxPanel(this, -1, wxPoint(120,200), wxSize(300,200));

  m_statBox = new wxStaticBox(m_panel, -1, "", wxPoint(15, 30), wxSize(270, 120));

  m_statText1 = new wxStaticText(m_panel, -1, "Please specify the data range and the value", wxPoint(5,5), wxSize(290, 20), wxALIGN_CENTRE);
  m_statText2 = new wxStaticText(m_panel, -1, "for the weighting matrix:", wxPoint(5,19), wxSize(290, 20), wxALIGN_CENTRE);

  m_statText3 = new wxStaticText(m_panel, -1, "From:", wxPoint(25,50), wxSize(30, 20), wxALIGN_LEFT);
  m_cbFromDay = new wxComboBox(m_panel, -1, "", wxPoint(65, 50), wxSize(43, 20), 31, LS_DAYS, wxCB_READONLY);
  m_cbFromMonth = new wxComboBox(m_panel, -1, "", wxPoint(113, 50), wxSize(100, 20), 12, LS_MONTHS, wxCB_READONLY);
  m_tbFromYear = new wxTextCtrl(m_panel, - 1, "2000", wxPoint(218, 50), wxSize(60, 20));
  
  m_cbFromDay->SetValue(fromDay);
  m_cbFromMonth->SetValue(fromMonth);
  m_tbFromYear->SetValue(fromYear);

  m_statText4 = new wxStaticText(m_panel, -1, "To:", wxPoint(25,80), wxSize(30, 20), wxALIGN_LEFT);
  m_cbToDay = new wxComboBox(m_panel, -1, "", wxPoint(65, 80), wxSize(43, 20), 31, LS_DAYS, wxCB_READONLY);
  m_cbToMonth = new wxComboBox(m_panel, -1, "", wxPoint(113, 80), wxSize(100, 20), 12, LS_MONTHS, wxCB_READONLY);
  m_tbToYear = new wxTextCtrl(m_panel, - 1, "2000", wxPoint(218, 80), wxSize(60, 20));

  m_cbToDay->SetValue(toDay);
  m_cbToMonth->SetValue(toMonth);
  m_tbToYear->SetValue(toYear);

  m_statText5 = new wxStaticText(m_panel, -1, "Value: ", wxPoint(25, 110), wxSize(35,20), wxALIGN_LEFT);
  m_tbVal = new wxTextCtrl(m_panel, -1, val, wxPoint(65, 110), wxSize(50,20));

  m_bSubmit = new wxButton(m_panel, CEPBTN_WEIGHT_SUBMIT, "Ok", wxPoint(15,160));
  m_bSubmit->SetDefault();
  m_bCancel = new wxButton(m_panel, CEPBTN_WEIGHT_CANCEL, "Cancel", wxPoint(110,160));
  m_bGo = new wxButton(m_panel, CEPBTN_WEIGHT_GO, "Do VCV", wxPoint(205, 160));

  Center();
  ShowModal();
}

void cepLsWeight::dlgWeightOnQuit(wxCommandEvent& WXUNUSED(event))
{
  m_fromDay = m_fromMonth = m_fromYear = m_toDay = m_toMonth = m_toYear = m_val = "-1";
  
  EndModal(1);
  Destroy();
}

void cepLsWeight::dlgWeightOnGo(wxCommandEvent& WXUNUSED(event))
{
  m_go = true;

  m_val = m_tbVal->GetValue();

  m_fromDay = m_cbFromDay->GetValue();
  m_fromMonth = m_cbFromMonth->GetValue();
  m_fromYear = m_tbFromYear->GetValue();

  m_toDay = m_cbToDay->GetValue();
  m_toMonth = m_cbToMonth->GetValue();
  m_toYear = m_tbToYear->GetValue().c_str();

  EndModal(2);
  Destroy();
}
void cepLsWeight::dlgWeightOnOK(wxCommandEvent& WXUNUSED(event))
{
  m_val = m_tbVal->GetValue();

  m_fromDay = m_cbFromDay->GetValue();
  m_fromMonth = m_cbFromMonth->GetValue();
  m_fromYear = m_tbFromYear->GetValue();

  m_toDay = m_cbToDay->GetValue();
  m_toMonth = m_cbToMonth->GetValue();
  m_toYear = m_tbToYear->GetValue().c_str();

  cout << "on ok " << m_fromDay << " " << m_fromMonth << " " << m_fromYear << endl
       << m_toDay << " " << m_toMonth << " " << m_toYear << endl;
  EndModal(0);
  Destroy();
}

const wxString & cepLsWeight::getWeight()
{
  return m_val;
}

const wxString & cepLsWeight::getToDay()
{
  return m_toDay;
}

const wxString & cepLsWeight::getToMonth()
{
  return m_toMonth;
}

const wxString & cepLsWeight::getToYear()
{
  return m_toYear;
}

const wxString & cepLsWeight::getFromDay()
{
  return m_fromDay;
}

const wxString & cepLsWeight::getFromMonth()
{
  return m_fromMonth;
}

const wxString & cepLsWeight::getFromYear()
{
  return m_fromYear;
}

const bool & cepLsWeight::getDoVCV()
{

  return m_go;
}

cepLsUi::cepLsUi() {}

void cepLsUi::showIsReweight()
{
  wxMessageDialog dlg(NULL, "Do you wish to use the Automatic Re-weighting Function?", "Least Squares Regression", wxYES_NO|wxCANCEL);

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
  cepLsWeight *weight = NULL;
  bool isValid = false;
  cepDate *fromDate = NULL,
          *toDate = NULL;

  fromDate = new cepDate(startDate);
  toDate = new cepDate(endDate);
          
  weight = new cepLsWeight(fromDate->getDay().c_str(), fromDate->getMonthName().c_str(), fromDate->getYear().c_str(),
                           toDate->getDay().c_str(), toDate->getMonthName().c_str(), toDate->getYear().c_str(),
                           cepToString(val).c_str());

  //while values entered is invalid
  while(isValid == false)
  {
    isValid = true;

    if(weight->getToDay() == "-1")
    {
      cout << "in cancel " << endl;
      //if cancel was selected
      m_fromDay = "-1.0";
      m_toDay = "-1.0";
      m_val = "-1.0";
      return;
    }                          
    else
    {
      m_fromDay = weight->getFromDay();
      m_fromMonth = weight->getFromMonth();
      m_fromYear = weight->getFromYear();
      m_toDay = weight->getToDay();
      m_toMonth = weight->getToMonth();
      m_toYear = weight->getToYear();
      m_val = weight->getWeight();
      m_go = weight->getDoVCV();

      fromDate = new cepDate(atoi(m_fromDay.c_str()), m_fromMonth.c_str(), atoi(m_fromYear.c_str()));
      toDate = new cepDate(atoi(m_toDay.c_str()), m_toMonth.c_str(), atoi(m_toYear.c_str()));

      //test for invalid dates
      if((fromDate->getDecimalDate() == -1) || (toDate->getDecimalDate() == -1))
      {
        isValid = false;
        weight = new cepLsWeight(m_fromDay, m_fromMonth, m_fromYear,
                                 m_toDay, m_toMonth, m_toYear,
                                 m_val);
        continue;
      }

      //test if start date is > end date
      if(fromDate->getDecimalDate() > toDate->getDecimalDate())
      {
        cepError("Error. Invalid date entered", cepError::sevWarning).display();
        isValid = false;
        weight = new cepLsWeight(m_fromDay, m_fromMonth, m_fromYear,
                                 m_toDay, m_toMonth, m_toYear,
                                 m_val);
        continue;
      }

      //test for invalid weighting value     
      for(size_t i = 0; i < m_val.Length(); i ++)
      {
        if((cepIsNumeric(m_val.GetChar(i)) == false) ||
          (atof(m_val.c_str()) < 0))
        {
          cepError("Error. Weighting value is invalid", cepError::sevWarning).display();
          isValid = false;
          weight = new cepLsWeight(m_fromDay, m_fromMonth, m_fromYear,
                                   m_toDay, m_toMonth, m_toYear,
                                   m_val);
          break;
        }
      }
    }
  }

  cout << "go values " << m_fromDay << " " << m_fromMonth << " " << m_fromYear << endl
       << m_toDay << " " << m_toMonth << " " << m_toYear << endl;
   
}

const int & cepLsUi::getIsReweight()
{
  return m_isReweight;
}

const bool cepLsUi::getWhichDir(char dir)
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
      return false;
  }
}

const int & cepLsUi::getIsReadP()
{
  return m_isReadP;
}

const string & cepLsUi::getfNameP()
{
  return m_filename;
}

const double cepLsUi::getWeight()
{
  if(m_val == "-1.0")
  {
    return -1.0;
  }

  return atof(m_val.c_str());
}

const double cepLsUi::getFromDate()
{
  if(m_fromDay == "-1.0")
  {
    return -1.0;
  }            
  
  return cepDate(atoi(m_fromDay.c_str()), m_fromMonth.c_str(), atoi(m_fromYear.c_str())).getDecimalDate();
}

const double cepLsUi::getToDate()
{
  if(m_toDay == "-1.0")
  {
    return -1.0;
  }
  
  return cepDate(atoi(m_toDay.c_str()), m_toMonth.c_str(), atoi(m_toYear.c_str())).getDecimalDate();
}

const bool & cepLsUi::getDoVCV()
{
  return m_go;
}
