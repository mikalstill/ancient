/*
  * Imp for the Windowing wxWindows UI implementation
  * Copyright (C) Kristy Van Der Vlist             2002
  * Copyright (C) Blake Swadling                   2002
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
#include "cepwindowchebyshev.h"
#include "cepDataWindower.h"
#include <bits/nan.h>
#include <sstream>
  
BEGIN_EVENT_TABLE (cepWindowBandwidth, wxDialog)
  EVT_BUTTON(CEPBTN_ATT_SUBMIT, cepWindowBandwidth::dlgBandwidthOnOK)
  EVT_BUTTON(CEPBTN_ATT_CANCEL, cepWindowBandwidth::dlgBandwidthOnQuit)
  EVT_CLOSE( cepWindowBandwidth::dlgBandwidthOnQuit)
END_EVENT_TABLE ()
                          
cepWindowBandwidth::cepWindowBandwidth( bool getBandwidth ):
  wxDialog((wxDialog *) NULL, -1, "Specify Transition Bandwidth", wxPoint(120,120), wxSize(250, 142)),
  m_panel(NULL),
  m_statBox(NULL),
  m_statText1(NULL), m_statText2(NULL), m_statText3(NULL), m_statText4(NULL), m_statText5(NULL),
  m_tbSize(NULL), m_tbOverlap(NULL), m_tbBandwidth(NULL),
  m_bSubmit(NULL), m_bCancel(NULL),
  m_size(UNINITIALISED_STR), m_overlap(UNINITIALISED_STR), m_bandwidth(UNINITIALISED_STR),
  aborted( false )
{
  ostringstream currentSize;
  currentSize << cepDataWindower::getSize();
  ostringstream currentOverlap;
  currentOverlap << cepDataWindower::getOverlap();
  ostringstream currentBW;
  currentBW << cepWindowChebyshev::getTransitionBandwidth();

  int step = 22;
  int inputY = 35;
  if( getBandwidth ) {
    SetSize(250,175);
  }
  
  // panel and frame
  // the size of these depends on whther we are requesting transition bandwidth
  m_panel = new wxPanel(this, -1, wxPoint(120,120), wxSize(250,(getBandwidth?120+step:120)));
  int boxHeight =  getBandwidth?110:65;
  m_statBox = new wxStaticBox(m_panel, -1, "", wxPoint(25, inputY-15), wxSize(200, boxHeight));

  // the text labels
  m_statText1 = new wxStaticText(m_panel, -1, "Please specify the Window Parameters", wxPoint(5,5), wxSize(240, 20), wxALIGN_CENTRE);

  m_statText2 = new wxStaticText(m_panel, -1, "Size:", wxPoint(35,inputY), wxSize(70, 20), wxALIGN_LEFT);
  m_tbSize = new wxTextCtrl(m_panel, -1, currentSize.str().c_str(), wxPoint(100, inputY), wxSize(115, 20));

  m_statText3 = new wxStaticText(m_panel, -1, "Overlap:", wxPoint(35,inputY+step), wxSize(70, 20), wxALIGN_LEFT);
  m_tbOverlap = new wxTextCtrl(m_panel, -1, currentOverlap.str().c_str(), wxPoint(100, inputY+step), wxSize(115, 20));

  // only display if we are asking for bandwidth
  if( getBandwidth ) {
     m_statText4 = new wxStaticText(m_panel, -1, "Normalised Transition Bandwidth", wxPoint(5,inputY+(2*step)), wxSize(240, 20), wxALIGN_CENTRE);
     m_statText5 = new wxStaticText(m_panel, -1, "NTB:", wxPoint(35,inputY+(3*step)), wxSize(70, 20), wxALIGN_LEFT);
     m_tbBandwidth = new wxTextCtrl(m_panel, -1, currentBW.str().c_str(), wxPoint(100, inputY+(3*step)), wxSize(115, 20));
  }
 
  // the buttons
  int buttonHeight = 30;
  m_bSubmit = new wxButton(m_panel, CEPBTN_ATT_SUBMIT, "Ok", wxPoint(25,boxHeight+buttonHeight));
  m_bSubmit->SetDefault();
  m_bCancel = new wxButton(m_panel, CEPBTN_ATT_CANCEL, "Cancel", wxPoint(145,boxHeight+buttonHeight));
  
  Center();
  ShowModal();
}


int cepWindowBandwidth::getSize()
{
  if( aborted ) return UNINITIALISED_INT;
  
  for(size_t i = 0; i < m_size.Length(); i ++)
  {
    if( !cepIsNumeric(m_size.GetChar(i)) )
    {
      return -1;
    }
  }
  return (int)(atof(m_size.c_str()));
}
 
int cepWindowBandwidth::getOverlap()
{
  if( aborted ) return UNINITIALISED_INT;
  
  for(size_t i = 0; i < m_overlap.Length(); i ++)
  {
    if( !cepIsNumeric(m_overlap.GetChar(i)) )
    {
      return -1;
    }
  }
  return (int)(atof(m_overlap.c_str()));
}


double cepWindowBandwidth::getBandwidth()
{
  if( aborted ) return UNINITIALISED_FLOAT;
  
  for(size_t i = 0; i < m_bandwidth.Length(); i ++)
  {
    if( !cepIsNumeric(m_bandwidth.GetChar(i)) )
    {
      return NAN;
    }
  }
  return (atof(m_bandwidth.c_str()));
}
       
void cepWindowBandwidth::dlgBandwidthOnQuit(wxCommandEvent& WXUNUSED(event))
{
  //if cancel or quit button pressed
  aborted = true;  
  EndModal(1);
  Destroy();
}

void cepWindowBandwidth::dlgBandwidthOnOK(wxCommandEvent& WXUNUSED(event))
{
  aborted = false;
  
  m_size = m_tbSize->GetValue();
  m_overlap = m_tbOverlap->GetValue();
  // check if we have access to the bandwidth box
  if( m_tbBandwidth != NULL ) {
    m_bandwidth = m_tbBandwidth->GetValue();
  }
  EndModal(0);
  Destroy();
}

bool cepWindowBandwidth::cancelled() {
  return aborted;
}

const char*  cepWindowBandwidth::UNINITIALISED_STR = "";
const int    cepWindowBandwidth::UNINITIALISED_INT = 0xCAFEBABE;
const double cepWindowBandwidth::UNINITIALISED_FLOAT = 1.602e-19;




cepWindowUi::cepWindowUi() {}

void cepWindowUi::showBandwidth()
{
  cepWindowBandwidth wa( true );

  m_size = wa.getSize();
  m_overlap = wa.getOverlap();
  m_bandwidth = wa.getBandwidth();
  aborted = wa.cancelled();
  
}


void cepWindowUi::show()
{
  cepWindowBandwidth wa( false );

  m_size = wa.getSize();
  m_overlap = wa.getOverlap();
  aborted = wa.cancelled();
}
  
int cepWindowUi::getSize()
{
  return m_size;
}
int cepWindowUi::getOverlap()
{
  return m_overlap;
}
double cepWindowUi::getBandwidth()
{
  return m_bandwidth;
}
bool cepWindowUi::cancelled()
{
  return aborted;
}

cepError cepWindowUi::checkValues()
{
  if( m_size!=cepWindowBandwidth::UNINITIALISED_INT && m_size == 0 ) return cepError("invalid window size requested");
  if( m_overlap!=cepWindowBandwidth::UNINITIALISED_INT && m_overlap < 0 ) return cepError("invalid overlap requested");
  if( m_overlap > m_size ) return cepError("overlap exceeds window size");
  return cepError();
}

cepError cepWindowUi::checkChebValues()
{
  if( m_bandwidth < 0 || m_bandwidth > 0.5 ) return cepError("invalid transition bandwidth value");
  return cepError();
}
