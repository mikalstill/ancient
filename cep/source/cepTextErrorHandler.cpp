/* 
 *   Imp for the CEP dataset
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

#include "cepCore.h"
#include "cepTextErrorHandler.h"

cepTextErrorHandler::cepTextErrorHandler()
{
  // todo_mikal: Should this version use the config stuff as well?
}

cepTextErrorHandler::~cepTextErrorHandler()
{
}

void cepTextErrorHandler::initConfig()
{
}

void cepTextErrorHandler::displayError( class cepError & err )
{
  // Log everything for now
  // todo_mikal improve
  logError( err );

  if ( err.getMessage() != "")
  {
    cerr << err.getTitle() << err.getMessage();
    err.doTerminate();
  }
}

void cepTextErrorHandler::logError( class cepError & err )
{
  gLog << err.getMessage() << " (" << err.getTitle () << ")" << endl;
}

int cepTextErrorHandler::getIcon ( class cepError err )
{
  return 0;
}
