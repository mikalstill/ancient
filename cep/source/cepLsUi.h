/*
 * Imp for the least squares wxWindows UI implementation
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

#ifndef __CEPLSUI_H
#define __CEPLSUI_H

#include <iostream>
#include <string>

#include <wx/dialog.h>
#include <wx/button.h>
#include <wx/msgdlg.h>
#include <wx/checkbox.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/combobox.h>

#include "cepUI.h"
#include "cepDate.h"
#include "cepUtility.h"

/******************************************************************************
DOCBOOK START

FUNCTION cepLsShowDir


PURPOSE Displays the "Select Direction" dialog box for the Least Squares user interface

SYNOPSIS START
The follwing is an example of how to create this object.

cepLsShowDir sd;

SYNOPSIS END

DESCRIPTION START

An implementation of the <command>cepLsShowDir</command> class
which displays a custom wxWindows dialog box.
</para>

<para>
<command>bool getWhichDir(char dir)</command>
Returns true if the given direction is selected, otherwise false is returned.
<para><itemizedlist>
  <listitem><para>dir:-The data direction. Must be one of x, y or z</para></listitem>
</itemizedlist></para>

DESCRIPTION END

DOCBOOK END
******************************************************************************/

class cepLsShowDir:public wxDialog
{
public:

  //show the choose direction dialog box
  cepLsShowDir ();

  //get the values returned from the dialog box                
  bool getWhichDir (char dir);

  //the on Quit event
  void dlgDirOnQuit (wxCommandEvent & event);
  //the on Ok event
  void dlgDirOnOK (wxCommandEvent & event);

private:
  //declare the indivdual elements that make up the dialog box
    wxPanel * m_panel;
  wxStaticBox *m_statBox;
  wxStaticText *m_statText1, *m_statText2, *m_statText3;
  wxCheckBox *m_cbDirX, *m_cbDirY, *m_cbDirZ;
  wxButton *m_bSubmit, *m_bCancel;

  DECLARE_EVENT_TABLE ()};

/******************************************************************************
DOCBOOK START

FUNCTION cepLsWeight


PURPOSE Displays the "Weight Data" dialog box for the user interface

SYNOPSIS START
The follwing is an example of how to create this object.

cepLsWeight dr(fromDay, fromMonth, fromYear,
               toDay, toMonth, toYear,
               val);

<para><itemizedlist>
  <listitem><para>fromDay:- is a wxString that contains the start day</para></listitem>
  <listitem><para>fromMonth:- is a wxString that contains the start month</para></listitem>
  <listitem><para>fromYear:- is a wxString that contains the start year</para></listitem>
  <listitem><para>toDay:- is a wxString that contains the finish day</para></listitem>
  <listitem><para>toMonth:- is a wxString that contains the finish month</para></listitem>
  <listitem><para>toYear:- is a wxString that contains the finish year</para></listitem>
  <listitem><para>val:- is a wxString that contains the default weighting value</para></listitem>
</itemizedlist></para>

SYNOPSIS END

DESCRIPTION START

An implementation of the <command>cepLsWeight</command> class
which displays a custom wxWindows dialog box.
</para>

<para>
<command>const wxString & getFromDay()</command>
Returns the start day selected. If -1 is returned this action was canceled.
</para>

<para>
<command>const wxString & getFromMonth()</command>
Returns the start month selected. If -1 is returned this action was canceled.
</para>

<para>
<command>const wxString & getFromYear()</command>
Returns the start year selected. If -1 is returned this action was canceled.
</para>

<para>
<command>const wxString & getToDay()</command>
Returns the finish day selected. If -1 is returned this action was canceled.
</para>

<para>
<command>const wxString & getToMonth()</command>
Returns the finish month selected. If -1 is returned this action was canceled.
</para>

<para>
<command>const wxString & getToYear()</command>
Returns the weight selected. If -1 is returned this action was canceled.
</para>

<para>
<command>const wxString & getWeight()</command>
Returns the start day is selected. If -1 is returned this action was canceled.
</para>

<para>
<command>const bool & getDoVCV()</command>
Returns true if the VCV least squares transformation is to be prefomed now.

DESCRIPTION END

DOCBOOK END
******************************************************************************/

const wxString LS_DAYS[31] =
  { "01", "02", "03", "04", "05", "06", "07", "08", "09",
  "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21",
  "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"
};

const wxString LS_MONTHS[12] =
  { "January", "February", "March", "April", "May",
  "June", "July", "August", "September", "October",
  "November", "December"
};

class cepLsWeight:public wxDialog
{
public:
  cepLsWeight (wxString fromDay, wxString fromMonth, wxString fromYear,
	       wxString toDay, wxString toMonth, wxString toYear,
	       wxString val);

  //gets values entered
  const wxString & getToDay ();
  const wxString & getToMonth ();
  const wxString & getToYear ();

  const wxString & getFromDay ();
  const wxString & getFromMonth ();
  const wxString & getFromYear ();

  const wxString & getWeight ();

  //have we finished weighting?
  const bool & getDoVCV ();

  //the on Quit event
  void dlgWeightOnQuit (wxCommandEvent & event);
  //the on Ok event
  void dlgWeightOnOK (wxCommandEvent & event);

  //preform VCV Least Squares transfomation
  void dlgWeightOnGo (wxCommandEvent & event);

private:
  //holds objects for the dialog box
    wxPanel * m_panel;
  wxStaticBox *m_statBox;
  wxStaticText *m_statText1, *m_statText2, *m_statText3, *m_statText4,
    *m_statText5;
  wxComboBox *m_cbToDay, *m_cbToMonth, *m_cbFromDay, *m_cbFromMonth;
  wxTextCtrl *m_tbToYear, *m_tbFromYear, *m_tbVal;
  wxButton *m_bSubmit, *m_bCancel, *m_bGo;

  //holds the selected values
  wxString m_fromDay,
    m_fromMonth, m_fromYear, m_toDay, m_toMonth, m_toYear, m_val;
  bool m_go;			//hove we finished re-weighting?

  DECLARE_EVENT_TABLE ()};
/******************************************************************************
DOCBOOK START

FUNCTION cepLsUi


PURPOSE Displays the Least Squares user interface

SYNOPSIS START
The follwing is an example of how to create this object.

cepLsUi lsui;

SYNOPSIS END

DESCRIPTION START

An implementation of the <command>cepLsUi</command> class
which displays all dialog boxes associated with the Least Squares GUI.
</para>

<para>
<command>void showIsReweight()</command>
Shows the re-weighting dialog box.
</para>

<para>
<command>void showWhichDir()</command>
Shows the choose direction dialog box.
</para>

<para>
<command>void showIsReadP(string dir)</command>
Shows the read from file dialog box.
</para>

<para>
<command>void showGetfNameP()</command>
Shows the choose file dialog box.
</para>

<para>
<para>
<command>const int & getIsReweight()</command>
Get the value returned from the dialog box. Returns 1 for yes, 0 for no or -1
for cancel.
</para>

<para>
<command>void showWeight(double startDate, double endDate, double val)</command>
Shows the "Weight Data" dialog box.

<para><itemizedlist>
  <listitem><para>fromDate:- is a cepDate that contains the the defaut start date</para></listitem>
  <listitem><para>toDate:- is a cepDate that contains the the defaut finish date</para></listitem>
  <listitem><para>val:- is a double that contains the the defaut weight for the given points</para></listitem>
</itemizedlist></para>
</para>

<command>const bool getWhichDir(char dir)</command>
Returns true if the given direction is selected, otherwise false is returned.
<para><itemizedlist>
  <listitem><para>dir:-The data direction. Must be one of x, y or z</para></listitem>
</itemizedlist></para>
</para>

<para>
<command>const int & getIsReadP()</command>
Get the value returned from the dialog box. Returns 1 for yes, 0 for no or -1
for cancel.
</para>

<para>
<command>const string & getfNameP()</command>
Returns the full path of the file selected for opening. If getFilename() returns
"" the user has canceled this operation.
</para>

<para>
<command>const double getFromDate()</command>
Returns the defined start date as a decimal date. If a value of -1 is returned
the action was canceled.
</para>

<para>
<command>const double getToDate()</command>
Returns the defined finished date as a decimal date.  If a value of -1 is returned
the action was canceled.
</para>

<para>
<command>const double getWeight()</command>
Returns the defined weighting value.  If a value of -1 is returned
the action was canceled.
</para>

<para>
<command>const bool & getDoVCV()</command>
Returns true if the VCV least squares transformation is to be prefomed now.

DESCRIPTION END

DOCBOOK END
******************************************************************************/

class cepLsUi
{
public:
  cepLsUi ();

  //show the re-weighting dialog box
  void showIsReweight ();
  //show the choose direction dialog box
  void showWhichDir ();
  //show the read from file dialog box
  void showIsReadP (string dir);
  //show the choose file dialog box
  void showGetfNameP ();

  //shows the weighting dialog box
  void showWeight (double startDate, double endDate, double val);

  //get values from the re-weight dialog box
  const int &getIsReweight ();

  //get the value for a given direction returned from
  //the choose diretion dialog box
  const bool getWhichDir (char dir);

  //get the value from the read from file dialog box
  const int &getIsReadP ();

  //get the name of the selected file
  const string & getfNameP ();

  //get the weight value selected
  const double getWeight ();

  //get the weight date range for the selected values
  const double getFromDate ();
  const double getToDate ();

  //have we finished re-weighting
  const bool & getDoVCV ();
private:
  int m_isReweight;		//holds the value returned from the reweighting dialog box
  bool m_doDirX,		//was direction X selected?
    m_doDirY,			//was direction Y selected?
    m_doDirZ;			//was direction Z selected?
  int m_isReadP;		//holds the value returned from the read from file dialog box
  string m_filename;		//holds the file name selected in the read file dialog box

  //holds the values returned from the "Select Weight" dialog box
  wxString m_fromDay,
    m_fromMonth, m_fromYear, m_toDay, m_toMonth, m_toYear, m_val;

  bool m_go;			//have we finished weighting the data? 
};

#endif //end __CEPLSUI_H
