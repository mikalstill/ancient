/////////////////////////////////////////////////////////////////////////////
// Name:        doc.h
// Purpose:     Document classes
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: cepDoc.h,v 1.1 2002-06-02 10:13:02 u964076 Exp $
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
// #pragma interface
#endif

#ifndef __DOCSAMPLEH__
#define __DOCSAMPLEH__

#include "wx/docview.h"

class cepDoc:public wxDocument
{
DECLARE_DYNAMIC_CLASS (cepDoc) private:
public:
  cepDoc (void);
   ~cepDoc (void);

#if wxUSE_STD_IOSTREAM
    ostream & SaveObject (ostream & text_stream);
    istream & LoadObject (istream & text_stream);
#else
    wxOutputStream & SaveObject (wxOutputStream & stream);
    wxInputStream & LoadObject (wxInputStream & stream);
#endif
};
#endif
