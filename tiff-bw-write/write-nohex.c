#include <stdio.h>
#include <tiffio.h>

int main(int argc, char *argv[]){
  // Define an image -- this is 32 pixels by 9 pixels
  char buffer[25 * 144] = { ...boring hex omitted... };

  TIFF *image;

  // Open the TIFF file
  if((image = TIFFOpen("output.tif", "w")) == NULL){
    printf("Could not open output.tif for writing\n");
    exit(42);
  }

  // We need to set some values for basic tags before we can add any data
  TIFFSetField(image, TIFFTAG_IMAGEWIDTH, 25 * 8);
  TIFFSetField(image, TIFFTAG_IMAGELENGTH, 144);
  TIFFSetField(image, TIFFTAG_BITSPERSAMPLE, 1);
  TIFFSetField(image, TIFFTAG_SAMPLESPERPIXEL, 1);
  TIFFSetField(image, TIFFTAG_ROWSPERSTRIP, 144);

  TIFFSetField(image, TIFFTAG_COMPRESSION, COMPRESSION_CCITTFAX4);
  TIFFSetField(image, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISWHITE);
  TIFFSetField(image, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
  TIFFSetField(image, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

  TIFFSetField(image, TIFFTAG_XRESOLUTION, 150.0);
  TIFFSetField(image, TIFFTAG_YRESOLUTION, 150.0);
  TIFFSetField(image, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);
  
  // Write the information to the file
  TIFFWriteEncodedStrip(image, 0, buffer, 25 * 144);

  // Close the file
  TIFFClose(image);
}
