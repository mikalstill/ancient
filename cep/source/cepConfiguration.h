/* 
   Imp for the CEP dataset
   Copyright (C) Michael Still                    2002
   Copyright (C) Blake Swadling                   2002
   
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

#ifndef CEP_CONFIGURATION_H
#define CEP_CONFIGURATION_H

#include <map>
#include <iostream.h>
#include <fstream>
#include <functional>
#include <string>

#include "cepError.h"

using namespace std;

/**A drop in replacement for the original cepConfiguration.
  *This implements a whiteboard pattern.
  *@author Blake Swadling
  *@version $Revision: 1.11 $
  */

class cepConfiguration
{
public:
  ~cepConfiguration ();
  static void initialise(  const string & filename );
  static cepConfiguration & getInstance();

  // Get the value for a given configuration item, including default
  cepError getValue (const string & valkey, const string & defval,
                     string & outval);
  cepError getValue (const string & valkey, const bool & defval, bool & outval);
  cepError getValue (const string & valkey, const int &defval, int &outval);

  cepError setValue (const string & valkey, const string & value);
  cepError setValue (const string & valkey, const int &value);
  cepError setValue (const string & valkey, const bool & value);

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
  static const string & getDefaultConfigPath();
};

#endif // ! CEP_CONFIGURATION_H
