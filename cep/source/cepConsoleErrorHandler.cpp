/* 
 *   Imp for the CEP dataset
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
#include "cepConsoleErrorHandler.h"

cepConsoleErrorHandler::cepConsoleErrorHandler ()
{
  // init any resources ... we have none
}

cepConsoleErrorHandler::~cepConsoleErrorHandler ()
{
  // release any resources ... we have none
}

void
cepConsoleErrorHandler::initConfig ()
{
  // do we need to init the config? doublful
}

void
cepConsoleErrorHandler::displayError (class cepError & err)
{
  if (err.isReal ())
    {
      cerr << err.getTitle () << ": " << err.getMessage () << endl;
      // we are testing, no termination please !!!!
      // err.doTerminate();
    }
}

int
cepConsoleErrorHandler::getIcon (class cepError err)
{
  // no icons
  return 0;
}

void
cepConsoleErrorHandler::logError (class cepError & error)
{
  // no logging. if you want this use the test error handler
}
