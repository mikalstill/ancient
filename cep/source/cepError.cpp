 /* 
    Imp for the CEP error
    Copyright (C) Michael Still                    2002

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
  */

#include "core.h"

#ifdef __WXGTK__
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include "wx/txtstrm.h"

#if !wxUSE_DOC_VIEW_ARCHITECTURE
#error You must set wxUSE_DOC_VIEW_ARCHITECTURE to 1 in setup.h!
#endif
#endif

// todo_mikal: copy constructor, should have actioned status

cepError::cepError ():
m_msg (), m_level (cepError::sevOk), m_actioned (true)
{
}

cepError::cepError (const string& msg):
m_msg (msg), m_level (cepError::sevErrorFatal), m_actioned (false)
{
}

cepError::cepError (const string& msg, cepError::severity level):
m_msg (msg), m_level (level), m_actioned (false)
{
}

cepError::~cepError ()
{
  if (!m_actioned)
    cepDebugPrint ("cepError was not actioned: " + m_msg);
}

bool cepError::isReal ()
{
  m_actioned = true;
  return m_msg != "";
}

void
cepError::clear ()
{
  m_actioned = true;
  m_msg = "";
}

void
cepError::log()
{
  gLog << m_msg << " (severity " << m_level << ")" << endl;
  m_actioned = true;
}

void
cepError::display ()
{
  m_actioned = true;

  // Log everything for now
  // todo_mikal improve
  log();

  if(gDisplayParams[(int) m_level].get() == cepTSB::stUndefined)
    {
      // Deliberately dropping cepError return value here
      bool dodisp;
      gConfiguration->getValue(string("cepErrordisplaylevel") + 
			       cepItoa((int) m_level), true, dodisp);
      gDisplayParams[(int) m_level].set(dodisp);
    }

  if(gDisplayParams[(int) m_level].get() == cepTSB::stFalse)
    return;

  if (m_msg != "")
    {
#ifdef __WXGTK__
      wxMessageBox(m_msg.c_str(), getTitle().c_str(), 
		   wxOK | wxCENTRE | getIcon());
#else
      cout << getTitle() << ": " << m_msg << endl;
#endif
    }
  else
    // This presents a smaller danger of an infinite loop
    cepDebugPrint ("Display requested on empty cepError");
}

string cepError::getTitle()
{
  switch (m_level)
    {
    case sevOk:
      return "Ok";

    case sevDebug:
      return "Debug";
      
    case sevInformational:
      return "Informational";
      
    case sevWarning:
      return "Warning";
      
    case sevErrorRecoverable:
      return "Recoverable Error";
      
    case sevErrorFatal:
      return "Fatal error";
      
    default:
      return "UNKNOWN ERROR LEVEL";
    }
}

int cepError::getIcon()
{
#ifdef __WXGTK__
  switch (m_level)
    {
    case sevOk:
      return 0;

    case sevDebug:
      return wxICON_QUESTION;
      
    case sevInformational:
      return wxICON_INFORMATION;
      
    case sevWarning:
      return wxICON_EXCLAMATION;
      
    case sevErrorRecoverable:
      return wxICON_ERROR;
      
    case sevErrorFatal:
      return wxICON_ERROR;
      
    default:
      return wxICON_QUESTION;
    }
#endif

  return 0;
  }
