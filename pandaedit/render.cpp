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

pdfRender::pdfRender (pdfDoc *theDoc, int pageno):
m_doc(theDoc),
m_mode (rmGraphics),
m_plot (NULL),
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
	return false;

#if defined HAVE_LIBMPLOT
      if ((m_plot = plot_newplot (m_width, m_height)) == NULL)
	{
	  debug(dlError, "Failed to initialize new page plot");
	  return false;
	}
#else
      debug(dlError, "Libmplot not found at configure time. Graphics "
	    "functionality is therefore not available");
#endif
    }

  if((m_doc->getPage(m_pageno).getCommandCount() == 0) && !parseStream())
    {
      return false;
    }

  processCommandString(m_doc->getPage(m_pageno).getCommandStream(), false);
  return true;
}

bool
pdfRender::parseStream ()
{
  for(unsigned int i = 0; i < m_contents.size(); i++){
    if(!processContentsObject(m_contents[i]))
      return false;
  }
}

bool
pdfRender::processContentsObject(const object& contents){
  // Read the stream and action any commands
  char *stream;
  unsigned long inset, length;
  bool needStreamClean (false);

  // This used to exit if there was an empty page stream. It now just renders
  // nothing... This might mean that a stream which couldn't be decompressed
  // is ignored entirely...
  stream = ((object) contents).getStream (needStreamClean, length);
  debug(dlTrace, string("Process page stream of length ") + 
	toString((long) length));
  if((stream == NULL) || (length == 0)){
    debug(dlTrace, "Empty page description stream");
    return true;
  }

  processCommandString(stream, true);

  if (needStreamClean)
    delete[]stream;
  return true;
}

void
pdfRender::processCommandString(string commandString, bool parsing)
{
  // todo_mikal: this might be too slow because of the accessor
  string line;
  unsigned int inset = 0;
  m_commandString = "";
  while (inset < commandString.length())
    {
      if ((commandString[inset] != '\n') && (commandString[inset] != '\r'))
	{
	  line += commandString[inset];
	}
      else
	{
	  // Process the line
	  debug(dlTrace, string("Process line \"") + line + 
		string("\" when ") + (parsing ? string("") : string("not ")) +
		string("parsing"));
	  processLine (line, parsing);
	  line = "";
	}

      inset++;
    }
}

void
pdfRender::processLine (string line, bool parsing)
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

#if defined HAVE_LIBMPLOT
  char *state = plot_persiststate(m_plot);
  debug(dlTrace, string("Plot state: ") + string(state));
  free(state);
#else
  debug(dlError, "Libmplot not found at configure time. Graphics functionality"
	" is therefore not available");
#endif

  stringArray tokens (line, " ");
  string sarg;
  bool sargMode (false);
  for (unsigned int i = 0; i < tokens.size (); i++)
    {
      if (tokens[i] == "")
	{
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
	parsing ? command_b () : render_b();
      else if ("b*" == tokens[i])
	parsing ? command_bstar () : render_bstar();
      else if ("B" == tokens[i])
	parsing ? command_B () : render_B();
      else if ("B*" == tokens[i])
	parsing ? command_Bstar () : render_Bstar();
      else if ("BT" == tokens[i])
	parsing ? command_BT () : render_BT();

      else if ("c" == tokens[i])
	parsing ? command_c () : render_c();
      else if ("cm" == tokens[i])
	parsing ? command_cm () : render_cm();

      else if ("Do" == tokens[i])
	parsing ? command_Do () : render_Do();

      else if ("ET" == tokens[i])
	parsing ? command_ET () : render_ET();

      else if ("f" == tokens[i])
	parsing ? command_f () : render_f();
      else if ("f*" == tokens[i])
	parsing ? command_fstar () : render_fstar();
      else if ("F" == tokens[i])
	parsing ? command_F () : render_F();

      else if ("g" == tokens[i])
	parsing ? command_g () : render_g();
      else if ("G" == tokens[i])
	parsing ? command_G () : render_G();

      else if ("h" == tokens[i])
	parsing ? command_h () : render_h();

      else if ("l" == tokens[i])
	parsing ? command_l () : render_l();

      else if ("m" == tokens[i])
	parsing ? command_m () : render_m();

      else if ("q" == tokens[i])
	parsing ? command_q () : render_q();
      else if ("Q" == tokens[i])
	parsing ? command_Q () : render_Q();

      else if ("re" == tokens[i])
	parsing ? command_re () : render_re();
      else if ("rg" == tokens[i])
	parsing ? command_rg () : render_rg();
      else if ("RG" == tokens[i])
	parsing ? command_RG () : render_RG();

      else if ("S" == tokens[i])
	parsing ? command_S () : render_S();

      else if ("Td" == tokens[i])
	parsing ? command_Td () : render_Td();
      else if ("TD" == tokens[i])
	parsing ? command_TD () : render_TD();
      else if ("Tf" == tokens[i])
	parsing ? command_Tf () : render_Tf();
      else if ("Tj" == tokens[i])
	parsing ? command_Tj () : render_Tj();
      else if ("TL" == tokens[i])
	parsing ? command_TL () : render_TL();
      else if ("Tm" == tokens[i])
	parsing ? command_Tm () : render_Tm();
      else if ("Tr" == tokens[i])
	parsing ? command_Tr () : render_Tr();

      else if ("v" == tokens[i])
	parsing ? command_v () : render_v();

      else if ("w" == tokens[i])
	parsing ? command_w () : render_w();

      else if ("y" == tokens[i])
	parsing ? command_y () : render_y();

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

#if defined HAVE_LIBMPLOT
  raster = plot_getraster (m_plot);
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
pdfRender::appendCommand(string commandString)
{
  m_doc->getPage(m_pageno).appendCommand(commandString);
}
