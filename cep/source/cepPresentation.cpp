
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

cepPresentation::cepPresentation (long width, long height, cepMatrix<double> *ds, string offset):
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
  m_haveLs(false),
  m_offset(offset),
  m_freqDomain(false)
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
  long dummyminval1, dummyminval2;
  long dummyrange;
  createBitmap (dummyscale1, dummyscale2, dummyminval1, dummyminval2, dummyrange);

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
cepPresentation::createBitmap (float& horizScale, float& vertScale, long& xminval, long& yminval,
			       long& yrange)
{
#if defined HAVE_LIBMPLOT
  plot_state *graph;
  cepError err;

  if ((graph = plot_newplot (m_width, m_height)) == NULL)
    return cepError("Could not initialise a new plot", 
		    cepError::sevErrorRecoverable);

  plot_setfontcolor(graph, m_fontColor.red, m_fontColor.green, m_fontColor.blue);
  string fontfile;
  err = m_config->getValue("ui-graph-font", "n022003l.pfb", fontfile);
  if(err.isReal()){
    err.display();
    fontfile = "n022003l.pfb";
  }
  plot_setfont(graph, (char *) fontfile.c_str(), 10);

  if(!m_haveMaxima){
    m_xmaxval = (unsigned int) (m_ds->getMaxValue(cepDataset::colDate) * 10000);
    if(m_ds->getError().isReal()){
      m_ds->getError().display();
    }

    m_xminval = (unsigned int) (m_ds->getMinValue(cepDataset::colDate) * 10000);
    if(m_ds->getError().isReal()){
      m_ds->getError().display();
    }

    m_ymaxval = (unsigned int) (m_ds->getMaxValue(cepDataset::colSample) * 10000);
    if(m_ds->getError().isReal()){
      m_ds->getError().display();
    }

    m_yminval = (unsigned int) (m_ds->getMinValue(cepDataset::colSample) * 10000);
    if(m_ds->getError().isReal()){
      m_ds->getError().display();
    }

    m_emaxval = (unsigned int) (m_ds->getMaxValue(cepDataset::colError) * 10000);
    if(m_ds->getError().isReal()){
      m_ds->getError().display();
    }

    cepDebugPrint("Graph extents: x = [" + cepToString(m_xminval) + " - " + 
		  cepToString(m_xmaxval) + "] y = [" + cepToString(m_yminval) + " - " +
		  cepToString(m_ymaxval) + "] e = " + cepToString(m_emaxval));
    m_haveMaxima = true;
  }

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
  yrange = ymaxval - yminval;
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
    for(int i = 0; i < 10; i++){
      plot_setlinestart(graph, graphInset, graphInset + (m_height - graphInset * 2) * i / 9);
      plot_addlinesegment(graph, m_width - graphInset, graphInset + (m_height - graphInset * 2) * i / 9);
      plot_strokeline(graph);
      plot_endline(graph);
    }

    string gridSpacing = "Grid spacing is " + cepToString(((float) yrange) / 10 / 10000, true);
    plot_settextlocation(graph, graphInset * 2, graphInset);
    plot_writestring(graph, (char *) gridSpacing.c_str());  
  }

  ////////////////////////////////////////////////////////////////////////////////
  // If we are using errors, then we draw these underneath
  // You can't have error bars in the frequency domain
  if(m_useErrors && !m_freqDomain){
    cepDebugPrint("Displaying errors");
   
    plot_setlinecolor(graph, m_errorColor.red, m_errorColor.green,
		      m_errorColor.blue);
    for(int tno = 0; tno < m_ds->getNumTables(); tno++){
      if(m_ds->getError().isReal()){
	m_ds->getError().display();
	break;
      }

      for(int i = 0; i < m_ds->getNumRows(); i++){
	if(m_ds->getError().isReal()){
	  m_ds->getError().display();
	  break;
	}

	long convdate = (long) (m_ds->getValue(i, cepDataset::colDate, tno) * 10000);
	if(m_ds->getError().isReal()){
	  m_ds->getError().display();
	  break;
	}
	
	long convsample = (long) (m_ds->getValue(i, cepDataset::colSample, tno) * 10000);
	if(m_ds->getError().isReal()){
	  m_ds->getError().display();
	  break;
	}
	
	long converror = (long) (m_ds->getValue(i, cepDataset::colError, tno) * 10000);
	if(m_ds->getError().isReal()){
	  m_ds->getError().display();
	  break;
	}
	
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
  string ltext;

  // Minimum value horizontal
  const int textHeight = 5;
  plot_setlinecolor(graph, m_fontColor.red, m_fontColor.green, m_fontColor.blue);
  plot_setlinestart(graph, graphInset, m_height - textHeight - 12);
  plot_addlinesegment(graph, graphInset, m_height - textHeight - 9);
  plot_strokeline(graph);
  plot_endline(graph);

  if(m_freqDomain){
    ltext = cepToString((float) m_xminval / 10000);
  }
  else{
    cepDate startDate((float) m_xminval / 10000);
    ltext = startDate.getShortDate();
  }

  plot_settextlocation(graph, graphInset, m_height - textHeight);
  plot_writestring(graph, (char *) ltext.c_str());  
  
  // Midpoint value horizontal
  plot_setlinestart(graph, m_width / 2, m_height - textHeight - 12);
  plot_addlinesegment(graph, m_width / 2, m_height - textHeight - 9);
  plot_strokeline(graph);
  plot_endline(graph);

  if(m_freqDomain){
    ltext = cepToString((float) ((m_xmaxval - m_xminval) / 2 + m_xminval) / 10000);
  }
  else{
    cepDate midDate((float) ((m_xmaxval - m_xminval) / 2 + m_xminval) / 10000);
    ltext = midDate.getShortDate();
  }

  plot_settextlocation(graph, 
		       (m_width / 2) - 
		       (plot_stringwidth(graph, (char *) ltext.c_str())/ 2), 
		       m_height - textHeight);
  plot_writestring(graph, (char *) ltext.c_str());

  // Maximum value horizontal
  plot_setlinestart(graph, m_width - graphInset, m_height - textHeight - 12);
  plot_addlinesegment(graph, m_width - graphInset, m_height - textHeight - 9);
  plot_strokeline(graph);
  plot_endline(graph);

  if(m_freqDomain){
    ltext = cepToString((float) m_xmaxval / 10000);
  }
  else{
    cepDate endDate((float) m_xmaxval / 10000);
    ltext = endDate.getShortDate();
  }
  
  plot_settextlocation(graph, m_width - 
		       plot_stringwidth(graph, (char *) ltext.c_str()) - graphInset, 
		       m_height - textHeight);
  plot_writestring(graph, (char *) ltext.c_str());

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
  for(int tno = 0; tno < m_ds->getNumTables(); tno++){
    if(m_ds->getError().isReal()){
      m_ds->getError().display();
      break;
    }

    bool lineStarted = false;
    plot_setlinecolor(graph, m_lineColor.red, m_lineColor.green,
		      m_lineColor.blue);
    for(int i = 0; i < m_ds->getNumRows(); i++){
      if(m_ds->getError().isReal()){
	m_ds->getError().display();
	break;
      }
      
      long convdate = (long) (m_ds->getValue(i, cepDataset::colDate, tno) * 10000);
      if(m_ds->getError().isReal()){
	m_ds->getError().display();
	break;
      }
      
      long convsample = (long) (m_ds->getValue(i, cepDataset::colSample, tno) * 10000);
      if(m_ds->getError().isReal()){
	m_ds->getError().display();
	break;
      }
      
      unsigned int xpoint = (unsigned int) ((convdate - m_xminval) / vertScale + graphInset);
      unsigned int ypoint = (unsigned int) ((yrange - convsample + yminval) / horizScale + 
					    graphInset);

      if(m_freqDomain){
	if(!lineStarted){
	  plot_setlinestart(graph, xpoint, ypoint);
	  lineStarted = true;
	}
	else
	  plot_addlinesegment(graph, xpoint, ypoint);
      }
      else{
	plot_setlinestart(graph, xpoint - 1, ypoint);
	plot_addlinesegment(graph, xpoint, ypoint - 1);
	plot_addlinesegment(graph, xpoint + 1, ypoint);
	plot_addlinesegment(graph, xpoint, ypoint + 1);
	plot_closeline(graph);
	plot_strokeline(graph);
	plot_endline(graph);
      }
    }

    if(m_freqDomain && lineStarted){
      plot_strokeline(graph);
      plot_endline(graph);
    }
  }

  ////////////////////////////////////////////////////////////////////////////////
  // The least squares line (if any) is on top of _everything_ else
  // you also can't have one of these in the frequency domain...
  if(m_haveLs && !m_freqDomain){
    cepDebugPrint("Plotting LS line of best fit");

    // Write the equation onto the graph
    string lsEqn = "LS equation: y = " + cepToString(m_b1, true) + "x ";
    if(m_b2 > 0){
      lsEqn += "+ " + cepToString(m_b2, true);
    }
    else{
      lsEqn += "- " + cepToString(m_b2, true).substr(1, 30);
    }

    plot_settextlocation(graph, graphInset * 2, graphInset + graphInset);
    plot_writestring(graph, (char *) lsEqn.c_str());  
    
    // And draw the actual line
    cepDebugPrint("Last point: " + cepToString(xrange * m_b1));

    unsigned int xintercept = (unsigned int) ((yrange - (m_b2 * 10000) + yminval) / horizScale + graphInset);
    unsigned int lastpoint = (unsigned int) ((yrange - (xrange * m_b1) + yminval - (m_b2 * 10000)) / 
					     horizScale + graphInset);
    cepDebugPrint("Line of best fit: " + cepToString(xintercept) + ", " + cepToString(lastpoint) +
		  " (" + cepToString(xrange) + ")");
    cepDebugPrint("Args: " + cepToString(m_b1) + " + " + cepToString(m_b2) + " = " + cepToString(m_b1 + m_b2));

    plot_setlinestart(graph, graphInset, xintercept);
    plot_addlinesegment(graph, m_width - graphInset, lastpoint);
    plot_strokeline(graph);
    plot_endline(graph);
  }
  else if(m_freqDomain){
    string freqEnergy = "Energy = " + cepToString(m_e, true);
    plot_settextlocation(graph, graphInset * 2, graphInset + graphInset);
    plot_writestring(graph, (char *) freqEnergy.c_str()); 
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
			    long& xminval, long& yminval, long& yrange)
{
#if defined HAVE_LIBMPLOT
#if defined HAVE_LIBPNG
  cepDebugPrint ("Generating PNG for: " + filename);

  cepError e = createBitmap(horizScale, vertScale, xminval, yminval, yrange);
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

void cepPresentation::setLsParams(double b1, double b2){
  m_haveLs = true;
  m_freqDomain = false;

  m_b1 = b1;
  m_b2 = b2;
}

void cepPresentation::setFreqParams(float energy){
  m_haveLs = false;
  m_freqDomain = true;
  m_e = energy;
}
