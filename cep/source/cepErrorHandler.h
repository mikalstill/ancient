 /* 
  * Imp for the CEP error handler
  * Copyright (C) Blake Swadling                   2002
  * Copyright (C) Michael Still                    2002
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
  * Mass Ave, Cambridge, MA 02139, USA. 
*/

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
