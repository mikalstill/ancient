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

#include "cepConfiguration.h"
#include <stdlib.h>
#include <sstream>

cepConfiguration *cepConfiguration::config = 0;
  
cepConfiguration::cepConfiguration (const string & filename)
{
  path.assign (filename);
  // disard any error here. If we try to display it we get a segfault
  
  cepError err = load (path);
  if( err.isReal() ) {
    load ( getDefaultConfigPath() );
  }

}

cepConfiguration::~cepConfiguration ()
{
  delete( (cepConfiguration *)config );
}


void cepConfiguration::initialise(  const string & filename )
{
  config = new cepConfiguration( filename );
}

cepConfiguration & cepConfiguration::getInstance() {
  if( config == NULL ) {
    config = new cepConfiguration( getDefaultConfigPath());
  }
  return *config;
}

/**
 * Loads configuration data from the specified config file.
 *
 * @param filename the name of the file holding configuration data
 * @return the resulting error. check error.isReal() to see if an error was generated
 */
cepError cepConfiguration::load (const string & filename)
{
  cepError err;

  ifstream cfgFile ((const char *)filename.c_str (), ios::in);

  if (!cfgFile)
  {
    err =
      cepError ("failed to open config file" + filename,
                cepError::sevDebug);
  }
  else
  {
    err = readConfig (cfgFile);
  }
  return err;
}

/**
 * Saves configuration data to the specified config file.
 *
 * @param filename the name of the file to write the configuration data to
 * @return the resulting error. check error.isReal() to see if an error was generated
 */
cepError cepConfiguration::save (const string & filename)
{
  cepError err;
  ofstream out ((const char *)filename.c_str (), ios::trunc);

  writeConfig (out);
  return err;
}

/**
 * reads the configuration data from an input stream.
 *
 * @param in the input stream to read the data from
 * @return the resulting error. check error.isReal() to see if an error was generated
 */
cepError cepConfiguration::readConfig (ifstream & in)
{
  cepError err;

  pair < string, string > data;
  pair < map_t::iterator, bool > p;

  char buf[80];

  in.getline (buf, 80);

  while (strlen (buf) > 0)
  {
    err = parseConfigEntry ((const char *)buf, data);
    if (!err.isReal ())
    {
      p = map.insert (data);
      if (!p.second)
      {
        cepDebugPrint("Configuration database duplicate entry " + 
		      (*(p.first)).first + "->" + (*(p.first)).second);
      }
    }
    in.getline (buf, 80);
  }
  return err;
}

/**
 * parses a single configuration entry
 *
 * @param entry the string holding the configuration data
 * @param data a pair which will be populated with the data from the entry
 * @return the resulting error. check error.isReal() to see if an error was generated.
 *         If parsing the entry fails then we return a warning only, since we can
 *         easilty resort to default values
 */
cepError cepConfiguration::parseConfigEntry (const string & entry,
                                             pair < string, string > &data)
{
  bool success = true;
  char *tmp = strdup (entry.c_str ());

  // read the key
  char *ptr = strtok (tmp, "= ");

  if (ptr != NULL)
  {
    data.first = string (ptr);
  }
  else
  {
    success = false;
  }

  // read the value 
  ptr = strtok (NULL, " ");
  if (ptr != NULL)
  {
    data.second = string (ptr);
  }
  else
  {
    success = false;
  }

  free (tmp);                   // plug the leaks

  // check for failure
  if (!success)
  {
    return cepError ("failed to parse config value " + entry,
                     cepError::sevDebug);
  }
  return cepError ();
}

cepError cepConfiguration::writeConfig (ofstream & out)
{
  for (map_t::const_iterator i = map.begin (); i != map.end (); ++i)
  {
    out << i->first << "=" << i->second << endl;
  }

  return cepError();
}

cepError cepConfiguration::getValue (const string & valkey,
                                     const string & defval, string & outval)
{
  bool defaulted = false;
  map_t::const_iterator i = map.find (valkey);

  if (i == map.end ()) outval = defval;
  else outval = map[valkey];

  cepDebugPrint("Configuration database get <string> : requested " + valkey
    + " and returned " + outval + (defaulted ? " (default)" : ""));
  return cepError ();
}

cepError cepConfiguration::getValue (const string & valkey, const bool & defval,
                                     bool & outval)
{
  bool defaulted = false;
  map_t::const_iterator i = map.find (valkey);

  if (i == map.end ())
  {
    outval = defval;
    defaulted = true;
  }
  else
  {
    outval = (map[valkey] == "true");
  }
  cepDebugPrint("Configuration database get bool requested " + valkey
    + " and returned " + (outval ? "true" : "false")
    + (defaulted ? " (default)" : ""));
  return cepError ();
}

cepError cepConfiguration::getValue (const string & valkey, const int &defval,
                                     int &outval)
{
  bool defaulted = false;
  map_t::const_iterator i = map.find (valkey);

  if (i == map.end ())
  {
    outval = defval;
    defaulted = true;
  }
  else
  {
    outval = atoi (map[valkey].c_str ());
  }
  cepDebugPrint("Configuration database get int requested " + valkey
    + " and returned " + cepToString(outval) + (defaulted ? " (default)" : ""));
  return cepError ();
}

cepError cepConfiguration::setValue (const string & valkey,
                                     const string & value)
{
  map[valkey] = value;
  cepDebugPrint("Configuration database set string setting " + valkey + " to " + map[valkey]);
  return save (path);

}

cepError cepConfiguration::setValue (const string & valkey, const int &value)
{
  ostringstream oss;

  oss << value;
  map[valkey] = oss.str ();
  cepDebugPrint("Configuration database set int setting " + valkey + " to " + map[valkey]);
  return save (path);

}

cepError cepConfiguration::setValue (const string & valkey, const bool & value)
{
  map[valkey] = (value ? string ("true") : string ("false"));
  cepDebugPrint("Configuration database set bool setting " + valkey + " to " + map[valkey]);
  return save (path);
}

const string cepConfiguration::getDefaultConfigPath()
{
  char *homedir = getenv ("HOME");

  // Default to home dir or CWD
  if (homedir != NULL)
    return string(string(homedir) + "/.cep");
  else return string(".cep");
}
