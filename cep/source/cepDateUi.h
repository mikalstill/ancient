/*
 * Imp for the Date wxWindows UI implementation
 *
 *   Copyright (C) Daniel Fernandez                 2002
 *   Copyright (C) Michael Still                    2002
 *   Copyright (C) Blake Swadling                   2002
 *   Copyright (C) Kristy Van Der Vlist             2002
 *   Copyright (C) Nick Wheatstone                  2002
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

#ifndef __CEPDATEUI_H
#define __CEPDATEUI_H

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
#include "cepDate.h"

/******************************************************************************
DOCBOOK START

FUNCTION cepDateRange


PURPOSE Displays the "Select Date Range" dialog box for the user interface

SYNOPSIS START
The follwing is an example of how to create this object.

cepDateRange dr(toDate, fromDate);

<para><itemizedlist>
  <listitem><para>fromDate:- is a cepDate that contains the the defaut start date</para></listitem>
  <listitem><para>toDate:- is a cepDate that contains the the defaut finish date</para></listitem>
</itemizedlist></para>


SYNOPSIS END

DESCRIPTION START

An implementation of the <command>cepDataRange</command> class
which displays a custom wxWindows dialog box.
</para>

<para>
<command>double getFromDate()</command>
Returns the defined start date as a decimal date. If a value of -1 is returned
the specified date was invalid. If -2 is returned this action was canceled
</para>

<para>
<command>double getToDate()</command>
Returns the defined finished date as a decimal date. If a value of -1 is returned
the specified date was invalid. If -2 is returned this action was canceled

DESCRIPTION END

DOCBOOK END
******************************************************************************/

const wxString DATE_DAYS[31] =
  { "01", "02", "03", "04", "05", "06", "07", "08", "09",
  "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21",
  "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"
};

const wxString DATE_MONTHS[12] =
  { "January", "February", "March", "April", "May",
  "June", "July", "August", "September", "October",
  "November", "December"
};


class cepDateRange:public wxDialog
{
public:

  //Displays the "Select Date Range" dialog box
  cepDateRange (cepDate toDate, cepDate fromDate);

  //gets the date values entered
  const double &getToDate ();
  const double &getFromDate ();

  //the on Quit event
  void dlgDateOnQuit (wxCommandEvent & event);
  //the on Ok event
  void dlgDateOnOK (wxCommandEvent & event);

private:
  //declerations for the elements of the dialog box
    wxPanel * m_panel;
  wxStaticBox *m_statBox;
  wxStaticText *m_statText1, *m_statText2, *m_statText3, *m_statText4;
  wxComboBox *m_cbToDay, *m_cbToMonth, *m_cbFromDay, *m_cbFromMonth;
  wxTextCtrl *m_tbToYear, *m_tbFromYear;
  wxButton *m_bSubmit, *m_bCancel;

  //the selected dates as decimals
  double m_toDate, m_fromDate;
  DECLARE_EVENT_TABLE ()};

/******************************************************************************
DOCBOOK START

FUNCTION cepDateUi


PURPOSE Displays the Date user interface

SYNOPSIS START
The follwing is an example of how to create this object.

cepDateUi dateUi;

SYNOPSIS END

DESCRIPTION START

An implementation of the <command>cepDateUi</command> which displays the
date range GUI.
</para>

<para>
<command>void showDateRange(cepDate toDate, cepDate fromDate)</command>
Show the "Select Date Range" dialog box.

<para><itemizedlist>
  <listitem><para>fromDate:- is a cepDate that contains the the defaut start date</para></listitem>
  <listitem><para>toDate:- is a cepDate that contains the the defaut finish date</para></listitem>
</itemizedlist></para>

</para>

<para>
<command>double getFromDate()</command>
Returns the defined start date as a decimal date. If a value of -1 is returned
the specified date was invalid. If -2 is returned this action was canceled
</para>

<para>
<command>double getToDate()</command>
Returns the defined finished date as a decimal date. If a value of -1 is returned
the specified date was invalid. If -2 is returned this action was canceled


DESCRIPTION END

SEEALSO cepDate

DOCBOOK END
******************************************************************************/
class cepDateUi
{
public:
  cepDateUi ();

  //shows the "Select Date Range" dialog box
  void showDateRange (cepDate toDate, cepDate fromDate);

  //gets the date value as decimals
  double &getToDate ();
  double &getFromDate ();

private:
  double m_toDate,		//the finish date as a decimal
    m_fromDate;			//the start date as a decimal
};



#endif
