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

cepError::cepError ():
m_msg (), m_level (cepError::sevOk), m_actioned (true)
{
}

cepError::cepError (string msg):
m_msg (msg), m_level (cepError::sevErrorFatal), m_actioned (false)
{
}

cepError::cepError (string msg, cepError::severity level):
m_msg (msg), m_level (level), m_actioned (false)
{
}

cepError::~cepError ()
{
  if (!m_actioned)
    {
      cepError dbg ("cepError was not actioned: " + m_msg,
		    cepError::sevDebug);
      dbg.display ();
    }
}

bool
cepError::isReal ()
{
  m_actioned = true;
  return m_msg != "";
}

void
cepError::display ()
{
  m_actioned = true;

  if (!gOptions.errorDisplay[m_level])
    return;

  // todo_mikal: integrate with UI
  if (m_msg != "")
    {
      switch (m_level)
	{
	case sevOk:
	  return;
	  break;

	case sevDebug:
	  cout << "Debug: ";
	  break;

	case sevInformational:
	  cout << "Informational: ";
	  break;

	case sevWarning:
	  cout << "Warning: ";
	  break;

	case sevErrorRecoverable:
	  cout << "Recoverable Error: ";
	  break;

	case sevErrorFatal:
	  cout << "Fatal error: ";
	  break;

	default:
	  cout << "UNKNOWN ERROR LEVEL: ";
	  break;
	}

      cout << m_msg << endl;
    }
  else
    {
      cepError dbg ("Display requested on empty cepError",
		    cepError::sevDebug);
      dbg.display ();
    }
}
