/***************************************************************************
                          cepwxerrohandler.cpp  -  description
                             -------------------
    begin                : Fri Aug 9 2002
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

#include "cepWxErrorHandler.h"

cepWxErrorHandler::cepWxErrorHandler()
{
  initConfig();
}

cepWxErrorHandler::~cepWxErrorHandler()
{
}

void cepWxErrorHandler::initConfig()
{
  config = (cepConfiguration *)&cepConfiguration::getInstance();
}


void cepWxErrorHandler::displayError( class cepError & err )
{

  // Log everything for now
  // todo_mikal improve
  logError( err );

  /*
  if (gDisplayParams[(int)level].get () == cepTSB::stUndefined)
  {
    // Deliberately dropping cepError return value here
    bool dodisp = false;

    config->getValue (string ("cepErrordisplaylevel") +
                 cepItoa ((int)level), true, dodisp);

    gDisplayParams[(int)level].set (dodisp);
  }

  if (gDisplayParams[(int)level].get () == cepTSB::stFalse)
    return;
  */
  string msg = err.getMessage();
  if ( msg != "")
  {
#ifdef __WXGTK__
    wxMessageBox ( msg.c_str (), getTitle( err ).c_str (),
                  wxOK | wxCENTRE | getIcon( err ));
#else
    cout << getTitle () << ": " << err.getMessage() << endl;
#endif

  }
  else
  {
    cout << "you did not check isReal() prior to calling this did you :)" << endl;
  }

}
void cepWxErrorHandler::logError( class cepError & error )
{
  
}


int cepWxErrorHandler::getIcon ( class cepError err )
{

#ifdef __WXGTK__
  switch (err.getSeverity())
  {
  case cepError::sevOk:
    return 0;

  case cepError::sevDebug:
    return wxICON_QUESTION;

  case cepError::sevInformational:
    return wxICON_INFORMATION;

  case cepError::sevWarning:
    return wxICON_EXCLAMATION;

  case cepError::sevErrorRecoverable:
    return wxICON_ERROR;

  case cepError::sevErrorFatal:
    return wxICON_ERROR;

  default:
    return wxICON_QUESTION;
  }
#endif

  return 0;
}


string cepWxErrorHandler::getTitle ( class cepError err )
{
  switch (err.getSeverity())
  {
  case err.sevOk:
    return "Ok";

  case err.sevDebug:
    return "Debug";

  case cepError::sevInformational:
    return "Informational";

  case cepError::sevWarning:
    return "Warning";

  case cepError::sevErrorRecoverable:
    return "Recoverable Error";

  case cepError::sevErrorFatal:
    return "Fatal error";

  default:
    return "UNKNOWN ERROR LEVEL";
  }
}

