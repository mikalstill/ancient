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


#include "cepErrorHandler.h"
#include "cepConfiguration.h"

/**an error handler that makes us of the wxWindows UI toolkit
  *@author Blake Swadling
  */

class cepWxErrorHandler : public cepErrorHandler  {
public: 
	cepWxErrorHandler();
	~cepWxErrorHandler();

   void displayError( class cepError & error );
   void logError( class cepError & error );

private:
  cepConfiguration *config;
  void initConfig();
  int getIcon( class cepError err );
  string getTitle ( class cepError err );

};

#endif  // ! __CEP_WXERRORHANDLER_H
