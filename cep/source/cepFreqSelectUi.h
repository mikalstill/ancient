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

class cepDateRange: public wxDialog
{
public:

  //Displays the "Select Date Range" dialog box
  cepDateRange(cepDate toDate, cepDate fromDate);

  //gets the date values entered
  const double & getToDate();
  const double & getFromDate();
  
  //the on Quit event
  void dlgDateOnQuit(wxCommandEvent& event);
  //the on Ok event
  void dlgDateOnOK(wxCommandEvent& event);

private:
  //declerations for the elements of the dialog box
  wxPanel *m_panel;
  wxStaticBox *m_statBox;
  wxStaticText *m_statText1, *m_statText2, *m_statText3, *m_statText4;
  wxComboBox *m_cbToDay, *m_cbToMonth, *m_cbFromDay, *m_cbFromMonth;
  wxTextCtrl *m_tbToYear, *m_tbFromYear;
  wxButton *m_bSubmit, *m_bCancel;

  //the selected dates as decimals
  double m_toDate, m_fromDate;
  DECLARE_EVENT_TABLE ()
};

class cepDateUi
{
public:
  cepDateUi();

  //shows the "Select Date Range" dialog box
  void showDateRange(cepDate toDate, cepDate fromDate);

  //gets the date value as decimals
  double & getToDate();
  double & getFromDate();

private:
  double m_toDate,      //the finish date as a decimal
         m_fromDate;    //the start date as a decimal
};


  
#endif
