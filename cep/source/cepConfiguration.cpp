/* 
   Imp for the CEP configuration thingie
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

#include "core.h"

/******************************************************************************
DOCBOOK START

FUNCTION cepConfiguration
PURPOSE store user definable configuration information

SYNOPSIS START
cepConfiguration::cepConfiguration(string& persistFile);

SYNOPSIS END

DESCRIPTION This class stores user definable configuration information such as the size of the main window on startup. It is implemented using <command>trivsql</command>, a trivial SQL implementation from Michael Still (mikal@stillhq.com).

DOCBOOK END
******************************************************************************/

cepConfiguration::cepConfiguration(const string& filename)
{
  // todo_mikal: trivsql should be changed to fix this
  char *tmp = strdup(filename.c_str());
  m_dbState = trivsql_opendb(tmp);
  free(tmp);

  m_path = filename;
}

cepError cepConfiguration::getValue(const string& valkey, 
				    const string& defval,
				    string& outval)
{
  // todo_mikal: read database
  outval = defval;
  return cepError();
}

cepError cepConfiguration::getValue(const string& valkey, 
				    const bool& defval,
				    bool& outval)
{
  // todo_mikal: read database
  outval = defval;
  return cepError();
}

cepError cepConfiguration::setValue(const string& valkey,
				   const int& value)
{
  char *tmp;
  string sql = "INSERT INTO cepConfig (" + valkey + ") VALUES ('" + cepItoa(value) + "');";
  
  {
    cepError dbg("Executing " + sql + " against the configuration database at " + m_path,
		 cepError::sevDebug);
    dbg.display();
  }
  
  // TODO mikal fix const problem
  trivsql_recordset *rs;
  tmp = strdup(sql.c_str());
  rs = trivsql_execute(m_dbState, tmp);
  free(tmp);

  // Check for errors from the recordset
  string table;
  switch(rs->errno){
  case TRIVSQL_FALSE:
  case TRIVSQL_TRUE:
    return cepError();
    
  case TRIVSQL_NOSUCHTABLE:
    table = "CREATE TABLE cepConfig ('version');";
    tmp = strdup(table.c_str());
    free(rs);
    rs = trivsql_execute(m_dbState, tmp);
    free(tmp);

    switch(rs->errno){
    case TRIVSQL_FALSE:
    case TRIVSQL_TRUE:
      return setValue(valkey, value);

    default:
      return cepError("Database write recover had error: " + 
		      cepItoa(rs->errno),
		      cepError::sevErrorRecoverable);
    }
    break;

  default:
    return cepError("Database write had error: " + cepItoa(rs->errno),
		    cepError::sevErrorRecoverable);
  }
}
