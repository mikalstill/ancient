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

#ifndef __CEPINTERPUI_H
#define __CEPINTERPUI_H

#include <iostream>

#include <wx/dialog.h>
#include <wx/button.h>
#include <wx/radiobut.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/string.h>


#include "cepUI.h"
#include "cepUtility.h"

#include <string>
#include <bits/nan.h>

/******************************************************************************
DOCBOOK START

FUNCTION cepInterpShowRate


PURPOSE Displays the "Specify Sample Rate" dialog box for the Interpolation user interface

SYNOPSIS START
The follwing is an example of how to create this object.

cepInterpShowRate sr(val, units);

<para><itemizedlist>
  <listitem><para>val:- the default sample rate</para></listitem>
  <listitem><para>units:- the units, must be one of 1 (years), 12 (months) or 365.25 (days)</para></listitem>
</itemizedlist></para>

SYNOPSIS END

DESCRIPTION START

An implementation of the <command>cepInterpShowRate</command> class
which displays a custom wxWindows dialog box.
</para>

<para>
<command>double getSample()</command>
Returns the specified sample rate. If a value of -1 is returned this operation
has been canceled.
</para>

<para>units getSampleUnits()</command>
Returns the units selected. Units can be one of:-
<para><itemizedlist>
  <listitem><para>1 (years)</para></listitem>
  <listitem><para>12 (months)</para></listitem>
  <listitem><para>365.25 (days)</para></listitem>
</itemizedlist></para>

DESCRIPTION END

DOCBOOK END
******************************************************************************/
class cepInterpShowRate:public wxDialog
{
public:

  //show the "Specify Sample Rate" dialog box
  cepInterpShowRate (const wxString & val, const double &units);

  //returns the sample rate specified
  const wxString & getSample ();

  const double &getSampleUnits ();

  //the on Quit event
  void dlgRateOnQuit (wxCommandEvent & event);
  //the on Ok event
  void dlgRateOnOK (wxCommandEvent & event);

private:
  //declerations for the elements of the dialog box
    wxPanel * m_panel;
  wxStaticBox *m_statBox;
  wxStaticText *m_statText1, *m_statText2, *m_statText3;
  wxRadioButton *m_rbYear, *m_rbDays, *m_rbHours;
  wxTextCtrl *m_tbSample;
  wxButton *m_bSubmit, *m_bCancel;

  wxString m_sampleRate;	//sample 
  double m_sampleUnits;

  DECLARE_EVENT_TABLE ()};

/******************************************************************************
DOCBOOK START

FUNCTION cepInterpUi


PURPOSE Displays Interpolationh user interface

SYNOPSIS START
The follwing is an example of how to create this object.

cepInterpUi interpUi;

SYNOPSIS END

DESCRIPTION START

An implementation of the <command>cepInterpUi</command> class
which displays the interpolation GUI.
</para>

<para>
<command>void showSampleRate(double val)</command>
Shows the interpolation GUI.

<para><itemizedlist>
  <listitem><para>val:- the default value of the sample rate. This is usually one (1)</para></listitem>
</itemizedlist></para>
</para>

<para>
<command>double getSampleRate()</command>
Gets the sample rate selected by the user. If a value of -1 is returned this operation
has been canceled.
</para>


DESCRIPTION END

DOCBOOK END
******************************************************************************/
class cepInterpUi
{
public:
  cepInterpUi ();

  //shows the "get sample rate" dialog box
  void showSampleRate (double val);

  //returns the specified sample rate
  const double getSampleRate ();

private:
    wxString m_sampleRate;	//stores the specified sample rate
  double m_sampleUnits;
};



#endif //end __CEPINTERP_H
