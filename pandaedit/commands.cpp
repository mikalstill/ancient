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

/////////////////////////////////////////////////////////////////////
// Fill and stroke with non-zero winding rule
void
pdfRender::command_b ()
{
  m_commandString += "b\n";
}

void
pdfRender::command_B ()
{
  m_commandString += "B\n";
}

void
pdfRender::command_bstar ()
{
  m_commandString += "b*\n";
}

void
pdfRender::command_Bstar ()
{
  m_commandString += "B*\n";
}

// Enter text mode
void
pdfRender::command_BT ()
{
  //  if (m_mode != rmText)
  //  appendCommand();
  m_commandString = "BT\n";
}

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

  m_commandString += toString(x1) + string(" ") +
    toString(y1) + string(" ") +
    toString(x2) + string(" ") +
    toString(y2) + string(" ") +
    toString(x3) + string(" ") +
    toString(y3) + string(" c\n");
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
  if (!m_doc->getPage(m_pageno).getDict ().getValue ("Resources", resdict))
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
  m_controlPoints.push_back(wxPoint(0, 0));
  m_controlPoints.push_back(wxPoint(rast.getWidth(), rast.getHeight()));
  appendCommand(object::cImage);
  m_raster = NULL;
}

// Exit text mode
void
pdfRender::command_ET ()
{
  m_commandString += "ET\n";
  //  appendCommand();
  m_commandString = "";
  m_controlString = "";
}

// Fill using non zero winding
void
pdfRender::command_f ()
{
  m_commandString += "f\n";
}

// Fill using even odd rule
void
pdfRender::command_fstar ()
{
  m_commandString += "f*\n";
}

// Fill using non zero winding
void
pdfRender::command_F ()
{
  m_commandString += "F\n";
}

// Grayscale stroke
void
pdfRender::command_g ()
{
  float g;
  g = atof (m_arguements.top ().c_str ());
  m_commandString += toString(g) + string(" g\n");
}

// Grayscale fill
void
pdfRender::command_G ()
{
  float g;
  g = atof (m_arguements.top ().c_str ());
  m_commandString += toString(g) + string(" G\n");
}

// tweaked
// Close line
void
pdfRender::command_h ()
{
  m_controlPoints.push_back(m_controlPoints[0]);
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

  m_controlPoints.push_back(translateGraphicsPoint(wxPoint(x, m_height - y)));
}

// Move graphics cursor to a given location
// tweaked
void
pdfRender::command_m ()
{
  unsigned int x, y;

  // It is assumed that this is the start of a new command
  if(m_controlPoints.size() != 0)
    {
      debug(dlError, "Move command in the middle of drawing");
      m_controlPoints.clear();
    }

  // Pop our arguements (reverse order)
  y = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  x = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();

  m_controlPoints.push_back(translateGraphicsPoint(wxPoint(x, m_height - y)));
}

// Save graphics state
void
pdfRender::command_q ()
{
  //  appendCommand();
  m_commandString = "q\n";
}

// Restore graphics state
void
pdfRender::command_Q ()
{
  m_commandString += "Q\n";

  //  appendCommand();
  m_commandString = "";
  m_controlString = "";
}

// A rectangle
void
pdfRender::command_re ()
{
  unsigned int left, top, right, bottom;

  // Pop our arguements (reverse order)
  bottom = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  right = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  top = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  left = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();

  m_commandString += toString(left) + string(" ") +
    toString(top) + string(" ") +
    toString(right) + string(" ") +
    toString(bottom) + string(" re\n");
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
  m_commandString += "TD\n";
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

  m_commandString += fontName + " " + fontSize + " Tf\n";
}

// Show the text
void
pdfRender::command_Tj ()
{
  m_commandString += "Tj\n";
}

// Set the text leading
void
pdfRender::command_TL ()
{
  m_commandString += "TL\n";
}

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

  m_commandString += toString(vals[0]) + string(" ") +
    toString(vals[1]) + string(" ") +
    toString(vals[2]) + string(" ") +
    toString(vals[3]) + string(" ") +
    toString(vals[4]) + string(" ") +
    toString(vals[5]) + string(" Tm\n");
}

void
pdfRender::command_Tr ()
{
  m_commandString += "Tr\n";
}

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

  m_commandString += toString(x1) + string(" ") +
    toString(y1) + string(" ") +
    toString(x2) + string(" ") +
    toString(y2) + string(" v\n");
}

void
pdfRender::command_w ()
{
  int w;

  // Pop arguement
  w = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();

  m_commandString += toString(w) + string(" w\n");
}

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

  m_commandString += toString(x1) + string(" ") +
    toString(y1) + string(" ") +
    toString(x2) + string(" ") +
    toString(y2) + string(" y\n");
}
