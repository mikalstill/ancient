/* 
 *   Imp for the CEP dataset
 *   Copyright (C) Michael Still                    2002
 *   Copyright (C) Blake Swadling                   2002
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
/******************************************************************************
DOCBOOK START

FUNCTION <!-- class name -->
cepConfiguration


PURPOSE <!-- use cases -->
provides a persistance mechanism for configuration data

SYNOPSIS START
<!-- how to instantiate one? -->
Since it is sensible to have cnfiguration stored centrally,
this config mechanism is implemented as a singleton. To use it
you need to to 2 things.
1) initialise the configuration database for a specific config file (optional)
2) get a reference to the configuration. If the configuration ahs not been initialised
then the default configuration file is used (currently ~/.cep)

for example this will initialise the config database to "testcfg" in the current working
directory and will get a reference to the central config object ...

  cepConfiguration::initialise("testcfg");
  cepConfiguration cfg = cepConfiguration::getInstance();

SYNOPSIS END

DESCRIPTION START
<!-- description goes here -->
<para>A configuration class (using the singleton pattern) which allows
access to a single configuration store.
</para>


<para> <!-- per function descriptions -->
<command>static void initialise( const string &path )</command>
initialises the configuration database usng the specifed file. if the file
cannot be accessed then an attempt is made to initialise the default path.
this is static and must be called like so ..
  cepConfiguration::initialise( mypath );
</para>
<para> <!-- per function descriptions -->
<command>static cepConfiguration::getInstance()</command>
Gets a reference to the current configuration object.
this is a static an must be called like so ...
  cepConfiguration cfg = cepConfiguration::getInstance();
</para>
<para> <!-- per function descriptions -->
<command>void getVal( const string &key, const string &default, const string &value )</command>
gets a string value by key. if the value does not exist then the default is returned
</para>
<para> <!-- per function descriptions -->
<command>void getVal( const string &key, int default, int value )</command>
gets an int value by key. if the value does not exist then the default is returned
</para>
<para> <!-- per function descriptions -->
<command>void getVal( const string &key, bool default, bool value )</command>
gets a boolean value by key. if the value does not exist then the default is returned
</para>

<para> <!-- per function descriptions -->
<command>void getVal( const string &key, const string &value )</command>
places a string value into configuration using the key for id. If the value exists it
is overwritten
</para>
<para> <!-- per function descriptions -->
<command>void getVal( const string &key, int value )</command>
places an int value into configuration using the key for id. If the value exists it
is overwritten
</para>
<para> <!-- per function descriptions -->
<command>void getVal( const string &key, bool value )</command>
places a boolean value into configuration using the key for id. If the value exists it
is overwritten
</para>
DESCRIPTION END

DOCBOOK END
******************************************************************************/

#ifndef CEP_CONFIGURATION_H
#define CEP_CONFIGURATION_H

#include <map>
#include <iostream>
#include <fstream>
#include <functional>
#include <string>

#include "cepError.h"

using namespace std;

/**A drop in replacement for the original cepConfiguration.
  *This implements a whiteboard pattern.
  *@author Blake Swadling
  *@version $Revision: 1.16 $
  */

class cepConfiguration
{
public:
  ~cepConfiguration ();
  static void initialise(  const string & filename );
  static cepConfiguration & getInstance();

  // Get the value for a given configuration item, including default
  cepError getValue (const string & valkey, const string & defval, string & outval);
  cepError getValue (const string & valkey, const bool & defval, bool & outval);
  cepError getValue (const string & valkey, const int &defval, int &outval);
  cepError getValue (const string & valkey, const double &defval, double &outval);

  cepError setValue (const string & valkey, const string & value);
  cepError setValue (const string & valkey, const bool & value);
  cepError setValue (const string & valkey, const int &value);
  cepError setValue (const string & valkey, const double &value);

protected:
  // make this a singleton. There can be only one!
  cepConfiguration (const string & filename);

private:
  static cepConfiguration *config;
  
  typedef map < string, string, less < string > >map_t;
  map_t map;
  string path;

  cepError load (const string & filename);
  cepError save (const string & filename);
  cepError readConfig ( ifstream & in);
  cepError parseConfigEntry (const string & entry, pair < string,
                             string > &data);
  cepError writeConfig (ofstream & out);
  static const string getDefaultConfigPath();
};

#endif // ! CEP_CONFIGURATION_H
