
/***************************************************************************
                          cepconfiguration.cpp  -  description
                             -------------------
    begin                : Sun Aug 4 2002
    copyright            : (C) 2002 by Blake Swadling
    email                : blake@swadling.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "cepCore.h"
#include "cepConfiguration.h"
#include <stdlib.h>
#include <sstream>

cepConfiguration::cepConfiguration (const string & filename)
{
  path.assign (filename);
  // disard any error here. If we try to display it we get a segfault
  load (path);

}

cepConfiguration::~cepConfiguration ()
{
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
                cepError::sevErrorFatal);
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
        cout << "duplicate entry " << (*(p.first)).
          first << "->" << (*(p.first)).second << endl;
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
                     cepError::sevWarning);
  }
  return cepError ();
}

cepError cepConfiguration::writeConfig (ofstream & out)
{
  cepError err;

  for (map_t::const_iterator i = map.begin (); i != map.end (); ++i)
  {
    out << i->first << "=" << i->second << endl;
  }

  return err;
}

cepError cepConfiguration::getValue (const string & valkey,
                                     const string & defval, string & outval)
{
  bool defaulted = false;
  map_t::const_iterator i = map.find (valkey);

  if (i == map.end ())
  {
    outval = defval;
  }
  else
 {
    outval = map[valkey];
  }
  cout << "<get<string> : requested " << valkey
    << " and returned " << outval << (defaulted ? " (default)" : "") << endl;
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
  cout << "<get(bool)> : requested " << valkey
    << " and returned " << (outval ? "true" : "false")
    << (defaulted ? " (default)" : "") << endl;
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
  cout << "<get(int)> : requested " << valkey
    << " and returned " << outval << (defaulted ? " (default)" : "") << endl;
  return cepError ();
}

cepError cepConfiguration::setValue (const string & valkey,
                                     const string & value)
{
  map[valkey] = value;
  cout << "<set(string)> : setting " << valkey << " to " << map[valkey] << endl;
  return save (path);

}

cepError cepConfiguration::setValue (const string & valkey, const int &value)
{
  ostringstream oss;

  oss << value;
  map[valkey] = oss.str ();
  cout << "<set(int)> : setting " << valkey << " to " << map[valkey] << endl;
  return save (path);

}

cepError cepConfiguration::setValue (const string & valkey, const bool & value)
{
  map[valkey] = (value ? string ("true") : string ("false"));
  cout << "<set(bool)> : setting " << valkey << " to " << map[valkey] << endl;
  return save (path);
}
