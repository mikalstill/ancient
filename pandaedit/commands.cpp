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
  if (m_mode != rmText)
    appendCommand(m_commandString);
  m_commandString = "BT\n";
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

  m_commandString += toString(x1) + string(" ") +
    toString(y1) + string(" ") +
    toString(x2) + string(" ") +
    toString(y2) + string(" ") +
    toString(x3) + string(" ") +
    toString(y3) + string(" c\n");
}

// Set the graphics matrix
void
pdfRender::command_cm ()
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
    toString(vals[5]) + string(" cm\n");
}

void
pdfRender::command_Do ()
{
  string arg;
  arg = m_arguements.top ();
  m_arguements.pop ();

  appendCommand(m_commandString);
  m_commandString = arg + string(" Do\n");
}

// Exit text mode
void
pdfRender::command_ET ()
{
  m_commandString += "ET\n";
  appendCommand(m_commandString);
  m_commandString = "";
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

// Close line
void
pdfRender::command_h ()
{
  m_commandString += "h\n";
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

  m_commandString += toString(x) + string(" ") + toString(y) + string(" l\n");
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

  m_commandString += toString(x) + string(" ") + toString(y) + string(" m\n");
}

// Save graphics state
void
pdfRender::command_q ()
{
  appendCommand(m_commandString);
  m_commandString = "q\n";
}

// Restore graphics state
void
pdfRender::command_Q ()
{
  m_commandString += "Q\n";
  appendCommand(m_commandString);
  m_commandString = "";
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

  m_commandString += toString(left) + string(" ") +
    toString(top) + string(" ") +
    toString(right) + string(" ") +
    toString(bottom) + string(" re\n");
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

  m_commandString += toString(r) + string(" ") +
    toString(g) + string(" ") +
    toString(b) + string(" rg\n");
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

  m_commandString += toString(r) + string(" ") +
    toString(g) + string(" ") +
    toString(b) + string(" RG\n");
}

void
pdfRender::command_S ()
{
  m_commandString += "S\n";
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
  y2 = m_height - atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  x2 = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  y1 = m_height - atoi (m_arguements.top ().c_str ());
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
  y2 = m_height - atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  x2 = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  y1 = m_height - atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();
  x1 = atoi (m_arguements.top ().c_str ());
  m_arguements.pop ();

  m_commandString += toString(x1) + string(" ") +
    toString(y1) + string(" ") +
    toString(x2) + string(" ") +
    toString(y2) + string(" y\n");
}
