/***************************************************************************
                          cepErrorHandler.h  -  description
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

#ifndef __CEP_ERROR_HANDLER_H
#define __CEP_ERROR_HANDLER_H

#include "cepError.h"

class cepErrorHandler {
public:
  virtual ~cepErrorHandler();
  virtual void displayError( class cepError & error ) = 0;
  virtual void logError( class cepError & error ) = 0;
   
protected:
  cepErrorHandler();
// BS - how to get this to work? This header is included in cepError
/* friend int cepError::getSeverity(); */
private:
   
};

#endif // ! __CEP_ERROR_HANDLER_H
