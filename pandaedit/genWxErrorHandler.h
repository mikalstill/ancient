 /* 
  * wxWindows version of the error handler
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

/******************************************************************************
DOCBOOK START

FUNCTION <!-- class name -->
genWxErrorHandler


PURPOSE <!-- use cases -->
provides a graphical error notification mechanism

SYNOPSIS START
<!-- how to instantiate one? -->
To make use of this class, the user must get an instance of one,
and then subscribe it as the current error handler. Subscription is
via a static method on genError as folows

genErrorHandler handler = new genWxErrorHandler();
genError::setErrorHandler( handler );
SYNOPSIS END

DESCRIPTION START
<!-- description goes here -->
<para>An implementation of the <command>genErrorHandler</command> class
which displays <command>genErrors</command> using wxWindows. The errors
are displayed as custom message boxes.
</para>

<para> <!-- per function descriptions -->
<command>void displayError( class genError & error )</command>
Displays a specific error using the wx windows toolkit
</para>
<para> <!-- per function descriptions -->
<command>void logError( class genError & error )</command>
Logs a specific error using an global log file
</para>
DESCRIPTION END

SEEALSO genError genErrorHandler genTextErrorhandler genConsoleErrorHandler
DOCBOOK END
******************************************************************************/

#ifndef __CEP_WXERRORHANDLER_H
#define __CEP_WXERRORHANDLER_H


#ifdef __WXGTK__
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include "wx/txtstrm.h"

#if !wxUSE_DOC_VIEW_ARCHITECTURE
#error You must set wxUSE_DOC_VIEW_ARCHITECTURE to 1 in setup.h!
#endif
#endif


#include "genErrorHandler.h"

class genWxErrorHandler : public genErrorHandler  {
public: 
	genWxErrorHandler();
	~genWxErrorHandler();

   void displayError( class genError & error );
   void logError( class genError & error );

private:
  int getIcon( class genError err );
};

#endif  // ! __CEP_WXERRORHANDLER_H
