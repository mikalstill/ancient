
/* 
 *   UI for the CEP program
 *   Copyright (C) Michael Still                    2002
 *   Copyright (C) Kristy Van Der Vlist             2002
 *   Copyright (C) Blake swadling                   2002
 *
 *   Based on a demo which originally had this notice:
 *  
 *   /////////////////////////////////////////////////////////////////////////
 *   // Name:        view.cpp
 *   // Purpose:     View classes
 *   // Author:      Julian Smart
 *   // Created:     04/01/98
 *   // Copyright:   (c) Julian Smart and Markus Holzem
 *   // Licence:     wxWindows license
 *   /////////////////////////////////////////////////////////////////////////
 *
 *   [The wxWindows license is compatible with the GNU GPL, as it is the GNU
 *   LGPL]
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

#ifdef __GNUG__
// #pragma implementation
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_DOC_VIEW_ARCHITECTURE
#error You must set wxUSE_DOC_VIEW_ARCHITECTURE to 1 in setup.h!
#endif

#include "genUI.h"
#include "pdfDoc.h"
#include "pdfView.h"
#include "render.h"

#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <iomanip>

#include <wx/colour.h>
#include <wx/colordlg.h>

IMPLEMENT_DYNAMIC_CLASS (pdfView, wxView)
BEGIN_EVENT_TABLE (pdfView, wxView)
END_EVENT_TABLE ()

pdfView::pdfView ()
{
  canvas = (genCanvas *) NULL;
  frame = (wxFrame *) NULL;
  m_parentFrame = (wxFrame *) NULL;
  m_page = 0;
}

void
pdfView::setParentFrame (wxFrame * parentFrame)
{
  m_parentFrame = parentFrame;
}

pdfView::~pdfView ()
{
}

// What to do when a view is created. Creates actual
// windows for displaying the view.
bool
pdfView::OnCreate (wxDocument * doc, long WXUNUSED (flags))
{
  frame = wxGetApp ().CreateChildFrame (doc, this, TRUE);
  frame->SetTitle ("");

  //  m_parentFrame = wxGetApp().GetMainFrame();
  canvas = GetMainFrame ()->CreateCanvas (this, frame);

#ifdef __X__
  // X seems to require a forced resize
  frame->SetSize (-1, -1, x, y);
#endif

  frame->Show (TRUE);
  Activate (TRUE);
  return TRUE;
}

// Sneakily gets used for default print/preview
// as well as drawing on the screen.
void
pdfView::OnDraw (wxDC * dc)
{
  dc->SetFont (*wxNORMAL_FONT);
  dc->SetPen (*wxBLACK_PEN);

  pdfDoc *theDoc = (pdfDoc *) GetDocument ();
  if(!theDoc->isReady()){
    printf("DEBUG: Draw ignored because PDF document not ready\n");
    return;
  }

  string& filename = m_renders[m_page];
  printf("DEBUG: Page render cache names \"%s\"\n", filename.c_str());
  if(filename == ""){
    try{
      object foo(-1, -1);
      pdf* thePDF = theDoc->getPDF();

      // Find the catalog -- I could probably miss this step, but it seems like
      // a good idea for now...
      object& catalog = foo;
      if(!thePDF->findObject(dictitem::diTypeName, "Type", "Catalog", catalog)){
	fprintf(stderr, "Bad PDF: No catalog\n");
	exit(1);
      }

      // Now find the pages object as refered to by the catalog
      if(!catalog.hasDictItem(dictitem::diTypeObjectReference, "Pages")){
	fprintf(stderr, "Bad PDF: No pages object refered to in catalog\n");
	exit(1);
      }

      object& pages = foo;
      if(!catalog.getDict().getValue("Pages", *thePDF, pages)){
	fprintf(stderr, "Bad PDF: Could not get pages object, but the catalog references it!\n");
	exit(1); 
      } 

      // Now find all the page objects referenced in the pages object
      string kids;
      if(!pages.getDict().getValue("Kids", kids)){
	fprintf(stderr, "Bad PDF: No pages found in PDF\n");
	exit(1);
      }
      
      // Find the pages, and then display the first page
      objectlist pagelist(kids, thePDF);
      pdfRender renPage(*thePDF, pagelist[m_page], m_page);
      if(!renPage.render()){
	fprintf(stderr, "Page render failed\n");
	exit(1);
      }
      printf("DEBUG: PNG filename is %s\n",
	     renPage.getPNGfile().c_str());

      // Now push that into the cache
      filename = renPage.getPNGfile();
    }
    catch(...){
      fprintf(stderr, "Exception caught\n");
      exit(40);
    }
  }

  // And now we can assume that there is a PNG somewhere out there we can paint...
  // todo_mikal: shrinking options
  // todo_mikal: center if smaller than window
  printf("DEBUG: Painting PNG \"%s\"\n", filename.c_str());
  try
    {
      wxImage theImage (filename.c_str(), wxBITMAP_TYPE_PNG);
      wxBitmap theBitmap (theImage.ConvertToBitmap ());
      dc->DrawBitmap (theBitmap, 0, 0);
    }
  catch (...)
    {
      printf("DEBUG: Exception caught in the graph draw routine");
    }
}

void
pdfView::OnUpdate (wxView * WXUNUSED (sender), wxObject * WXUNUSED (hint))
{
  if (canvas)
    canvas->Refresh ();
}

// Clean up windows used for displaying the view.
bool
pdfView::OnClose (bool deleteWindow)
{
  if (!GetDocument ()->Close ())
    return FALSE;

  // Clear the canvas in case we're in single-window mode,
  // and the canvas stays.
  genDebugPrint ("Clean up the canvas");

  // todo: The following line was causing a segv
  // canvas->Clear ();
  canvas->m_view = (wxView *) NULL;
  canvas = (genCanvas *) NULL;

  if (frame)
    frame->SetTitle (wxTheApp->GetAppName ());

  SetFrame ((wxFrame *) NULL);
  Activate (FALSE);

  if (deleteWindow)
    {
      genDebugPrint ("Close the window");
      delete frame;
      return TRUE;
    }
  return TRUE;
}
