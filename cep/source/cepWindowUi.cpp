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
#include <bits/nan.h>
  
BEGIN_EVENT_TABLE (cepWindowBandwidth, wxDialog)
  EVT_BUTTON(CEPBTN_ATT_SUBMIT, cepWindowBandwidth::dlgBandwidthOnOK)
  EVT_BUTTON(CEPBTN_ATT_CANCEL, cepWindowBandwidth::dlgBandwidthOnQuit)
  EVT_CLOSE( cepWindowBandwidth::dlgBandwidthOnQuit)
END_EVENT_TABLE ()
                          
cepWindowBandwidth::cepWindowBandwidth():
  wxDialog((wxDialog *) NULL, -1, "Specify Transition Bandwidth", wxPoint(120,120), wxSize(250, 120))
{
  // panel and frame
  m_panel = new wxPanel(this, -1, wxPoint(120,120), wxSize(250,120));
  m_statBox = new wxStaticBox(m_panel, -1, "", wxPoint(25, 35), wxSize(200, 40));

  // the text labels
  m_statText1 = new wxStaticText(m_panel, -1, "Please specify the Normalised Transition", wxPoint(5,5), wxSize(240, 20), wxALIGN_CENTRE);
  m_statText2 = new wxStaticText(m_panel, -1, "Bandwidth for Dolph-Chebyshev", wxPoint(5,19), wxSize(240, 20), wxALIGN_CENTRE);
  m_statText3 = new wxStaticText(m_panel, -1, "NTB:", wxPoint(35,48), wxSize(70, 20), wxALIGN_LEFT);
  m_tbBandwidth = new wxTextCtrl(m_panel, -1, "0.0", wxPoint(100, 47), wxSize(115, 20));

  // the buttons
  m_bSubmit = new wxButton(m_panel, CEPBTN_ATT_SUBMIT, "Ok", wxPoint(25,80));
  m_bCancel = new wxButton(m_panel, CEPBTN_ATT_CANCEL, "Cancel", wxPoint(145,80));

  Center();
  ShowModal();
}

double cepWindowBandwidth::getBandwidth()
{
  for(size_t i = 0; i < m_bandwidth.Length(); i ++)
  {
    if(cepIsNumeric(m_bandwidth.GetChar(i)) == false)
    {
      return NAN;
    }
  }
  return (atof(m_bandwidth.c_str()));
}
        
void cepWindowBandwidth::dlgBandwidthOnQuit(wxCommandEvent& WXUNUSED(event))
{
  //if cancel or quit button pressed 
  m_bandwidth = "-1";
  
  EndModal(1);
  Destroy();
}

void cepWindowBandwidth::dlgBandwidthOnOK(wxCommandEvent& WXUNUSED(event))
{
  m_bandwidth = m_tbBandwidth->GetValue();
  
  EndModal(0);
  Destroy();
}

cepWindowUi::cepWindowUi() {}

void cepWindowUi::showBandwidth()
{
  cepWindowBandwidth wa;

  m_bandwidth = wa.getBandwidth();
}

double cepWindowUi::getBandwidth()
{
  return m_bandwidth;
}


const char* cepWindowBandwidth::UNINITIALISED_STR = "-1";
const int cepWindowBandwidth::UNINITIALISED = atoi(UNINITIALISED_STR);

