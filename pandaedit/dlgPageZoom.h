/*
 * A dialog which has been automatically generated by gendialog
 *
 *   Copyright (C) Michael Still                    2003
 *   Generated: Mon May 12 15:14:46 EST 2003

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

#ifndef __dlgPageZoom_H
#define __dlgPageZoom_H

#include <wx/panel.h>
#include <wx/dialog.h>
#include <wx/button.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/string.h>
#include <wx/combobox.h>

class dlgPageZoom:public wxDialog
{
public:
  dlgPageZoom ();
  void OnQuit (wxCommandEvent & event);
  void OnOK (wxCommandEvent & event);

private:
  wxPanel * m_panel;
  wxTextCtrl *m_edit2, *m_edit1, *m_edit0;
  wxButton *m_ok, *m_cancel;

  DECLARE_EVENT_TABLE ()
};

#endif
