/* 
   Imp for the CEP data presentation
   Copyright (C) Michael Still                    2002
   
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

#include "cepCore.h"
#include <libplot.h>
#include <png.h>
#include <panda/constants.h>
#include <panda/functions.h>

// This number needs to be positive, and preferably greater than 2...
// Having big numbers is more efficient, but wastes more RAM
const long CHUNKALLOC = 10;

// todo_mikal: -64000 means an invalid value. I think I need to think about
// this a little bit harder at some point...
const long INVALID = -64000;

cepPresentation::cepPresentation (long width, long height):
  m_data(1, long(INVALID))
{
  m_width = width;
  m_height = height;
  m_xTitle = "Undefined Axis Title";
  m_yTitle = "Undefined Axis Title";
  m_xUnit = -1;
  m_yUnit = -1;
  m_raster = NULL;

  m_xminval = 32000;
  m_xmaxval = -32000;
  m_yminval = 32000;
  m_ymaxval = -32000;

  m_data.resize(10);
}

void
cepPresentation::xAxisTitle (const string& title)
{
  m_xTitle = title;
}

void
cepPresentation::yAxisTitle (const string& title)
{
  m_yTitle = title;
}

void
cepPresentation::xAxisScale (int units)
{
  m_xUnit = units;
}

void
cepPresentation::yAxisScale (int units)
{
  m_yUnit = units;
}

cepError
cepPresentation::addDataPoint (long x, long y)
{
  cepDebugPrint("Current data vector size is: " + cepItoa(m_data.size()));
  cepDebugPrint ("Added (" + cepLtoa (x) + ", " + cepLtoa (y) + ")");

  // Values
  if (x >= m_data.size ())
    {
      try{
	m_data.resize (x + CHUNKALLOC);
	cepDebugPrint ("Resize presentation data to " + 
		       cepLtoa (x + CHUNKALLOC));
      }
      catch(...){
	return cepError("Could not add data point because of memory allocation error", cepError::sevErrorFatal);
      }
    }

  m_data[x] = y;

  if(x > m_xmaxval) m_xmaxval = x;
  if(x < m_xminval) m_xminval = x;

  if(x > m_ymaxval) m_ymaxval = y;
  if(x < m_yminval) m_yminval = y;

  return cepError();
}

cepError cepPresentation::createPDF (const string& filename)
{
  return cepError("TODO: This feature is not currently implemented. Please come again.", cepError::sevWarning);

  cepDebugPrint("Generating PDF for: " + filename);
  createBitmap();
  
  //  panda_init();
  //  panda_pdf doc = panda_open(filename.c_str(), "w");
  //  panda_page page = panda_newpage(doc, panda_pagesize_a4);
  //  panda_close(doc);

  return cepError ();
}

cepError cepPresentation::createBitmap ()
{
  plot_state *graph;

  cepDebugPrint("Generating bitmap. Dimensions: " + cepItoa(m_width) + 
		" by " + cepItoa(m_height) + ". X value range: " +
		cepItoa(m_xminval) + " to " + cepItoa(m_xmaxval) + 
		". Y value range: " + cepItoa(m_yminval) + " to " +
		cepItoa(m_ymaxval) + ".");

  if((graph = plot_newplot(m_width, m_height)) == NULL){
    fprintf(stderr, "Could not allocate a new plot\n");
    exit(1);
  }

  // Draw some axes
  plot_setlinecolor(graph, 0, 0, 0);
  plot_setlinestart(graph, 10, 10);
  plot_addlinesegment(graph, 10, 190);
  plot_strokeline(graph);
  plot_endline(graph);

  plot_setlinestart(graph, 10, 100);
  plot_addlinesegment(graph, 390, 100);
  plot_strokeline(graph);
  plot_endline(graph);

  // Draw data points
  bool linestarted(false);
  for(int i = 0; i < m_data.size(); i++){
    if(m_data[i] != INVALID){
      if(!linestarted){
	plot_setlinestart(graph, i, m_data[i]);
	linestarted = true;
      }
      else
	plot_addlinesegment(graph, i, m_data[i]);
    }
  }

  if(!linestarted)
    return cepError("No datapoints defined", cepError::sevErrorRecoverable);
  
  plot_strokeline(graph);
  plot_endline(graph);
  m_raster = plot_getraster(graph);
  return cepError();
}

cepError cepPresentation::createPNG (const string& filename)
{
  cepDebugPrint("Generating PNG for: " + filename);
  createBitmap();
  
  // Write out the PNG file
  FILE *image;
  unsigned int i;
  png_uint_32 rowbytes;
  png_structp png;
  png_infop info;
  png_bytepp row_pointers = NULL;
  plot_state *graph;
  float angle;
  int x;

  if((image = fopen(filename.c_str(), "wb")) == NULL){
    return cepError("Could not open the file \"" + filename + 
		    "\" for writing"); 
  }

  // Get ready for writing
  if ((png =
       png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL,
                                NULL)) == NULL){
    return cepError("Could not create write structure for PNG (possibly out of memory) while generating a PNG version of a graph");
  }

  // Get ready to specify important stuff about the image
  if ((info = png_create_info_struct (png)) == NULL){
    return cepError("Could not create PNG info structure for writing (possibly out of memory) while generating a PNG version of a graph");
  }

  if (setjmp (png_jmpbuf (png))){
    return cepError("Could not set the PNG jump value for writing while generating a PNG version of a graph");
  }

  // This is needed before IO will work (unless you define callbacks)
  png_init_io(png, image);

  // Define important stuff about the image
  info->channels = 3;
  info->pixel_depth = 24;
  png_set_IHDR (png, info, m_width, m_height, 8, PNG_COLOR_TYPE_RGB,
                PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                PNG_FILTER_TYPE_DEFAULT);
  png_write_info (png, info);

  // Write the image out
  if((row_pointers = (png_byte **) 
      malloc (m_height * sizeof (png_bytep))) == NULL){
    return cepError("Memory allocation error generating row pointers for PNG version of a graph");
  }

  for(i = 0; i < m_height; i++){
    row_pointers[i] = (png_byte *) (m_raster + (i * m_width * 3));
  }

  png_write_image (png, row_pointers);

  // Cleanup
  png_write_end (png, info);
  png_destroy_write_struct (&png, &info);
  fclose(image); 

  return cepError();
}
