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


class cepShowDir: public wxDialog
{
public:
  cepShowDir();
  cepShowDir(const wxString &title, int x, int y, int w, int h);
  ~cepShowDir();   

  bool getDoDir(char dir);
  
  void dlgDirOnQuit(wxCommandEvent& event);
  void dlgDirOnOK(wxCommandEvent& event);

private:
  wxPanel *panel;
  wxStaticBox *statBox;
  wxStaticText *statText1, *statText2, *statText3;
  wxCheckBox *cbDirX, *cbDirY, *cbDirZ;
  wxButton *bSubmit, *bCancel;

  bool gotValue;
  DECLARE_EVENT_TABLE ()
};

class cepLsUi
{
public:

  cepLsUi();
  ~cepLsUi();
  int showIsReweight();
  void showWhichDir();
  bool getWhichDir(char dir);

private:
  cepShowDir *dirDlg;

};

#endif //end __CEPLSUI_H
