
/*
 *   Imp for the CEP error
 *   Copyright (C) Michael Still                    2002
 *   Copyright (C) Blake Swadling                   2002
 *   Copyright (C) Kristy Van Der Vlist             2002 
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

#include "genError.h"

genError::genError ()
{
  m_message = "";
  m_level = genError::sevOk;
  m_actioned = true;
}

genError::genError (const string & msg)
{
  m_message = msg;
  m_level = genError::sevWarning;
  m_actioned = false;
}

genError::genError (const string & msg, genError::severity level)
{
  m_message = msg;
  m_level = level;
  m_actioned = false;
}

bool genError::m_handlerInstalled=false;
genErrorHandler * genError::m_handler = NULL;

genError::~genError ()
{
  if (!m_actioned)
    genDebugPrint ("genError was not actioned: " + m_message);
}

void genError::addErrorHandler( genErrorHandler& h )
{
  if( !m_handlerInstalled )
  {
    m_handler = &h;
    m_handlerInstalled = true;
  }
}

void genError::init()
{
  if((m_message != "") && (m_actioned == false))
  {
    genDebugPrint ("genError was not actioned: " + m_message);
  }
    
  m_actioned = false;
  m_level = genError::sevOk;
  m_message = "";
}

void genError::setError(const string & msg, genError::severity level)
{
  m_message = msg;
  m_level = level;
  m_actioned = false;
}

void genError::removeErrorHandler()
{
  if( m_handlerInstalled )
  {
    delete m_handler;
    m_handlerInstalled = false;
  }
}

bool genError::isReal ()
{
  m_actioned = true;
  return m_message != "";
}

void genError::clear ()
{
  m_actioned = true;
  m_message = "";
}

void genError::log ()
{
  if(m_handler != NULL)
    m_handler->logError( *this );
  m_actioned = true;
}

void genError::display ()
{
  if( m_handler != NULL ) {
    if( isReal() ) {
      m_handler->displayError( *this );
      }
    }
  m_actioned = true;
}

int genError::getSeverity()
{
  return m_level;
}

string & genError::getMessage()
{
  return m_message;
}

string genError::getTitle()
{
  switch (getSeverity())
  {
  case sevOk:
    return "Ok";

  case sevDebug:
    return "Debug";

  case sevInformational:
    return "Informational";

  case sevWarning:
    return "Warning";

  case sevErrorRecoverable:
    return "Recoverable Error";

  case sevErrorFatal:
    return "Fatal error";

  default:
    return "UNKNOWN ERROR LEVEL";
  }
}

void genError::doTerminate()
{
  if(m_level == sevErrorFatal)
    exit(-1);
}
