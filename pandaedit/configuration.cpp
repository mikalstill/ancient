/* 
 *   A class which handles persistance of configuration information
 *
 *   Originally developed for the Geodetic Data Modeling System, as a
 *   University of Canberra final year project by Blake Swadling and
 *   Michael Still. Extensively rewritten by Michael Still.
 *
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

#include <stdlib.h>
#include <sstream>
#include <stdio.h>

#include "configuration.h"
#include "stringArray.h"

configuration *
  configuration::config =
  0;

configuration::configuration (const string & filename)
{
  m_path = filename;
  if (!load(m_path))
    {
      load (getDefaultConfigPath ());
    }

}

configuration::~configuration ()
{
  delete ((configuration *) config);
}


void
configuration::initialise (const string & filename)
{
  config = new configuration (filename);
}

configuration & configuration::getInstance ()
{
  if (config == NULL)
    {
      config = new configuration (getDefaultConfigPath ());
    }
  return *config;
}

// Load the config from the specified file
bool configuration::load (const string & filename)
{
  ifstream  cfgFile ((const char *) filename.c_str (), ios::in);

  if (!cfgFile) return false;
  else return readConfig (cfgFile);
}

// Save to where we read from
bool configuration::save()
{
  return save(m_path);
}

// Save config data to specified path
bool configuration::save (const string & filename)
{
  ofstream  out ((const char *) filename.c_str (), ios::trunc);
  if(!out) return false;
  else return writeConfig (out);
}

bool configuration::readConfig (ifstream & in)
{
  pair < string, string > data;
  pair < map < string, string, less < string > >::iterator, bool > p;

  char buf[80];
  in.getline (buf, 80);
  while (strlen (buf) > 0)
    {
      stringArray tokens(buf, "= \t");
      if(tokens.size() < 2)
	printf("DEBUG: Skipped malformed config line\n");
      else{
	data.first = tokens[0];
	data.second = tokens[1];
	p = m_map.insert (data);
	if (!p.second)
	  printf("DEBUG: Repeated config entry\n");
      }
      in.getline (buf, 80);
    }

  return true;
}

bool configuration::writeConfig (ofstream & out)
{
  for (map < string, string, less < string > >::const_iterator i = m_map.begin (); i != m_map.end (); ++i)
    {
      out << i->first << "=" << i->second << endl;
    }

  return true;
}

void
  configuration::getValue (const string & valkey,
			      const string & defval, string & outval)
{
  map < string, string, less < string > >::const_iterator i = m_map.find (valkey);

  if (i == m_map.end ()){
    outval = defval;
    setValue(valkey, defval);
  }
  else
    outval = m_map[valkey];
}

void
  configuration::getValue (const string & valkey, const bool & defval,
			      bool & outval)
{
  map < string, string, less < string > >::const_iterator i = m_map.find (valkey);

  if (i == m_map.end ())
    {
      outval = defval;
      setValue(valkey, defval);
    }
  else
    {
      outval = (m_map[valkey] == "true");
    }
}

void
  configuration::getValue (const string & valkey, const int &defval,
			      int &outval)
{
  map < string, string, less < string > >::const_iterator i = m_map.find (valkey);

  if (i == m_map.end ())
    {
      outval = defval;
      setValue(valkey, defval);
    }
  else
    {
      outval = atoi (m_map[valkey].c_str ());
    }
}

void
  configuration::getValue (const string & valkey, const double &defval,
			      double &outval)
{
  map < string, string, less < string > >::const_iterator i = m_map.find (valkey);

  if (i == m_map.end ())
    {
      outval = defval;
      setValue(valkey, defval);
    }
  else
    {
      outval = atof (m_map[valkey].c_str ());
    }
}

bool
  configuration::setValue (const string & valkey, const string & value)
{
  m_map[valkey] = value;
  return save();

}

bool configuration::setValue (const string & valkey, const int &value)
{
  ostringstream
    oss;

  oss << value;
  m_map[valkey] = oss.str ();
  return save();

}

bool
  configuration::setValue (const string & valkey, const double &value)
{
  ostringstream oss;

  oss << value;
  m_map[valkey] = oss.str ();
  return save();

}

bool
  configuration::setValue (const string & valkey, const bool & value)
{
  m_map[valkey] = (value ? string ("true") : string ("false"));
  return save();
}

const string
configuration::getDefaultConfigPath ()
{
  char *homedir = getenv ("HOME");

  // Default to home dir or CWD
  if (homedir != NULL)
    return string (string (homedir) + "/.pandaedit");
  else
    return string ("pandaedit.conf");
}
