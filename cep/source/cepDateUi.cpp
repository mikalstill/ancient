/*
  * Imp for the Date wxWindows UI implementation
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

#include "cepDateUi.h"

BEGIN_EVENT_TABLE (cepDateRange, wxDialog)
  EVT_BUTTON(CEPBTN_ATT_SUBMIT, cepDateRange::dlgDateOnOK)
  EVT_BUTTON(CEPBTN_ATT_CANCEL, cepDateRange::dlgDateOnQuit)
  EVT_CLOSE( cepDateRange::dlgDateOnQuit)
END_EVENT_TABLE ()
                          
cepDateRange::cepDateRange(cepDate toDate, cepDate fromDate):
  wxDialog((wxDialog *) NULL, -1, "Select Data Range", wxPoint(120,200), wxSize(300, 200))
{
  m_panel = new wxPanel(this, -1, wxPoint(120,200), wxSize(300,200));

  m_statBox = new wxStaticBox(m_panel, -1, "", wxPoint(15, 30), wxSize(270, 120));

  m_statText1 = new wxStaticText(m_panel, -1, "Please specify the data range", wxPoint(5,5), wxSize(290, 20), wxALIGN_CENTRE);
  m_statText2 = new wxStaticText(m_panel, -1, "for the new data set:", wxPoint(5,19), wxSize(290, 20), wxALIGN_CENTRE);

  m_statText3 = new wxStaticText(m_panel, -1, "From:", wxPoint(25,60), wxSize(30, 20), wxALIGN_LEFT);
  m_cbFromDay = new wxComboBox(m_panel, -1, "", wxPoint(60, 60), wxSize(43, 20), 31, DATE_DAYS, wxCB_READONLY);
  m_cbFromMonth = new wxComboBox(m_panel, -1, "", wxPoint(108, 60), wxSize(100, 20), 12, DATE_MONTHS, wxCB_READONLY);
  m_tbFromYear = new wxTextCtrl(m_panel, - 1, "2000", wxPoint(213, 60), wxSize(60, 20));

  m_cbFromDay->SetValue(fromDate.getDay().c_str());
  m_cbFromMonth->SetValue(fromDate.getMonthName().c_str());
  m_tbFromYear->SetValue(fromDate.getYear().c_str());
  
  m_statText4 = new wxStaticText(m_panel, -1, "To:", wxPoint(25,100), wxSize(30, 20), wxALIGN_LEFT);
  m_cbToDay = new wxComboBox(m_panel, -1, "", wxPoint(60, 100), wxSize(43, 20), 31, DATE_DAYS, wxCB_READONLY);
  m_cbToMonth = new wxComboBox(m_panel, -1, "", wxPoint(108, 100), wxSize(100, 20), 12, DATE_MONTHS, wxCB_READONLY);
  m_tbToYear = new wxTextCtrl(m_panel, - 1, "2000", wxPoint(213, 100), wxSize(60, 20));

  m_cbToDay->SetValue(toDate.getDay().c_str());
  m_cbToMonth->SetValue(toDate.getMonthName().c_str());
  m_tbToYear->SetValue(toDate.getYear().c_str());
  
  m_bSubmit = new wxButton(m_panel, CEPBTN_RATE_SUBMIT, "Ok", wxPoint(60,160));
  m_bSubmit->SetDefault();
  m_bCancel = new wxButton(m_panel, CEPBTN_RATE_CANCEL, "Cancel", wxPoint(170,160));

  Center();
  ShowModal();
}

const double & cepDateRange::getToDate()
{
  return m_toDate;
}

const double & cepDateRange::getFromDate()
{
  return m_fromDate;
}

void cepDateRange::dlgDateOnQuit(wxCommandEvent& WXUNUSED(event))
{
  //set values to -2 if cancel is hit
  m_toDate = -2.0;
  m_fromDate = -2.0;
  
  EndModal(1);
  Destroy();
}

void cepDateRange::dlgDateOnOK(wxCommandEvent& WXUNUSED(event))
{
  //convert dates entered to a decimal date
  m_toDate = cepDate(atoi(m_cbToDay->GetValue().c_str()), m_cbToMonth->GetValue().c_str(), atoi(m_tbToYear->GetValue().c_str())).getDecimalDate();
  m_fromDate = cepDate(atoi(m_cbFromDay->GetValue().c_str()), m_cbFromMonth->GetValue().c_str(), atoi(m_tbFromYear->GetValue().c_str())).getDecimalDate();

  EndModal(0);
  Destroy();  
}

cepDateUi::cepDateUi() {}

void cepDateUi::showDateRange(cepDate toDate, cepDate fromDate)
{
  cepDateRange dr(toDate, fromDate);

  m_fromDate = dr.getFromDate();
  m_toDate = dr.getToDate();
}

double & cepDateUi::getToDate()
{
  return m_toDate;
}

double & cepDateUi::getFromDate()
{
  return m_fromDate;
}

