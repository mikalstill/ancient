/* 
   UI for the CEP program
   Copyright (C) Michael Still                    2002

   Based on a demo which originally had this notice:

   ////////////////////////////////////////////////////////////////////////////
   // Name:        doc.cpp
   // Purpose:     Implements document functionality
   // Author:      Julian Smart
   // Created:     04/01/98
   // Copyright:   (c) Julian Smart and Markus Holzem
   // Licence:     wxWindows license
   ////////////////////////////////////////////////////////////////////////////
  
   [The wxWindows license is compatible with the GNU GPL, as it is the GNU
   LGPL]
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
#include "wx/txtstrm.h"

#if !wxUSE_DOC_VIEW_ARCHITECTURE
#error You must set wxUSE_DOC_VIEW_ARCHITECTURE to 1 in setup.h!
#endif

#include "doc.h"
#include "view.h"

IMPLEMENT_DYNAMIC_CLASS (cepDatasetDoc, wxDocument)
  cepDatasetDoc::cepDatasetDoc (void)
{
}

cepDatasetDoc::~cepDatasetDoc (void)
{
  doodleSegments.DeleteContents (TRUE);
}

#if wxUSE_STD_IOSTREAM
ostream & cepDatasetDoc::SaveObject (ostream & stream)
{
  wxDocument::SaveObject (stream);

  wxInt32
    n =
    doodleSegments.
    Number ();
  stream << n << '\n';

  wxNode *
    node =
    doodleSegments.
    First ();
  while (node)
    {
      DoodleSegment *
	segment = (DoodleSegment *)
	node->
	Data ();
      segment->SaveObject (stream);
      stream << '\n';

      node = node->Next ();
    }

  return stream;
}
#else
wxOutputStream & cepDatasetDoc::SaveObject (wxOutputStream & stream)
{
  wxDocument::SaveObject (stream);

  wxTextOutputStream
  text_stream (stream);

  wxInt32
    n =
    doodleSegments.
    Number ();
  text_stream << n << '\n';

  wxNode *
    node =
    doodleSegments.
    First ();
  while (node)
    {
      DoodleSegment *
	segment = (DoodleSegment *)
	node->
	Data ();
      segment->SaveObject (stream);
      text_stream << '\n';

      node = node->Next ();
    }

  return stream;
}
#endif

#if wxUSE_STD_IOSTREAM
istream & cepDatasetDoc::LoadObject (istream & stream)
{
  wxDocument::LoadObject (stream);

  wxInt32
    n =
    0;
  stream >> n;

  for (int i = 0; i < n; i++)
    {
      DoodleSegment *
	segment =
	new
	DoodleSegment;
      segment->LoadObject (stream);
      doodleSegments.Append (segment);
    }

  return stream;
}
#else
wxInputStream & cepDatasetDoc::LoadObject (wxInputStream & stream)
{
  wxDocument::LoadObject (stream);

  wxTextInputStream
  text_stream (stream);

  wxInt32
    n =
    0;
  text_stream >> n;

  for (int i = 0; i < n; i++)
    {
      DoodleSegment *
	segment =
	new
	DoodleSegment;
      segment->LoadObject (stream);
      doodleSegments.Append (segment);
    }

  return stream;
}
#endif
DoodleSegment::DoodleSegment (void)
{
}

DoodleSegment::DoodleSegment (DoodleSegment & seg)
{
  wxNode *node = seg.lines.First ();
  while (node)
    {
      DoodleLine *line = (DoodleLine *) node->Data ();
      DoodleLine *newLine = new DoodleLine;
      newLine->x1 = line->x1;
      newLine->y1 = line->y1;
      newLine->x2 = line->x2;
      newLine->y2 = line->y2;

      lines.Append (newLine);

      node = node->Next ();
    }
}

DoodleSegment::~DoodleSegment (void)
{
  lines.DeleteContents (TRUE);
}

#if wxUSE_STD_IOSTREAM
ostream & DoodleSegment::SaveObject (ostream & stream)
{
  wxInt32
    n =
    lines.
    Number ();
  stream << n << '\n';

  wxNode *
    node =
    lines.
    First ();
  while (node)
    {
      DoodleLine *
	line = (DoodleLine *)
	node->
	Data ();
      stream << line->x1 << " " <<
	line->y1 << " " << line->x2 << " " << line->y2 << "\n";
      node = node->Next ();
    }

  return stream;
}
#else
wxOutputStream & DoodleSegment::SaveObject (wxOutputStream & stream)
{
  wxTextOutputStream
  text_stream (stream);

  wxInt32
    n =
    lines.
    Number ();
  text_stream << n << '\n';

  wxNode *
    node =
    lines.
    First ();
  while (node)
    {
      DoodleLine *
	line = (DoodleLine *)
	node->
	Data ();
      text_stream << line->x1 << " " <<
	line->y1 << " " << line->x2 << " " << line->y2 << "\n";
      node = node->Next ();
    }

  return stream;
}
#endif

#if wxUSE_STD_IOSTREAM
istream & DoodleSegment::LoadObject (istream & stream)
{
  wxInt32
    n =
    0;
  stream >> n;

  for (int i = 0; i < n; i++)
    {
      DoodleLine *
	line =
	new
	DoodleLine;
      stream >> line->x1 >> line->y1 >> line->x2 >> line->y2;
      lines.Append (line);
    }

  return stream;
}
#else
wxInputStream & DoodleSegment::LoadObject (wxInputStream & stream)
{
  wxTextInputStream
  text_stream (stream);

  wxInt32
    n =
    0;
  text_stream >> n;

  for (int i = 0; i < n; i++)
    {
      DoodleLine *
	line =
	new
	DoodleLine;
      text_stream >> line->x1 >> line->y1 >> line->x2 >> line->y2;
      lines.Append (line);
    }

  return stream;
}
#endif
void
DoodleSegment::Draw (wxDC * dc)
{
  wxNode *node = lines.First ();
  while (node)
    {
      DoodleLine *line = (DoodleLine *) node->Data ();
      dc->DrawLine (line->x1, line->y1, line->x2, line->y2);
      node = node->Next ();
    }
}

/*
 * Implementation of drawing command
 */

DrawingCommand::DrawingCommand (const wxString & name, int command,
				cepDatasetDoc * ddoc, DoodleSegment * seg):
wxCommand (TRUE, name)
{
  doc = ddoc;
  segment = seg;
  cmd = command;
}

DrawingCommand::~DrawingCommand (void)
{
  if (segment)
    delete segment;
}

bool
DrawingCommand::Do (void)
{
  switch (cmd)
    {
    case DOODLE_CUT:
      {
	// Cut the last segment
	if (doc->GetDoodleSegments ().Number () > 0)
	  {
	    wxNode *node = doc->GetDoodleSegments ().Last ();
	    if (segment)
	      delete segment;

	    segment = (DoodleSegment *) node->Data ();
	    delete node;

	    doc->Modify (TRUE);
	    doc->UpdateAllViews ();
	  }
	break;
      }
    case DOODLE_ADD:
      {
	doc->GetDoodleSegments ().Append (new DoodleSegment (*segment));
	doc->Modify (TRUE);
	doc->UpdateAllViews ();
	break;
      }
    }
  return TRUE;
}

bool
DrawingCommand::Undo (void)
{
  switch (cmd)
    {
    case DOODLE_CUT:
      {
	// Paste the segment
	if (segment)
	  {
	    doc->GetDoodleSegments ().Append (segment);
	    doc->Modify (TRUE);
	    doc->UpdateAllViews ();
	    segment = (DoodleSegment *) NULL;
	  }
	doc->Modify (TRUE);
	doc->UpdateAllViews ();
	break;
      }
    case DOODLE_ADD:
      {
	// Cut the last segment
	if (doc->GetDoodleSegments ().Number () > 0)
	  {
	    wxNode *node = doc->GetDoodleSegments ().Last ();
	    DoodleSegment *seg = (DoodleSegment *) node->Data ();
	    delete seg;
	    delete node;

	    doc->Modify (TRUE);
	    doc->UpdateAllViews ();
	  }
      }
    }
  return TRUE;
}
