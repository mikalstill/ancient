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

#include <iostream>
#include <fstream>

#include "genWxErrorHandler.h"

extern fstream gLog;

genWxErrorHandler::genWxErrorHandler()
{
}

genWxErrorHandler::~genWxErrorHandler()
{
}

void genWxErrorHandler::displayError( class genError & err )
{

  // Log everything for now
  // todo_mikal improve
  logError( err );

  /*
  if (gDisplayParams[(int)level].get () == genTSB::stUndefined)
  {
    // Deliberately dropping genError return value here
    bool dodisp = false;

    config->getValue (string ("error-display-") +
                 genToString ((int)level), true, dodisp);

    gDisplayParams[(int)level].set (dodisp);
  }

  if (gDisplayParams[(int)level].get () == genTSB::stFalse)
    return;
  */

  string msg = err.getMessage();
  if ( msg != "")
  {
    wxMessageBox ( msg.c_str (), err.getTitle().c_str (),
                  wxOK | wxCENTRE | getIcon( err ));
    err.doTerminate();
  }
}

void genWxErrorHandler::logError( class genError & err )
{
  gLog << err.getMessage() << " (" << err.getTitle () << ")" << endl;
}

int genWxErrorHandler::getIcon ( class genError err )
{
  switch (err.getSeverity())
  {
  case genError::sevOk:
    return 0;

  case genError::sevDebug:
    return wxICON_QUESTION;

  case genError::sevInformational:
    return wxICON_INFORMATION;

  case genError::sevWarning:
    return wxICON_EXCLAMATION;

  case genError::sevErrorRecoverable:
    return wxICON_ERROR;

  case genError::sevErrorFatal:
    return wxICON_ERROR;

  default:
    return wxICON_QUESTION;
  }

  return 0;
}
