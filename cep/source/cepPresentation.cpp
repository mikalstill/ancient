
/* 
 *   Imp for the CEP data presentation
 *   Copyright (C) Michael Still                    2002
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

#include "cepCore.h"

#if defined HAVE_LIBPLOT
#include <libplot.h>
#endif

#if defined HAVE_LIBPNG
#include <png.h>
#endif

#if defined HAVE_LIBPANDA
#include <panda/constants.h>
#include <panda/functions.h>
#endif

// This number needs to be positive, and preferably greater than 2...
// Having big numbers is more efficient, but wastes more RAM
const long CHUNKALLOC = 10;

// todo_mikal: -64000 means an invalid value. I think I need to think about
// this a little bit harder at some point...
const long INVALID = -64000;

cepPresentation::cepPresentation (long width, long height):
  m_data (1, long (INVALID)),
  m_width(width + 1),
  m_height(height),
  m_xTitle("Undefined Axis Title"),
  m_yTitle("Undefined Axis Title"),
  m_raster(NULL),
  m_xminval(2000000000),
  m_xmaxval(-2000000000),
  m_yminval(2000000000),
  m_ymaxval(-2000000000),
  m_useAverage(false)
{
  m_averageColor.red = 0;
  m_averageColor.green = 0;
  m_averageColor.blue = 0;

  m_axesColor.red = 0;
  m_axesColor.green = 0;
  m_axesColor.blue = 0;

  m_lineColor.red = 0;
  m_lineColor.green = 0;
  m_lineColor.blue = 0;
}

void
cepPresentation::xAxisTitle (const string & title)
{
  m_xTitle = title;
}

void
cepPresentation::yAxisTitle (const string & title)
{
  m_yTitle = title;
}

cepError cepPresentation::addDataPoint (long x, long y)
{
  cepDebugPrint ("Current data vector size is: " + cepItoa (m_data.size ()));
  cepDebugPrint ("Added (" + cepLtoa (x) + ", " + cepLtoa (y) + ")");

  // Values
  if (x >= m_data.size ())
  {
    try
    {
      int i = m_data.size();
      m_data.resize (x + CHUNKALLOC);
      for(; i < m_data.size(); i++)
	m_data[i] = INVALID;

      cepDebugPrint ("Resize presentation data to " + cepLtoa (x + CHUNKALLOC));
    }
    catch (...)
    {
      return
        cepError ("Could not add data point because of memory allocation error",
                  cepError::sevErrorFatal);
    }
  }

  m_data[x] = y;

  if (x > m_xmaxval)
    m_xmaxval = x;
  if (x < m_xminval)
    m_xminval = x;

  if (y > m_ymaxval)
    m_ymaxval = y;
  if (y < m_yminval)
    m_yminval = y;

  return cepError ();
}

cepError
cepPresentation::createPDF (const string & filename)
{
  return
    cepError
    ("TODO: This feature is not currently implemented. Please come again.",
     cepError::sevWarning);

#if defined HAVE_LIBPANDA
  cepDebugPrint ("Generating PDF for: " + filename);
  createBitmap ();

  // panda_init();
  // panda_pdf doc = panda_open(filename.c_str(), "w");
  // panda_page page = panda_newpage(doc, panda_pagesize_a4);
  // panda_close(doc);

  return cepError ();
#else
  return cepError ("Panda was missing at configure time");
#endif
}

cepError
cepPresentation::createBitmap ()
{
#if defined HAVE_LIBPLOT
  plot_state *graph;

  cepDebugPrint ("Generating bitmap. Dimensions: " + cepItoa (m_width) +
                 " by " + cepItoa (m_height) + ". X value range: " +
                 cepItoa (m_xminval) + " to " + cepItoa (m_xmaxval) +
                 ". Y value range: " + cepItoa (m_yminval) + " to " +
                 cepItoa (m_ymaxval) + ".");

  if ((graph = plot_newplot (m_width, m_height)) == NULL)
  {
    fprintf (stderr, "Could not allocate a new plot\n");
    exit (1);
  }

  unsigned int midpoint;
  float yscale;
  cepError err;
  bool linestarted (false);

  switch(m_currentView){
  case viewCentered:
    cepDebugPrint("Centered graph view");
    midpoint = m_height / 2;
    yscale = cepMax (m_ymaxval, cepAbs (m_yminval)) / (midpoint - 10);

    // Draw some axes for this view
    cepDebugPrint("This view type needs axes");
    plot_setlinecolor (graph, m_axesColor.red, m_axesColor.green, 
		       m_axesColor.blue);
    plot_setlinestart (graph, 10, 10);
    plot_addlinesegment (graph, 10, m_height - 10);
    plot_strokeline (graph);
    plot_endline (graph);
    
    plot_setlinestart (graph, 10, midpoint);
    plot_addlinesegment (graph, m_width - 10, midpoint);
    plot_strokeline (graph);
    plot_endline (graph);
    
    // Draw data points
    plot_setlinecolor (graph, m_lineColor.red, m_lineColor.green, 
		       m_lineColor.blue);
    for (int i = 0; i < m_data.size (); i++)
      {
	if (m_data[i] != INVALID)
	  {
	    if (!linestarted)
	      {
		plot_setlinestart (graph, i + 10, 
				   (unsigned int) (midpoint - (m_data[i] / yscale)));
		linestarted = true;
	      }
	    else
	      plot_addlinesegment (graph, i + 10, 
				   (unsigned int) (midpoint - (m_data[i] / yscale)));
	  }
      }
    break;

  case viewZoomed:
    cepDebugPrint("Zoomed graph view");
    midpoint = 0;
    yscale = ((float) (m_ymaxval - m_yminval)) / ((float) m_height);

    // Draw the data points
    plot_setlinecolor (graph, m_lineColor.red, m_lineColor.green, 
		       m_lineColor.blue);
    for (int i = 0; i < m_data.size (); i++)
      {
	if (m_data[i] != INVALID)
	  {
	    if (!linestarted)
	      {
		plot_setlinestart (graph, i + 10, 
				   (unsigned int) (m_height - 
				   ((m_data[i] - m_yminval) / yscale)));
		linestarted = true;
	      }
	    else{
	      plot_addlinesegment (graph, i + 10, 
				   (unsigned int) (m_height - 
				   ((m_data[i] - m_yminval) / yscale)));
	    }
	  }
      }    
    break;

  default:
    err = cepError("No such view", cepError::sevErrorFatal);
    err.display();
  }
    
  cepDebugPrint ("Midpoint is " + cepItoa (midpoint));
  cepDebugPrint ("Yscale is " + cepFtoa (yscale));
  
  if (!linestarted)
    return cepError ("No datapoints defined", cepError::sevErrorRecoverable);

  plot_strokeline (graph);
  plot_endline (graph);

  // Draw the average if required
  if(m_useAverage){
    cepDebugPrint("Drawing average line");
    plot_setlinecolor(graph, m_averageColor.red, m_averageColor.green,
		      m_averageColor.blue);
    plot_setlinestart(graph, 10, (unsigned int) midpoint - (m_average / yscale));
    plot_addlinesegment(graph, m_width, (unsigned int) midpoint - (m_average / yscale));
    plot_strokeline(graph);
    plot_endline(graph);
  }

  // Get the raster (in case we use it later)
  m_raster = plot_getraster (graph);
  return cepError ();
#else
  return cepError ("Libplot was not installed at configuration time");
#endif
}

cepError
cepPresentation::createPNG (const string & filename)
{
#if defined HAVE_LIBPLOT
#if defined HAVE_LIBPNG
  cepDebugPrint ("Generating PNG for: " + filename);
  createBitmap ();

  // Write out the PNG file
  FILE *image;
  unsigned int i;
  png_structp png;
  png_infop info;
  png_bytepp row_pointers = NULL;

  if ((image = fopen (filename.c_str (), "wb")) == NULL)
  {
    return cepError ("Could not open the file \"" + filename +
                     "\" for writing");
  }

  // Get ready for writing
  if ((png =
       png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL,
                                NULL)) == NULL)
  {
    return
      cepError
      ("Could not create write structure for PNG (possibly out of memory) while generating a PNG version of a graph");
  }

  // Get ready to specify important stuff about the image
  if ((info = png_create_info_struct (png)) == NULL)
  {
    return
      cepError
      ("Could not create PNG info structure for writing (possibly out of memory) while generating a PNG version of a graph");
  }

  if (setjmp (png_jmpbuf (png)))
  {
    return
      cepError
      ("Could not set the PNG jump value for writing while generating a PNG version of a graph");
  }

  // This is needed before IO will work (unless you define callbacks)
  png_init_io (png, image);

  // Define important stuff about the image
  info->channels = 3;
  info->pixel_depth = 24;
  png_set_IHDR (png, info, m_width, m_height, 8, PNG_COLOR_TYPE_RGB,
                PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                PNG_FILTER_TYPE_DEFAULT);
  png_write_info (png, info);

  // Write the image out
  if ((row_pointers = (png_byte **)
       malloc (m_height * sizeof (png_bytep))) == NULL)
  {
    return
      cepError
      ("Memory allocation error generating row pointers for PNG version of a graph");
  }

  for (i = 0; i < m_height; i++)
  {
    row_pointers[i] = (png_byte *) (m_raster + (i * m_width * 3));
  }

  png_write_image (png, row_pointers);

  // Cleanup
  png_write_end (png, info);
  png_destroy_write_struct (&png, &info);
  fclose (image);

  return cepError ();
#else
  return cepError ("Libpng was missing at configure time");
#endif
#else
  return cepError ("Libplot was missing at configure time");
#endif
}

void cepPresentation::useAverage(bool yesno)
{
  m_useAverage = yesno;
  recalculateAverage();
}

void cepPresentation::recalculateAverage()
{
  long temp = 0;
  int i;

  for(i = 0; i < m_data.size(); i++){
    temp += m_data[i];
  }

  m_average = temp / m_data.size();
  cepDebugPrint("Average for presentation is " + cepLtoa(m_average));
}

long cepPresentation::getAverage()
{
  return m_average;
}

void cepPresentation::setAverageColor(char red, char green, char blue)
{
  m_averageColor.red = red;
  m_averageColor.green = green;
  m_averageColor.blue = blue;
}

void cepPresentation::setAxesColor(char red, char green, char blue)
{
  m_axesColor.red = red;
  m_axesColor.green = green;
  m_axesColor.blue = blue;
}

void cepPresentation::setLineColor(char red, char green, char blue)
{
  m_lineColor.red = red;
  m_lineColor.green = green;
  m_lineColor.blue = blue;
}

void cepPresentation::setView(view v)
{
  m_currentView = v;
}
