/******************************************************************************
  Copyright (C) Michael Still 2002
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
******************************************************************************/

#include <stdio.h>
#include <png.h>
#include <math.h>
#include <libmplot.h>

char *words[] = {"This", "is", "a", "string", "which", "is", "quite", "long.",
		 "It", "demonstrates", "how", "to", "do", "word", "wrap", 
		 NULL};

#define CHECKEXIT if(stopPoint == stopCount) goto done; stopCount++;
#define PI 3.1415

int
main (int argc, char *argv[])
{
  FILE *image;
  unsigned int i, textx, texty;
  png_uint_32 rowbytes;
  png_structp png;
  png_infop info;
  png_bytepp row_pointers = NULL;
  plot_state *graph;
  char *raster;
  float angle, ctmAngle = 0.0;
  int x, count, stopPoint = -1, stopCount = 0;

  if((graph = plot_newplot(400, 200)) == NULL){
    fprintf(stderr, "Could not allocate a new plot\n");
    exit(1);
  }

  // TODO mikal: this should use getopt one day...
  if(argc > 0){
    stopPoint = atoi(argv[1]);
  }
  if(argc > 1){
    ctmAngle = atof(argv[2]);
  }

  plot_setctm(graph, 
	      cos (ctmAngle * PI / 180.0),
	      sin (ctmAngle * PI / 180.0),
	      -sin (ctmAngle * PI / 180.0),
	      cos (ctmAngle * PI / 180.0),
	      0, 0);
	      
  // Draw some axes
  CHECKEXIT // 0
  plot_setlinecolor(graph, 0, 0, 0);
  plot_setlinestart(graph, 10, 10);
  plot_addlinesegment(graph, 10, 190);
  plot_strokeline(graph);
  plot_endline(graph);

  CHECKEXIT // 1
  plot_setlinestart(graph, 10, 100);
  plot_addlinesegment(graph, 390, 100);
  plot_strokeline(graph);
  plot_endline(graph);

  // Draw the sin graph
  CHECKEXIT // 2
  plot_setlinecolor(graph, 255, 0, 0);
  plot_setlinedash(graph, 2, "\0\1");
  plot_setlinestart(graph, 10, 100);
  x = 10;
  for(angle = 0.0; angle < 12 * 3.1415; angle += 0.1){
    x++;
    plot_addlinesegment(graph, x, 100 - sin(angle) * 90); 
  }
  plot_strokeline(graph);
  plot_endline(graph);

  // Draw the cos graph
  CHECKEXIT // 3
  plot_setlinecolor(graph, 0, 255, 0);
  plot_setlinedash(graph, 0, "");
  plot_setlinewidth(graph, 2, 10);
  plot_setlinestart(graph, 10, 100);
  x = 10;
  for(angle = 0.0; angle < 12 * 3.1415; angle += 0.1){
    x++;
    plot_addlinesegment(graph, x, 100 - cos(angle) * 90); 
  }
  plot_strokeline(graph);
  plot_endline(graph);

  // Write out some text
  CHECKEXIT // 4
  plot_setfontcolor(graph, 26, 22, 249);
  plot_setfont(graph, "n019004l.pfb", 18);
  plot_settextlocation(graph, 20, 70);
  count = 0;
  while(words[count] != NULL){
    plot_gettextlocation(graph, &textx, &texty);
    if((textx + plot_stringwidth(graph, words[count])) > 380){
      if(texty + 70 > 180)
	break;

      plot_settextlocation(graph, 20, texty + 20);
    }

    plot_writestring(graph, words[count]);
    plot_writestring(graph, " ");
    count++;
  }

  CHECKEXIT // 5
  plot_settextlocation(graph, 200, 100);
  plot_writestringrot(graph, "0 0 0", 0);
  plot_settextlocation(graph, 200, 100);
  plot_writestringrot(graph, "90 90 90", 90);
  plot_settextlocation(graph, 200, 100);
  plot_writestringrot(graph, "180 180 180", 180);
  plot_settextlocation(graph, 200, 100);
  plot_writestringrot(graph, "270 270 270", 270);

  // Write out the PNG file
 done:
  raster = plot_getraster(graph);

  if((image = fopen("graph.png", "wb")) == NULL){
    fprintf(stderr, "Could not open the output image\n");
    exit(42);
  }

  // Get ready for writing
  if ((png =
       png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL,
                                NULL)) == NULL){
    fprintf(stderr, "Could not create write structure for PNG (out of memory?)");
    exit(42);
  }

  // Get ready to specify important stuff about the image
  if ((info = png_create_info_struct (png)) == NULL){
    fprintf(stderr,
            "Could not create PNG info structure for writing (out of memory?)");
    exit(42);
  }

  if (setjmp (png_jmpbuf (png))){
    fprintf(stderr, "Could not set the PNG jump value for writing");
    exit(42);
  }

  // This is needed before IO will work (unless you define callbacks)
  png_init_io(png, image);

  // Define important stuff about the image
  info->channels = 3;
  info->pixel_depth = 24;
  png_set_IHDR (png, info, 400, 200, 8, PNG_COLOR_TYPE_RGB,
                PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                PNG_FILTER_TYPE_DEFAULT);
  png_write_info (png, info);

  // Write the image out
  if((row_pointers = malloc (200 * sizeof (png_bytep))) == NULL){
    fprintf(stderr, "Could not allocate memory\n");
    exit(42);
  }

  for(i = 0; i < 200; i++){
    row_pointers[i] = raster + (i * 400 * 3);
  }

  png_write_image (png, row_pointers);

  // Cleanup
  png_write_end (png, info);
  png_destroy_write_struct (&png, &info);
  fclose(image); 
}
