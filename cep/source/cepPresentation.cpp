
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

#if defined HAVE_LIBMPLOT
#include <libmplot.h>
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

cepPresentation::cepPresentation (long width, long height, cepMatrix<double> *ds):
  m_width(width + 1),
  m_height(height),
  m_xTitle("Undefined Axis Title"),
  m_yTitle("Undefined Axis Title"),
  m_xminval(2000000000),
  m_xmaxval(-2000000000),
  m_yminval(2000000000),
  m_ymaxval(-2000000000),
  m_emaxval(-2000000000),
  m_useErrors(true),
  m_raster(NULL),
  m_ds(ds),
  m_haveMaxima(false)
{
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

// Find the maxima and minima in the matrix I am looking at
// todo_mikal: should this be part of the matrix class so it is more efficient?
void cepPresentation::findMaxMinMa()
{
  // todo_kristy: I should be able to just ask the matrix for these...
  cepDebugPrint("Finding matrix maxima and minima");
  for(int i = 0; i < m_ds->getNumRows(); i++){
    if ((long) (m_ds->getValue(i, cepDataset::colDate) * 10000) > m_xmaxval) 
      m_xmaxval = (long) (m_ds->getValue(i, cepDataset::colDate) * 10000);
    if ((long) (m_ds->getValue(i, cepDataset::colDate) * 10000) < m_xminval) 
      m_xminval = (long) (m_ds->getValue(i, cepDataset::colDate) * 10000);
    
    if ((long) (m_ds->getValue(i, cepDataset::colSample) * 10000) > m_ymaxval) 
      m_ymaxval = (long) (m_ds->getValue(i, cepDataset::colSample) * 10000);
    if ((long) (m_ds->getValue(i, cepDataset::colSample) * 10000) < m_yminval) 
      m_yminval = (long) (m_ds->getValue(i, cepDataset::colSample) * 10000);
    
    cepDebugPrint("Float version = " + cepToString(m_ds->getValue(i, 2)));
    cepDebugPrint("Comparing error: " + cepToString((long) (m_ds->getValue(i, 2) * 10000)) +
		  " at line " + cepToString(i));
    if ((long) (m_ds->getValue(i, cepDataset::colError) * 10000) > m_emaxval) 
      m_emaxval = (long) (m_ds->getValue(i, cepDataset::colError) * 10000);
  }
  cepDebugPrint("Found xmax = " + cepToString(m_xmaxval) + " xmin = " + cepToString(m_xminval) + 
		" ymax = " + cepToString(m_ymaxval) + " ymin = " + cepToString(m_yminval) + 
		" emax = " + cepToString(m_emaxval));
  
  m_haveMaxima = true;
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
  float dummyscale;
  long dummyrange;
  createBitmap (dummyscale, dummyrange);

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
cepPresentation::createBitmap (float& scale, long& minval)
{
#if defined HAVE_LIBMPLOT
  plot_state *graph;

  if(!m_haveMaxima)
    findMaxMinMa();

  if ((graph = plot_newplot (m_width, m_height)) == NULL)
    return cepError("Could not initialise a new plot", 
		    cepError::sevErrorRecoverable);

  // Draw a box around the graph
  plot_setlinestart(graph, 0, 0);
  plot_addlinesegment(graph, 0, m_height - 1);
  plot_addlinesegment(graph, m_width - 2, m_height - 1);
  plot_addlinesegment(graph, m_width - 2, 0);
  plot_closeline(graph);
  plot_strokeline(graph);
  plot_endline(graph);

  // How big are the maxima and minima?
  long ymaxval = m_useErrors ? m_ymaxval + m_emaxval : m_ymaxval;
  long yminval = m_useErrors ? m_yminval - m_emaxval : m_yminval;

  cepDebugPrint("yminval = " + cepToString(yminval) + " ymaxval = " + cepToString(ymaxval));
  cepDebugPrint("xminval = " + cepToString(m_xminval) + " xmaxval = " + cepToString(m_xmaxval));

  // Determine the vertical scaling factor
  long yrange = ymaxval - yminval;
  long xrange = m_xmaxval - m_xminval;

  // I had this bug once
  if(yminval > ymaxval){
    cepError err("Impossible maxima and minima in vertical direction: max = " + cepToString(ymaxval) +
	     " min = " + cepToString(yminval), cepError::sevErrorFatal);
    err.display();
  }

  float yscale = (float) yrange / (m_height - 20);
  float xscale = (float) xrange / (m_width - 20);
  scale = xscale;
  minval = m_xminval;
  
  cepDebugPrint("Dimensions of graph bitmap: " + cepToString(m_width) + " x " +
		cepToString(m_height));
  cepDebugPrint("Yscale is: " + cepToString(yscale));
  cepDebugPrint("Xscale is: " + cepToString(xscale));

  // If we are using errors, then we draw these underneath
  if(m_useErrors){
    cepDebugPrint("Displaying errors");
    plot_setlinecolor(graph, m_errorColor.red, m_errorColor.green,
		      m_errorColor.blue);
    for(int i = 0; i < m_ds->getNumRows(); i++){
      long convdate = (long) (m_ds->getValue(i, cepDataset::colDate) * 10000);
      long convsample = (long) (m_ds->getValue(i, cepDataset::colSample) * 10000);
      long converror = (long) (m_ds->getValue(i, cepDataset::colError) * 10000);
      unsigned int horiz = (unsigned int) ((convdate - m_xminval) / xscale + 10);

      cepDebugPrint("Plotting " + cepToString(convdate) +  " " + cepToString(convsample) + " " +
		    cepToString(converror) + " horiz = " + cepToString(horiz));

      // Vertical line
      plot_setlinestart(graph, horiz,
			(unsigned int) ((yrange - (convsample + converror) + 
					 yminval) / yscale + 10));
      plot_addlinesegment(graph, horiz,
			  (unsigned int) ((yrange - (convsample - converror) + yminval) 
					  / yscale + 10));
      plot_strokeline(graph);
      plot_endline(graph);
      
      // Top horizontal line
      plot_setlinestart(graph, horiz - 2, 
			(unsigned int) ((yrange - (convsample + converror) + yminval)
					/ yscale + 10));
      plot_addlinesegment(graph, horiz + 2, 
			  (unsigned int) ((yrange - (convsample + converror) + yminval) 
					  / yscale + 10));
      plot_strokeline(graph);
      plot_endline(graph);
      
      // Bottom horizontal line
      plot_setlinestart(graph, horiz - 2, 
			(unsigned int) ((yrange - (convsample - converror) + yminval)
					/ yscale + 10));
      plot_addlinesegment(graph, horiz + 2, 
			  (unsigned int) ((yrange - (convsample - converror) + yminval) 
					  / yscale + 10));
      plot_strokeline(graph);
      plot_endline(graph);
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

  // The horizontal axis
  plot_setlinestart(graph, 10, 
		    (unsigned int) ((yrange - 0 + yminval) / yscale + 10));
  plot_addlinesegment(graph, m_width - 10, 
		      (unsigned int) ((yrange - 0 + yminval) / yscale + 10));
  plot_strokeline(graph);
  plot_endline(graph);
  
  // The scale also belongs over the errors, but below the graph line
  plot_setfontcolor(graph, 26, 22, 249);
  plot_setfont(graph, "n022003l.pfb", 9);
  plot_settextlocation(graph, 20,
		       (unsigned int) ((yrange - 0 + yminval) / yscale + 10));
  plot_writestring(graph, (char *) m_yTitle.c_str());  

  // Now draw the actual graph
  plot_setlinecolor(graph, m_lineColor.red, m_lineColor.green,
		    m_lineColor.blue);
  for(int i = 0; i < m_ds->getNumRows(); i++){
    long convdate = (long) (m_ds->getValue(i, cepDataset::colDate) * 10000);
    long convsample = (long) (m_ds->getValue(i, cepDataset::colSample) * 10000);
    unsigned int xpoint = (unsigned int) ((convdate - m_xminval) / xscale + 10);
    unsigned int ypoint = (unsigned int) ((yrange - convsample + yminval) / yscale + 10);
    
    plot_setlinestart(graph, xpoint - 1, ypoint);
    plot_addlinesegment(graph, xpoint, ypoint - 1);
    plot_addlinesegment(graph, xpoint + 1, ypoint);
    plot_addlinesegment(graph, xpoint, ypoint + 1);
    plot_closeline(graph);
    plot_strokeline(graph);
    plot_endline(graph);
  }

  // Get the raster (in case we use it later)
  m_raster = plot_getraster (graph);
  return cepError ();
#else
  return cepError ("Libmplot was not installed at configuration time",
		   cepError::sevErrorFatal);
#endif
}

cepError
cepPresentation::createPNG (const string & filename, float& scale, long& range)
{
#if defined HAVE_LIBMPLOT
#if defined HAVE_LIBPNG
  cepDebugPrint ("Generating PNG for: " + filename);

  cepError e = createBitmap(scale, range);
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
  return cepError ("Libmplot was missing at configure time",
		   cepError::sevErrorFatal);
#endif
}

void cepPresentation::useErrors(bool yesno)
{
  m_useErrors = yesno;
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

