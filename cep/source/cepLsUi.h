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
#include <wx/button.h>
#include <wx/radiobut.h>
#include <wx/msgdlg.h>
#include <wx/checkbox.h>
#include <wx/frame.h>

static const int WHICH_DIR_OK = 1000;

class cepLsUi: public wxFrame
{
public:
  
  cepLsUi();
  ~cepLsUi();

  void onShowWhichDirOk(wxCommandEvent& event);
  
  int showIsReweight();
  void showWhichDir();
  bool getdoDir();
  
private:
  enum cepLsUidir
  {
    dirX = 0,
    dirY,
    dirZ
  };
  DECLARE_EVENT_TABLE ()

  bool doDirX, doDirY, doDirZ;
  wxButton *btnOk, *btnCancel;
  wxFrame *frame;
};

/*class cepLsUiFrame: public wxFrame
{
public:
  cepLsUiFrame(wxFrame *frame, char *title, int x, int y, int w, int h);
  ~cepLsUiFrame();

    void cepLsUiFrameOnQuit(wxCommandEvent& event);
    void cepLsUiFrameOnSize( wxSizeEvent& event );
    void cepLsUiFrameOnMove( wxMoveEvent& event );

private:
  
  wxFrame *fGetDir;
  wxCheckBox *cbDirX, *cbDirY, *cbDirZ;
  wxButton *bSubmit, *bCancel;

}; */
#endif //end __CEPLSUI_H
