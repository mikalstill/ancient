/////////////////////////////////////////////////////////////////////////////
// Name:        cepDoc.h
// Purpose:     Document classes
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: cepDoc.h,v 1.4 2002-07-14 12:38:32 u982087 Exp $
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
// #pragma interface
#endif

#ifndef __DOCSAMPLEH__
#define __DOCSAMPLEH__

#include "wx/docview.h"
#include "cepCore.h"
#include <wx/progdlg.h>

class cepDoc:public wxDocument
{
  DECLARE_DYNAMIC_CLASS (cepDoc) 

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
  
  void incrementProgress();
  cepDataset *getDataset();
  
 private:
  cepDataset *m_dataset;
  wxProgressDialog *m_progress;
  int m_progressCount;
};
#endif

void ds_progressCallback (int plane, long line);
