#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "render.h"
#include "utility.h"
#include "stringArray.h"
#include "configuration.h"

pdfRender::pdfRender(pdf& thePDF, object page):
  m_page(page),
  m_contents(-1, -1),
  m_mode(rmGraphics),
  m_invalid(true),
  m_plot(NULL),
  m_pdf(thePDF),
  m_hasLine(false)
{
  if(!m_page.hasDictItem(dictitem::diTypeObjectReference, "Contents")){
    fprintf(stderr, "Bad PDF: Page is blank\n");
  }
  if(!m_page.getDict().getValue("Contents", m_pdf, m_contents)){
    fprintf(stderr, "Bad PDF: Could not get contents object, but the page references it!\n");
  }

  m_invalid = false;
}

// Not yet implemented (private)
pdfRender::pdfRender(const pdfRender& other):
  m_page(-1, -1),
  m_contents(-1, -1),
  m_pdf(""),
  m_hasLine(false)
{}

bool pdfRender::render()
{
  // Find the size of the page and setup a plot
  // todo_mikal: fix this
  if(m_plot != NULL)
    printf("DEBUG: Memory leak as render called more than once\n");
  
  string mediaBox;
  m_page.getDict().getValue("MediaBox", mediaBox);
  stringArray boxArgs(mediaBox.substr(1, mediaBox.length() - 2), " ");

  m_width = atoi(boxArgs[2].c_str());
  m_height = atoi(boxArgs[3].c_str());
  printf("DEBUG: Page size is %d by %d\n", m_width, m_height);
  if((m_plot = plot_newplot(m_width, m_height)) == NULL){
    fprintf(stderr, "Failed to initialize new page plot\n");
    return false;
  }

  // Read the stream and action any commands
  unsigned int inset = 0;
  char *stream = m_contents.getStream();

  printf("DEBUG: Process page stream\n");

  // todo_mikal: this might be too slow because of the accessor
  string line;
  while(inset < m_contents.getStreamLength())
    {
      if(stream[inset] != '\n'){
	line += stream[inset];
      }
      else{
	// Process the line
	printf("DEBUG: Process line \"%s\"\n", line.c_str());
	processLine(line);
	line = "";
      }

      inset ++;
    }
  
  return true;
}

void pdfRender::processLine(string line)
{
  if(line.length() < 1){
    printf("DEBUG: Empty line\n");
    return;
  }

  stringArray tokens(line, " ");
  string sarg;
  bool sargMode(false);
  for(unsigned int i = 0; i < tokens.size(); i++)
    {
      if(tokens[i] == ""){}
      else if(tokens[i][0] == '('){
	sarg = tokens[i].substr(1, tokens[i].length() - 1);
	sargMode = true;
	printf("DEBUG: String argument started\n");
      }
      else if(tokens[i][tokens[i].length() - 1] == ')'){
	sarg += " " + tokens[i].substr(0, tokens[i].length() - 1);
	sargMode = false;
	printf("DEBUG: String arguement finished (%s)\n", sarg.c_str());
	pushArguement(sarg);
      }
      else if(sargMode) sarg += " " + tokens[i];

      else if(isNumber(tokens[i])) pushArguement(tokens[i]);
      else if(isName(tokens[i])) pushArguement(tokens[i]);
      else if("b" == tokens[i]) command_b();
      else if("b*" == tokens[i]) command_bstar();
      else if("B" == tokens[i]) command_B();
      else if("B*" == tokens[i]) command_Bstar();
      else if("BT" == tokens[i]) command_BT();
      else if("c" == tokens[i]) command_c();
      else if("ET" == tokens[i]) command_ET();
      else if("h" == tokens[i]) command_h();
      else if("l" == tokens[i]) command_l();
      else if("m" == tokens[i]) command_m();
      else if("q" == tokens[i]) command_q();
      else if("Q" == tokens[i]) command_Q();
      else if("re" == tokens[i]) command_re();
      else if("rg" == tokens[i]) command_rg();
      else if("rg" == tokens[i]) command_RG();
      else if("S" == tokens[i]) command_S();
      else if("Td" == tokens[i]) command_Td();
      else if("Tf" == tokens[i]) command_Tf();
      else if("Tj" == tokens[i]) command_Tj();
      else if("Tm" == tokens[i]) command_Tm();
      else if("Tr" == tokens[i]) command_Tr();
      else if("v" == tokens[i]) command_v();
      else if("w" == tokens[i]) command_w();
      else if("y" == tokens[i]) command_y();
      else
	printf("DEBUG: Dropped token %s\n", tokens[i].c_str());
    }
  if(sargMode){
    printf("DEBUG: String mode never ended on line. Line so far: %s\n", sarg.c_str());
  }
}

/////////////////////////////////////////////////////////////////////
void pdfRender::pushArguement(string arg)
{
  printf("DEBUG: Pushing arguement %s\n", arg.c_str());
  m_arguements.push(arg);
}

/////////////////////////////////////////////////////////////////////
// Fill and stroke with non-zero winding rule
void pdfRender::command_b()
{
  printf("DEBUG: b -- non zero winding fill not implemented, using even odd\n");
  command_bstar();
}

void pdfRender::command_B()
{
  printf("DEBUG: B -- non zero winding fill not implemented, using even odd\n");
  command_Bstar();
}

void pdfRender::command_bstar()
{
  if(m_mode != rmGraphics){
    printf("DEBUG: Not in graphics mode\n");
    return;
  }
  
  plot_closeline(m_plot);
  plot_strokeline(m_plot);
  plot_fillline(m_plot);
  m_hasLine = true;
}

void pdfRender::command_Bstar()
{
  if(m_mode != rmGraphics){
    printf("DEBUG: Not in graphics mode\n");
    return;
  }
  
  plot_strokeline(m_plot);
  plot_fillline(m_plot);
  m_hasLine = true;
}

// Enter text mode
void pdfRender::command_BT()
{
  printf("DEBUG: BT\n");
  if(m_mode == rmText){
    printf("DEBUG: Error -- already in text mode\n");
    return;
  }

  m_mode = rmText;
  m_textMatrix.setIdentity();
}

void pdfRender::command_c()
{
  unsigned int x1, y1, x2, y2, x3, y3;

  // Pop our arguements (reverse order)
  y3 = m_height - atoi(m_arguements.top().c_str());
  m_arguements.pop();
  x3 = atoi(m_arguements.top().c_str());
  m_arguements.pop();
  y2 = m_height - atoi(m_arguements.top().c_str());
  m_arguements.pop();
  x2 = atoi(m_arguements.top().c_str());
  m_arguements.pop();
  y1 = m_height - atoi(m_arguements.top().c_str());
  m_arguements.pop();
  x1 = atoi(m_arguements.top().c_str());
  m_arguements.pop();

  if(m_mode != rmGraphics){
    printf("DEBUG: Not in graphics mode\n");
    return;
  }
  
  printf("DEBUG: Add cubic curve segment %d, %d & %d, %d & %d, %d\n", 
	 x1, y1, x2, y2, x3, y3);
  plot_addcubiccurvesegment(m_plot, x1, y1, x2, y2, x3, y3);
  m_hasLine = true;
}

// Exit text mode
void pdfRender::command_ET()
{
  printf("DEBUG: ET\n");
  if(m_mode != rmText){
    printf("DEBUG: Error -- exitting non existant text mode\n");
  }
  m_mode = rmGraphics;
}

// Close line
void pdfRender::command_h()
{
  if(m_mode != rmGraphics){
    printf("DEBUG: Not in graphics mode\n");
    return;
  }
  
  printf("DEBUG: Close line\n");
  plot_closeline(m_plot);
  m_hasLine = true;
}

void pdfRender::command_l()
{
  unsigned int x, y;

  // Pop our arguements (reverse order)
  y = m_height - atoi(m_arguements.top().c_str());
  m_arguements.pop();
  x = atoi(m_arguements.top().c_str());
  m_arguements.pop();

  if(m_mode != rmGraphics){
    printf("DEBUG: Not in graphics mode\n");
    return;
  }
  
  printf("DEBUG: Add line segment %d, %d\n", x, y);
  plot_addlinesegment(m_plot, x, y);
  m_hasLine = true;
}

// Move graphics cursor to a given location
void pdfRender::command_m()
{
  unsigned int x, y;

  // Pop our arguements (reverse order)
  y = m_height - atoi(m_arguements.top().c_str());
  m_arguements.pop();
  x = atoi(m_arguements.top().c_str());
  m_arguements.pop();

  if(m_mode != rmGraphics){
    printf("DEBUG: Not in graphics mode\n");
    return;
  }
  
  if(m_hasLine)
    plot_endline(m_plot);
  printf("DEBUG: Move to %d, %d\n", x, y);
  plot_setlinestart(m_plot, x, y);
  m_hasLine = true;
}

void pdfRender::command_q()
{
  printf("DEBUG: Save graphics state [not implemented]\n");
}

void pdfRender::command_Q()
{
  printf("DEBUG: Restore graphics state [not implemented]\n");
}

// A rectangle
void pdfRender::command_re()
{
  unsigned int left, top, right, bottom;

  // Pop our arguements (reverse order)
  bottom = m_height - atoi(m_arguements.top().c_str());
  m_arguements.pop();
  right = atoi(m_arguements.top().c_str());
  m_arguements.pop();
  top = m_height - atoi(m_arguements.top().c_str());
  m_arguements.pop();
  left = atoi(m_arguements.top().c_str());
  m_arguements.pop();

  if(m_mode != rmGraphics){
    printf("DEBUG: Not in graphics mode\n");
    return;
  }
  
  printf("DEBUG: Rectangle %d, %d & %d, %d\n", 
	 left, top, right, bottom);
  plot_rectangle(m_plot, left, top, right, bottom);
  m_hasLine = true;
}

// Set RGB fill color
void pdfRender::command_rg()
{
  unsigned int r, g, b;

  // Pop our arguements (reverse order)
  b = atoi(m_arguements.top().c_str());
  m_arguements.pop();
  g = atoi(m_arguements.top().c_str());
  m_arguements.pop();
  r = atoi(m_arguements.top().c_str());
  m_arguements.pop();

  if(m_mode != rmGraphics){
    printf("DEBUG: Not in graphics mode\n");
    return;
  }
  
  printf("DEBUG: RGB fill color %d %d %d\n", r, g, b);
  plot_setfillcolor(m_plot, r, g, b);
  m_hasLine = true;
}

// Set RGB fill color
void pdfRender::command_RG()
{
  unsigned int r, g, b;

  // Pop our arguements (reverse order)
  b = atoi(m_arguements.top().c_str());
  m_arguements.pop();
  g = atoi(m_arguements.top().c_str());
  m_arguements.pop();
  r = atoi(m_arguements.top().c_str());
  m_arguements.pop();

  if(m_mode != rmGraphics){
    printf("DEBUG: Not in graphics mode\n");
    return;
  }
  
  printf("DEBUG: RGB line color %d %d %d\n", r, g, b);
  plot_setlinecolor(m_plot, r, g, b);
  m_hasLine = true;
}

void pdfRender::command_S()
{
  plot_strokeline(m_plot);
}


// Move text position
void pdfRender::command_Td()
{
  printf("DEBUG: Td [not implemented]\n");
}

void pdfRender::command_Tf()
{
  string fontName, fontSize;

  // Pop our arguements (reverse order)
  fontSize = m_arguements.top();
  m_arguements.pop();
  fontName = m_arguements.top();
  m_arguements.pop();

  printf("DEBUG: Tf font = %s size = %s\n", fontName.c_str(), fontSize.c_str());

  // Find the named font
  dictionary resources;
  dictionary fonts;
  object font(-1, -1);
  string fontResource, fontFile("px10.ttf");
  bool fontFound(false);

  if(!m_page.getDict().getValue("Resources", resources)){
    printf("DEBUG: Font not found (no resources)\n");
  }
  else if(!resources.getValue("Font", fonts)){
    printf("DEBUG: Font not found (no font entry in resources)\n");
  }
  else if(!fonts.getValue(fontName.substr(1, fontName.length()), m_pdf, font)){
    printf("DEBUG: Font not found (named font not listed in resources)\n");
  }
  else if(font.getDict().getValue("BaseFont", fontResource)){
    fontFound = true;
  }

  // Now map this name to a TrueType file somewhere on the system
  if(fontFound){
    printf("DEBUG: Lookup font %s\n", fontResource.c_str());
    configuration *config;
    config = (configuration *) & configuration::getInstance();
    config->getValue(string("basefont-") + fontResource + "-map", "px10.ttf",
		     fontFile);
  }
  
  printf("DEBUG: Using font filename %s\n", fontFile.c_str());
  plot_setfont(m_plot, (char *) fontFile.c_str(), atoi(fontSize.c_str()));
}

// Show the text
void pdfRender::command_Tj()
{
  printf("DEBUG: Tj %s\n", m_arguements.top().c_str());
  plot_settextlocation(m_plot, (unsigned int) m_textMatrix.getHorizontal(),
		       (unsigned int) (m_height - m_textMatrix.getVertical()));
  plot_writestring(m_plot, (char *) m_arguements.top().c_str());
  m_arguements.pop();
}

// Set the text matrix
void pdfRender::command_Tm()
{
  float vals[6];

  printf("DEBUG: Tm\n");
  for(int i = 0; i < 6; i++){
    vals[5 - i] = atof(m_arguements.top().c_str());
    m_arguements.pop();
  }

  m_textMatrix.setValues(vals);
}

void pdfRender::command_Tr()
{
  printf("DEBUG: Tr [not implemented]\n");
}

void pdfRender::command_v()
{
  unsigned int x1, y1, x2, y2;

  // Pop our arguements (reverse order)
  y2 = m_height - atoi(m_arguements.top().c_str());
  m_arguements.pop();
  x2 = atoi(m_arguements.top().c_str());
  m_arguements.pop();
  y1 = m_height - atoi(m_arguements.top().c_str());
  m_arguements.pop();
  x1 = atoi(m_arguements.top().c_str());
  m_arguements.pop();

  if(m_mode != rmGraphics){
    printf("DEBUG: Not in graphics mode\n");
    return;
  }
  
  printf("DEBUG: Add cubic curve segment %d, %d & %d, %d\n", 
	 x1, y1, x2, y2);
  plot_addquadraticcurvesegmentone(m_plot, x1, y1, x2, y2);
  m_hasLine = true;
}

void pdfRender::command_w()
{
  int w;

  // Pop arguement
  w = atoi(m_arguements.top().c_str());
  m_arguements.pop();
  
  if(m_mode != rmGraphics){
    printf("DEBUG: Not in graphics mode\n");
    return;
  }

  plot_setlinewidth(m_plot, w, w);
}

void pdfRender::command_y()
{
  unsigned int x1, y1, x2, y2;

  // Pop our arguements (reverse order)
  y2 = m_height - atoi(m_arguements.top().c_str());
  m_arguements.pop();
  x2 = atoi(m_arguements.top().c_str());
  m_arguements.pop();
  y1 = m_height - atoi(m_arguements.top().c_str());
  m_arguements.pop();
  x1 = atoi(m_arguements.top().c_str());
  m_arguements.pop();

  if(m_mode != rmGraphics){
    printf("DEBUG: Not in graphics mode\n");
    return;
  }
  
  printf("DEBUG: Add cubic curve segment %d, %d & %d, %d\n", 
	 x1, y1, x2, y2);
  plot_addquadraticcurvesegmenttwo(m_plot, x1, y1, x2, y2);
  m_hasLine = true;
}

string pdfRender::getPNGfile()
{
  if(m_invalid){
    printf("DEBUG: Page render is invalid\n");
    return "";
  }

  FILE *image;
  unsigned int i;
  png_uint_32 rowbytes;
  png_structp png;
  png_infop info;
  png_bytepp row_pointers = NULL;
  char *raster;

  raster = plot_getraster(m_plot);

  // Build a random filename
  char *cfname = strdup("/tmp/pandaedit.XXXXXX");;
  int fd;
  fd = mkstemp(cfname);
  close(fd);
  string tempName(cfname);
  free(cfname);

  if((image = fopen(tempName.c_str(), "wb")) == NULL){
    fprintf(stderr, "Could not open the output image\n");
    return "";
  }

  // Get ready for writing
  if ((png =
       png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL,
                                NULL)) == NULL){
    fprintf(stderr, "Could not create write structure for PNG (out of memory?)");
    return "";
  }

  // Get ready to specify important stuff about the image
  if ((info = png_create_info_struct (png)) == NULL){
    fprintf(stderr,
            "Could not create PNG info structure for writing (out of memory?)");
    return "";
  }

  if (setjmp (png_jmpbuf (png))){
    fprintf(stderr, "Could not set the PNG jump value for writing");
    return "";
  }

  // This is needed before IO will work (unless you define callbacks)
  png_init_io(png, image);

  // Define important stuff about the image
  info->channels = 3;
  info->pixel_depth = 24;
  png_set_IHDR (png, info, m_width, m_height, 8, PNG_COLOR_TYPE_RGB,
                PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                PNG_FILTER_TYPE_DEFAULT);
  png_write_info (png, info);

  // Write the image out
  if((row_pointers = (png_byte **) malloc (m_height * sizeof (png_bytep))) == NULL){
    fprintf(stderr, "Could not allocate memory\n");
    exit(42);
  }

  for(i = 0; i < m_height; i++){
    row_pointers[i] = (png_byte *) (raster + (i * m_width * 3));
  }

  png_write_image (png, row_pointers);

  // Cleanup
  png_write_end (png, info);
  png_destroy_write_struct (&png, &info);
  fclose(image); 

  return tempName;
}

