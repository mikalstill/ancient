/*
  * Imp for the Interpolation wxWindows UI implementation
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
  
BEGIN_EVENT_TABLE (cepInterpShowRate, wxDialog)
  EVT_BUTTON(CEPBTN_RATE_SUBMIT, cepInterpShowRate::dlgRateOnOK)
  EVT_BUTTON(CEPBTN_RATE_CANCEL, cepInterpShowRate::dlgRateOnQuit)
  EVT_CLOSE( cepInterpShowRate::dlgRateOnQuit)
END_EVENT_TABLE ()
                          
cepInterpShowRate::cepInterpShowRate():
  wxDialog((wxDialog *) NULL, -1, "Specify Sample Rate", wxPoint(120,120), wxSize(200, 200))
{
  m_panel = new wxPanel(this, -1, wxPoint(120,120), wxSize(200,200));

  m_statBox = new wxStaticBox(m_panel, -1, "", wxPoint(15, 30), wxSize(170, 120));

  m_statText1 = new wxStaticText(m_panel, -1, "Please specify the sample rate", wxPoint(5,5), wxSize(190, 20), wxALIGN_CENTRE);
  m_statText2 = new wxStaticText(m_panel, -1, "for the interpolated data:", wxPoint(5,19), wxSize(190, 20), wxALIGN_CENTRE);

  m_statText3 = new wxStaticText(m_panel, -1, "Sample Rate:", wxPoint(25,40), wxSize(100, 20), wxALIGN_LEFT);

  m_tbSample = new wxTextCtrl(m_panel, -1, "0.0", wxPoint(110, 40), wxSize(60, 20));

  m_rbYear = new wxRadioButton(m_panel, -1, "Years", wxPoint(25, 65), wxSize(120, 20), wxRB_GROUP);
  m_rbDays = new wxRadioButton(m_panel, -1, "Month", wxPoint(25, 90));
  m_rbHours = new wxRadioButton(m_panel, -1, "Days", wxPoint(25, 115));

  m_bSubmit = new wxButton(m_panel, CEPBTN_RATE_SUBMIT, "Ok", wxPoint(10,160));
  m_bCancel = new wxButton(m_panel, CEPBTN_RATE_CANCEL, "Cancel", wxPoint(110,160));

  Center();
  ShowModal();
}

double cepInterpShowRate::getSample()
{
  for(size_t i = 0; i < m_sampleRate.Length(); i ++)
  {
    if(cepIsNumeric(m_sampleRate.GetChar(i)) == false)
    {
      return -2.0;
    }
  }

  return (atof(m_sampleRate.c_str()));
}

cepInterpShowRate::units cepInterpShowRate::getUnits()
{
  return m_sampleUnits;
}
        
void cepInterpShowRate::dlgRateOnQuit(wxCommandEvent& WXUNUSED(event))
{
  //if cancel or quit button pressed 
  m_sampleRate = "-1";
  
  EndModal(1);
  Destroy();
}

void cepInterpShowRate::dlgRateOnOK(wxCommandEvent& WXUNUSED(event))
{
  if(m_rbYear->GetValue() == true)
  {
    m_sampleUnits = years;
  }
  else
  {
    if(m_rbDays->GetValue() == true)
    {
      m_sampleUnits = days;
    }
    else
    {
      if(m_rbHours->GetValue() == true)
      {
        m_sampleUnits = hours;
      }
      else
      {
        m_sampleUnits = unknowen;
      }
    }
  }

  m_sampleRate = m_tbSample->GetValue();
  
  EndModal(0);
  Destroy();  
}

cepInterpUi::cepInterpUi() {}

void cepInterpUi::showSampleRate()
{
  cepInterpShowRate sr;

  m_sampleUnits = sr.getUnits();
  m_sampleRate = sr.getSample();
}

double cepInterpUi::getSampleRate()
{
  return m_sampleRate;
}

cepInterpShowRate::units cepInterpUi::getUnits()
{
  return m_sampleUnits;
}
