 /* 
  * Imp for the CEP error handler
  * Copyright (C) Blake Swadling                   2002
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

/******************************************************************************
DOCBOOK START

FUNCTION cepError
PURPOSE abstract class which is to be extended by any error handler. Used to
decouple the error mechanism from the user interface

SYNOPSIS START

SYNOPSIS END

DESCRIPTION START
This class is used to display errors in a flexible manner. extending classes need
to implement <command>displayError</command> and <command>logError</command>. To
use the error mechanism
</para>
  virtual void displayError( class cepError & error ) = 0;
  virtual void logError( class cepError & error ) = 0;
<para>

<para>
<command>virtual void displayError( class cepError & error ) = 0;</command>
the contract for this method is that it should take some action to display
the error in a meaningful way to the user.
</para>
<para>
<command>virtual void logError( class cepError & error ) = 0;</command>
the contract for this method is that it should take some action to make a
persistant record of the errorthe error for the user.
</para>

SEEALSO cepError cepWxErrorHandler cepTextErrorhandler cepConsoleErrorHandler
DOCBOOK END
******************************************************************************/

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
private:
   
};

#endif // ! __CEP_ERROR_HANDLER_H
