
/* 
 *   Imp for the CEP error
 *   Copyright (C) Michael Still                    2002
 *   Copyright (C) Blake Swadling                   2002
 *   Copyright (C) Kristy Van Der Vlist             2002
 *   
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *   
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *   
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef CEP_ERROR_HEADER
#define CEP_ERROR_HEADER

#include <string>
#include "genErrorHandler.h"
#include "utility.h"

using namespace std;

class genError
{
public:
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

    genError ();
    genError (const string & msg);
    genError (const string & msg, severity level);
  static void addErrorHandler (class genErrorHandler & h);
  static void removeErrorHandler ();
   ~genError ();

  void init ();
  void setError (const string & msg, severity level);
  bool isReal ();
  void clear ();
  void log ();
  void display ();
  void doTerminate ();

  // BS - remove these when we get the friend thing for genErrorHandler sorted
    string & getMessage ();
  int getSeverity ();
  string getTitle ();

private:

  static bool m_handlerInstalled;
  static class genErrorHandler *m_handler;

  string m_message;
  severity m_level;
  bool m_actioned;
};

#endif
