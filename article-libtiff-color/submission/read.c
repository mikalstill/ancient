#include <stdio.h>
#include <tiffio.h>

int main(int argc, char *argv[]){
  TIFF *image;
  uint32 width, height, *raster;
  tsize_t stripSize;
  unsigned long imagesize, c, d, e;

  // Open the TIFF image
  if((image = TIFFOpen(argv[1], "r")) == NULL){
    fprintf(stderr, "Could not open incoming image\n");
    exit(42);
  }

  // Find the width and height of the image
  TIFFGetField(image, TIFFTAG_IMAGEWIDTH, &width);
  TIFFGetField(image, TIFFTAG_IMAGELENGTH, &height);
  imagesize = height * width + 1;
  
  if((raster = (uint32 *) malloc(sizeof(uint32) * imagesize)) == NULL){
    fprintf(stderr, "Could not allocate enough memory\n");
    exit(42);
  }

  // Read the image into the memory buffer
  if(TIFFReadRGBAStrip(image, 0, raster) == 0){
    fprintf(stderr, "Could not read image\n");
    exit(42);
  }

  // Here I fix the reversal of the image (vertically) and show you how to get the color values from each pixel
  d = 0;
  for(e = height - 1; e != -1; e--){
    for(c = 0; c < width; c++){
      // Red = TIFFGetR(raster[e * width + c]);
      // Green = TIFFGetG(raster[e * width + c]);
      // Blue = TIFFGetB(raster[e * width + c]);
    }
  }

  TIFFClose(image);
}
