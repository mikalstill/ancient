/*
  * Imp for the frequency range wxWindows UI implementation
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

#ifndef __CEPFREQUI_H
#define __CEPFREQUI_H

#include <iostream>

#include <wx/dialog.h>
#include <wx/button.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/string.h>
#include <wx/combobox.h>

#include "cepUI.h"
#include "cepUtility.h"

class cepFreqRange:public wxDialog
{
public:

  //Displays the "Select Date Range" dialog box
  cepFreqRange (double startFreq, double endFreq);

  //gets the date values entered
  const double &getStartFreq ();
  const double &getEndFreq ();

  //the on Quit event
  void dlgOnQuit (wxCommandEvent & event);
  //the on Ok event
  void dlgOnOK (wxCommandEvent & event);

private:
  //declerations for the elements of the dialog box
    wxPanel * m_panel;
  wxStaticBox *m_statBox;
  wxStaticText *m_statText1, *m_statText2, *m_statText3, *m_statText4;
  wxTextCtrl *m_tbStartFreq, *m_tbEndFreq;
  wxButton *m_bSubmit, *m_bCancel;

  //the selected dates as decimals
  double m_startFreq, m_endFreq;
  DECLARE_EVENT_TABLE ()};

class cepFreqUi
{
public:
  cepFreqUi ();

  //shows the "Select Date Range" dialog box
  void showFreqRange (double startFreq, double endFreq);

  //gets the date value as decimals
  double &getStartFreq ();
  double &getEndFreq ();

private:
  double m_startFreq,		//the finish date as a decimal
    m_endFreq;			//the start date as a decimal
};



#endif
