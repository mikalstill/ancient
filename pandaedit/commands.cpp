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

#include "linetool.h"

char *inflateraster(char *input, unsigned long width, unsigned long height, 
                    int bitdepth, int targetbitdepth, 
                    int channels, int targetchannels);

/////////////////////////////////////////////////////////////////////
// Fill and stroke with non-zero winding rule
void
pdfRender::command_b ()
{
  debug(dlError, "Need to implement b");
  m_commandString += "b\n";
}

void
pdfRender::command_B ()
{
  debug(dlError, "Need to implement B");
  m_commandString += "B\n";
}

void
pdfRender::command_bstar ()
{
  debug(dlError, "Need to implement b*");
  m_commandString += "b*\n";
}

void
pdfRender::command_Bstar ()
{
  debug(dlError, "Need to implement B*");
  m_commandString += "B*\n";
}

// tweaked
// Enter text mode
void
pdfRender::command_BT ()
{
  m_textMatrix.setIdentity();
  m_textLineMatrix.setIdentity();
}

// tweaked
void
pdfRender::command_c ()
{
  unsigned int x1, y1, x2, y2, x3, y3;

  // Pop our arguements (reverse order)
  y3 = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  x3 = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  y2 = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  x2 = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  y1 = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  x1 = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();

  if (m_mode != rmGraphics)
    {
      debug(dlTrace, "Not in graphics mode");
      return;
    }

  cmdControlPoint curved;
  curved.pt = translateGraphicsPoint(wxPoint(x1, m_height - y1));
  curved.cpt.push_back(translateGraphicsPoint(wxPoint(x2, m_height - y2)));
  curved.cpt.push_back(translateGraphicsPoint(wxPoint(x3, m_height - y3)));
  curved.modifier = ltmBezier;
  m_controlPoints.push_back(curved);
  debug(dlTrace, string("Added bezier curve point: ") +
	toString(curved.pt.x) + string(", ") + toString(curved.pt.y) +
	string(" CP1 ") + 
	toString(curved.cpt[0].x) + string(", ") + toString(curved.cpt[0].y) +
	string(" CP2 ") + 
	toString(curved.cpt[1].x) + string(", ") + toString(curved.cpt[1].y));
}

// tweaked
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

// tweaked
// Set the stroking colour space
void
pdfRender::command_CS ()
{
  string arg;
  arg = m_arguements.top ();
  m_arguements.pop ();
  if(arg == "/DeviceGray")
    {
      m_lineCS = csGray;
      m_lineColor.r = 0;
      m_lineColor.g = 0;
      m_lineColor.b = 0;
    }
  else if(arg == "/DeviceRGB")
    {
      m_lineCS = csRGB;
      m_lineColor.r = 0;
      m_lineColor.g = 0;
      m_lineColor.b = 0;
    }
  else
    {
      debug(dlError, string("Unknown color space for CS: \"") + arg +
	    string("\""));
    }
}

// tweaked
// Set fill color
void
pdfRender::command_cs ()
{
  string arg;
  arg = m_arguements.top ();
  m_arguements.pop ();
  if(arg == "/DeviceGray")
    {
      m_fillCS = csGray;
      m_fillColor.r = 0;
      m_fillColor.g = 0;
      m_fillColor.b = 0;
    }
  else if(arg == "/DeviceRGB")
    {
      m_fillCS = csRGB;
      m_fillColor.r = 0;
      m_fillColor.g = 0;
      m_fillColor.b = 0;
    }
  else
    {
      debug(dlError, string("Unknown color space for cs: \"") + arg +
	    string("\""));
    }
}

// tweaked
void
pdfRender::command_Do ()
{
  string arg;
  debug(dlTrace, "Do");

  // The arguement is the name of a resource entry
  arg = m_arguements.top ();
  m_arguements.pop ();

  dictionary resdict;
  object pobj(objNumNoSuch, objNumNoSuch);
  object& page = pobj;
  if(!m_doc->getPage(m_pageno, page))
    return;

  if (!page.getDict ().getValue ("Resources", resdict))
    {
      debug(dlError, "Resource dictionary not found");
      return;
    }

  dictionary xobj;
  if (!resdict.getValue ("XObject", xobj))
    {
      debug(dlError, "Resource dictionary not found");
      return;
    }

  object image (objNumNoSuch, objNumNoSuch);
  if (!xobj.getValue (arg.substr (1, arg.length () - 1), *(m_doc->getPDF()), 
                      image))
    {
      debug(dlError, "Named resource does not exist");
      return;
    }

  debug(dlTrace, "Process image command further");
  unsigned long length;
  raster rast (image);

  // This stream is packed eight pixels to the byte if it is a black and white
  // TIFF stream
  unsigned char *stream = (unsigned char *) image.getStream (rast, 
                                                             length);
  debug(dlTrace, string("Stream returned from object is ") + 
        toString((long) length) + string(" bytes"));
  
  // TODO mikal: unacceptable assumptions about the number of pixels per
  // sample
  // Expand the raster to the bit depth of the output raster (if needed)
  if(rast.getBitsPerSample() != 8)
    {
      unsigned char *newstream;
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
      m_raster = (unsigned char *) inflateraster((char *) newstream, 
						   rast.getWidth(), 
						   rast.getHeight(), 
						   8, 8, 1, 3);
      if((int) m_raster == -1){
	delete[] stream;
	debug(dlError, "Raster inflation failed");
	return;
      }

      delete[] stream;
    }
  else
    {
      m_raster = stream;
    }

  // And now append the command to the command chain for this page
  if(m_controlPoints.size() != 0)
    {
      debug(dlError, "Losing control points upon insertion of image");
      m_controlPoints.clear();
    }
  
  // TODO mikal: this will not place the image in the right spot
  cmdControlPoint foo;

  foo.pt = wxPoint(0, 0);
  m_controlPoints.push_back(foo);

  foo.pt = wxPoint(rast.getWidth(), rast.getHeight());
  m_controlPoints.push_back(foo);

  appendCommand(object::cImage);
  m_raster = NULL;
}

// tweaked
// Exit text mode
void
pdfRender::command_ET ()
{
  // Effectively a NOOP, simply discards the textMatrix and textLineMatrix
}

// Fill using non zero winding
void
pdfRender::command_f ()
{
  debug(dlError, "Need to implement f");
  m_commandString += "f\n";
}

// Fill using even odd rule
void
pdfRender::command_fstar ()
{
  debug(dlError, "Need to implement f*");
  m_commandString += "f*\n";
}

// Fill using non zero winding
void
pdfRender::command_F ()
{
  debug(dlError, "Need to implement F");
  m_commandString += "F\n";
}

// Grayscale stroke
void
pdfRender::command_g ()
{
  debug(dlError, "Need to implement g");
  float g;
  g = atof (m_arguements.top ().c_str ());
  m_commandString += toString(g) + string(" g\n");
}

// Grayscale fill
void
pdfRender::command_G ()
{
  debug(dlError, "Need to implement G");
  float g;
  g = atof (m_arguements.top ().c_str ());
  m_commandString += toString(g) + string(" G\n");
}

// tweaked
// Close line
void
pdfRender::command_h ()
{
  cmdControlPoint h;
  h.pt = m_subpath.top();
  m_subpath.pop();
  h.modifier = ltmNone;

  m_controlPoints.push_back(h);
}

// tweaked
// Line to
void
pdfRender::command_l ()
{
  unsigned int x, y;

  // Pop our arguements (reverse order)
  y = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  x = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();

  cmdControlPoint straight;
  straight.pt = translateGraphicsPoint(wxPoint(x, m_height - y));
  straight.modifier = ltmNone;
  m_controlPoints.push_back(straight);
}

// Move graphics cursor to a given location -- this can happen in the middle of
// a line!
// tweaked
void
pdfRender::command_m ()
{
  unsigned int x, y;

  // Pop our arguements (reverse order)
  y = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  x = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();

  cmdControlPoint mv;
  mv.pt = translateGraphicsPoint(wxPoint(x, m_height - y));
  if(m_controlPoints.size() == 0)
    mv.modifier = 0;
  else
    mv.modifier = ltmJump;
  m_controlPoints.push_back(mv);

  m_subpath.push(mv.pt);
}

// Save graphics state
void
pdfRender::command_q ()
{
  appendCommand(object::cSaveState);
}

// Restore graphics state
void
pdfRender::command_Q ()
{
  appendCommand(object::cRestoreState);
}

// tweaked
// A rectangle
void
pdfRender::command_re ()
{
  unsigned int x, y, w, h;

  // Pop our arguements (reverse order)
  h = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  w = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  y = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  x = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();

  // This is implemented in terms of other commands, as specified in the PDF
  // specification...
  m_arguements.push(toString(y));
  m_arguements.push(toString(x));
  command_m();

  m_arguements.push(toString(y));
  m_arguements.push(toString(x + w));
  command_l();

  m_arguements.push(toString(y + h));
  m_arguements.push(toString(x + w));
  command_l();

  m_arguements.push(toString(y + h));
  m_arguements.push(toString(x));
  command_l();

  command_h();
}

// tweaked
// Set RGB line color
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

  m_lineColor.r = (int) (r * 255.0);
  m_lineColor.g = (int) (g * 255.0);
  m_lineColor.b = (int) (b * 255.0);
}

// tweaked
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

  m_lineColor.r = (int) (r * 255.0);
  m_lineColor.g = (int) (g * 255.0);
  m_lineColor.b = (int) (b * 255.0);
}

// tweaked
// Stroke line
void
pdfRender::command_S ()
{
  // This ends the current instance of the line tool
  appendCommand(object::cLine);
  while(m_subpath.size() > 0)
    m_subpath.pop();
}

// tweaked
// Set colour for stroking operations
void
pdfRender::command_SC ()
{
  float r, g, b;

  switch(m_lineCS)
    {
    case csGray:
      r = atof (m_arguements.top ().c_str ());
      m_arguements.pop ();
      m_lineColor.r = (int) (r * 255.0);
      m_lineColor.g = (int) (r * 255.0);
      m_lineColor.b = (int) (r * 255.0);
      break;

    case csRGB:
      b = atof (m_arguements.top ().c_str ());
      m_arguements.pop ();
      g = atof (m_arguements.top ().c_str ());
      m_arguements.pop ();
      r = atof (m_arguements.top ().c_str ());
      m_arguements.pop ();

      m_lineColor.r = (int) (r * 255.0);
      m_lineColor.g = (int) (g * 255.0);
      m_lineColor.b = (int) (b * 255.0);
      break;
    }
}

// tweaked
// Set colour for non stroking operations
void
pdfRender::command_sc ()
{
  float r, g, b;

  switch(m_fillCS)
    {
    case csGray:
      r = atof (m_arguements.top ().c_str ());
      m_arguements.pop ();
      m_fillColor.r = (int) (r * 255.0);
      m_fillColor.g = (int) (r * 255.0);
      m_fillColor.b = (int) (r * 255.0);
      break;

    case csRGB:
      r = atof (m_arguements.top ().c_str ());
      m_arguements.pop ();
      g = atof (m_arguements.top ().c_str ());
      m_arguements.pop ();
      b = atof (m_arguements.top ().c_str ());
      m_arguements.pop ();

      m_fillColor.r = (int) (r * 255.0);
      m_fillColor.g = (int) (g * 255.0);
      m_fillColor.b = (int) (b * 255.0);
      break;
    }
}

// Move text position
void
pdfRender::command_Td ()
{
  debug(dlError, "Need to implement Td");

  float x, y;

  y = atof (m_arguements.top ().c_str ());
  m_arguements.pop ();
  x = atof (m_arguements.top ().c_str ());
  m_arguements.pop ();

  m_commandString += toString(x) + string(" ") +
    toString(y) + string(" Td\n");
}

// Move text position and set leading
void
pdfRender::command_TD ()
{
  debug(dlError, "Need to implement TD");

  m_commandString += "TD\n";
}

void
pdfRender::command_Tf ()
{
  debug(dlError, "Need to implement Tf");

  string fontName, fontSize;

  // Pop our arguements (reverse order)
  fontSize = m_arguements.top ();
  m_arguements.pop ();
  fontName = m_arguements.top ();
  m_arguements.pop ();

  m_commandString += fontName + " " + fontSize + " Tf\n";
}

// Show the text
void
pdfRender::command_Tj ()
{
  debug(dlError, "Need to implement Tj");

  m_commandString += "Tj\n";
}

// Set the text leading
void
pdfRender::command_TL ()
{
  debug(dlError, "Need to implement TL");

  m_commandString += "TL\n";
}

// tweaked
// Set the text matrix
void
pdfRender::command_Tm ()
{
  float vals[6];

  for (int i = 0; i < 6; i++)
    {
      vals[5 - i] = atof (m_arguements.top ().c_str ());
      m_arguements.pop ();
    }

  m_graphicsMatrix.setValues (vals);
}

void
pdfRender::command_Tr ()
{
  debug(dlError, "Need to implement Tr");

  m_commandString += "Tr\n";
}

// tweaked
void
pdfRender::command_v ()
{
  unsigned int x1, y1, x2, y2;

  // Pop our arguements (reverse order)
  y2 = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  x2 = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  y1 = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  x1 = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();

  if (m_mode != rmGraphics)
    {
      debug(dlTrace, "Not in graphics mode");
      return;
    }

  cmdControlPoint curved;
  curved.pt = translateGraphicsPoint(wxPoint(x1, m_height - y1));
  curved.cpt.push_back(translateGraphicsPoint(wxPoint(x2, m_height - y2)));
  curved.modifier = ltmHalfBezierOne;
  m_controlPoints.push_back(curved);
  debug(dlTrace, string("Added half bezier one curve point: ") +
	toString(curved.pt.x) + string(", ") + toString(curved.pt.y) +
	string(" CP ") + 
	toString(curved.cpt[0].x) + string(", ") + toString(curved.cpt[0].y));
}

void
pdfRender::command_w ()
{
  debug(dlError, "Need to implement w");

  int w;

  // Pop arguement
  w = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();

  m_commandString += toString(w) + string(" w\n");
}

// tweaked
void
pdfRender::command_y ()
{
  unsigned int x1, y1, x2, y2;

  // Pop our arguements (reverse order)
  y2 = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  x2 = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  y1 = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  x1 = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();

  if (m_mode != rmGraphics)
    {
      debug(dlTrace, "Not in graphics mode");
      return;
    }

  cmdControlPoint curved;
  curved.pt = translateGraphicsPoint(wxPoint(x1, m_height - y1));
  curved.cpt.push_back(translateGraphicsPoint(wxPoint(x2, m_height - y2)));
  curved.modifier = ltmHalfBezierTwo;
  m_controlPoints.push_back(curved);
  debug(dlTrace, string("Added half bezier two curve point: ") +
	toString(curved.pt.x) + string(", ") + toString(curved.pt.y) +
	string(" CP ") + 
	toString(curved.cpt[0].x) + string(", ") + toString(curved.cpt[0].y));
}
