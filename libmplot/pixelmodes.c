/******************************************************************************
  Copyright (C) Michael Still 2002, 2004
  
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
#include <libmplot.h>

int
main (int argc, char *argv[])
{
  FILE *image;
  unsigned int i, pm;
  unsigned long gnux, gnuy;
  png_uint_32 rowbytes;
  png_structp png;
  png_infop info;
  png_bytepp row_pointers = NULL;
  plot_state *graph;
  char *raster, *gnu, *newgnu, *input, *newinput;
  int gnubitdepth, gnuchannels;

  if((graph = plot_newplot(420, 120)) == NULL){
    fprintf(stderr, "Could not allocate a new plot\n");
    exit(1);
  }

  // An example of the behaviours of the various pixel modes
  plot_setlinewidth(graph, 4, 4);
  for(pm = 0; pm < 3; pm++)
    {
      plot_setpixelmode(graph, pm);
      plot_setlinecolor(graph, 255, 0, 0);
      for(i = 10; i < 111; i += 10)
	{
	  plot_setlinestart(graph, i + pm * 150, 10);
	  plot_addlinesegment(graph, i + pm * 150, 110);
	  plot_strokeline(graph);
	  plot_endline(graph);
	}
      
      plot_setlinecolor(graph, 0, 255, 0);
      for(i = 10; i < 111; i += 10)
	{
	  plot_setlinestart(graph, 10 + pm * 150, i);
	  plot_addlinesegment(graph, 110 + pm * 150, i);
	  plot_strokeline(graph);
	  plot_endline(graph);
	}
      
      plot_setlinecolor(graph, 0, 0, 255);
      for(i = 10; i < 111; i += 10)
	{
	  plot_setlinestart(graph, i + pm * 150, 10);
	  plot_addlinesegment(graph, 110 + pm * 150, i);
	  plot_strokeline(graph);
	  plot_endline(graph);
	}
    }

  // Dump the image to the file
  raster = plot_getraster(graph);

  if((image = fopen("output.png", "wb")) == NULL){
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
  png_set_IHDR (png, info, 420, 120, 8, PNG_COLOR_TYPE_RGB,
                PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                PNG_FILTER_TYPE_DEFAULT);
  png_write_info (png, info);

  // Write the image out
  if((row_pointers = malloc (120 * sizeof (png_bytep))) == NULL){
    fprintf(stderr, "Could not allocate memory\n");
    exit(42);
  }

  for(i = 0; i < 120; i++){
    row_pointers[i] = raster + (i * 420 * 3);
  }

  png_write_image (png, row_pointers);

  // Cleanup
  png_write_end (png, info);
  png_destroy_write_struct (&png, &info);
  fclose(image); 
}
