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

pdfRender::pdfRender (pdf & thePDF, object page, object pages, int pageno):
m_page (page),
m_pages (pages),
m_mode (rmGraphics),
m_invalid (true),
m_plot (NULL),
m_pdf (thePDF),
m_hasLine (false),
m_pageno (pageno)
{
  if (!m_page.hasDictItem (dictitem::diTypeObjectReference, "Contents"))
    {
      debug(dlTrace, "Contents is not an object reference");
    }
  if (!m_page.getDict ().getValue ("Contents", m_pdf, m_contents))
    {
      debug(dlError,
	    "Bad PDF: Could not get contents object, but the page references it!");
    }

  m_invalid = false;
}

// Not yet implemented (private)
pdfRender::pdfRender (const pdfRender & other):
m_page (objNumNoSuch, objNumNoSuch),
m_pages (other.m_pages),
m_pdf (""),
m_hasLine (false),
m_pageno (-1)
{
}

bool
pdfRender::render ()
{
  // Find the size of the page and setup a plot
  // todo_mikal: fix this
  debug(dlTrace, "Commence render");

  if (m_plot != NULL)
    debug(dlTrace, "Memory leak as render called more than once");

  string mediaBox;
  m_page.getDict ().getValue ("MediaBox", mediaBox);
  if(mediaBox == ""){
    debug(dlTrace, "No page size specified in page object, trying pages object...");
    m_pages.getDict ().getValue ("MediaBox", mediaBox);
  }
  if(mediaBox == ""){
    debug(dlError, "The document does not specify a page size");
    return false;
  }

  stringArray boxArgs (mediaBox.substr (1, mediaBox.length () - 2), " ");
  m_width = atoi (boxArgs[2].c_str ());
  m_height = atoi (boxArgs[3].c_str ());
  debug(dlTrace, string("Page size is ") + toString(m_width) + string(" by ") +
	toString(m_height));
  if((m_width == 0) || (m_height == 0)){
    debug(dlError, "Invalid page size");
    return false;
  }

  if ((m_plot = plot_newplot (m_width, m_height)) == NULL)
    {
      debug(dlError, "Failed to initialize new page plot");
      return false;
    }

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

  stream = ((object) contents).getStream (needStreamClean, length);
  debug(dlTrace, string("Process page stream of length ") + 
	toString((long) length));
  if((stream == NULL) || (length == 0)){
    debug(dlError, "Invalid page description stream");
    return false;
  }

  // todo_mikal: this might be too slow because of the accessor
  string line;
  inset = 0;
  m_commandString = "";
  while (inset < length)
    {
      if ((stream[inset] != '\n') && (stream[inset] != '\r'))
	{
	  line += stream[inset];
	}
      else
	{
	  // Process the line
	  debug(dlTrace, string("Process line \"") + line + string("\""));
	  processLine (line);
	  line = "";
	}

      inset++;
    }

  if (needStreamClean)
    delete[]stream;
  return true;
}

void
pdfRender::processLine (string line)
{
  if (line.length () < 1)
    {
      debug(dlTrace, "Empty line");
      return;
    }

  char *state = plot_persiststate(m_plot);
  debug(dlTrace, string("Plot state: ") + string(state));
  free(state);

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

string
pdfRender::getPNGfile ()
{
  if (m_invalid)
    {
      debug(dlTrace, "Page render is invalid");
      return "";
    }

  FILE *image;
  unsigned int i;
  png_uint_32 rowbytes;
  png_structp png;
  png_infop info;
  png_bytepp row_pointers = NULL;
  char *raster;

  raster = plot_getraster (m_plot);

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
render::appendCommand(string commandString)
{
  debug(dlTrace, "Implement append command");
}
