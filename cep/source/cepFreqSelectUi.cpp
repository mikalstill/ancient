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
  EVT_BUTTON(CEPBTN_FREQ_SUBMIT, cepFreqRange::dlgOnOK)
  EVT_BUTTON(CEPBTN_FREQ_CANCEL, cepFreqRange::dlgOnQuit)
  EVT_CLOSE( cepFreqRange::dlgOnQuit)
END_EVENT_TABLE ()
                          
cepFreqRange::cepFreqRange(double startFreq, double endFreq):
  wxDialog((wxDialog *) NULL, -1, "Select Frequency Range", wxPoint(120,200), wxSize(300, 200))
{
  m_panel = new wxPanel(this, -1, wxPoint(120,200), wxSize(300,200));

  m_statBox = new wxStaticBox(m_panel, -1, "", wxPoint(15, 30), wxSize(270, 120));

  m_statText1 = new wxStaticText(m_panel, -1, "Please specify the freqeuncy", wxPoint(5,5), wxSize(290, 20), wxALIGN_CENTRE);
  m_statText2 = new wxStaticText(m_panel, -1, "range for the new data set:", wxPoint(5,19), wxSize(290, 20), wxALIGN_CENTRE);

  m_statText3 = new wxStaticText(m_panel, -1, "Start Frequency:", wxPoint(25,60), wxSize(30, 20), wxALIGN_LEFT);
  m_tbStartFreq = new wxTextCtrl(m_panel, - 1, wxString(cepToString(startFreq).c_str()), wxPoint(213, 60), wxSize(60, 20));

  m_statText4 = new wxStaticText(m_panel, -1, "End Frequency:", wxPoint(25,100), wxSize(30, 20), wxALIGN_LEFT);
  m_tbEndFreq = new wxTextCtrl(m_panel, - 1, wxString(cepToString(endFreq).c_str()), wxPoint(213, 100), wxSize(60, 20));

  m_bSubmit = new wxButton(m_panel, CEPBTN_FREQ_SUBMIT, "Ok", wxPoint(60,160));
  m_bSubmit->SetDefault();
  m_bCancel = new wxButton(m_panel, CEPBTN_FREQ_CANCEL, "Cancel", wxPoint(170,160));

  Center();
  ShowModal();
}

const double & cepFreqRange::getEndFreq()
{
  return m_endFreq;
}

const double & cepFreqRange::getStartFreq()
{
  return m_startFreq;
}

void cepFreqRange::dlgOnQuit(wxCommandEvent& WXUNUSED(event))
{
  //set values to -2 if cancel is hit
  m_endFreq = -2.0;
  m_startFreq = -2.0;
  
  EndModal(1);
  Destroy();
}

void cepFreqRange::dlgOnOK(wxCommandEvent& WXUNUSED(event))
{
  //convert dates entered to a decimal date

  m_startFreq = atof(m_tbStartFreq->GetValue().c_str());
  m_endFreq = atof(m_tbEndFreq->GetValue().c_str());
    
  EndModal(0);
  Destroy();  
}

cepFreqUi::cepFreqUi() {}

void cepFreqUi::showFreqRange(double startFreq, double endFreq)
{
  cepFreqRange fr(startFreq, endFreq);

  m_startFreq = fr.getStartFreq();
  m_endFreq = fr.getEndFreq();
}

double & cepFreqUi::getStartFreq()
{
  return m_startFreq;
}

double & cepFreqUi::getEndFreq()
{
  return m_endFreq;
}

