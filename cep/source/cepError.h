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

/******************************************************************************
DOCBOOK START

FUNCTION cepDebugPrint
PURPOSE print out debugging information via cepError

SYNOPSIS START
cepDebugPrint(string debugMessage);
SYNOPSIS END

DESCRIPTION START
<command>cepDebugPrint</command> creates a cepError of debug severity and immediately displays it -- this will normally result in the debug message being logged. The macro also pack into the message the name of the source file which called <command>cepDebugPrint</command>, and the line number that the call occurred at, for ease of debugging.
DESCRIPTION END

SEEALSO cepDebug
DOCBOOK END
******************************************************************************/

// This prints out some debugging information which is useful, use it instead
// of:
// cepError cepDebugPrint("Some text", cepError::sevDebug);
// cepDebugPrint.display();

///////////////////////////////////////////////////////////////////////////////
// THIS CANNOT DO A DISPLAY, AS THE CONFIG DB CODE USES THIS TO LOG DB PROBLEMS
// AND cepError::display() USES THE CONFIG DB...
///////////////////////////////////////////////////////////////////////////////

#define cepDebugPrint(errmsg) \
  { \
    cepError newname_cepDebugPrint(string("") + string(errmsg) + string(" at ") + string(__FILE__) + \
			string(":") + cepItoa(__LINE__), cepError::sevDebug); \
    newname_cepDebugPrint.log(); \
  }

/******************************************************************************
DOCBOOK START

FUNCTION cepError
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
SYNOPSIS END

DESCRIPTION START
This class is used to display and log messages from the user interface to the user.
</para>

<para>
The enumeration <command>severity</command> defines the possible error levels available to <command>cepError</command>. These levels have differing default resposes within the <command>cepError</command> class, depending on their perceived importance:

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
<command>cepError ();</command>
Default constructor. This will create a <command>cepError</command> with a default severity of <command>sevOk</command>.
</para>

<para>
<command>cepError (const string&amp; msg);</command>
Contruct a <command>cepError</command> with a severity of <command>sevErrorFatal</command>.
</para>

<para>
<command>cepError (const string& msg, severity level);</command>
Construct a <command>cepError</command> with the specified severity.
</para>

<para>
<command>~cepError ();</command>
//todo_mikal: make sure this is true
If the <command>cepError</command> has not been actioned (checked if real, copied, displayed or logged) by the time the destructor is called, then it will be actioned at this point.
</para>

<para>
<command>bool isReal ();</command>
If the <command>cepError</command> has a message string, then return true.
</para>

<para>
<command>void clear();</command>
Zero the <command>cepError</command>.
</para>

<para>
<command>void log();</command>
Write the <command>cepError</command> to the log file.
</para>

<para>
<command>void display ();</command>
Display the <command>cepError</command> if the display configuration allows it.
</para>

<para>
<command>string getTitle();</command>
Return the title which should be used in the display of this <command>cepError</command>.
</para>

<para>
<command>int getIcon();</command>
Return the icon identifier which should be used with the <command>cepError</command>.
DESCRIPTION END

SEEALSO cepDebugPrint cepConfiguration
DOCBOOK END
******************************************************************************/

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
};

#endif
