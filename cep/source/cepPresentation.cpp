
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
#include "cepPresentation.h"

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

const long INVALID = -2000000000;

cepPresentation::cepPresentation (long width, long height):
  m_width(width + 1),
  m_height(height),
  m_xTitle("Undefined Axis Title"),
  m_yTitle("Undefined Axis Title"),
  m_xminval(2000000000),
  m_xmaxval(-2000000000),
  m_yminval(2000000000),
  m_ymaxval(-2000000000),
  m_emaxval(-2000000000),
  m_useAverage(false),
  m_data (1, long (INVALID)),
  m_useErrors(true),
  m_errors (1, long(0)),
  m_raster(NULL)
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

  m_errorColor.red = 0;
  m_errorColor.green = 0;
  m_errorColor.blue = 0;
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

cepError cepPresentation::addDataPoint (long x, long y, long error)
{
  cepDebugPrint ("Current data vector size is: " + 
		 cepToString (m_data.size ()));
  cepDebugPrint ("Added (" + cepToString (x) + ", " + 
		 cepToString (y) + "," + cepToString(error) + ")");

  // Values
  if ((unsigned int) x >= m_data.size ())
  {
    try
    {
      unsigned int i = m_data.size();
      m_data.resize (x + CHUNKALLOC);
      for(; i < m_data.size(); i++)
	m_data[i] = INVALID;
      m_errors.resize (x + CHUNKALLOC);

      cepDebugPrint ("Resize presentation data to " + 
		     cepToString (x + CHUNKALLOC));
    }
    catch (...)
    {
      return
        cepError ("Could not add data point because of memory allocation error",
                  cepError::sevErrorFatal);
    }
  }

  m_data[x] = y;
  m_errors[x] = error;

  if (x > m_xmaxval)
    m_xmaxval = x;
  if (x < m_xminval)
    m_xminval = x;

  if (y > m_ymaxval)
    m_ymaxval = y;
  if (y < m_yminval)
    m_yminval = y;

  if (error > m_emaxval)
    m_emaxval = error;

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

  if ((graph = plot_newplot (m_width, m_height)) == NULL)
    return cepError("Could not initialise a new plot", 
		    cepError::sevErrorRecoverable);

  // How big are the maxima and minima?
  long ymaxval = m_useErrors ? m_ymaxval + m_emaxval : m_ymaxval;
  long yminval = m_useErrors ? m_yminval - m_emaxval : m_yminval;

  cepDebugPrint("yminval = " + cepToString(yminval) + 
		" ymaxval = " + cepToString(ymaxval));

  // Correct for a centered view
  if(m_currentView == viewCentered){
    if(ymaxval > yminval) yminval = -ymaxval;
    else ymaxval = -yminval;
  }

  cepDebugPrint("yminval = " + cepToString(yminval) + 
		" ymaxval = " + cepToString(ymaxval));

  // Determine the vertical scaling factor
  long yrange = ymaxval - yminval;
  float oldyscale = yrange * 10 / (m_height - 20);
  float yscale = (float) yrange / (m_height - 20);

  cepDebugPrint("Dimensions of graph bitmap: " + cepToString(m_width) + " x " +
		cepToString(m_height));
  cepDebugPrint("Yscale is: " + cepToString(yscale) + " new scale " + cepToString(oldyscale));

  // If we are using errors, then we draw these underneath
  if(m_useErrors){
    cepDebugPrint("Displaying errors");
    plot_setlinecolor(graph, m_errorColor.red, m_errorColor.green,
		      m_errorColor.blue);
    for(unsigned int i = 0; i < m_data.size(); i++){
      if(m_data[i] != INVALID){
	// Vertical line
	plot_setlinestart(graph, i + 10, (unsigned int) 
			  ((yrange - (m_data[i] + m_errors[i]) + 
			   yminval) / yscale + 10));
	plot_addlinesegment(graph, i + 10, (unsigned int)
			    ((yrange - (m_data[i] - m_errors[i]) + yminval) 
			    / yscale + 10));
	plot_strokeline(graph);
	plot_endline(graph);

	// Top horizontal line
	plot_setlinestart(graph, i + 8, (unsigned int)
			  ((yrange - (m_data[i] + m_errors[i]) + yminval)
			   / yscale + 10));
	plot_addlinesegment(graph, i + 12, (unsigned int)
			    ((yrange - (m_data[i] + m_errors[i]) + yminval) 
			    / yscale + 10));
	plot_strokeline(graph);
	plot_endline(graph);

	// Bottom horizontal line
	plot_setlinestart(graph, i + 8, (unsigned int)
			  ((yrange - (m_data[i] - m_errors[i]) + yminval)
			   / yscale + 10));
	plot_addlinesegment(graph, i + 12, (unsigned int) 
			    ((yrange - (m_data[i] - m_errors[i]) + yminval) 
			     / yscale + 10));
	plot_strokeline(graph);
	plot_endline(graph);
      }
    }
  }

  // Draw the axes. We want the graph to be over this, but the error lines to
  // be underneath
  plot_setlinecolor(graph, m_axesColor.red, m_axesColor.green,
		    m_axesColor.blue);
  plot_setlinestart(graph, 9, 0);
  plot_addlinesegment(graph, 9, m_height);
  plot_strokeline(graph);
  plot_endline(graph);

  // In the zoomed view, there might not be an x axis...
  //  if((m_currentView == viewCentered) || 
  //     ((m_currentView == viewZoomed) && (ymaxval > 0) && (yminval < 0))){
  plot_setlinestart(graph, 10, (unsigned int) 
		    ((yrange - 0 + yminval) / yscale + 10));
  plot_addlinesegment(graph, m_width - 10, (unsigned int) 
		      ((yrange - 0 + yminval) / yscale + 10));
  plot_strokeline(graph);
  plot_endline(graph);
  //  }
  //  else cepDebugPrint("Skipping x axis");
  
  // Now draw the actual graph
  plot_setlinecolor(graph, m_lineColor.red, m_lineColor.green,
		    m_lineColor.blue);
  for(unsigned int i = 0; i < m_data.size(); i++){
    if(m_data[i] != INVALID){
      unsigned int xpoint = i + 10;
      unsigned int ypoint = (unsigned int) ((yrange - m_data[i] + yminval) / yscale + 10);

      plot_setlinestart(graph, xpoint - 1, ypoint);
      plot_addlinesegment(graph, xpoint, ypoint - 1);
      plot_addlinesegment(graph, xpoint + 1, ypoint);
      plot_addlinesegment(graph, xpoint, ypoint + 1);
      plot_closeline(graph);
      plot_strokeline(graph);
      plot_endline(graph);
    }
  }

  // Now put the text annotations onto the bitmap
  plot_setfontcolor(graph, 26, 22, 249);
  // todo_mikal: check for existance
  plot_setfont(graph, "n019004l.pfb", 12);
  // plot_setfont(graph, "/usr/share/fonts/default/Type1/n021004l.pfb", 20);
  plot_settextlocation(graph, 20, 70);
  //plot_writestring(graph, m_yTitle);

  // Get the raster (in case we use it later)
  m_raster = plot_getraster (graph);
  return cepError ();
#else
  return cepError ("Libplot was not installed at configuration time",
		   cepError::sevErrorRecoverable);
#endif
}

cepError
cepPresentation::createPNG (const string & filename)
{
#if defined HAVE_LIBPLOT
#if defined HAVE_LIBPNG
  cepDebugPrint ("Generating PNG for: " + filename);

  cepError e = createBitmap();
  if(e.isReal())
    return e;

  // Write out the PNG file
  FILE *image;
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

  for (long i = 0; i < m_height; i++)
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
  return cepError ("Libpng was missing at configure time",
		   cepError::sevErrorRecoverable);
#endif
#else
  return cepError ("Libplot was missing at configure time",
		   cepError::sevErrorRecoverable);
#endif
}

void cepPresentation::useAverage(bool yesno)
{
  m_useAverage = yesno;
  recalculateAverage();
}

void cepPresentation::useErrors(bool yesno)
{
  m_useErrors = yesno;
}

void cepPresentation::recalculateAverage()
{
  long temp = 0;

  for(unsigned int i = 0; i < m_data.size(); i++){
    temp += m_data[i];
  }

  m_average = temp / m_data.size();
  cepDebugPrint("Average for presentation is " + cepToString(m_average));
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

void cepPresentation::setErrorColor(char red, char green, char blue)
{
  m_errorColor.red = red;
  m_errorColor.green = green;
  m_errorColor.blue = blue;
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

