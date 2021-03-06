/* 
 *   Canvas for the CEP program
 *
 *   Copyright (C) Michael Still                    2002
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

#include "cepUI.h"
#include "cepDoc.h"
#include "cepView.h"
#include "cepPresentation.h"
#include "cepPlot.h"
#include "cepLsUi.h"
#include "cepDate.h"
#include "cepDateUi.h"

#include "cepLs.h"

#include <stdlib.h>
#include <unistd.h>

#include <wx/colour.h>
#include <wx/colordlg.h>

#include "cepCanvas.h"

BEGIN_EVENT_TABLE (cepCanvas, wxScrolledWindow)
EVT_MOUSE_EVENTS (cepCanvas::OnMouseEvent) END_EVENT_TABLE ()
// Define a constructor for my canvas
  cepCanvas::cepCanvas (wxView * v, wxFrame * frame, const wxPoint & pos,
			const wxSize & size, long style):
wxScrolledWindow (frame, -1, pos, size, style),
m_view (v),
m_selectXStart (-1),
m_select (selNone),
m_frame (frame)
{
  m_config = (cepConfiguration *) & cepConfiguration::getInstance ();
  m_isFreq[0] = false;
  m_isFreq[1] = false;
  m_isFreq[2] = false;
}

// Define the repainting behaviour
void
cepCanvas::OnDraw (wxDC & dc)
{
  if (m_view)
    m_view->OnDraw (&dc);
}

// Process mouse events, these can include mouse moves, and clicks
void
cepCanvas::OnMouseEvent (wxMouseEvent & event)
{
  wxClientDC dc (this);
  PrepareDC (dc);

  // This sets the device context so that our drawing causes an inversion, lines are drawn with black,
  // and polygons are filled with black.
  dc.SetLogicalFunction (wxINVERT);
  dc.SetPen (*wxBLACK_PEN);
  dc.SetBrush (*wxBLACK_BRUSH);
  wxPoint pt (event.GetLogicalPosition (dc));

  string msg ("Mouse location: " + cepToString (pt.x) + ", " +
	      cepToString (pt.y));
  ((cepFrame *) wxGetApp ().GetTopWindow ())->SetStatusText (msg.c_str (), 0);

  string dirName;
  cepDataset::direction selDir = determineGraph (pt.y, dirName);

  if (selDir < 0)
    {
      cepDebugPrint ("Determined graph as: " + cepToString (selDir));
      return;
    }
  if (selDir > 2)
    {
      cepDebugPrint ("Determined graph as: " + cepToString (selDir));
      return;
    }

  int top, bottom, width;
  graphPlacement (selDir, top, bottom, width);

  // Is the mouse down?
  if (event.LeftIsDown () || event.RightIsDown ())
    {
      int cwidth, cheight;
      m_frame->GetSize (&cwidth, &cheight);

      // Record the type of selection
      if (event.LeftIsDown () && (m_select != selLeft))
	{
	  cepDebugPrint ("Reset selection region (left)");
	  m_selectXStart = -1;
	  m_select = selLeft;
	}
      else if (event.RightIsDown () && (m_select != selRight))
	{
	  cepDebugPrint ("Reset selection region (right)");
	  m_selectXStart = -1;
	  m_select = selRight;
	}

      if (m_selectXStart == -1)
	{
	  m_selectXStart = m_selectXPrevious = pt.x;
	  m_selDir = selDir;
	  m_selDirString = dirName;

	  float startExtracted =
	    ((m_selectXStart - 10) * m_horizScale[selDir] +
	     m_xminval[selDir]) / 10000;
	  string startDate;
	  if (!m_isFreq[selDir])
	    {
	      cepDate start (startExtracted);
	      startDate =
		start.getDay () + " " + start.getShortMonthName () + " " +
		start.getYear ();
	    }
	  else
	    {
	      startDate = cepToString (startExtracted, true) + " C/D";
	    }

	  ((cepFrame *) wxGetApp ().GetTopWindow ())->
	    SetStatusText (startDate.c_str (), 2);
	  dc.DrawLine (m_selectXStart, 0, m_selectXStart, cheight);
	}
      else
	{
	  m_selectXEnd = pt.x;

	  float startExtracted =
	    ((m_selectXStart - 10) * m_horizScale[selDir] +
	     m_xminval[selDir]) / 10000;
	  float endExtracted =
	    ((m_selectXEnd - 10) * m_horizScale[selDir] +
	     m_xminval[selDir]) / 10000;
	  string sel;
	  if (!m_isFreq[selDir])
	    {
	      cepDate start (startExtracted);
	      string startDate =
		start.getDay () + " " + start.getShortMonthName () + " " +
		start.getYear ();

	      cepDate end (endExtracted);
	      string endDate =
		end.getDay () + " " + end.getShortMonthName () + " " +
		end.getYear ();

	      sel = string (startDate + " to " + endDate);
	    }
	  else
	    {
	      sel =
		cepToString (startExtracted,
			     true) + " C/D to " + cepToString (endExtracted,
							       true) + " C/D";
	    }

	  ((cepFrame *) wxGetApp ().GetTopWindow ())->SetStatusText (sel.
								     c_str (),
								     2);

	  // Draw the highlight
	  if ((m_selectXPrevious + 1) == m_selectXEnd)
	    {
	      dc.DrawLine (m_selectXPrevious + 1, 0, m_selectXPrevious + 1,
			   cheight);
	    }
	  if ((m_selectXPrevious + 1) < m_selectXEnd)
	    {
	      for (int i = m_selectXPrevious; i < m_selectXEnd; i++)
		dc.DrawLine (i + 1, 0, i + 1, cheight);
	    }
	  else if (m_selectXPrevious > m_selectXEnd)
	    {
	      for (int i = m_selectXEnd; i < m_selectXPrevious; i++)
		dc.DrawLine (i + 1, 0, i + 1, cheight);
	    }
	  m_selectXPrevious = m_selectXEnd;
	}
    }
  else if (m_selectXStart != -1)
    {
      // Process the selection, and then move on with our lives...
      int selectXStart = m_selectXStart;
      m_selectXStart = -1;

      float startExtracted =
	((selectXStart - 20) * m_horizScale[selDir] +
	 m_xminval[selDir]) / 10000;
      float endExtracted =
	((m_selectXEnd - 20) * m_horizScale[selDir] +
	 m_xminval[selDir]) / 10000;
      if (startExtracted > endExtracted)
	{
	  float temp = endExtracted;
	  endExtracted = startExtracted;
	  startExtracted = temp;
	}

      cepDate startDate (startExtracted);
      cepDate endDate (endExtracted);

      // Allow the user to tweak the dates entered
      cepDateUi dateUI;
      dateUI.showDateRange (endDate, startDate);
      if (dateUI.getToDate () < 0)
	{
	  Refresh ();
	  return;
	}
      if (dateUI.getFromDate () < 0)
	{
	  Refresh ();
	  return;
	}
      cepDebugPrint ("User editted selection region to: " +
		     cepToString (dateUI.getFromDate ()) + " to " +
		     cepToString (dateUI.getToDate ()));

      // Clear the status bar
      ((cepFrame *) wxGetApp ().GetTopWindow ())->SetStatusText ("", 2);

      // Extract the dataset
      cepDoc *theDoc = (cepDoc *) m_view->GetDocument ();
      cepDataset *theDataset = theDoc->getDataset ();

      // We can only handle this event if we are ready
      if (theDataset && theDataset->isReady ())
	{
	  cepDataset newds;
	  cepDebugPrint ("Processing selected region");

	  switch (m_select)
	    {
	    case selLeft:
	      cepDebugPrint ("Zoom on selected region");
	      newds =
		theDataset->filter (dateUI.getFromDate (),
				    dateUI.getToDate ());
	      if (newds.getMatrix ((cepDataset::direction) 0) == NULL)
		{
		  cepError
		    err ("The selected region did not contain any datapoints",
			 cepError::sevErrorRecoverable);
		  err.display ();
		  Refresh ();
		  return;
		}
	      break;

	    case selRight:
//      cepDebugPrint("Replace on selected region");
//      newds = theDataset->replace(dateUI.getFromDate(), dateUI.getToDate(), 0.0);
//      if(newds.getMatrix((cepDataset::direction) 0) == NULL){
//        cepError err("The selected region did not contain any datapoints",
//                     cepError::sevErrorRecoverable);
//        err.display();
	      Refresh ();
	      return;
//      }
	      break;

	    case selNone:
	    default:
	      cepDebugPrint ("Unknown select command");
	      Refresh ();
	      return;
	    }

	  cepDebugPrint ("Opening select output dataset");
	  char *cfname = strdup ("/tmp/cep.XXXXXX");
	  int fd;
	  fd = mkstemp (cfname);
	  close (fd);

	  string newcfname (string (cfname) + "~" + theDataset->getName ());
	  newds.write (newcfname.c_str ());

	  wxGetApp ().m_docManager->
	    CreateDocument (string (newcfname + ".dat1").c_str (),
			    wxDOC_SILENT);
	  free (cfname);
	}
      else
	{
	  cepError
	    notReady
	    ("The dataset is not ready for that action to be performed",
	     cepError::sevWarning);
	  notReady.display ();
	}

      // Force a redraw of the canvas
      Refresh ();
    }
  else
    {
      // The left button is not down, so just show the date we are hovering over
      float xextracted =
	((pt.x - 20) * m_horizScale[selDir] + m_xminval[selDir]) / 10000;
      cepDate hoverDate (xextracted);

      float yextracted = (m_yrange[selDir] + m_yminval[selDir] -
			  ((float) (pt.y - top - 20) * m_vertScale[selDir])) /
	10000;

      string hover;

      if (!m_isFreq[selDir])
	{
	  hover =
	    hoverDate.getDay () + " " + hoverDate.getShortMonthName () + " " +
	    hoverDate.getYear () + " (" + cepToString (xextracted,
						       true) + ") reading " +
	    cepToString (yextracted, true);
	}
      else
	{
	  hover = cepToString (xextracted, true) + " C/D reading " +
	    cepToString (yextracted, true);
	}

      ((cepFrame *) wxGetApp ().GetTopWindow ())->SetStatusText (hover.
								 c_str (), 2);
    }
}

// Determine which graph we are over (and display it to the user in the status bar)
cepDataset::direction cepCanvas::determineGraph (int y, string & name)
{
  if (y < 0)
    {
      ((cepFrame *) wxGetApp ().GetTopWindow ())->
	SetStatusText ("No graph selected", 1);
      name = "Unknown";
      return cepDataset::dirUnknown;
    }

  bool showX, showY, showZ;
  int
    graphCount;
  cepError err;

  err = graphStatus (showX, showY, showZ, graphCount);
  if (err.isReal ())
    err.display ();

  if (graphCount == 0)
    {
      ((cepFrame *) wxGetApp ().GetTopWindow ())->
	SetStatusText ("No graphs visible", 1);
      name = "Unknown";
      return cepDataset::dirUnknown;
    }

  int
    width,
    height,
    presHeight,
    graph;
  GetSize (&width, &height);
  presHeight = height / graphCount;
  graph = y / presHeight;

  if (showX && (graph-- == 0))
    {
      ((cepFrame *) wxGetApp ().GetTopWindow ())->SetStatusText ("North", 1);
      name = "North";
      return cepDataset::dirX;
    }
  if (showY && (graph-- == 0))
    {
      ((cepFrame *) wxGetApp ().GetTopWindow ())->SetStatusText ("East", 1);
      name = "East";
      return cepDataset::dirY;
    }
  if (showZ && (graph-- == 0))
    {
      ((cepFrame *) wxGetApp ().GetTopWindow ())->SetStatusText ("Up", 1);
      name = "Up";
      return cepDataset::dirZ;
    }

  ((cepFrame *) wxGetApp ().GetTopWindow ())->SetStatusText ("Unknown graph",
							     1);
  name = "Unknown";
  return cepDataset::dirUnknown;
}

// Determine what graphs are currenly being drawn, and a total count
cepError
cepCanvas::graphStatus (bool & x, bool & y, bool & z, int &count)
{
  x = m_showx;
  y = m_showy;
  z = m_showz;

  count = 0;
  if (x)
    count++;
  if (y)
    count++;
  if (z)
    count++;

  return cepError ();
}

// Find the rectangle on the canvas occupied by a given graph
cepError
  cepCanvas::graphPlacement (cepDataset::direction dir, int &top, int &bottom,
			     int &width)
{
  // Get the total size of the canvas
  int cwidth, cheight;
  m_frame->GetSize (&cwidth, &cheight);
  cepDebugPrint ("Canvas size: " + cepToString (cwidth) + " x " +
		 cepToString (cheight));

  // Determine how many graphs we are showing, and which ones
  bool show[(int) cepDataset::dirUnknown];
  int graphCount;
  cepError err =
    graphStatus (show[(int) cepDataset::dirX], show[(int) cepDataset::dirY],
		 show[(int) cepDataset::dirZ], graphCount);
  if (err.isReal ())
    return err;

  // No graphs showing
  if (graphCount == 0)
    {
      top = bottom = width = -1;
      return cepError ();
    }

  // This graph not showing
  if (!show[(int) dir])
    {
      top = bottom = width = -1;
      return cepError ();
    }

  int presDrop = 0;
  int presHeight = cheight / graphCount;

  if (dir == cepDataset::dirX)
    {
      top = presDrop;
      bottom = presDrop + presHeight;
      width = cwidth;
      return cepError ();
    }

  if (show[cepDataset::dirX])
    {
      presDrop += presHeight;
    }

  if (dir == cepDataset::dirY)
    {
      top = presDrop;
      bottom = presDrop + presHeight;
      width = cwidth;
      return cepError ();
    }

  if (show[cepDataset::dirY])
    {
      presDrop += presHeight;
    }

  if (dir == cepDataset::dirZ)
    {
      top = presDrop;
      bottom = presDrop + presHeight;
      width = cwidth;
      return cepError ();
    }

  top = bottom = width = -1;
  return cepError ();
}
