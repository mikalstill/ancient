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

///////////////////////////////////////////////////////////////////////////////
// NEVER DO A cepError::display() IN THIS FILE...
///////////////////////////////////////////////////////////////////////////////

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
  // todo_mikal: trivsql should be changed to fix this
  string sql("SELECT " + valkey + " FROM cepConfig;");
  trivsql_recordset* rs;

  char *tmp = strdup(sql.c_str());
  cepDebugPrint(sql);
  rs = trivsql_execute(m_dbState, tmp);
  free(tmp);

  if(rs == NULL){
    return cepError(string("NULL recordset returned by SELECT from cepConfig") +
		    string(" with COLUMN ") + valkey, 
		    cepError::sevErrorRecoverable);
  }

  switch(rs->errno){
  case TRIVSQL_FALSE:
    cepDebugPrint("Lookup for this database key produced no results");
    outval = defval;
    return cepError();

  case TRIVSQL_TRUE:
    trivsql_rsmovefirst(rs);
    if(trivsql_rseof(rs) != TRIVSQL_TRUE){
      tmp = trivsql_rsfield(rs, 0);
      if(tmp != NULL)
	outval = string(tmp);
      else{
	cepDebugPrint("Recordset field was NULL, defaulting");
	outval = defval;
	return setValue(valkey, defval);
      }
      free(tmp);
      return cepError();
    }
    free(rs);
    
    // The recordset was empty
    cepDebugPrint("Empty recordset returned");
    outval = defval;
    return setValue(valkey, defval);

  default:
    cepDebugPrint("getValue failed with error: " + cepItoa(rs->errno));\
    outval = defval;
    free(rs);
    return setValue(valkey, defval);
  }
  
  return cepError();
}

cepError cepConfiguration::getValue(const string& valkey, 
				    const bool& defval,
				    bool& outval)
{
  string myov;
  cepError ce;

  cepDebugPrint("Redirecting bool getValue to string");
  ce = getValue(valkey, string(defval ? "true" : "false"), myov);
  if(ce.isReal())
    {
      return ce;
    }

  outval = (myov == "true");
  return cepError();
}

cepError cepConfiguration::getValue(const string& valkey, 
				    const int& defval,
				    int& outval)
{
  string myov;
  cepError ce;

  cepDebugPrint("Redirecting int getValue to string");
  ce = getValue(valkey, cepItoa(defval), myov);
  if(ce.isReal())
    {
      return ce;
    }

  outval = atoi(myov.c_str());
  return cepError();
}

// todo_mikal: this should use an update, not an insert
cepError cepConfiguration::setValue(const string& valkey,
				   const string& value)
{
  // todo_mikal: fix this
  char *tmp;

  // todo_mikal: I'm not sure why gcc insists on having this here...
  // I should look into it sometime...
  string sql("UPDATE cepConfig SET " + valkey + " = '" + 
	     value + "';");
  string table("CREATE TABLE cepConfig (" + valkey + "); INSERT INTO cepConfig (" + valkey + ") VALUES ('placeholder');");
  string alter("ALTER cepConfig ADD COLUMN " + valkey + ";");
  
  // TODO mikal fix const problem
  trivsql_recordset *rs;
  tmp = strdup(sql.c_str());
  cepDebugPrint(sql);
  rs = trivsql_execute(m_dbState, tmp);
  free(tmp);

  // Check for errors from the recordset
  switch(rs->errno){
  case TRIVSQL_FALSE:
  case TRIVSQL_TRUE:
    return cepError();
    
  case TRIVSQL_NOSUCHTABLE:
    cepDebugPrint("Recovering from no such table error");
    cepDebugPrint("CREATE TABLE cepConfig (version);");
    rs = trivsql_execute(m_dbState, "CREATE TABLE cepConfig (version);");
    if(rs == NULL){
      return cepError("NULL recordset returned by CREATE TABLE cepConfig",
		      cepError::sevErrorRecoverable);
    }

    switch(rs->errno){
    case TRIVSQL_FALSE:
    case TRIVSQL_TRUE:
      cepDebugPrint("Attempt to set the value again having created the table");      
      return setValue(valkey, value);

    default:
      return cepError("Database write recover had error: " + 
		      cepItoa(rs->errno),
		      cepError::sevErrorRecoverable);
    }
    break;

  case TRIVSQL_NOSUCHCOLUMN:
    cepDebugPrint("Recovering from a no such column error");
    tmp = strdup(alter.c_str());
    cepDebugPrint(alter);
    rs = trivsql_execute(m_dbState, tmp);
    free(tmp);
    if(rs == NULL){
      return cepError("NULL recordset returned by ALTER TABLE cepConfig",
		      cepError::sevErrorRecoverable);
    }

    switch(rs->errno){
    case TRIVSQL_FALSE:
    case TRIVSQL_TRUE:
      cepDebugPrint("Attempt to set the value again having added the column");      
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

cepError cepConfiguration::setValue(const string& valkey, const int& value){
    return setValue(valkey, cepItoa(value));
}

cepError cepConfiguration::setValue(const string& valkey, const bool& value){
    return setValue(valkey, value ? "true" : "false");
}
