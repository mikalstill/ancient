/******************************************************************************
  TIFF mound. Takes a simple subset of the TIFF format, and dumps it to stdout,
  this is used so that we can ECB that image to see what it looks like
  afterward...

  Copyright (c) Michael Still 2001
  Released under the terms of the GNU GPL
******************************************************************************/

#include <stdio.h>
#include <tiffio.h>

int main(int argc, char *argv[]){
  TIFF *input;
  uint32 *raster, width, height;
  unsigned long c, imagesize;

  fprintf(stderr, "Open the TIFF image...\n");
  if((input = TIFFOpen("input.tif", "r")) == NULL){
    fprintf(stderr, "Could not open input.tif\n");
    exit(42);
  }

  fprintf(stderr, "Grab a whole bunch of memory...\n");
  TIFFGetField(input, TIFFTAG_IMAGEWIDTH, &width);
  TIFFGetField(input, TIFFTAG_IMAGELENGTH, &height);
  imagesize = height * width + 1;
  if((raster = (uint32 *) malloc(sizeof(uint32) * imagesize)) == NULL){
    fprintf(stderr, "Could not allocate enough memory\n");
    exit(42);
  }

  fprintf(stderr, "Dumping the image to stdout...\n");
  if(TIFFReadRGBAStrip(input, 0, raster) == 0){
    fprintf(stderr, "Could not read image\n");
    exit(42);
  }

  for(c = 0; c < imagesize; c += 2){
    fprintf(stdout, "%c%c%c%c%c%c  ", TIFFGetR(raster[c]), TIFFGetG(raster[c]),
	    TIFFGetB(raster[c]), TIFFGetR(raster[c + 1]), 
	    TIFFGetG(raster[c + 1]), TIFFGetB(raster[c + 1]));
  }
}
