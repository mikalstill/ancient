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
  TIFF *input, *output;
  uint32 width, height;
  char *raster;
  unsigned long offset, imagesize;
  int b, c, i;

  fprintf(stderr, "Open the TIFF image...\n");
  if((input = TIFFOpen("input.tif", "r")) == NULL){
    fprintf(stderr, "Could not open input.tif\n");
    exit(42);
  }

  if((output = TIFFOpen("output.tif", "w")) == NULL){
    fprintf(stderr, "Could not open output.tif\n");
    exit(42);
  }

  fprintf(stderr, "Grab a whole bunch of memory...\n");
  TIFFGetField(input, TIFFTAG_IMAGEWIDTH, &width);
  TIFFGetField(input, TIFFTAG_IMAGELENGTH, &height);
  imagesize = height * width + 1;
  if((raster = malloc(sizeof(char) * imagesize * 3)) == NULL){
    fprintf(stderr, "Could not allocate enough memory\n");
    exit(42);
  }

  fprintf(stderr, "Grabbing the image from stdin...\n");
  offset = i = 0;
  while((c = fgetc(stdin)) != EOF){
    if(i < 6){
      printf("%02x", c);
      fflush(stdout);
      raster[offset] = c;
      offset++;
    }
    else{
      printf("-");
      fflush(stdout);
    }

    i++;
    if(i == 8) i = 0;
  }

  TIFFSetField(output, TIFFTAG_IMAGEWIDTH, width);
  TIFFSetField(output, TIFFTAG_IMAGELENGTH, height);
  TIFFSetField(output, TIFFTAG_COMPRESSION, COMPRESSION_DEFLATE);
  TIFFSetField(output, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  TIFFSetField(output, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
  TIFFSetField(output, TIFFTAG_BITSPERSAMPLE, 8);
  TIFFSetField(output, TIFFTAG_SAMPLESPERPIXEL, 3);

  fprintf(stderr, "Writing the image to disc...\n");
  if(TIFFWriteEncodedStrip(output, 0, raster, imagesize * 3) == 0){
    fprintf(stderr, "Could not read image\n");
    exit(42);
  }

  TIFFClose(input);
  TIFFClose(output);
}
