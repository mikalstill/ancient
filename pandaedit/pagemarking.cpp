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

#if defined HAVE_LIBMPLOT
#else
#warning "Lots of functionality is missing because of a lack of libmplot"
#endif

char *inflateraster(char *input, unsigned long width, unsigned long height, 
                    int bitdepth, int targetbitdepth, 
                    int channels, int targetchannels);

/////////////////////////////////////////////////////////////////////
// Fill and stroke with non-zero winding rule
void
pdfRender::render_b ()
{
  debug(dlInformational, 
	"b -- non zero winding fill not implemented, using even odd");
  render_bstar ();
}

void
pdfRender::render_B ()
{
  debug(dlInformational, 
	"B -- non zero winding fill not implemented, using even odd");
  render_Bstar ();
}

void
pdfRender::render_bstar ()
{
  if (m_mode != rmGraphics)
    {
      debug(dlTrace, "Not in graphics mode");
      return;
    }

#if defined HAVE_LIBMPLOT
  plot_closeline (m_plot);
  plot_strokeline (m_plot);
  plot_fillline (m_plot);
#else
  debug(dlError, "Libmplot not found at configure time. Graphics functionality"
	" is therefore not available");
#endif
  m_hasLine = true;
}

void
pdfRender::render_Bstar ()
{
  if (m_mode != rmGraphics)
    {
      debug(dlTrace, "Not in graphics mode");
      return;
    }

#if defined HAVE_LIBMPLOT
  plot_strokeline (m_plot);
  plot_fillline (m_plot);
#else
  debug(dlError, "Libmplot not found at configure time. Graphics functionality"
	" is therefore not available");
#endif
  m_hasLine = true;
}

// Enter text mode
void
pdfRender::render_BT ()
{
  debug(dlTrace, "BT");
  if (m_mode == rmText)
    {
      debug(dlTrace, "Error -- already in text mode");
      return;
    }

  m_mode = rmText;
  m_textMatrix.setIdentity ();
  m_textLineMatrix = m_textMatrix;
}

void
pdfRender::render_c ()
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

#if defined HAVE_LIBMPLOT
  plot_addcubiccurvesegment (m_plot, x1, y1, x2, y2, x3, y3);
#else
  debug(dlError, "Libmplot not found at configure time. Graphics functionality"
	" is therefore not available");
#endif
  m_hasLine = true;
}

// Set the graphics matrix
void
pdfRender::render_cm ()
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
pdfRender::render_Do ()
{
  string arg;
  debug(dlTrace, "Do");

  // The arguement is the name of a resource entry
  arg = m_arguements.top ();
  m_arguements.pop ();

  dictionary resdict;
  if (!m_doc->getPage(m_pageno).getDict ().getValue ("Resources", resdict))
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

  object image (objNumNoSuch, objNumNoSuch);
  if (!xobj.getValue (arg.substr (1, arg.length () - 1), *(m_doc->getPDF()), 
		      image))
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

  debug(dlTrace, string("Image width and height at insertion time: ") +
	toString(rast.getWidth()) + string(" by ") +
	toString(rast.getHeight()));
  newstream2 = (unsigned char *) inflateraster((char *) newstream, 
					       rast.getWidth(), 
					       rast.getHeight(), 
					       8, 8, 1, 3);
  if((int) newstream2 == -1){
    free(stream);
    debug(dlError, "Raster inflation failed");
    return;
  }

#if defined HAVE_LIBMPLOT  
  plot_overlayraster(m_plot, (char *) newstream2, 0, 0, 
		     m_width, m_height, 
		     rast.getWidth(), rast.getHeight(), 0);
#else
  debug(dlError, "Libmplot not found at configure time. Graphics functionality"
	" is therefore not available");
#endif
  if(needStreamClean)
    free(stream);
  free(newstream2);
}

// Exit text mode
void
pdfRender::render_ET ()
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
pdfRender::render_f ()
{
  debug(dlInformational, 
	"f -- non zero winding fill not implemented, using even odd");
  render_fstar ();
}

// Fill using even odd rule
void
pdfRender::render_fstar ()
{
  if (m_mode != rmGraphics)
    {
      debug(dlTrace, "Not in graphics mode");
      return;
    }

  debug(dlTrace, "Even odd fill");
#if defined HAVE_LIBMPLOT
  plot_fillline (m_plot);
#else
  debug(dlError, "Libmplot not found at configure time. Graphics functionality"
	" is therefore not available");
#endif
  m_hasLine = true;
}

// Fill using non zero winding
void
pdfRender::render_F ()
{
  debug(dlInformational, 
	"F -- non zero winding fill not implemented, using even odd");
  render_fstar ();
}

// Grayscale stroke
void
pdfRender::render_g ()
{
  float g;
  g = atof (m_arguements.top ().c_str ());

  if (m_mode != rmGraphics)
    {
      debug(dlTrace, "Not in graphics mode");
      return;
    }

  debug(dlTrace, string("Set grayscale stroke ") + toString(g));
#if defined HAVE_LIBMPLOT
  plot_setlinecolor (m_plot, (unsigned int) (255 * g),
		     (unsigned int) (255 * g), (unsigned int) (255 * g));
#else
  debug(dlError, "Libmplot not found at configure time. Graphics functionality"
	" is therefore not available");
#endif
  m_hasLine = true;
}

// Grayscale fill
void
pdfRender::render_G ()
{
  float g;
  g = atof (m_arguements.top ().c_str ());

  if (m_mode != rmGraphics)
    {
      debug(dlTrace, "Not in graphics mode");
      return;
    }

  debug(dlTrace, string("Set grayscale fill ") + toString(g));
#if defined HAVE_LIBMPLOT
  plot_setfillcolor (m_plot, (unsigned int) (255 * g),
		     (unsigned int) (255 * g), (unsigned int) (255 * g));
#else
  debug(dlError, "Libmplot not found at configure time. Graphics functionality"
	" is therefore not available");
#endif
  m_hasLine = true;
}

// Close line
void
pdfRender::render_h ()
{
  if (m_mode != rmGraphics)
    {
      debug(dlTrace, "Not in graphics mode");
      return;
    }

  debug(dlTrace, "Close line");
#if defined HAVE_LIBMPLOT
  plot_closeline (m_plot);
#else
  debug(dlError, "Libmplot not found at configure time. Graphics functionality"
	" is therefore not available");
#endif
  m_hasLine = true;
}

// Line to
void
pdfRender::render_l ()
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

#if defined HAVE_LIBMPLOT
  debug(dlTrace, "Adding line segment");
  plot_addlinesegment (m_plot, x, y);
#else
  debug(dlError, "Libmplot not found at configure time. Graphics functionality"
	" is therefore not available");
#endif
  m_hasLine = true;
}

// Move graphics cursor to a given location
void
pdfRender::render_m ()
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

#if defined HAVE_LIBMPLOT
  if (m_hasLine)
    plot_endline (m_plot);
  debug(dlTrace, "Setting line start");
  plot_setlinestart (m_plot, x, y);
#else
  debug(dlError, "Libmplot not found at configure time. Graphics functionality"
	" is therefore not available");
#endif
  m_hasLine = true;
}

// Save graphics state
void
pdfRender::render_q ()
{
  debug(dlInformational, "Save graphics state [not implemented]");
}

// Restore graphics state
void
pdfRender::render_Q ()
{
  debug(dlInformational, "Restore graphics state [not implemented]");
}

// A rectangle
void
pdfRender::render_re ()
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

#if defined HAVE_LIBMPLOT
  plot_rectangle (m_plot, left, top, right, bottom);
#else
  debug(dlError, "Libmplot not found at configure time. Graphics functionality"
	" is therefore not available");
#endif
  m_hasLine = true;
}

// Set RGB fill color
void
pdfRender::render_rg ()
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

#if defined HAVE_LIBMPLOT
  plot_setfillcolor (m_plot, (unsigned int) (255 * r),
		     (unsigned int) (255 * g), (unsigned int) (255 * b));
#else
  debug(dlError, "Libmplot not found at configure time. Graphics functionality"
	" is therefore not available");
#endif
  m_hasLine = true;
}

// Set RGB fill color
void
pdfRender::render_RG ()
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

#if defined HAVE_LIBMPLOT
  plot_setlinecolor (m_plot, (unsigned int) (255 * r),
		     (unsigned int) (255 * g), (unsigned int) (255 * b));
#else
  debug(dlError, "Libmplot not found at configure time. Graphics functionality"
	" is therefore not available");
#endif
  m_hasLine = true;
}

void
pdfRender::render_S ()
{
#if defined HAVE_LIBMPLOT
  debug(dlTrace, "Stroking line");
  plot_strokeline (m_plot);
#else
  debug(dlError, "Libmplot not found at configure time. Graphics functionality"
	" is therefore not available");
#endif
}


// Move text position
void
pdfRender::render_Td ()
{
  matrix newvals;
  float x, y;

  y = atof (m_arguements.top ().c_str ());
  m_arguements.pop ();
  x = atof (m_arguements.top ().c_str ());
  m_arguements.pop ();

  newvals.setIdentity();
  newvals.setVertical(y);
  newvals.setHorizontal(x);

  m_textMatrix = newvals * m_textLineMatrix;
  m_textLineMatrix = m_textMatrix;
}

// Move text position and set leading
void
pdfRender::render_TD ()
{
  debug(dlInformational, "TD [not implemented]");
  render_Td();

  // Need to push -y back on here (I think)
  render_TL();
}

void
pdfRender::render_Tf ()
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
  object font (objNumNoSuch, objNumNoSuch);
  string fontResource, fontFile ("px10.ttf");
  bool fontFound (false);

  if (!m_doc->getPage(m_pageno).getDict ().getValue ("Resources", resources))
    {
      debug(dlTrace, "Font not found (no resources)");
    }
  else if (!resources.getValue ("Font", fonts))
    {
      debug(dlTrace, "Font not found (no font entry in resources)");
    }
  else if (!fonts.
	   getValue (fontName.substr (1, fontName.length ()), 
		     *(m_doc->getPDF()), font))
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
#if defined HAVE_LIBMPLOT
  if(plot_setfont (m_plot, (char *) fontFile.c_str (), 
		   atoi (fontSize.c_str ())) < 0){
    debug(dlError, "Could not change to the specified font");
  }
#else
  debug(dlError, "Libmplot not found at configure time. Graphics functionality"
	" is therefore not available");
#endif
}

// Show the text
void
pdfRender::render_Tj ()
{
  debug(dlTrace, string("Tj ") + m_arguements.top ());
#if defined HAVE_LIBMPLOT
  plot_settextlocation (m_plot, (unsigned int) m_textMatrix.getHorizontal (),
			(unsigned int) (m_height -
					m_textMatrix.getVertical ()));
  plot_writestring (m_plot, (char *) m_arguements.top ().c_str ());
#else
  debug(dlError, "Libmplot not found at configure time. Graphics functionality"
	" is therefore not available");
#endif
  m_arguements.pop ();
}

// Set the text leading
void
pdfRender::render_TL ()
{
  debug(dlInformational, "TL [not implemented]");
}

// Set the text matrix
void
pdfRender::render_Tm ()
{
  float vals[6];

  debug(dlTrace, "Tm");
  for (int i = 0; i < 6; i++)
    {
      debug(dlTrace, string("Get arguement ") + toString(i));
      vals[5 - i] = atof (m_arguements.top ().c_str ());
      m_arguements.pop ();
    }

  m_textMatrix.setValues (vals);
  m_textLineMatrix = m_textMatrix;
}

void
pdfRender::render_Tr ()
{
  debug(dlInformational, "Tr [not implemented]");
}

void
pdfRender::render_v ()
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

#if defined HAVE_LIBMPLOT
  plot_addquadraticcurvesegmentone (m_plot, x1, y1, x2, y2);
#else
  debug(dlError, "Libmplot not found at configure time. Graphics functionality"
	" is therefore not available");
#endif
  m_hasLine = true;
}

void
pdfRender::render_w ()
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

#if defined HAVE_LIBMPLOT
  plot_setlinewidth (m_plot, w, w);
#else
  debug(dlError, "Libmplot not found at configure time. Graphics functionality"
	" is therefore not available");
#endif
}

void
pdfRender::render_y ()
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

#if defined HAVE_LIBMPLOT
  plot_addquadraticcurvesegmenttwo (m_plot, x1, y1, x2, y2);
#else
  debug(dlError, "Libmplot not found at configure time. Graphics functionality"
	" is therefore not available");
#endif
  m_hasLine = true;
}
