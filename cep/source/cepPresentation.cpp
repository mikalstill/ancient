
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
#include "cepDate.h"
#include "cepConfiguration.h"

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

cepPresentation::cepPresentation (long width, long height, cepMatrix<double> *ds, double b1, double b2,
				  bool haveLs):
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
  m_useGrid(true),
  m_raster(NULL),
  m_ds(ds),
  m_haveMaxima(false),
  m_b1(b1),
  m_b2(b2),
  m_haveLs(haveLs)
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

  m_fontColor.red = 0;
  m_fontColor.green = 0;
  m_fontColor.blue = 0;

  m_gridColor.red = 0;
  m_gridColor.green = 0;
  m_gridColor.blue = 0;

  cepDebugPrint("Presentation has a LS line: " + cepToString(haveLs));
  m_config = (cepConfiguration *)&cepConfiguration::getInstance();
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

cepError
cepPresentation::createPDF (const string & filename)
{
  return
    cepError
    ("TODO: This feature is not currently implemented. Please come again.",
     cepError::sevWarning);

#if defined HAVE_LIBPANDA
  cepDebugPrint ("Generating PDF for: " + filename);
  float dummyscale1, dummyscale2;
  long dummyrange1, dummyrange2;
  createBitmap (dummyscale1, dummyscale2, dummyrange1, dummyrange2);

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
cepPresentation::createBitmap (float& horizScale, float& vertScale, long& xminval, long& yminval)
{
#if defined HAVE_LIBMPLOT
  plot_state *graph;
  cepError err;

  if(!m_haveMaxima){
    m_xmaxval = (unsigned int) (m_ds->getMaxValue(cepDataset::colDate) * 10000);
    m_xminval = (unsigned int) (m_ds->getMinValue(cepDataset::colDate) * 10000);
    m_ymaxval = (unsigned int) (m_ds->getMaxValue(cepDataset::colSample) * 10000);
    m_yminval = (unsigned int) (m_ds->getMinValue(cepDataset::colSample) * 10000);
    m_emaxval = (unsigned int) (m_ds->getMaxValue(cepDataset::colError) * 10000);

    cepDebugPrint("Graph extents: x = [" + cepToString(m_xminval) + " - " + 
		  cepToString(m_xmaxval) + "] y = [" + cepToString(m_yminval) + " - " +
		  cepToString(m_ymaxval) + "] e = " + cepToString(m_emaxval));
    m_haveMaxima = true;
  }

  if ((graph = plot_newplot (m_width, m_height)) == NULL)
    return cepError("Could not initialise a new plot", 
		    cepError::sevErrorRecoverable);

  ////////////////////////////////////////////////////////////////////////////////
  // Draw a box around the graph
  cepDebugPrint("Boxing plot: " + cepToString(m_width) + " x " + cepToString(m_height));
  plot_setlinestart(graph, 0, 0);
  plot_addlinesegment(graph, 0, m_height - 1);
  plot_addlinesegment(graph, m_width - 2, m_height - 1);
  plot_addlinesegment(graph, m_width - 2, 0);
  plot_closeline(graph);
  plot_strokeline(graph);
  plot_endline(graph);

  ////////////////////////////////////////////////////////////////////////////////
  // How big are the maxima and minima?
  cepDebugPrint("Determine scaling");
  long ymaxval = m_useErrors ? m_ymaxval + m_emaxval : m_ymaxval;
  yminval = m_useErrors ? m_yminval - m_emaxval : m_yminval;

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

  // And this bug (one datapoint has a range of zero)
  if(xrange == 0.0){
    xrange = m_xminval;
  }

  const int graphInset = 20;
  horizScale = (float) yrange / (m_height - graphInset * 2);
  vertScale = (float) xrange / (m_width - graphInset * 2);
  xminval = m_xminval;
  
  cepDebugPrint("Dimensions of graph bitmap: " + cepToString(m_width) + " x " +
		cepToString(m_height));
  cepDebugPrint("HorizScale is: " + cepToString(horizScale));
  cepDebugPrint("VertScale is: " + cepToString(vertScale));

  ////////////////////////////////////////////////////////////////////////////////
  // The grid is always at the absolute bottom of the painting stack
  if(m_useGrid){
    plot_setlinecolor(graph, m_gridColor.red, m_gridColor.green, m_gridColor.blue);
    for(int i = graphInset; i < m_height - graphInset; i += (m_height - graphInset * 2) / 10){
      plot_setlinestart(graph, graphInset, i);
      plot_addlinesegment(graph, m_width - graphInset, i);
      plot_strokeline(graph);
      plot_endline(graph);
    }
  }

  ////////////////////////////////////////////////////////////////////////////////
  // If we are using errors, then we draw these underneath
  if(m_useErrors){
    cepDebugPrint("Displaying errors");
    plot_setlinecolor(graph, m_errorColor.red, m_errorColor.green,
		      m_errorColor.blue);
    for(int i = 0; i < m_ds->getNumRows(); i++){
      long convdate = (long) (m_ds->getValue(i, cepDataset::colDate) * 10000);
      if(m_ds->getError().isReal())
	return m_ds->getError();

      long convsample = (long) (m_ds->getValue(i, cepDataset::colSample) * 10000);
      if(m_ds->getError().isReal())
	return m_ds->getError();

      long converror = (long) (m_ds->getValue(i, cepDataset::colError) * 10000);
      if(m_ds->getError().isReal())
	return m_ds->getError();

      unsigned int horiz = (unsigned int) ((convdate - m_xminval) / vertScale + graphInset);

      // Vertical line
      plot_setlinestart(graph, horiz,
			(unsigned int) ((yrange - (convsample + converror) + 
					 yminval) / horizScale + graphInset));
      plot_addlinesegment(graph, horiz,
			  (unsigned int) ((yrange - (convsample - converror) + yminval) 
					  / horizScale + graphInset));
      plot_strokeline(graph);
      plot_endline(graph);
      
      // Top horizontal line
      plot_setlinestart(graph, horiz - 2, 
			(unsigned int) ((yrange - (convsample + converror) + yminval)
					/ horizScale + graphInset));
      plot_addlinesegment(graph, horiz + 2, 
			  (unsigned int) ((yrange - (convsample + converror) + yminval) 
					  / horizScale + graphInset));
      plot_strokeline(graph);
      plot_endline(graph);
      
      // Bottom horizontal line
      plot_setlinestart(graph, horiz - 2, 
			(unsigned int) ((yrange - (convsample - converror) + yminval)
					/ horizScale + graphInset));
      plot_addlinesegment(graph, horiz + 2, 
			  (unsigned int) ((yrange - (convsample - converror) + yminval) 
					  / horizScale + graphInset));
      plot_strokeline(graph);
      plot_endline(graph);
    }
   }

  // Draw the axes. We want the graph to be over this, but the error lines to
  // be underneath
  cepDebugPrint("Plotting axes");
  plot_setlinecolor(graph, m_axesColor.red, m_axesColor.green,
		    m_axesColor.blue);
  plot_setlinestart(graph, graphInset - 1, 0);
  plot_addlinesegment(graph, graphInset - 1, m_height);
  plot_strokeline(graph);
  plot_endline(graph);

  // The horizontal axis
  plot_setlinestart(graph, graphInset, 
		    (unsigned int) ((yrange - 0 + yminval) / horizScale + graphInset));
  plot_addlinesegment(graph, m_width - graphInset, 
		      (unsigned int) ((yrange - 0 + yminval) / horizScale + graphInset));
  plot_strokeline(graph);
  plot_endline(graph);
  
  ////////////////////////////////////////////////////////////////////////////////
  // The scale also belongs over the errors, but below the graph line
  plot_setfontcolor(graph, m_fontColor.red, m_fontColor.green, m_fontColor.blue);
  string fontfile;
  err = m_config->getValue("ui-graph-font", "n022003l.pfb", fontfile);
  if(err.isReal()){
    err.display();
    fontfile = "n022003l.pfb";
  }
  plot_setfont(graph, (char *) fontfile.c_str(), 10);

  // Minimum value horizontal
  const int textHeight = 5;
  plot_setlinecolor(graph, m_fontColor.red, m_fontColor.green, m_fontColor.blue);
  plot_setlinestart(graph, graphInset, m_height - textHeight - 12);
  plot_addlinesegment(graph, graphInset, m_height - textHeight - 9);
  plot_strokeline(graph);
  plot_endline(graph);

  cepDate startDate((float) m_xminval / 10000);
  plot_settextlocation(graph, graphInset, m_height - textHeight);
  plot_writestring(graph, (char *) startDate.getShortDate().c_str());  
  
  // Midpoint value horizontal
  plot_setlinestart(graph, m_width / 2, m_height - textHeight - 12);
  plot_addlinesegment(graph, m_width / 2, m_height - textHeight - 9);
  plot_strokeline(graph);
  plot_endline(graph);

  cepDate midDate((float) ((m_xmaxval - m_xminval) / 2 + m_xminval) / 10000);
  plot_settextlocation(graph, 
		       (m_width / 2) - 
		       (plot_stringwidth(graph, (char *) midDate.getShortDate().c_str())/ 2), 
		       m_height - textHeight);
  plot_writestring(graph, (char *) midDate.getShortDate().c_str());

  // Maximum value horizontal
  plot_setlinestart(graph, m_width - graphInset, m_height - textHeight - 12);
  plot_addlinesegment(graph, m_width - graphInset, m_height - textHeight - 9);
  plot_strokeline(graph);
  plot_endline(graph);

  cepDate endDate((float) m_xmaxval / 10000);
  plot_settextlocation(graph, m_width - 
		       plot_stringwidth(graph, (char *) endDate.getShortDate().c_str()) - graphInset, 
		       m_height - textHeight);
  plot_writestring(graph, (char *) endDate.getShortDate().c_str());

  /////////////////////////
  // Minimum value vertical
  plot_setlinestart(graph, textHeight + 10, m_height - graphInset);
  plot_addlinesegment(graph, textHeight + 13, m_height - graphInset);
  plot_strokeline(graph);
  plot_endline(graph);
  
  plot_settextlocation(graph, textHeight + 10, m_height - graphInset);
  plot_writestringrot(graph, (char *) cepToString((float) m_yminval / 10000, true).c_str(), 90);

  // Maximum value vertical
  plot_setlinestart(graph, textHeight + 10, graphInset);
  plot_addlinesegment(graph, textHeight + 13, graphInset);
  plot_strokeline(graph);
  plot_endline(graph);

  plot_settextlocation(graph, textHeight + 10,
		       plot_stringheight(graph, (char *) 
					 cepToString((float) m_ymaxval / 10000, true).c_str()) + 
		       graphInset);
  plot_writestringrot(graph, (char *) cepToString((float) m_ymaxval / 10000, true).c_str(), 90);
  
  ////////////////////////////////////////////////////////////////////////////////
  // Now draw the actual graph
  cepDebugPrint("Plotting graph");
  plot_setlinecolor(graph, m_lineColor.red, m_lineColor.green,
		    m_lineColor.blue);
  for(int i = 0; i < m_ds->getNumRows(); i++){
    long convdate = (long) (m_ds->getValue(i, cepDataset::colDate) * 10000);
      if(m_ds->getError().isReal())
	return m_ds->getError();

    long convsample = (long) (m_ds->getValue(i, cepDataset::colSample) * 10000);
      if(m_ds->getError().isReal())
	return m_ds->getError();

    unsigned int xpoint = (unsigned int) ((convdate - m_xminval) / vertScale + graphInset);
    unsigned int ypoint = (unsigned int) ((yrange - convsample + yminval) / horizScale + graphInset);
    
    plot_setlinestart(graph, xpoint - 1, ypoint);
    plot_addlinesegment(graph, xpoint, ypoint - 1);
    plot_addlinesegment(graph, xpoint + 1, ypoint);
    plot_addlinesegment(graph, xpoint, ypoint + 1);
    plot_closeline(graph);
    plot_strokeline(graph);
    plot_endline(graph);
  }

  ////////////////////////////////////////////////////////////////////////////////
  // The least squares line (if any) is on top of _everything_ else
  if(m_haveLs){
    cepDebugPrint("Plotting LS line of best fit: " + cepToString(m_b2));
    plot_setlinestart(graph, graphInset, (unsigned int) m_b2 * 10000);
    plot_addlinesegment(graph, m_width, (unsigned int) m_b2 * 10000);
    plot_strokeline(graph);
    plot_endline(graph);
  }

  cepDebugPrint("Finishing plotting");

  ////////////////////////////////////////////////////////////////////////////////
  // Get the raster (in case we use it later)
  m_raster = plot_getraster (graph);
  return cepError ();
#else
  return cepError ("Libmplot was not installed at configuration time",
		   cepError::sevErrorFatal);
#endif
}

cepError
cepPresentation::createPNG (const string & filename, float& horizScale, float& vertScale, 
			    long& xrange, long& yrange)
{
#if defined HAVE_LIBMPLOT
#if defined HAVE_LIBPNG
  cepDebugPrint ("Generating PNG for: " + filename);

  cepError e = createBitmap(horizScale, vertScale, xrange, yrange);
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

void cepPresentation::useGrid(bool yesno)
{
  m_useGrid = yesno;
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

void cepPresentation::setFontColor(char red, char green, char blue)
{
  m_fontColor.red = red;
  m_fontColor.green = green;
  m_fontColor.blue = blue;
}

void cepPresentation::setGridColor(char red, char green, char blue)
{
  m_gridColor.red = red;
  m_gridColor.green = green;
  m_gridColor.blue = blue;
}

