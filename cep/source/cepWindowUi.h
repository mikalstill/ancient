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

#ifndef __CEPWINDOWUI_H
#define __CEPWINDOWUI_H

#include <iostream.h>

#include <wx/dialog.h>
#include <wx/button.h>
#include <wx/radiobut.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/string.h>

#include "cepUI.h"
#include "cepUtility.h"

/******************************************************************************
DOCBOOK START

FUNCTION cepWindowAttenuation


PURPOSE Displays the "Get Attenuation" dialog box for the Windowing user interface

SYNOPSIS START
The follwing is an example of how to create this object.

cepWindowAttenuation wa;

SYNOPSIS END

DESCRIPTION START

An implementation of the <command>cepWindowAttenuation</command> class
which displays a custom wxWindows dialog box.
</para>

<para>
<command>void showAttenuation()</command>
Show the "Specify attenuation dialog box"
</para>

<para>
<command>double getAttenuation()</command>
Returns the specified attenuation

DESCRIPTION END

DOCBOOK END
******************************************************************************/

class cepWindowAttenuation: public wxDialog
{
public:

  //Displays the "Set Attenuation" dialog box
  cepWindowAttenuation();

  //gets the attenuation value
  double getAttenuation();
  
  //the on Quit event
  void dlgAttOnQuit(wxCommandEvent& event);
  //the on Ok event
  void dlgAttOnOK(wxCommandEvent& event);

private:
  //declerations for the elements of the dialog box
  wxPanel *m_panel;
  wxStaticBox *m_statBox;
  wxStaticText *m_statText1, *m_statText2, *m_statText3;
  wxTextCtrl *m_tbAttenuation;
  wxButton *m_bSubmit, *m_bCancel;
  wxString m_attenuation;
  
  DECLARE_EVENT_TABLE ()
};

/******************************************************************************
DOCBOOK START

FUNCTION cepWindowUi


PURPOSE Displays the Windowing user interface

SYNOPSIS START
The follwing is an example of how to create this object.

cepWindowUi windowUi;

SYNOPSIS END

DESCRIPTION START

An implementation of the <command>cepWindowUi</command> which displays the
Windowing GUI.
</para>



<para>
<command>void showAttenuation()</command>
Show the "Specify attenuation" dialog box
</para>

<para>
<command>void getAtteunation()</command>
Returns the specified attenuation

DESCRIPTION END

DOCBOOK END
******************************************************************************/
class cepWindowUi
{
public:
  cepWindowUi();

  //shows the "set attenuation dialog box"
  void showAttenuation();

  //gets the attenuation value
  double getAttenuation();
private:
  double m_attenuation;   //the specified attenuation value
};


  
#endif //end __CEPWINDOWUI_H
