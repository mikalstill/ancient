#include <stdio.h>

#include "render.h"
#include "utility.h"
#include "stringArray.h"

pdfRender::pdfRender(pdf& thePDF, object page):
  m_page(page),
  m_contents(-1, -1),
  m_mode(rmUndefined),
  m_invalid(true),
  m_plot(NULL)
{
  if(!m_page.hasDictItem(dictitem::diTypeObjectReference, "Contents")){
    fprintf(stderr, "Bad PDF: Page is blank\n");
  }
  if(!m_page.getDict().getValue("Contents", thePDF, m_contents)){
    fprintf(stderr, "Bad PDF: Could not get contents object, but the page references it!\n");
  }

  m_invalid = false;
}

// Not yet implemented (private)
pdfRender::pdfRender(const pdfRender& other):
  m_page(-1, -1),
  m_contents(-1, -1)
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
      if(tokens[i][0] == '('){
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
      else if("BT" == tokens[i]) commandBT();
      else if("ET" == tokens[i]) commandET();
      else if("Td" == tokens[i]) commandTd();
      else if("Tf" == tokens[i]) commandTf();
      else if("Tj" == tokens[i]) commandTj();
      else if("Tm" == tokens[i]) commandTm();
      else if("Tr" == tokens[i]) commandTr();
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
// Enter text mode
void pdfRender::commandBT()
{
  printf("DEBUG: BT\n");
  if(m_mode == rmText){
    printf("DEBUG: Error -- already in text mode\n");
    return;
  }

  m_mode = rmText;
  m_textMatrix.setIdentity();
}

// Exit text mode
void pdfRender::commandET()
{
  printf("DEBUG: ET [not implmented]\n");
}

// Move text position
void pdfRender::commandTd()
{
  printf("DEBUG: Td [not implemented]\n");
}

void pdfRender::commandTf()
{
  string fontname, fontsize;

  fontsize = m_arguements.top();
  m_arguements.pop();
  fontname = m_arguements.top();
  m_arguements.pop();

  printf("DEBUG: Tf font = %s size = %s\n", fontname.c_str(), fontsize.c_str());
  plot_setfont(m_plot, "px10.ttf", atoi(fontsize.c_str()));
}

// Show the text
void pdfRender::commandTj()
{
  printf("DEBUG: Tj %s\n", m_arguements.top().c_str());
  plot_settextlocation(m_plot, (unsigned int) m_textMatrix.getHorizontal(),
		       (unsigned int) (m_height - m_textMatrix.getVertical()));
  plot_writestring(m_plot, (char *) m_arguements.top().c_str());
  m_arguements.pop();
}

// Set the text matrix
void pdfRender::commandTm()
{
  float vals[6];

  printf("DEBUG: Tm\n");
  for(int i = 0; i < 6; i++){
    vals[5 - i] = atof(m_arguements.top().c_str());
    m_arguements.pop();
  }

  m_textMatrix.setValues(vals);
}

void pdfRender::commandTr()
{
  printf("DEBUG: Tr [not implemented]\n");
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

  if((image = fopen("output.png", "wb")) == NULL){
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

  return "output.png";
}
