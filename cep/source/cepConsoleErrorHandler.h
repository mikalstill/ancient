 /* 
  * wxWindows version of the error handler
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

FUNCTION <!-- class name -->
cepConsoleErrorHandler


PURPOSE <!-- use cases -->
provides a console ONLY error reporting mechanism. Used primarily for testing

SYNOPSIS START
<!-- how to instantiate one? -->
To make use of this class, the user must get an instance of one,
and then subscribe it as the current error handler. Subscription is
via a static method on cepError as follows

cepErrorHandler handler = new cepConsoleErrorHandler();
cepError::setErrorHandler( handler );
SYNOPSIS END

DESCRIPTION START
<!-- description goes here -->
<para>An implementation of the <command>cepErrorHandler</command> class
which displays <command>cepErrors</command> using std error only.
</para>

<para> <!-- per function descriptions -->
<command>void displayError( class cepError & error )</command>
Displays a specific error
</para>
<para> <!-- per function descriptions -->
<command>void logError( class cepError & error )</command>
logging is disabled
</para>
DESCRIPTION END

SEEALSO cepError cepErrorHandler cepTextErrorhandler cepWxErrorHandler
DOCBOOK END
******************************************************************************/

#ifndef __CEP_CONSOLEERRORHANDLER_H
#define __CEP_CONSOLEERRORHANDLER_H


#include "cepErrorHandler.h"

/** An error handler that uses std error ONLY
  * @author Blake Swadling
  */

class cepConsoleErrorHandler : public cepErrorHandler  {
public: 
	cepConsoleErrorHandler();
	~cepConsoleErrorHandler();

   void displayError( class cepError & error );
   void logError( class cepError & error );

private:
  void initConfig();
  int getIcon( class cepError err );

};

#endif  // ! __CEP_TEXTERRORHANDLER_H
