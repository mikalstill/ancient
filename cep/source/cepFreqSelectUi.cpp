/*
  * Imp for the frequency wxWindows UI implementation
  * Copyright (C) Kristy Van Der Vlist             2002
  * Copyright (C) Michael Still                    2002
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

#include "cepFreqSelectUi.h"

BEGIN_EVENT_TABLE (cepFreqRange, wxDialog)
  EVT_BUTTON(CEPBTN_ATT_SUBMIT, cepFreqRange::dlgOnOK)
  EVT_BUTTON(CEPBTN_ATT_CANCEL, cepFreqRange::dlgOnQuit)
  EVT_CLOSE( cepFreqRange::dlgOnQuit)
END_EVENT_TABLE ()
                          
cepFreqRange::cepFreqRange(cepDate toDate, cepDate fromDate):
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

const double & cepFreqRange::getTo()
{
  return m_to;
}

const double & cepFreqRange::getFrom()
{
  return m_from;
}

void cepFreqRange::dlgOnQuit(wxCommandEvent& WXUNUSED(event))
{
  //set values to -2 if cancel is hit
  m_to = -2.0;
  m_from = -2.0;
  
  EndModal(1);
  Destroy();
}

void cepFreqRange::dlgOnOK(wxCommandEvent& WXUNUSED(event))
{
  //convert dates entered to a decimal date
  m_to = cep(atoi(m_cbToDay->GetValue().c_str()), m_cbToMonth->GetValue().c_str(), atoi(m_tbToYear->GetValue().c_str())).getDecimal();
  m_from = cep(atoi(m_cbFromDay->GetValue().c_str()), m_cbFromMonth->GetValue().c_str(), atoi(m_tbFromYear->GetValue().c_str())).getDecimal();

  EndModal(0);
  Destroy();  
}

cepFreqUi::cepFreqUi() {}

void cepFreqUi::showRange(cep to, cep from)
{
  cepFreqRange dr(to, from);

  m_from = dr.getFrom();
  m_to = dr.getTo();
}

double & cepFreqUi::getTo()
{
  return m_to;
}

double & cepFreqUi::getFrom()
{
  return m_from;
}

