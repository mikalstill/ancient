/* 
 *   A class which handles persistance of configuration information
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

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <map>
#include <iostream>
#include <fstream>
#include <functional>
#include <string>

using namespace std;

class configuration
{
public:
  ~configuration ();
  static void initialise (const string & filename);
  static configuration & getInstance ();

  // Get the value for a given configuration item, including default
  void getValue (const string & valkey, const string & defval,
		 string & outval);
  void getValue (const string & valkey, const bool & defval, bool & outval);
  void getValue (const string & valkey, const int &defval, int &outval);
  void getValue (const string & valkey, const double &defval, double &outval);

  bool setValue (const string & valkey, const string & value);
  bool setValue (const string & valkey, const bool & value);
  bool setValue (const string & valkey, const int &value);
  bool setValue (const string & valkey, const double &value);

protected:
    configuration (const string & filename);

private:
  static configuration *config;

    map < string, string, less < string > >m_map;
  string m_path;

  bool load (const string & filename);
  bool save ();
  bool save (const string & filename);
  bool readConfig (ifstream & in);
  bool writeConfig (ofstream & out);
  static const string getDefaultConfigPath ();
};

#endif
