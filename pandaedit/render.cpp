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

char *inflateraster(char *input, unsigned long width, unsigned long height, 
                    int bitdepth, int targetbitdepth, 
                    int channels, int targetchannels);

pdfRender::pdfRender (pdf & thePDF, object page, object pages, int pageno):
m_page (page),
m_pages (pages),
m_contents (-1, -1),
m_mode (rmGraphics),
m_invalid (true),
m_plot (NULL),
m_pdf (thePDF),
m_hasLine (false),
m_pageno (pageno)
{
  if (!m_page.hasDictItem (dictitem::diTypeObjectReference, "Contents"))
    {
      debug(dlError, "Bad PDF: Page is blank");
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
m_page (-1, -1),
m_pages (other.m_pages),
m_contents (-1, -1),
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

  // Read the stream and action any commands
  char *stream;
  unsigned long inset, length;
  bool needStreamClean (false);

  stream = m_contents.getStream (needStreamClean, length);
  debug(dlTrace, string("Process page stream of length ") + toString((long) length));
  if((stream == NULL) || (length == 0)){
    debug(dlError, "Invalid page description stream");
    return false;
  }

  // todo_mikal: this might be too slow because of the accessor
  string line;
  inset = 0;
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
      else if ("rg" == tokens[i])
	command_RG ();
      else if ("S" == tokens[i])
	command_S ();
      else if ("Td" == tokens[i])
	command_Td ();
      else if ("Tf" == tokens[i])
	command_Tf ();
      else if ("Tj" == tokens[i])
	command_Tj ();
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
	debug(dlTrace, string("Dropped token ") + tokens[i]);
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

/////////////////////////////////////////////////////////////////////
// Fill and stroke with non-zero winding rule
void
pdfRender::command_b ()
{
  debug(dlTrace, "b -- non zero winding fill not implemented, using even odd");
  command_bstar ();
}

void
pdfRender::command_B ()
{
  debug(dlTrace, "B -- non zero winding fill not implemented, using even odd");
  command_Bstar ();
}

void
pdfRender::command_bstar ()
{
  if (m_mode != rmGraphics)
    {
      debug(dlTrace, "Not in graphics mode");
      return;
    }

  plot_closeline (m_plot);
  plot_strokeline (m_plot);
  plot_fillline (m_plot);
  m_hasLine = true;
}

void
pdfRender::command_Bstar ()
{
  if (m_mode != rmGraphics)
    {
      debug(dlTrace, "Not in graphics mode");
      return;
    }

  plot_strokeline (m_plot);
  plot_fillline (m_plot);
  m_hasLine = true;
}

// Enter text mode
void
pdfRender::command_BT ()
{
  debug(dlTrace, "BT");
  if (m_mode == rmText)
    {
      debug(dlTrace, "Error -- already in text mode");
      return;
    }

  m_mode = rmText;
  m_textMatrix.setIdentity ();
}

void
pdfRender::command_c ()
{
  unsigned int x1, y1, x2, y2, x3, y3;

  // Pop our arguements (reverse order)
  y3 = m_height - atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  x3 = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  y2 = m_height - atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  x2 = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  y1 = m_height - atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  x1 = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();

  if (m_mode != rmGraphics)
    {
      debug(dlTrace, "Not in graphics mode");
      return;
    }

  plot_addcubiccurvesegment (m_plot, x1, y1, x2, y2, x3, y3);
  m_hasLine = true;
}

// Set the graphics matrix
void
pdfRender::command_cm ()
{
  float vals[6];

  debug(dlTrace, "cm");
  for (int i = 0; i < 6; i++)
    {
      vals[5 - i] = atof (m_arguements.top ().c_str ());
      m_arguements.pop ();
    }

  m_graphicsMatrix.setValues (vals);
}

// todo_mikal: this will only work with black and white images for now...
// todo_mikal: do all error paths cleanup enough?
void
pdfRender::command_Do ()
{
  string arg;
  debug(dlTrace, "Do");

  // The arguement is the name of a resource entry
  arg = m_arguements.top ();
  m_arguements.pop ();

  dictionary resdict;
  if (!m_page.getDict ().getValue ("Resources", resdict))
    {
      debug(dlTrace, "Resource dictionary not found");
      return;
    }

  dictionary xobj;
  if (!resdict.getValue ("XObject", xobj))
    {
      debug(dlTrace, "Resource dictionary not found");
      return;
    }

  object image (-1, -1);
  if (!xobj.getValue (arg.substr (1, arg.length () - 1), m_pdf, image))
    {
      debug(dlTrace, "Named resource does not exist");
      return;
    }

  debug(dlTrace, "We should do something with this now...");
  bool needStreamClean (false);
  unsigned long length;
  raster rast (image);

  // This stream is packed eight pixels to the byte if it is a black and white
  // TIFF stream
  unsigned char *stream = (unsigned char *) image.getStream (rast, 
							     needStreamClean, 
							     length);
  debug(dlTrace, string("Stream returned from object is ") + 
	toString((long) length) + string(" bytes"));

  // Expand the raster to the bit depth of the output raster
  unsigned char *newstream, *newstream2;
  // todo_mikal: eliminate mallocs and frees
  if((newstream = (unsigned char *) malloc(length * 8)) == NULL){
    debug(dlTrace, "Could not allocate memory for raster conversion");
    return;
  }

  memset(newstream, 0, length * 8);
  unsigned int o = 0;
  for(unsigned int i = 0; i < length; i++){
    if(!(stream[i] & 128)) newstream[o] = 255;
    o++;
    if(!(stream[i] & 64)) newstream[o] = 255;
    o++;
    if(!(stream[i] & 32)) newstream[o] = 255;
    o++;
    if(!(stream[i] & 16)) newstream[o] = 255;
    o++;
    if(!(stream[i] & 8)) newstream[o] = 255;
    o++;
    if(!(stream[i] & 4)) newstream[o] = 255;
    o++;
    if(!(stream[i] & 2)) newstream[o] = 255;
    o++;
    if(!(stream[i] & 1)) newstream[o] = 255;
    o++;
  }

  newstream2 = (unsigned char *) inflateraster((char *) newstream, 
					       rast.getWidth(), rast.getHeight(), 
			     8, 8, 1, 3);
  if((int) newstream2 == -1){
    free(stream);
    debug(dlError, "Raster inflation failed");
    return;
    }

  plot_overlayraster(m_plot, (char *) newstream2, 0, 0, 
		     m_width, m_height, 
		     rast.getWidth(), rast.getHeight(), 0);
  if(needStreamClean)
    free(stream);
  free(newstream2);
}

// Exit text mode
void
pdfRender::command_ET ()
{
  debug(dlTrace, "ET");
  if (m_mode != rmText)
    {
      debug(dlTrace, "Error -- exitting non existant text mode");
    }
  m_mode = rmGraphics;
}

// Fill using non zero winding
void
pdfRender::command_f ()
{
  debug(dlTrace, "f -- non zero winding fill not implemented, using even odd");
  command_fstar ();
}

// Fill using even odd rule
void
pdfRender::command_fstar ()
{
  if (m_mode != rmGraphics)
    {
      debug(dlTrace, "Not in graphics mode");
      return;
    }

  debug(dlTrace, "Even odd fill");
  plot_fillline (m_plot);
  m_hasLine = true;
}

// Fill using non zero winding
void
pdfRender::command_F ()
{
  debug(dlTrace, "F -- non zero winding fill not implemented, using even odd");
  command_fstar ();
}

// Grayscale stroke
void
pdfRender::command_g ()
{
  float g;
  g = atof (m_arguements.top ().c_str ());

  if (m_mode != rmGraphics)
    {
      debug(dlTrace, "Not in graphics mode");
      return;
    }

  debug(dlTrace, string("Set grayscale stroke ") + toString(g));
  plot_setlinecolor (m_plot, (unsigned int) (255 * g),
		     (unsigned int) (255 * g), (unsigned int) (255 * g));
  m_hasLine = true;
}

// Grayscale fill
void
pdfRender::command_G ()
{
  float g;
  g = atof (m_arguements.top ().c_str ());

  if (m_mode != rmGraphics)
    {
      debug(dlTrace, "Not in graphics mode");
      return;
    }

  debug(dlTrace, string("Set grayscale fill ") + toString(g));
  plot_setfillcolor (m_plot, (unsigned int) (255 * g),
		     (unsigned int) (255 * g), (unsigned int) (255 * g));
  m_hasLine = true;
}

// Close line
void
pdfRender::command_h ()
{
  if (m_mode != rmGraphics)
    {
      debug(dlTrace, "Not in graphics mode");
      return;
    }

  debug(dlTrace, "Close line");
  plot_closeline (m_plot);
  m_hasLine = true;
}

// Line to
void
pdfRender::command_l ()
{
  unsigned int x, y;

  // Pop our arguements (reverse order)
  y = m_height - atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  x = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();

  if (m_mode != rmGraphics)
    {
      debug(dlTrace, "Not in graphics mode");
      return;
    }

  plot_addlinesegment (m_plot, x, y);
  m_hasLine = true;
}

// Move graphics cursor to a given location
void
pdfRender::command_m ()
{
  unsigned int x, y;

  // Pop our arguements (reverse order)
  y = m_height - atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  x = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();

  if (m_mode != rmGraphics)
    {
      debug(dlTrace, "Not in graphics mode");
      return;
    }

  if (m_hasLine)
    plot_endline (m_plot);
  plot_setlinestart (m_plot, x, y);
  m_hasLine = true;
}

void
pdfRender::command_q ()
{
  debug(dlTrace, "Save graphics state [not implemented]");
}

void
pdfRender::command_Q ()
{
  debug(dlTrace, "Restore graphics state [not implemented]");
}

// A rectangle
void
pdfRender::command_re ()
{
  unsigned int left, top, right, bottom;

  // Pop our arguements (reverse order)
  bottom = m_height - atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  right = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  top = m_height - atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  left = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();

  if (m_mode != rmGraphics)
    {
      debug(dlTrace, "Not in graphics mode");
      return;
    }

  plot_rectangle (m_plot, left, top, right, bottom);
  m_hasLine = true;
}

// Set RGB fill color
void
pdfRender::command_rg ()
{
  float r, g, b;

  // Pop our arguements (reverse order)
  b = atof (m_arguements.top ().c_str ());
  m_arguements.pop ();
  g = atof (m_arguements.top ().c_str ());
  m_arguements.pop ();
  r = atof (m_arguements.top ().c_str ());
  m_arguements.pop ();

  if (m_mode != rmGraphics)
    {
      debug(dlTrace, "Not in graphics mode");
      return;
    }

  plot_setfillcolor (m_plot, (unsigned int) (255 * r),
		     (unsigned int) (255 * g), (unsigned int) (255 * b));
  m_hasLine = true;
}

// Set RGB fill color
void
pdfRender::command_RG ()
{
  float r, g, b;

  // Pop our arguements (reverse order)
  b = atof (m_arguements.top ().c_str ());
  m_arguements.pop ();
  g = atof (m_arguements.top ().c_str ());
  m_arguements.pop ();
  r = atof (m_arguements.top ().c_str ());
  m_arguements.pop ();

  if (m_mode != rmGraphics)
    {
      debug(dlTrace, "Not in graphics mode");
      return;
    }

  plot_setlinecolor (m_plot, (unsigned int) (255 * r),
		     (unsigned int) (255 * g), (unsigned int) (255 * b));
  m_hasLine = true;
}

void
pdfRender::command_S ()
{
  plot_strokeline (m_plot);
}


// Move text position
void
pdfRender::command_Td ()
{
  debug(dlTrace, "Td [not implemented]");
}

void
pdfRender::command_Tf ()
{
  string fontName, fontSize;

  // Pop our arguements (reverse order)
  fontSize = m_arguements.top ();
  m_arguements.pop ();
  fontName = m_arguements.top ();
  m_arguements.pop ();

  // Find the named font
  dictionary resources;
  dictionary fonts;
  object font (-1, -1);
  string fontResource, fontFile ("px10.ttf");
  bool fontFound (false);

  if (!m_page.getDict ().getValue ("Resources", resources))
    {
      debug(dlTrace, "Font not found (no resources)");
    }
  else if (!resources.getValue ("Font", fonts))
    {
      debug(dlTrace, "Font not found (no font entry in resources)");
    }
  else if (!fonts.
	   getValue (fontName.substr (1, fontName.length ()), m_pdf, font))
    {
      debug(dlTrace, "Font not found (named font not listed in resources)");
    }
  else if (font.getDict ().getValue ("BaseFont", fontResource))
    {
      fontFound = true;
    }

  // Now map this name to a TrueType file somewhere on the system
  if (fontFound)
    {
      debug(dlTrace, string("Lookup font ") + fontResource);
      configuration *config;
      config = (configuration *) & configuration::getInstance ();
      config->getValue (string ("basefont-") + fontResource + "-map",
			"px10.ttf", fontFile);
    }

  debug(dlTrace, string("Using font filename ") + fontFile);
  plot_setfont (m_plot, (char *) fontFile.c_str (), atoi (fontSize.c_str ()));
}

// Show the text
void
pdfRender::command_Tj ()
{
  debug(dlTrace, string("Tj ") + m_arguements.top ());
  plot_settextlocation (m_plot, (unsigned int) m_textMatrix.getHorizontal (),
			(unsigned int) (m_height -
					m_textMatrix.getVertical ()));
  plot_writestring (m_plot, (char *) m_arguements.top ().c_str ());
  m_arguements.pop ();
}

// Set the text matrix
void
pdfRender::command_Tm ()
{
  float vals[6];

  debug(dlTrace, "Tm");
  for (int i = 0; i < 6; i++)
    {
      vals[5 - i] = atof (m_arguements.top ().c_str ());
      m_arguements.pop ();
    }

  m_textMatrix.setValues (vals);
}

void
pdfRender::command_Tr ()
{
  debug(dlTrace, "Tr [not implemented]");
}

void
pdfRender::command_v ()
{
  unsigned int x1, y1, x2, y2;

  // Pop our arguements (reverse order)
  y2 = m_height - atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  x2 = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  y1 = m_height - atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  x1 = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();

  if (m_mode != rmGraphics)
    {
      debug(dlTrace, "Not in graphics mode");
      return;
    }

  plot_addquadraticcurvesegmentone (m_plot, x1, y1, x2, y2);
  m_hasLine = true;
}

void
pdfRender::command_w ()
{
  int w;

  // Pop arguement
  w = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();

  if (m_mode != rmGraphics)
    {
      debug(dlTrace, "Not in graphics mode");
      return;
    }

  plot_setlinewidth (m_plot, w, w);
}

void
pdfRender::command_y ()
{
  unsigned int x1, y1, x2, y2;

  // Pop our arguements (reverse order)
  y2 = m_height - atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  x2 = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  y1 = m_height - atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  x1 = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();

  if (m_mode != rmGraphics)
    {
      debug(dlTrace, "Not in graphics mode");
      return;
    }

  plot_addquadraticcurvesegmenttwo (m_plot, x1, y1, x2, y2);
  m_hasLine = true;
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
