/*
  * Imp for the least squares wxWindows UI implementation
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

#ifndef __CEPLSUI_H
#define __CEPLSUI_H

#include <iostream.h>
#include <string>

#include <wx/dialog.h>
#include <wx/button.h>
#include <wx/radiobut.h>
#include <wx/msgdlg.h>
#include <wx/checkbox.h>
#include <wx/statbox.h>
#include <wx/stattext.h>

#include "cepUI.h"

/******************************************************************************
DOCBOOK START

FUNCTION cepLsIsReweight


PURPOSE Displays the re-weighting dialog box for the Least Squares user interface

SYNOPSIS START
The follwing is an example of how to create this object.

cepLsIsReweight rw;

SYNOPSIS END

DESCRIPTION START

An implementation of the <command>cepLsIsReweight</command> class
which displays a wxWindows mesage dialog box.
</para>

<para>
<command>cepLsIsReweight()</command>
Display the re-weighting dialog box.

<para>
<command>int getIsReweight();
Get the value returned from the dialog box. Returns 1 for yes, 0 for no or -1
for cancel.

DESCRIPTION END

DOCBOOK END
******************************************************************************/

class cepLsIsReweight: public wxMessageDialog
{
public:

  //display the re-weighting dialog
  cepLsIsReweight();

  //get the value returned from the dialog box
  int getIsReweight();

private:
  int m_isReweight;
};

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
<command>cepLsShowDir()</command>
Displays the choose direction dialog box
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

class cepLsShowDir: public wxDialog
{
public:

  //show the choose direction dialog box
  cepLsShowDir();

  //get the values returned from the dialog box                
  bool getWhichDir(char dir);

  //the on Quit event
  void dlgDirOnQuit(wxCommandEvent& event);
  //the on Ok event
  void dlgDirOnOK(wxCommandEvent& event);

private:
  wxPanel *m_panel;
  wxStaticBox *m_statBox;
  wxStaticText *m_statText1, *m_statText2, *m_statText3;
  wxCheckBox *m_cbDirX, *m_cbDirY, *m_cbDirZ;
  wxButton *m_bSubmit, *m_bCancel;

  DECLARE_EVENT_TABLE ()
};

/******************************************************************************
DOCBOOK START

FUNCTION cepLsReadP


PURPOSE Displays the read from file dialog box for the Least Squares user interface

SYNOPSIS START
The follwing is an example of how to create this object.

cepLsReadP rp;

SYNOPSIS END

DESCRIPTION START

An implementation of the <command>cepLsReadP</command> class
which displays a wxWindows mesage dialog box.
</para>

<para>
<command>cepLsReadP(string dir)</command>
Display the read from file dialog box for the specified direction.
<para><itemizedlist>
  <listitem><para>dir:-The data direction. Must be one of x, y or z</para></listitem>
</itemizedlist></para>
</para>

<para>
<command>int getIsReadP()</command>
Get the value returned from the dialog box. Returns 1 for yes, 0 for no or -1
for cancel.

DESCRIPTION END

DOCBOOK END
******************************************************************************/

class cepLsReadP: public wxMessageDialog
{
public:

  //display the read from file dialog box
  cepLsReadP(string dir);

  int getIsReadP();
private:
  int m_isRead;
};

/******************************************************************************
DOCBOOK START

FUNCTION cepLsShowFile


PURPOSE Displays the open a file dialog box for the Least Squares user interface

SYNOPSIS START
The follwing is an example of how to create this object.

cepLsShowFile sf;

SYNOPSIS END

DESCRIPTION START

An implementation of the <command>cepLsShowFile</command> class
which displays a wxWindows mesage dialog box.
</para>

<para>
<command>cepLsShowFile</command>
Display the open file dialog box for the specified direction.
</para>

<para>
<command>string getFilename()</command>
Returns the full path of the file selected for opening. If getFilename() returns
"" the user has canceled this operation.

DESCRIPTION END

DOCBOOK END
******************************************************************************/

class cepLsShowFile: public wxFileDialog
{
public:
  //show the choose file dialog box
  cepLsShowFile();

  //gets the name of the file choosen
  string getFilename();

private:
  string m_filename;
};

/******************************************************************************
DOCBOOK START

FUNCTION cepLsUi


PURPOSE Displays the open a file dialog box for the Least Squares user interface

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
<command>int getIsReweight()</command>
Get the value returned from the dialog box. Returns 1 for yes, 0 for no or -1
for cancel.
</para>

<para>
<command>bool getWhichDir(char dir)</command>
Returns true if the given direction is selected, otherwise false is returned.
<para><itemizedlist>
  <listitem><para>dir:-The data direction. Must be one of x, y or z</para></listitem>
</itemizedlist></para>
</para>

<para>
<command>int getIsReadP()</command>
Get the value returned from the dialog box. Returns 1 for yes, 0 for no or -1
for cancel.
</para>

<para>
<command>string getfNameP()</command>
Returns the full path of the file selected for opening. If getFilename() returns
"" the user has canceled this operation.

DESCRIPTION END

DOCBOOK END
******************************************************************************/

class cepLsUi
{
public:
  cepLsUi();

  //show the re-weighting dialog box
  void showIsReweight();
  //show the choose direction dialog box
  void showWhichDir();
  //show the read from file dialog box
  void showIsReadP(string dir);
  //show the choose file dialog box
  void showGetfNameP();

  //get values from the re-weight dialog box
  int getIsReweight();
  //get the value for a given direction returned from
  //the choose diretion dialog box
  bool getWhichDir(char dir);
  //get the value from the read from file dialog box
  int getIsReadP();
  //get the name of the selected file
  string getfNameP();

private:
  int m_isReweight;
  bool m_doDirX, m_doDirY, m_doDirZ;
  int m_isReadP;
  string m_filename;
};

  
  
#endif //end __CEPLSUI_H
