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

class cepConfiguration
{
 public:
  // Filename is the file which the config database should be persisted to
  cepConfiguration (const string& filename);
  
  // Get the value for a given configuration item, including default
  cepError getValue(const string& valkey, const string& defval, 
		    string& outval);
  cepError getValue(const string& valkey, const bool& defval, bool& outval);
  
  cepError setValue(const string& valkey, const int& value);
  
 private:
  trivsql_state *m_dbState;
  string m_path;
};

#endif
