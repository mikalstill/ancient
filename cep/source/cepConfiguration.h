
/***************************************************************************
                          cepconfiguration2.h  -  description
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
  *@version $Revision: 1.10 $
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
