/* 
   Imp for the CEP configuration storage thingie
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

#include <trivsql.h>

#ifndef CEP_CONFIG_HEADER
#define CEP_CONFIG_HEADER

/******************************************************************************
DOCBOOK START

FUNCTION cepConfiguration
PURPOSE store user definable configuration information

SYNOPSIS START
cepConfiguration::cepConfiguration(string& persistFile);

SYNOPSIS END

DESCRIPTION START
This class stores user definable configuration information such as the size of the main window on startup. It is implemented using <command>trivsql</command>, a trivial SQL implementation from Michael Still (mikal@stillhq.com).
</para>

<para>
<command>cepConfiguration (const string&amp; filename);</command>
Constructor, <command>filename</command> defines the location of the configuration database. One will be created if there is no database at this location already.
</para>

<para>
<command>cepError getValue(const string&amp; valkey, const string&amp; defval, string&amp; outval);</command>
Get a string value from the configuration database. <command>valkey</command> defines the unique keyname for this value, and <command>defval</command> defines the default value to be used if a value isn't found in the database. If the database lacks a value for this key, then the default value is also written into the database for next time.
</para>

<para>
<command> cepError getValue(const string&amp; valkey, const bool&amp; defval, bool&amp; outval);</command>
The same as the string version of <command>getValue</command>, but with a bool value.
</para>

<para>
<command>cepError getValue(const string&amp; valkey, const int&amp; defval, int&amp; outval);</command>
The same as the string version of <command>getValue</command>, but with an int value.
</para>

<para>
<command>cepError setValue(const string&amp; valkey, const string&amp; value);</command>
Set a string value in the configuration database with key <command>valkey</command> to the value <command>value</command>.
</para>

<para>
<command>cepError setValue(const string&amp; valkey, const int&amp; value);</command>
The same as the string version of <command>setValue</command>, but for an int.
</para>

<para>
<command>cepError setValue(const string&amp; valkey, const bool&amp; value);</command>
The same as the string version of <command>setValue</command>, but for a bool.
DESCRIPTION END

DOCBOOK END
******************************************************************************/

class cepConfiguration
{
 public:
  // Filename is the file which the config database should be persisted to
  cepConfiguration (const string& filename);
  
  // Get the value for a given configuration item, including default
  cepError getValue(const string& valkey, const string& defval, 
		    string& outval);
  cepError getValue(const string& valkey, const bool& defval, bool& outval);  
  cepError getValue(const string& valkey, const int& defval, int& outval);
  
  cepError setValue(const string& valkey, const string& value);
  cepError setValue(const string& valkey, const int& value);
  cepError setValue(const string& valkey, const bool& value);
  
 private:
  trivsql_state *m_dbState;
  string m_path;
};

#endif
