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
#include <libmplot.h>

#define EXPAND 10

int
main (int argc, char *argv[])
{
  FILE *image;
  unsigned int i, x, y, lightcount, lcr, lcg, lcb, lcy;
  unsigned long width, height;
  unsigned char sig[8];
  int bitdepth, colourtype;
  png_uint_32 rowbytes;
  png_structp png;
  png_infop info;
  png_bytepp row_pointers = NULL;
  plot_state *graph;
  char *raster, *simraster;
  float scale;
  int count, size;

  // Open the file
  if ((image = fopen (argv[1], "rb")) == NULL){
    fprintf(stderr, "Could not open the specified PNG file.\n");
    exit(0);
  }

  // Check that it really is a PNG file
  fread(sig, 1, 8, image);
  if(!png_check_sig(sig, 8)){
    printf("This file is not a valid PNG file\n");
    fclose(image);
    return;
  }

  // Start decompressing
  if((png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, 
				   NULL, NULL)) == NULL){
    fprintf(stderr, "Could not create a PNG read structure (out of memory?)");
    exit(0);
  }

  if((info = png_create_info_struct(png)) == NULL){
    fprintf(stderr, "Could not create PNG info structure (out of memory?)");
    exit(0);
  }

  // If pnginfo_error did not exit, we would have to call 
  // png_destroy_read_struct
  if(setjmp(png_jmpbuf(png))){
    fprintf(stderr, "Could not set PNG jump value");
    exit(0);
  }

  // Get ready for IO and tell the API we have already read the image signature
  png_init_io(png, image);
  png_set_sig_bytes(png, 8);
  png_read_info(png, info);
  png_get_IHDR(png, info, &width, &height, &bitdepth, &colourtype, NULL, 
	       NULL, NULL);
  printf("Input image is %d by %d pixels\n", width, height);

  if (colourtype == PNG_COLOR_TYPE_PALETTE)
    png_set_expand (png);
  //  if(colourtype & PNG_COLOR_MASK_ALPHA)
  png_set_strip_alpha (png);
  png_read_update_info (png, info);

  rowbytes = png_get_rowbytes (png, info);
  if((row_pointers = malloc (height * sizeof (png_bytep))) == NULL){
    fprintf(stderr, "Could not allocate memory\n");
    exit(42);
  }

  // Space for the bitmap
  if((raster = (unsigned char *) malloc ((rowbytes * height) + 1)) == NULL){
    fprintf(stderr, "Could not allocate memory\n");
    exit(42);
  }

  // Get the image bitmap
  for (i = 0; i < height; ++i)
    row_pointers[i] = raster + (i * rowbytes);
  png_read_image (png, row_pointers);
  png_read_end (png, NULL);
  fclose (image);

  // This cleans things up for us in the PNG library
  png_destroy_read_struct (&png, &info, NULL);

  /////////////////////////////////////////////////////////////////////////////
  // Now process the bitmap
  /////////////////////////////////////////////////////////////////////////////
  printf("Generating simulation\n");
  if((graph = plot_newplot(width * EXPAND + (EXPAND * 2), 
			   height * EXPAND + (EXPAND * 2))) == NULL){
    fprintf(stderr, "Could not allocate a new plot\n");
    exit(1);
  }

  lightcount = lcr = lcg = lcb = lcy = 0;
  for(y = 0; y < height; y++){
    printf("%2d: ", y);
    for(x = 0; x < width; x++){
      i = y * width + x;

      if((raster[i * 3] == raster[i * 3 + 1]) &&
	 (raster[i * 3 + 1] == raster[i * 3 + 2]) &&
	 ((unsigned char) raster[i * 3] == 255)){
	printf("---");
      }
      else{
	lightcount++;
	if((unsigned char) raster[i * 3] == 255) lcr++;
	else if((unsigned char) raster[i * 3 + 1] == 255) lcg++;
	else if((unsigned char) raster[i * 3 + 2] == 255) lcb++;
	else lcy++;

	if((argc > 2) && (strcmp(argv[2], "-d") == 0)){
	  printf("###");
	  plot_setlinestart(graph, x * EXPAND + EXPAND - 1, y * EXPAND + EXPAND - 1);
	  plot_addlinesegment(graph, x * EXPAND + EXPAND + 1, y * EXPAND + EXPAND + 1);
	  plot_strokeline(graph);
	  plot_endline(graph);

	  plot_setlinestart(graph, x * EXPAND + EXPAND - 1, y * EXPAND + EXPAND + 1);
	  plot_addlinesegment(graph, x * EXPAND + EXPAND + 1, y * EXPAND + EXPAND - 1);
	  plot_strokeline(graph);
	  plot_endline(graph);
	}
	else{
	  for(scale = 0.125; scale < 0.51; scale *= 2){
	    printf("#");
	    plot_setfillcolor(graph, 
			      (int)((unsigned char) raster[i * 3] * scale), 
			      (int)((unsigned char) raster[i * 3 + 1] * scale), 
			      (int)((unsigned char) raster[i * 3 + 2] * scale));
	    
	    size = (int) (EXPAND * scale * 2);
	    
	    plot_virtualcircle(graph, x * EXPAND + EXPAND, y * EXPAND + EXPAND, size);
	    plot_mixfillline(graph, 0.5);
	    plot_endline(graph);
	  }
	    
	  fflush(stdout);
	}
      }
      printf(" ");
    }
    printf("\n");
  }

  printf("Number of lights used: %d (%d red, %d green, %d blue, %d yellow)\n", 
	 lightcount, lcr, lcg, lcb, lcy);

  /////////////////////////////////////////////////////////////////////////////
  // Write out the light simulation
  /////////////////////////////////////////////////////////////////////////////
  printf("Writing out simulation\n");
  simraster = plot_getraster(graph);

  if((image = fopen("final.png", "wb")) == NULL){
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
  png_set_IHDR (png, info, width * EXPAND + (EXPAND * 2), 
		height * EXPAND + (EXPAND * 2), 8, PNG_COLOR_TYPE_RGB,
                PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                PNG_FILTER_TYPE_DEFAULT);
  png_write_info (png, info);

  // Write the image out
  if((row_pointers = malloc ((height * EXPAND + (EXPAND * 2)) * sizeof (png_bytep))) == NULL){
    fprintf(stderr, "Could not allocate memory\n");
    exit(42);
  }

  for(i = 0; i < height * EXPAND + (EXPAND * 2); i++){
    row_pointers[i] = simraster + (i * (width * EXPAND + (EXPAND * 2)) * 3);
  }

  png_write_image (png, row_pointers);

  // Cleanup
  png_write_end (png, info);
  png_destroy_write_struct (&png, &info);
  fclose(image); 
}
