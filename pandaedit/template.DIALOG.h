/*
 * A dialog which has been automatically generated by gendialog
 *
 *   Copyright (C) Michael Still                    2003
 *   Generated: %GENTIME%
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

#ifndef __%CLASSNAME%_H
#define __%CLASSNAME%_H

#include <wx/panel.h>
#include <wx/dialog.h>
#include <wx/button.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/string.h>
#include <wx/combobox.h>

class %CLASSNAME%:public wxDialog
{
public:
  %CLASSNAME% ();
  void OnQuit (wxCommandEvent & event);
  void OnOK (wxCommandEvent & event);

private:
  wxPanel * m_panel;
  %EDITBOXES%
  wxButton *m_ok, *m_cancel;

  DECLARE_EVENT_TABLE ()
};

#endif
