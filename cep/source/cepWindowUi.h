/*
 * Imp for the Windowing wxWindows UI implementation
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

#ifndef __CEPWINDOWUI_H
#define __CEPWINDOWUI_H

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

/******************************************************************************
DOCBOOK START

FUNCTION cepWindowBandwidth


PURPOSE Displays the "Get Bandwidth" dialog box for the Windowing user interface

SYNOPSIS START
The follwing is an example of how to create this object.

cepWindowBandwidth wa;

SYNOPSIS END

DESCRIPTION START

An implementation of the <command>cepWindowBandwidth</command> class
which displays a custom wxWindows dialog box.
</para>

<para>

<command>void showBandwidth()</command>
Show the "Specify Bandwidth dialog box"
</para>

<para>
<command>double getBandwidth()</command>
Returns the specified Bandwidth

DESCRIPTION END

DOCBOOK END
******************************************************************************/

class cepWindowBandwidth:public wxDialog
{
public:

  //Displays the "Set Bandwidth" dialog box
  cepWindowBandwidth (bool getBandwidth);

  //gets the Size value
  int getSize ();
  //gets the Overlap value
  int getOverlap ();
  //gets the Bandwidth value
  double getBandwidth ();


  //the on Quit event
  void dlgBandwidthOnQuit (wxCommandEvent & event);
  //the on Ok event
  void dlgBandwidthOnOK (wxCommandEvent & event);

  bool cancelled ();

private:
  static const char *UNINITIALISED_STR;
public:
  static const int UNINITIALISED_INT;
  static const double UNINITIALISED_FLOAT;

private:
  //declerations for the elements of the dialog box
    wxPanel * m_panel;
  wxStaticBox *m_statBox;
  wxStaticText *m_statText1, *m_statText2, *m_statText3, *m_statText4,
    *m_statText5;
  wxTextCtrl *m_tbSize, *m_tbOverlap, *m_tbBandwidth;
  wxButton *m_bSubmit, *m_bCancel;
  wxString m_size, m_overlap, m_bandwidth;

  bool aborted;

  DECLARE_EVENT_TABLE ()};

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
command>void showBandwidth()</command>
Show the "Specify Bandwidth" dialog box
</para>

<para>
<command>void getBandwidth()</command>
Returns the specified Bandwidth

DESCRIPTION END

DOCBOOK END
******************************************************************************/
class cepWindowUi
{
public:
  cepWindowUi ();

  //shows the "set Bandwidth dialog box"
  cepError showChebyshev ();
  cepError show ();

  //gets the Bandwidth value
  int getSize ();
  int getOverlap ();
  double getBandwidth ();

  bool cancelled ();
  cepError checkValues ();
  cepError checkChebValues ();


private:
  int m_size;			//the specified Size value
  int m_overlap;		//the specified Overlap value
  double m_bandwidth;		//the specified Bandwidth value
  bool aborted;

  bool check2n (int n);
};



#endif //end __CEPWINDOWUI_H
