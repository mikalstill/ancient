/******************************************************************************
  Copyright (C) Michael Still 2003
  
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


  Command line arguements are:
    - output filename
    - font path
    - font size
    - character (just one!)
******************************************************************************/

#include <stdio.h>
#include <png.h>
#include <libmplot.h>

int
main (int argc, char *argv[])
{
  FILE *image;
  unsigned int i, x, y;
  png_uint_32 rowbytes;
  png_structp png;
  png_infop info;
  png_bytepp row_pointers = NULL;
  plot_state *graph, *enlarged;
  char *raster, *enlargedraster;
  unsigned int height, width, dheight, dwidth;

  if(argc != 5){
    fprintf(stderr, "Wrong number of arguements: %d\n", argc);
    exit(42);
  }
  fprintf(stderr, "Output file = %s\n", argv[1]);
  fprintf(stderr, "Font = %s\n", argv[2]);
  fprintf(stderr, "Size = %s\n", argv[3]);
  fprintf(stderr, "Character = %s\n", argv[4]);

  if((graph = plot_newplot(500, 500)) == NULL){
    fprintf(stderr, "Could not allocate a new plot\n");
    exit(1);
  }

  plot_setfont (graph, argv[2], atoi(argv[3]));
  plot_settextlocation (graph, 10, atoi(argv[3]));
  plot_writestring (graph, argv[4]);
  width =  plot_stringwidth (graph, argv[4]);
  dwidth = width * 10 + 10;
  height = atoi(argv[3]) * 1.5; // todo_mikal: this is a hack
  dheight = height * 10 + 10;
  
  if((enlarged = plot_newplot(dwidth, dheight)) == NULL){
    fprintf(stderr, "Could not allocate a new plot\n");
    exit(1);
  }

  // And now draw the character...
  raster = plot_getraster(graph);
  for(x = 0; x < width; x++){
    for(y = 0; y < height; y++){
      if(raster[(y * 500 + x + 10) * 3] == 0){
	plot_setlinestart(enlarged, x * 10 + 5, y * 10 + 5);
	plot_addlinesegment(enlarged, x * 10 + 15, y * 10 + 15);
	plot_strokeline(enlarged);
	plot_endline(enlarged);
	
	plot_setlinestart(enlarged, x * 10 + 5, y * 10 + 15);
	plot_addlinesegment(enlarged, x * 10 + 15, y * 10 + 5);
	plot_strokeline(enlarged);
	plot_endline(enlarged);
      }
    }
  }

  // And now draw a light gray grid over the top so that the "pixels" are clear
  plot_setlinecolor(enlarged, 230, 230, 230);
  for(x = 5; x < dwidth; x += 10){
    plot_setlinestart(enlarged, x, 0);
    plot_addlinesegment(enlarged, x, dheight);
    plot_strokeline(enlarged);
    plot_endline(enlarged);
  }
  for(y = 5; y < dheight; y += 10){
    plot_setlinestart(enlarged, 0, y);
    plot_addlinesegment(enlarged, dwidth, y);
    plot_strokeline(enlarged);
    plot_endline(enlarged);
  }

  // A boarder
  plot_setlinecolor(enlarged, 0, 0, 0);
  plot_rectangle(enlarged, 0, 0, dwidth - 1, dheight - 1);
  plot_strokeline(enlarged);
  plot_endline(enlarged);

  if((image = fopen(argv[1], "wb")) == NULL){
    fprintf(stderr, "Could not open the output image\n");
    exit(42);
  }

  // Get ready for writing
  enlargedraster = plot_getraster(enlarged);
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
  png_set_IHDR (png, info, dwidth, dheight, 8, PNG_COLOR_TYPE_RGB,
                PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                PNG_FILTER_TYPE_DEFAULT);
  png_write_info (png, info);

  // Write the image out
  if((row_pointers = malloc (dheight * sizeof (png_bytep))) == NULL){
    fprintf(stderr, "Could not allocate memory\n");
    exit(42);
  }

  for(i = 0; i < dheight; i++){
    row_pointers[i] = enlargedraster + (i * dwidth * 3);
  }

  png_write_image (png, row_pointers);

  // Cleanup
  png_write_end (png, info);
  png_destroy_write_struct (&png, &info);
  fclose(image);
}
