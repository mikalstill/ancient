/*
 * Imp for the Interpolation wxWindows UI implementation
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

#include "cepInterpUi.h"

BEGIN_EVENT_TABLE (cepInterpShowRate, wxDialog)
EVT_BUTTON (CEPBTN_RATE_SUBMIT, cepInterpShowRate::dlgRateOnOK)
EVT_BUTTON (CEPBTN_RATE_CANCEL, cepInterpShowRate::dlgRateOnQuit)
EVT_CLOSE (cepInterpShowRate::dlgRateOnQuit)
END_EVENT_TABLE ()cepInterpShowRate::cepInterpShowRate (const wxString & val,
							const double &units):
wxDialog ((wxDialog *) NULL, -1, "Specify Sample Rate", wxPoint (120, 120),
	  wxSize (200, 130))
{
  m_panel = new wxPanel (this, -1, wxPoint (120, 120), wxSize (200, 130));

  m_statBox =
    new wxStaticBox (m_panel, -1, "", wxPoint (15, 30), wxSize (170, 50));

  m_statText1 =
    new wxStaticText (m_panel, -1, "Please specify the sample rate",
		      wxPoint (5, 5), wxSize (190, 20), wxALIGN_CENTRE);
  m_statText2 =
    new wxStaticText (m_panel, -1, "for the interpolated data ",
		      wxPoint (5, 19), wxSize (190, 20), wxALIGN_CENTRE);

  m_statText3 =
    new wxStaticText (m_panel, -1, "Sample Rate:", wxPoint (25, 40),
		      wxSize (100, 20), wxALIGN_LEFT);

  m_tbSample =
    new wxTextCtrl (m_panel, -1, val, wxPoint (110, 40), wxSize (70, 20));

//   m_rbYear = new wxRadioButton(m_panel, -1, "Years", wxPoint(25, 65), wxSize(120, 20), wxRB_GROUP);
//   m_rbDays = new wxRadioButton(m_panel, -1, "Month", wxPoint(25, 90));
//   m_rbHours = new wxRadioButton(m_panel, -1, "Days", wxPoint(25, 115));
//
//   if(units == 1.0)
//   {
//       m_rbYear->SetValue(true);
//   }
//   else
//   {
//     if(units == 12.0)
//     {
//       m_rbDays->SetValue(true);
//     }
//     else
//     {
//       m_rbHours->SetValue(true);
//     }
//   }

  m_bSubmit =
    new wxButton (m_panel, CEPBTN_RATE_SUBMIT, "Ok", wxPoint (10, 90));
  m_bSubmit->SetDefault ();
  m_bCancel =
    new wxButton (m_panel, CEPBTN_RATE_CANCEL, "Cancel", wxPoint (110, 90));

  Center ();
  ShowModal ();
}

const
  wxString &
cepInterpShowRate::getSample ()
{
  return m_sampleRate;
}

const double &
cepInterpShowRate::getSampleUnits ()
{
  return m_sampleUnits;
}

void
cepInterpShowRate::dlgRateOnQuit (wxCommandEvent & WXUNUSED (event))
{
  //if cancel or quit button pressed 
  m_sampleRate = "-1";
  m_sampleUnits = -1.0;

  EndModal (1);
  Destroy ();
}

void
cepInterpShowRate::dlgRateOnOK (wxCommandEvent & WXUNUSED (event))
{
/*  if(m_rbYear->GetValue() == true)
  {*/
  m_sampleUnits = 1.0;
//   }
//   else
//   {
//     if(m_rbDays->GetValue() == true)
//     {
//       m_sampleUnits = 12.0;
//     }
//     else
//     {
//       if(m_rbHours->GetValue() == true)
//       {
//         m_sampleUnits = 365.25;
//       }
//     }
//   }

  m_sampleRate = m_tbSample->GetValue ();

  EndModal (0);
  Destroy ();
}

cepInterpUi::cepInterpUi ()
{
}

void
cepInterpUi::showSampleRate (double val)
{
  bool isValid = false;
  cepInterpShowRate *rate;
  rate = new cepInterpShowRate (cepToString (val).c_str (), 356.25);

  m_sampleRate = rate->getSample ();
  m_sampleUnits = rate->getSampleUnits ();

  //if cancel was selected
  if ((m_sampleRate == "-1") && (m_sampleUnits == -1.0))
    {
      return;
    }

  //while sample rate was an invalid value
  while (isValid == false)
    {
      isValid = true;
      for (size_t i = 0; i < m_sampleRate.Length (); i++)
	{
	  //if cancel was selected
	  if ((m_sampleRate == "-1") && (m_sampleUnits == -1.0))
	    {
	      return;
	    }

	  //if sample rate entered was not a number
	  if ((cepIsNumeric (m_sampleRate.GetChar (i)) == false) ||
	      (atof (m_sampleRate.c_str ()) <= 0))
	    {
	      cepError ("Error. Sample rate must be a number greater than 0",
			cepError::sevWarning).display ();
	      rate = new cepInterpShowRate (m_sampleRate, m_sampleUnits);
	      m_sampleRate = rate->getSample ();
	      m_sampleUnits = rate->getSampleUnits ();
	      isValid = false;
	      break;
	    }
	}
    }

  if (rate != NULL)
    {
      delete rate;
    }
}

const double
cepInterpUi::getSampleRate ()
{
  if ((m_sampleRate == "-1") && (m_sampleUnits == -1))
    {
      return -1;
    }

  return (atof (m_sampleRate.c_str ()) / m_sampleUnits);
}
