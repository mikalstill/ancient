
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

/******************************************************************************
DOCBOOK START

FUNCTION genDebugPrint
PURPOSE print out debugging information via genError

SYNOPSIS START
genDebugPrint(string debugMessage);
SYNOPSIS END

DESCRIPTION START
<command>genDebugPrint</command> creates a genError of debug severity and immediately displays it -- this will normally result in the debug message being logged. The macro also pack into the message the name of the source file which called <command>genDebugPrint</command>, and the line number that the call occurred at, for ease of debugging.
DESCRIPTION END

SEEALSO genDebug
DOCBOOK END
******************************************************************************/

// This prints out some debugging information which is useful, use it instead
// of:
// genError genDebugPrint("Some text", genError::sevDebug);
// genDebugPrint.display();

///////////////////////////////////////////////////////////////////////////////
// THIS CANNOT DO A DISPLAY, AS THE CONFIG DB CODE USES THIS TO LOG DB PROBLEMS
// AND genError::display() USES THE CONFIG DB...
///////////////////////////////////////////////////////////////////////////////
#define genDebugPrint(errmsg) \
  { \
    genError newname_genDebugPrint(string("") + string(errmsg) + string(" at ") + string(__FILE__) + \
			string(":") + genToString(__LINE__), genError::sevDebug); \
    newname_genDebugPrint.log(); \
  }

/******************************************************************************
DOCBOOK START

FUNCTION genError
PURPOSE store user definable configuration information

SYNOPSIS START
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
genError (const string& msg);
genError (const string& msg, severity level);
~genError ();

bool isReal ();
void clear();
void log();
void display ();
string getTitle();
int getIcon();
SYNOPSIS END

DESCRIPTION START
This class is used to display and log messages from the user interface to the user.
</para>

<para>
The enumeration <command>severity</command> defines the possible error levels available to <command>genError</command>. These levels have differing default resposes within the <command>genError</command> class, depending on their perceived importance:

//todo_mikal: implement error level actions as defined here
<orderedlist>
<listitem><para><command>sevOk</command>: this error is not really an error, and will not be displayed. This is the default error level.</para></listitem>
<listitem><para><command>sevDebug</command>: this is a debugging message, which will normally be logged, but not displayed. There may be a very large number of these during the normal operation of the application.</para></listitem>
<listitem><para><command>sevInformational</command>: this is an informational message which will be displayed, and logged.</para></listitem>
<listitem><para><command>sevWarning</command>: this is a warning that would normally be of interest to the user. It will be logged and displayed by default.</para></listitem>
<listitem><para><command>sevErrorRecoverable</command>: this level reports an error, which the application can recover from. It will be displayed and logged by default.</para></listitem>
<listitem><para><command>sevErrorFatal</command>: this is an error. It will be by default displayed, logged, and the application will exit after the user has acknowledged the error. Display of this error type cannot be disabled by the user.</para></listitem>
</orderedlist>
</para>

<para>
<command>genError ();</command>
Default constructor. This will create a <command>genError</command> with a default severity of <command>sevOk</command>.
</para>

<para>
<command>genError (const string&amp; msg);</command>
Contruct a <command>genError</command> with a severity of <command>sevErrorFatal</command>.
</para>

<para>
<command>genError (const string& msg, severity level);</command>
Construct a <command>genError</command> with the specified severity.
</para>

<para>
<command>~genError ();</command>
//todo_mikal: make sure this is true
If the <command>genError</command> has not been actioned (checked if real, copied, displayed or logged) by the time the destructor is called, then it will be actioned at this point.
</para>

<para>
<command>bool isReal ();</command>
If the <command>genError</command> has a message string, then return true.
</para>

<para>
<command>void clear();</command>
Zero the <command>genError</command>.
</para>

<para>
<command>void log();</command>
Write the <command>genError</command> to the log file.
</para>

<para>
<command>void display ();</command>
Display the <command>genError</command> if the display configuration allows it.
</para>

<para>
<command>string getTitle();</command>
Return the title which should be used in the display of this <command>genError</command>.
</para>

<para>
<command>int getIcon();</command>
Return the icon identifier which should be used with the <command>genError</command>.
</para>

<para>
<command>static void addErrorHandler( genErrorHandler );</command>
add a specific error handler. this will be used to display and log errors as necesary.
if an error handler is already subscribed, then this will fail and the previous handler
will be undisturbed. Use removeErrorhander before assigning a new one.
If No error handler is added then display and log will do nothing.
</para>

<para>
<command>static void removeErrorHandler();</command>
Removes the current error handler.
DESCRIPTION END

SEEALSO genErrorHandler genWxErrorHandler genTextErrorhandler genConsoleErrorHandler
DOCBOOK END
******************************************************************************/

#include <string>
#include "genErrorHandler.h"
#include "genUtility.h"

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
