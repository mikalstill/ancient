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

#include "cepWindowUi.h"
  
BEGIN_EVENT_TABLE (cepWindowAttenuation, wxDialog)
  EVT_BUTTON(CEPBTN_ATT_SUBMIT, cepWindowAttenuation::dlgAttOnOK)
  EVT_BUTTON(CEPBTN_ATT_CANCEL, cepWindowAttenuation::dlgAttOnQuit)
  EVT_CLOSE( cepWindowAttenuation::dlgAttOnQuit)
END_EVENT_TABLE ()
                          
cepWindowAttenuation::cepWindowAttenuation():
  wxDialog((wxDialog *) NULL, -1, "Specify Attenuation", wxPoint(120,120), wxSize(200, 120))
{
  m_panel = new wxPanel(this, -1, wxPoint(120,120), wxSize(200,120));

  m_statBox = new wxStaticBox(m_panel, -1, "", wxPoint(15, 30), wxSize(170, 40));

  m_statText1 = new wxStaticText(m_panel, -1, "Please specify the Attenuation", wxPoint(5,5), wxSize(190, 20), wxALIGN_CENTRE);
  m_statText2 = new wxStaticText(m_panel, -1, "for the Windowed data:", wxPoint(5,19), wxSize(190, 20), wxALIGN_CENTRE);

  m_statText3 = new wxStaticText(m_panel, -1, "Attenuation:", wxPoint(25,40), wxSize(100, 20), wxALIGN_LEFT);

  m_tbAttenuation = new wxTextCtrl(m_panel, -1, "0.0", wxPoint(110, 40), wxSize(60, 20));

  m_bSubmit = new wxButton(m_panel, CEPBTN_ATT_SUBMIT, "Ok", wxPoint(10,80));
  m_bCancel = new wxButton(m_panel, CEPBTN_ATT_CANCEL, "Cancel", wxPoint(110,80));

  Center();
  ShowModal();
}

double cepWindowAttenuation::getAttenuation()
{
  for(size_t i = 0; i < m_attenuation.Length(); i ++)
  {
    if(cepIsNumeric(m_attenuation.GetChar(i)) == false)
    {
      return -2.0;
    }
  }

  return (atof(m_attenuation.c_str()));
}
        
void cepWindowAttenuation::dlgAttOnQuit(wxCommandEvent& WXUNUSED(event))
{
  //if cancel or quit button pressed 
  m_attenuation = "-1";
  
  EndModal(1);
  Destroy();
}

void cepWindowAttenuation::dlgAttOnOK(wxCommandEvent& WXUNUSED(event))
{
  m_attenuation = m_tbAttenuation->GetValue();
  
  EndModal(0);
  Destroy();  
}

cepWindowUi::cepWindowUi() {}

void cepWindowUi::showAttenuation()
{
  cepWindowAttenuation wa;

  m_attenuation = wa.getAttenuation();
}

double cepWindowUi::getAttenuation()
{
  return m_attenuation;
}

