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

#include "linetool.h"

#define SELECTWIDTH 10

extern int gUniqueSelection;

pdfRender::pdfRender (pdfDoc *theDoc, int pageno, float xscale, float yscale):
m_doc(theDoc),
m_mode (rmGraphics),
m_plot (NULL),
m_select (NULL),
m_hasLine (false),
m_pageno (pageno),
m_xscale (xscale),
m_yscale (yscale),
m_raster (NULL)
{
  m_graphicsMatrix.setIdentity();
  m_textMatrix.setIdentity();
}

bool
pdfRender::render ()
{
  debug(dlTrace, "Starting render");
  return true;
}

bool
pdfRender::parseStream ()
{
  // Find all the contents objects
  object pobj(objNumNoSuch, objNumNoSuch);
  object& page = pobj;
  if(!m_doc->getPage(m_pageno, page))
    return false;
  
  page.getDict().getValue("Contents", *(m_doc->getPDF()), m_contents);
  debug(dlTrace, string("Commence stream parsing for a ") + 
	toString(m_contents.size()) + string(" object stream"));
  for(unsigned int i = 0; i < m_contents.size(); i++){
    object contents(objNumNoSuch, objNumNoSuch);
    if(m_contents.item(i, contents) && !processContentsObject(contents))
      {
	debug(dlError, "Processing of a command stream failed");
	return false;
      }
  }

  debug(dlTrace, "Finished parsing");
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

  if(m_commandString != "")
    debug(dlTrace, "Command string wasn't NULL");
  m_commandString = "";
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
      else if ((tokens[i][tokens[i].length () - 1] == ')'))
	{
	  sarg += " " + tokens[i].substr (0, tokens[i].length () - 1);
	  sargMode = false;
	  debug(dlTrace, string("String arguement finished (") + sarg +
		string(")"));
	  pushArguement (sarg);
	}
      else if ((tokens[i][tokens[i].length () - 3] == ')'))
	{
	  // iText uses ")Tj" instead of ") Tj"
	  sarg += " " + tokens[i].substr (0, tokens[i].length () - 3);
	  sargMode = false;
	  debug(dlTrace, string("String arguement finished in iText manner (") + 
		sarg + string(")"));
	  pushArguement (sarg);

	  tokens[i] = tokens[i].substr (tokens[i].length () - 2, 2);
	  debug(dlTrace, string("Reset the token to: " + tokens[i]));
	  --i;
	  continue;
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
      else if ("CS" == tokens[i])
	command_CS ();
      else if ("cs" == tokens[i])
	command_cs ();

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
      else if ("RG" == tokens[i])
	command_RG ();
      else if ("rg" == tokens[i])
	command_rg ();

      else if ("S" == tokens[i])
	command_S ();
      else if ("SC" == tokens[i])
	command_SC ();
      else if ("sc" == tokens[i])
	command_sc ();

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
	debug(dlError, string("Dropped token ") + tokens[i]);
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
      debug(dlError, "Could not create PNG info structure for writing "
	    "(out of memory?)");
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

  unsigned int width = (unsigned int) (m_width * m_xscale);
  unsigned int height = (unsigned int) (m_height * m_yscale);

  debug(dlTrace, string("Page dimensions: ") + toString(m_width) +
	string(" by ") + toString(m_height));
  debug(dlTrace, string("Page scaling: ") + toString(m_xscale) +
	string(" by ") + toString(m_yscale));
  debug(dlTrace, string("Setting PNG IHDR to: ") + 
	toString(width) + string(" by ") + toString(height));

  png_set_IHDR (png, info, width, height, 8, PNG_COLOR_TYPE_RGB,
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);
  png_write_info (png, info);

  // Write the image out
  if ((row_pointers =
       (png_byte **) malloc (height * sizeof (png_bytep))) == NULL)
    {
      debug(dlError, "Could not allocate memory");
      exit (42);
    }

  for (i = 0; i < height; i++)
    {
      row_pointers[i] = (png_byte *) (raster + (width * i * 3));
    }

  png_write_image (png, row_pointers);

  // Cleanup
  png_write_end (png, info);
  png_destroy_write_struct (&png, &info);
  fclose (image);

  return tempName;
}

//void
//pdfRender::appendCommand(object::commandType type)
//{
  // This test didn't work out, as text drawing commands don't have any control points
  //if(m_controlPoints.size() == 0)
  //  return;

  // Determine if this command is an append target
//  command cmd;
//  object pobj(objNumNoSuch, objNumNoSuch);
//  object& page = pobj;
//  if(!m_doc->getPage(m_pageno, page))
//    return;
//
//  if(page.getLastCommand(cmd))
//    {
      // Only supported for line commands
//      if(cmd.type == object::cLine)
//	goto newcommand;
//
//      if(cmd.controlPoints.size() == 0)
//	goto newcommand;
//      if(cmd.type != type)
//	goto newcommand;

      // TODO mikal: this line needs to be brought up to speed with the
      // new cmdControlPoint structure...
//      if(cmd.controlPoints[cmd.controlPoints.size() - 1].pt != 
//	 m_controlPoints[0].pt)
//	goto newcommand;

 //      if(cmd.liner != m_lineColor.r)
// 	goto newcommand;
//       if(cmd.lineg != m_lineColor.g)
// 	goto newcommand;
//       if(cmd.lineb != m_lineColor.b)
// 	goto newcommand;

//       if(cmd.fillr != m_fillColor.r)
// 	goto newcommand;
//       if(cmd.fillg != m_fillColor.g)
// 	goto newcommand;
//       if(cmd.fillb != m_fillColor.b)
// 	goto newcommand;

//       if(cmd.ctm != m_graphicsMatrix)
// 	goto newcommand;

//       // After all that, we can now append the command to the previous top
//       // of the stack... We ask the document to make the change, because doing
//       // it ourselves is considered bad form...
//       debug(dlTrace, "Joining command");
//       debug(dlTrace, string("Length before: ") + 
// 	    toString(cmd.controlPoints.size()));

//       for(unsigned int i = 1; i < m_controlPoints.size(); i++)
// 	cmd.controlPoints.push_back(m_controlPoints[i]);
//       m_doc->rewriteCommand(m_pageno, page.getCommandCount() - 1,
// 			    (object::commandType) cmd.type, cmd.controlPoints);
      
//       debug(dlTrace, string("Length after: ") + 
// 	    toString(cmd.controlPoints.size()));
//       m_controlPoints.clear();
//       return;
//     }

//  newcommand:
//   debug(dlTrace, "Command not eligible for join");
//   command newcmd;
//   newcmd.unique = gUniqueSelection++;
//   newcmd.controlPoints = m_controlPoints;
//   newcmd.type = type;
//   newcmd.liner = m_lineColor.r;
//   newcmd.lineg = m_lineColor.g;
//   newcmd.lineb = m_lineColor.b;
//   newcmd.fillr = m_fillColor.r;
//   newcmd.fillg = m_fillColor.g;
//   newcmd.fillb = m_fillColor.b;
//   newcmd.rast = m_raster;
//   newcmd.ctm = m_graphicsMatrix;

//   newcmd.text = m_text;
//   newcmd.font = m_font;
//   newcmd.size = m_size;
//   newcmd.textMatrix = m_textMatrix;

//   // Ask the document to append the command
//   debug(dlTrace, string("Appending a command with the unique id ") +
// 	toString(newcmd.unique));
//   debug(dlTrace, string("m_text is: " + m_text));
//   m_doc->appendCommand(m_pageno, newcmd);
//   m_controlPoints.clear();

//   // TODO: Free m_raster
// }

wxPoint
pdfRender::translateGraphicsPoint(wxPoint in)
{
  wxPoint out;
  // TODO mikal: rotation not implemented...
  out.x = in.x + (int) m_graphicsMatrix.getHorizontal();
  out.y = in.y + (int) m_graphicsMatrix.getVertical();
  return out;
}
