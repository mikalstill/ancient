#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <tiffio.h>

#include "render.h"
#include "utility.h"
#include "stringArray.h"
#include "configuration.h"
#include "raster.h"
#include "verbosity.h"
#include "idmangle.h"

#define SELECTWIDTH 10

pdfRender::pdfRender (pdfDoc *theDoc, int pageno):
m_doc(theDoc),
m_mode (rmGraphics),
m_plot (NULL),
m_select (NULL),
m_hasLine (false),
m_pageno (pageno)
{
}

bool
pdfRender::render ()
{
  if(m_plot == NULL)
    {
      // Find the size of the page and setup a plot
      debug(dlTrace, "Commence render");
      if(!m_doc->getPageSize(m_pageno, m_width, m_height))
	{
	  debug(dlError, "Could not determine target page size");
	  return false;
	}

#if defined HAVE_LIBMPLOT
      if ((m_plot = plot_newplot (m_width, m_height)) == NULL)
	{
	  debug(dlError, "Failed to initialize new page plot");
	  return false;
	}

      if ((m_select = plot_newplot (m_width, m_height)) == NULL)
	{
	  debug(dlError, "Failed to initialize new page plot");
	  return false;
	}
      plot_setlinewidth(m_select, SELECTWIDTH, SELECTWIDTH);
#else
      debug(dlError, "Libmplot not found at configure time. Graphics "
	    "functionality is therefore not available");
#endif
    }

  // If we don't have any preparsed commands, then we need to hit the document
  // and find the drawing commands. This puts them into the page command array
  if(m_doc->getPage(m_pageno).getCommandCount() == 0)
    {
      // Stopped checking the error return here, as no drawing commands now
      // results in just an empty PNG file
      parseStream();
    }

  // Now we need to process each of the elements in the command array
  debug(dlTrace, "Commence rendering the preprocessed commands");
  for(int cmdcnt = 0; cmdcnt < m_doc->getPage(m_pageno).getCommandCount();
      cmdcnt++)
    {
      if(m_select)
	{
	  int cmdid = m_doc->getPage(m_pageno).getCommandId(cmdcnt);
	  unsigned char c1, c2, c3;
	  mangle(cmdid, c1, c2, c3);

	  plot_setlinecolor(m_select, c1, c2, c3);
	  plot_setfillcolor(m_select, c1, c2, c3);
	}
      else
	{
	  debug(dlError, "Object selection will fail due to lack of select "
		"raster");
	}

      // Paint the object
      object::commandType type;
      vector<wxPoint> controlPoints = m_doc->getPage(m_pageno).
	getCommand(cmdcnt, type);
      if(controlPoints.size() > 0)
	{
	  switch(type)
	    {
	    case object::cLine:
	      if(controlPoints.size() > 1)
		{
		  plot_setlinestart(m_plot, controlPoints[0].x,
				    controlPoints[0].y);
		  plot_setlinestart(m_select, controlPoints[0].x,
				    controlPoints[0].y);
		  
		  for(unsigned int i = 1; i < controlPoints.size(); i++)
		    {
		      plot_addlinesegment(m_plot, controlPoints[i].x,
					  controlPoints[i].y);
		      plot_addlinesegment(m_select, controlPoints[i].x,
					  controlPoints[i].y);
		    }
		  plot_strokeline(m_plot);
		  plot_strokeline(m_select);
		  plot_endline(m_plot);
		  plot_endline(m_select);
		}
	      break;

	    default:
	      debug(dlError, "Unknown drawing command");
	    }
	}
    }
  return true;
}

bool
pdfRender::parseStream ()
{
  // Find all the contents objects
  m_doc->getPage(m_pageno).getDict().getValue("Contents", *(m_doc->getPDF()),
					      m_contents);

  debug(dlTrace, string("Commence stream parsing for a ") + 
	toString(m_contents.size()) + string(" object stream"));
  for(unsigned int i = 0; i < m_contents.size(); i++){
    if(!processContentsObject(m_contents[i]))
      {
	debug(dlError, "Processing of a command stream failed");
	return false;
      }
  }

  return true;
}

bool
pdfRender::processContentsObject(const object& contents){
  debug(dlTrace, "Process a contents object");

  // Read the stream and action any commands
  char *stream;
  unsigned long inset, length;

  // This used to exit if there was an empty page stream. It now just renders
  // nothing... This might mean that a stream which couldn't be decompressed
  // is ignored entirely...
  stream = ((object) contents).getStream (length);
  debug(dlTrace, string("Page stream is: ") +
   	binaryToString((void *) stream, length));
  if(length != 0){
    processCommandString(stream, length);
  }

  delete[]stream;
  return true;
}

void
pdfRender::processCommandString(char *commandString, unsigned int length)
{
  // todo_mikal: this might be too slow because of the accessor
  string line;
  unsigned int inset = 0;

  m_commandString = "";
  m_controlString = "";

  while (inset < length)
    {
      if ((commandString[inset] != '\n') && (commandString[inset] != '\r'))
	{
	  line += commandString[inset];
	}
      else
	{
	  // Process the line
	  debug(dlTrace, string("Process line \"") + line + 
		string("\""));
	  processLine (line);
	  line = "";
	}

      inset++;
    }

  // Process the leftover line (if there is one)
  if(line != "")
    {
      debug(dlTrace, string("Process leftover line \"") + line + 
	    string("\""));
      processLine (line);
      line = "";
    }

  // We might have a left over fragment of a command block as well
  if(m_commandString != "")
    {
      debug(dlTrace, "Appending command fragment");
      //      appendCommand();
      m_commandString = "";
      m_controlString = "";
    }
}

void
pdfRender::processLine (string line)
{
  if (line.length () < 1)
    {
      debug(dlTrace, "Empty line");
      return;
    }

  if(m_plot == NULL)
    {
      debug(dlError, "Not plot for this page defined");
      return;
    }

  debug(dlTrace, "Processing a page description line");

  // TODO mikal: Do we need this any more?
#if defined HAVE_LIBMPLOT
  char *state = plot_persiststate(m_plot);
  debug(dlTrace, string("Plot state: ") + string(state));
  free(state);
#else
  debug(dlError, "Libmplot not found at configure time. Graphics functionality"
	" is therefore not available");
#endif

  stringArray tokens (line, " ");
  debug(dlTrace, string("Line token count: ") + toString(tokens.size()));
  string sarg;
  bool sargMode (false);
  for (unsigned int i = 0; i < tokens.size (); i++)
    {
      debug(dlTrace, string("Process token: ") + tokens[i]);
      if (tokens[i] == "")
	{
	  debug(dlTrace, "Empty command token");
	}
      else if (tokens[i][0] == '(')
	{
	  sarg = tokens[i].substr (1, tokens[i].length () - 1);
	  sargMode = true;
	  debug(dlTrace, "String argument started");
	}
      else if (tokens[i][tokens[i].length () - 1] == ')')
	{
	  sarg += " " + tokens[i].substr (0, tokens[i].length () - 1);
	  sargMode = false;
	  debug(dlTrace, string("String arguement finished (") + sarg +
		string(")"));
	  pushArguement (sarg);
	}
      else if (sargMode)
	sarg += " " + tokens[i];

      else if (isNumber (tokens[i]))
	pushArguement (tokens[i]);
      else if (isName (tokens[i]))
	pushArguement (tokens[i]);

      else if ("b" == tokens[i])
	command_b ();
      else if ("b*" == tokens[i])
	command_bstar ();
      else if ("B" == tokens[i])
	command_B ();
      else if ("B*" == tokens[i])
	command_Bstar ();
      else if ("BT" == tokens[i])
	command_BT ();

      else if ("c" == tokens[i])
	command_c ();
      else if ("cm" == tokens[i])
	command_cm ();

      else if ("Do" == tokens[i])
	command_Do ();

      else if ("ET" == tokens[i])
	command_ET ();

      else if ("f" == tokens[i])
	command_f ();
      else if ("f*" == tokens[i])
	command_fstar ();
      else if ("F" == tokens[i])
	command_F ();

      else if ("g" == tokens[i])
	command_g ();
      else if ("G" == tokens[i])
	command_G ();

      else if ("h" == tokens[i])
	command_h ();

      else if ("l" == tokens[i])
	command_l ();

      else if ("m" == tokens[i])
	command_m ();

      else if ("q" == tokens[i])
	command_q ();
      else if ("Q" == tokens[i])
	command_Q ();

      else if ("re" == tokens[i])
	command_re ();
      else if ("rg" == tokens[i])
	command_rg ();
      else if ("RG" == tokens[i])
	command_RG ();

      else if ("S" == tokens[i])
	command_S ();

      else if ("Td" == tokens[i])
	command_Td ();
      else if ("TD" == tokens[i])
	command_TD ();
      else if ("Tf" == tokens[i])
	command_Tf ();
      else if ("Tj" == tokens[i])
	command_Tj ();
      else if ("TL" == tokens[i])
	command_TL ();
      else if ("Tm" == tokens[i])
	command_Tm ();
      else if ("Tr" == tokens[i])
	command_Tr ();

      else if ("v" == tokens[i])
	command_v ();

      else if ("w" == tokens[i])
	command_w ();

      else if ("y" == tokens[i])
	command_y ();

      else
	debug(dlInformational, string("Dropped token ") + tokens[i]);
    }
  if (sargMode)
    {
      debug(dlTrace, string("String mode never ended on line. Line so far: ") +
	      sarg);
    }
}

/////////////////////////////////////////////////////////////////////
void
pdfRender::pushArguement (string arg)
{
  debug(dlTrace, string("Pushing arguement ") + arg);
  m_arguements.push (arg);
}

char *
pdfRender::getSelectRaster()
{
  return plot_getraster(m_select);
}

string
pdfRender::getPNGfile ()
{
  FILE *image;
  unsigned int i;
  png_uint_32 rowbytes;
  png_structp png;
  png_infop info;
  png_bytepp row_pointers = NULL;
  char *raster;

  bool selectDebugOn;
  configuration *config;
  config = (configuration *) & configuration::getInstance ();
  config->getValue ("pref-selectdebug", true, selectDebugOn);

#if defined HAVE_LIBMPLOT
  if(!selectDebugOn)
    raster = plot_getraster (m_plot);
  else
    raster = plot_getraster (m_select);
#else
  debug(dlError, "Libmplot not found at configure time. Graphics functionality"
	" is therefore not available");
  raster = NULL;
#endif

  // Build a random filename
  debug(dlTrace, "Generating a temporary filename");
  char cfname[100];
  snprintf (cfname, 100, "/tmp/pandaedit.p%03d.XXXXXX", m_pageno);
  int fd;
  fd = mkstemp (cfname);
  close (fd);
  string tempName (cfname);
  debug(dlTrace, "Finished generating temporary filename");

  if ((image = fopen (tempName.c_str (), "wb")) == NULL)
    {
      debug(dlError, "Could not open the output image");
      return "";
    }

  // Get ready for writing
  if ((png =
       png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL,
				NULL)) == NULL)
    {
      debug(dlError,
	       "Could not create write structure for PNG (out of memory?)");
      return "";
    }

  // Get ready to specify important stuff about the image
  if ((info = png_create_info_struct (png)) == NULL)
    {
      debug(dlError,
	       "Could not create PNG info structure for writing (out of memory?)");
      return "";
    }

  if (setjmp (png_jmpbuf (png)))
    {
      debug(dlError, "Could not set the PNG jump value for writing");
      return "";
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
  if ((row_pointers =
       (png_byte **) malloc (m_height * sizeof (png_bytep))) == NULL)
    {
      debug(dlError, "Could not allocate memory");
      exit (42);
    }

  for (i = 0; i < m_height; i++)
    {
      row_pointers[i] = (png_byte *) (raster + (i * m_width * 3));
    }

  png_write_image (png, row_pointers);

  // Cleanup
  png_write_end (png, info);
  png_destroy_write_struct (&png, &info);
  fclose (image);

  return tempName;
}

void
pdfRender::appendCommand(object::commandType type)
{
  m_doc->getPage(m_pageno).appendCommand(type, m_controlPoints);
  m_controlPoints.clear();
}
