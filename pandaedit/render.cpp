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
  m_textLineMatrix.setIdentity();
}

bool
pdfRender::render ()
{
  debug(dlTrace, "Starting render");
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
      if ((m_plot = plot_newplot ((unsigned int) (m_width * m_xscale), 
				  (unsigned int) (m_height * m_yscale))) 
	  == NULL)
	{
	  debug(dlError, "Failed to initialize new page plot");
	  return false;
	}

      if ((m_select = plot_newplot ((unsigned int) (m_width * m_xscale), 
				    (unsigned int) (m_height * m_yscale))) 
	  == NULL)
	{
	  debug(dlError, "Failed to initialize new page plot");
	  return false;
	}
      plot_setlinewidth(m_select, SELECTWIDTH, SELECTWIDTH);
#else
      debug(dlError, "Libmplot not found at configure time. Graphics "
	    "functionality is therefore not available");
      return false;
#endif
    }

  // There is an initial command state which we grab now
  char *plotState;
  plotState = plot_persiststate(m_plot);
  m_graphicsStates.push(plotState);
  free(plotState);

  // If we don't have any preparsed commands, then we need to hit the document
  // and find the drawing commands. This puts them into the page command array
  object pobj(objNumNoSuch, objNumNoSuch);
  object& page = pobj;
  if(!m_doc->getPage(m_pageno, page))
    return false;

  if(page.getCommandCount() == 0)
    {
      // Stopped checking the error return here, as no drawing commands now
      // results in just an empty PNG file
      parseStream();
    }

  // Now we need to process each of the elements in the command array
  page = pobj;
  if(!m_doc->getPage(m_pageno, page))
    return false;
  debug(dlTrace, "Commence rendering the preprocessed commands");
  for(int cmdcnt = 0; cmdcnt < page.getCommandCount(); cmdcnt++)
    {
      debug(dlTrace, string("Processing command: ") + toString(cmdcnt));
      if(m_select)
	{
	  debug(dlTrace, "Set up selection hinting for object");
	  int cmdid = page.getCommandId(cmdcnt);
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

      // Set the colours for the object
      debug(dlTrace, "Set object colours");
      int r, g, b;
      page.getCommandLineColor(cmdcnt, r, g, b);
      plot_setlinecolor(m_plot, r, g, b);
      page.getCommandFillColor(cmdcnt, r, g, b);
      plot_setfillcolor(m_plot, r, g, b);

      // Set the CTM
      matrix ctm;
      page.getCommandCTM(cmdcnt, ctm);
      
      debug(dlTrace, string("Setting CTM to ") +
	    toString(ctm.getRawItem(0, 0)) + string(" ") +
	    toString(ctm.getRawItem(1, 1)) + string(" ") +
	    toString(ctm.getRawItem(0, 1)) + string(" ") +
	    toString(ctm.getRawItem(1, 1)) + string(" ") +
	    toString(ctm.getRawItem(0, 2)) + string(" ") +
	    toString(ctm.getRawItem(1, 2)));

      plot_setctm(m_plot, 
		  ctm.getRawItem(0, 0), ctm.getRawItem(1, 1),
		  ctm.getRawItem(0, 1), ctm.getRawItem(1, 1),
		  ctm.getRawItem(0, 2), ctm.getRawItem(1, 2));
      plot_setctm(m_select, 
		  ctm.getRawItem(0, 0), ctm.getRawItem(1, 1),
		  ctm.getRawItem(0, 1), ctm.getRawItem(1, 1),
		  ctm.getRawItem(0, 2), ctm.getRawItem(1, 2));

      // Paint the object
      debug(dlTrace, "Paint object");
      object::commandType type;
      vector<cmdControlPoint> controlPoints = 
	page.getCommandPoints(cmdcnt, type);
      
#define SCALEDPT(in, mem) \
      (unsigned int) (controlPoints[in].mem.x * m_xscale), \
      (unsigned int) (controlPoints[in].mem.y * m_yscale)

      if(controlPoints.size() > 0)
	{
	  debug(dlTrace, "The object contains control points");
	  char *plotState;

	  // HINT: implement something for your new command here...
	  switch(type)
	    {
	    case object::cLine:
	      debug(dlTrace, "Object is a line");
	      if(controlPoints.size() > 1)
		{
		  plot_setlinestart(m_plot, SCALEDPT(0, pt));
		  plot_setlinestart(m_select, SCALEDPT(0, pt));
		  debug(dlTrace, string("Line start: ") + 
			toString(controlPoints[0].pt.x) + string(", ") +
			toString(controlPoints[0].pt.y) + 
			string(" modifier: ") + 
			toString(controlPoints[0].modifier));

		  for(unsigned int i = 1; i < controlPoints.size(); i++)
		    {
		      debug(dlTrace, string("Line segment: ") + 
			toString(controlPoints[i].pt.x) + string(", ") +
			toString(controlPoints[i].pt.y) + 
			string(" modifier: ") + 
			toString(controlPoints[i].modifier));

		      switch(controlPoints[i].modifier)
			{
			case ltmNone:
			  plot_addlinesegment(m_plot, SCALEDPT(i, pt));
			  plot_addlinesegment(m_select, SCALEDPT(i, pt));
			  break;

			case ltmBezier:
			  // TODO mikal: bad assumption as to number of
			  // control points...
			  plot_addcubiccurvesegment(m_plot, 
						    SCALEDPT(i, pt),
						    SCALEDPT(i, cpt[0]),
						    SCALEDPT(i, cpt[1]));
			  plot_addcubiccurvesegment(m_select, 
						    SCALEDPT(i, pt),
						    SCALEDPT(i, cpt[0]),
						    SCALEDPT(i, cpt[1]));
			  break;

			case ltmHalfBezierOne:
			  // TODO mikal: bad assumption as to number of
			  // control points...
			  plot_addquadraticcurvesegmentone(m_plot, 
							   SCALEDPT(i, pt),
							   SCALEDPT(i, cpt[0]));
			  plot_addquadraticcurvesegmentone(m_select, 
							   SCALEDPT(i, pt),
							   SCALEDPT(i, cpt[0]));
			  break;

			case ltmHalfBezierTwo:
			  // TODO mikal: bad assumption as to number of
			  // control points...
			  plot_addquadraticcurvesegmenttwo(m_plot, 
							   SCALEDPT(i, pt),
							   SCALEDPT(i, cpt[0]));
			  plot_addquadraticcurvesegmenttwo(m_select, 
							   SCALEDPT(i, pt),
							   SCALEDPT(i, cpt[0]));
			  break;

			case ltmJump:
			  // TODO mikal: what about unstroked lines?
			  debug(dlTrace, "Start stroking jump");
			  plot_strokeline(m_plot);
			  plot_strokeline(m_select);
			  debug(dlTrace, "Finish stroking");
			  plot_endline(m_plot);
			  plot_endline(m_select);
			  debug(dlTrace, "Clean up");

			  plot_setlinestart(m_plot, SCALEDPT(i, pt));
			  plot_setlinestart(m_select, SCALEDPT(i, pt));
			  break;

			default:
			  debug(dlError, "Unknown line modifier");
			  break;
			}
		    }

		  // TODO mikal: what about unstroked lines?
		  debug(dlTrace, "Start stroking");
		  plot_strokeline(m_plot);
		  plot_strokeline(m_select);
		  debug(dlTrace, "Finish stroking");
		  plot_endline(m_plot);
		  plot_endline(m_select);
		  debug(dlTrace, "Clean up");
		}
	      break;

	    case object::cImage:
	      debug(dlTrace, "Object is an image");
	      if(controlPoints.size() != 2)
		{
		  debug(dlError, 
			"Wrong number of control points for an image");
		}
	      else
		{
		  //		  if(page.getCommandRaster(cmdcnt) != NULL)
		    //		    plot_overlayraster(m_plot, 
		    //	       (char *) page.getCommandRaster(cmdcnt), 
		    //	       controlPoints[0].pt.x, 
		    //	       controlPoints[0].pt.y,
		    //	       controlPoints[1].pt.x, 
		    //	       controlPoints[1].pt.y,
		    //	       controlPoints[1].pt.x, 
		    //	       controlPoints[1].pt.y,
		    //	       0);
		  //else
		  //debug(dlTrace, "Image had a NULL raster");
		}
	      break;

	    case object::cSaveState:
	      debug(dlTrace, "Object is a state save");
	      plotState = plot_persiststate(m_plot);
	      m_graphicsStates.push(plotState);
	      free(plotState);
	      break;

	    case object::cRestoreState:
	      debug(dlTrace, "Object is a state restore");
	      if(m_graphicsStates.size() > 0)
		{
		  plot_applystate(m_plot, 
				  (char *) m_graphicsStates.top().c_str());
		  m_graphicsStates.pop();
		}
	      else{
		debug(dlTrace, "No state to apply");
	      }
	      break;

	    default:
	      debug(dlError, "Unknown drawing command");
	    }
	}
    }

  debug(dlTrace, "Finished painting");
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

void
pdfRender::appendCommand(object::commandType type)
{
  if(m_controlPoints.size() == 0)
    return;

  // Determine if this command is an append target
  command cmd;
  object pobj(objNumNoSuch, objNumNoSuch);
  object& page = pobj;
  if(!m_doc->getPage(m_pageno, page))
    return;

  if(page.getLastCommand(cmd))
    {
      // Only supported for line commands
      if(cmd.type == object::cLine)
	goto newcommand;

      if(cmd.controlPoints.size() == 0)
	goto newcommand;
      if(cmd.type != type)
	goto newcommand;

      // TODO mikal: this line needs to be brought up to speed with the
      // new cmdControlPoint structure...
      if(cmd.controlPoints[cmd.controlPoints.size() - 1].pt != 
	 m_controlPoints[0].pt)
	goto newcommand;

      if(cmd.liner != m_lineColor.r)
	goto newcommand;
      if(cmd.lineg != m_lineColor.g)
	goto newcommand;
      if(cmd.lineb != m_lineColor.b)
	goto newcommand;

      if(cmd.fillr != m_fillColor.r)
	goto newcommand;
      if(cmd.fillg != m_fillColor.g)
	goto newcommand;
      if(cmd.fillb != m_fillColor.b)
	goto newcommand;

      if(cmd.ctm != m_graphicsMatrix)
	goto newcommand;

      // After all that, we can now append the command to the previous top
      // of the stack... We ask the document to make the change, because doing
      // it ourselves is considered bad form...
      debug(dlTrace, "Joining command");
      debug(dlTrace, string("Length before: ") + 
	    toString(cmd.controlPoints.size()));

      for(unsigned int i = 1; i < m_controlPoints.size(); i++)
	cmd.controlPoints.push_back(m_controlPoints[i]);
      m_doc->rewriteCommand(m_pageno, page.getCommandCount() - 1,
			    (object::commandType) cmd.type, cmd.controlPoints);
      
      debug(dlTrace, string("Length after: ") + 
	    toString(cmd.controlPoints.size()));
      m_controlPoints.clear();
      return;
    }

 newcommand:
  debug(dlTrace, "Command not eligible for join");
  command newcmd;
  newcmd.unique = gUniqueSelection++;
  newcmd.controlPoints = m_controlPoints;
  newcmd.type = type;
  newcmd.liner = m_lineColor.r;
  newcmd.lineg = m_lineColor.g;
  newcmd.lineb = m_lineColor.b;
  newcmd.fillr = m_fillColor.r;
  newcmd.fillg = m_fillColor.g;
  newcmd.fillb = m_fillColor.b;
  newcmd.rast = m_raster;
  newcmd.ctm = m_graphicsMatrix;

  // Ask the document to append the command
  debug(dlTrace, string("Appending a command with the unique id ") +
	toString(newcmd.unique));
  m_doc->appendCommand(m_pageno, newcmd);
  m_controlPoints.clear();
}

wxPoint
pdfRender::translateGraphicsPoint(wxPoint in)
{
  wxPoint out;
  // TODO mikal: rotation not implemented...
  out.x = in.x + (int) m_graphicsMatrix.getHorizontal();
  out.y = in.y + (int) m_graphicsMatrix.getVertical();
  return out;
}
