 /* 
  * Imp for the CEP error Copyright (C) Michael Still 2002
  * 
  * This program is free software; you can redistribute it and/or modify it
  * under the terms of the GNU General Public License as published by the Free
  * Software Foundation; either version 2 of the License, or (at your option)
  * any later version.
  * 
  * This program is distributed in the hope that it will be useful, but WITHOUT 
  * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  * more details.
  * 
  * You should have received a copy of the GNU General Public License along
  * with this program; if not, write to the Free Software Foundation, Inc., 675 
  * Mass Ave, Cambridge, MA 02139, USA. */
#include "cepError.h"



// todo_mikal: copy constructor, should have actioned status

cepError::cepError ()
{
  message = "";
  level = cepError::sevOk;
  actioned = true;
}

cepError::cepError (const string & msg)
{
  message = msg;
  level = cepError::sevErrorFatal;
  actioned = false;
}

cepError::cepError (const string & msg, cepError::severity level)
{
  message = msg;
  level = level;
  actioned = false;
}

bool cepError::handlerInstalled=false;
cepErrorHandler * cepError::handler = NULL;

cepError::~cepError ()
{
  if (!actioned)
    cepDebugPrint ("cepError was not actioned: " + message);
}

void cepError::addErrorHandler( cepErrorHandler& h )
{
  if( !handlerInstalled )
  {
    handler = &h;
    handlerInstalled = true;
  }
}

bool cepError::isReal ()
{
  actioned = true;
  return message != "";
}

void cepError::clear ()
{
  actioned = true;
  message = "";
}

void cepError::log ()
{
  handler->logError( *this );
  actioned = true;
}

void cepError::display ()
{
  if( handler != NULL ) {
    handler->displayError( *this );
  }
  actioned = true;
}

int cepError::getSeverity()
{
  return level;
}

string & cepError::getMessage()
{
  return message;
}
