/*
 * A dialog which prompts for a page size
 *
 *   Copyright (C) Michael Still                    2003
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

#include "dlgPageSize.h"

BEGIN_EVENT_TABLE (dlgPageSize, wxDialog) 
EVT_BUTTON (wxID_OK, dlgPageSize::OnOK) 
EVT_BUTTON (wxID_CANCEL, dlgPageSize::OnQuit) 
EVT_CLOSE (dlgPageSize::OnQuit) 
END_EVENT_TABLE ()

dlgPageSize::dlgPageSize ():
  wxDialog ((wxDialog *) NULL, -1, "Select page size", wxPoint (100, 100),
	    wxSize (300, 160)),
  m_valid(false)
{
  m_panel = new wxPanel (this, -1, wxPoint (100, 100), wxSize (300, 200));

  new wxStaticText (m_panel, -1, "Please specify the size of the pages:",
		    wxPoint (5, 5), wxSize (290, 20), wxALIGN_LEFT);

  // TODO mikal: drop down with page sizes

  new wxStaticText (m_panel, -1, "Width:", wxPoint (5, 75),
		    wxSize (60, 20), wxALIGN_RIGHT);
  m_width =
    new wxTextCtrl (m_panel, -1, "200", wxPoint (70, 75), wxSize (50, 20));

  new wxStaticText (m_panel, -1, "Height:", wxPoint (145, 75), wxSize (60, 20),
		    wxALIGN_RIGHT);
  m_height =
    new wxTextCtrl (m_panel, -1, "200", wxPoint (210, 75), wxSize (50, 20));

  // TODO mikal: there are also validators and things like that we can use
  // here to make sure the user enters only a number...
  m_ok = new wxButton (m_panel, wxID_OK, "Ok", wxPoint (60, 120));
  m_ok->SetDefault ();

  m_cancel = new wxButton (m_panel, wxID_CANCEL, "Cancel", wxPoint (170, 120));

  Center ();
  ShowModal ();
}

void
dlgPageSize::OnQuit (wxCommandEvent & WXUNUSED (event))
{
  m_valid = false;
  EndModal (1);
  Destroy ();
}

void
dlgPageSize::OnOK (wxCommandEvent & WXUNUSED (event))
{
  m_valid = true;
  m_x = atoi(m_width->GetValue().c_str());
  m_y = atoi(m_height->GetValue().c_str());

  EndModal (0);
  Destroy ();
}

void dlgPageSize::getSize(int& x, int &y)
{
  if(!m_valid){
    x = -1;
    y = -1;
  }
  else{
    x = m_x;
    y = m_y;
  }
}
