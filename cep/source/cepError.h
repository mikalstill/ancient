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

#ifndef CEP_ERROR_HEADER
#define CEP_ERROR_HEADER

// This prints out some debugging information which is useful, use it instead
// of:
// cepError dbg("Some text", cepError::sevDebug);
// dbg.display();

///////////////////////////////////////////////////////////////////////////////
// THIS CANNOT DO A DISPLAY, AS THE CONFIG DB CODE USES THIS TO LOG DB PROBLEMS
// AND cepError::display() USES THE CONFIG DB...
///////////////////////////////////////////////////////////////////////////////

#define cepDebugPrint(errmsg) \
  { \
    cepError newnamedbg(string(errmsg) + string(" at ") + string(__FILE__) + \
			string(":") + cepItoa(__LINE__), cepError::sevDebug); \
    newnamedbg.log(); \
  }

class cepError
{
public:
  // todo_mikal: implement severity
  enum severity
  {
    sevOk = 0,
    sevDebug,
    sevInformational,
    sevWarning,
    sevErrorRecoverable,
    sevErrorFatal,
    sevMax
  };

    cepError ();
    cepError (const string& msg);
    cepError (const string& msg, severity level);
   ~cepError ();

  bool isReal ();
  void clear();
  void log();
  void display ();
  string getTitle();
  int getIcon();
  
private:
  string m_msg;
  severity m_level;
  bool m_actioned;
  cepTSB m_dodisplay[sevMax];
};

#endif
