#include <stdio.h>
#include <tiffio.h>

int main(int argc, char *argv[]){
  // Define an image -- this is 32 pixels by 9 pixels

  // 12345678 12345678 12345678 12345678

  // 00000000 00000000 00000000 00000000
  // 01100000 00011111 11111000 00000110
  // 01100000 11100000 00000111 00000110
  // 01100111 00000000 00000000 11100110
  // 01111000 00000000 00000000 00011110
  // 01100000 00000000 00000000 00000110
  // 01111111 11111111 11111111 11111110
  // 01100000 00000000 00000000 00000110
  // 00000000 00000000 00000000 00000000
  char buffer[32 * 9] = {0x00, 0x00, 0x00, 0x00,
			 0x60, 0x1F, 0xF8, 0x06,
			 0x60, 0xE0, 0x07, 0x06,
			 0x67, 0x00, 0x00, 0xE6,
			 0x78, 0x00, 0x00, 0x1E,
			 0x60, 0x00, 0x00, 0x06,
			 0x7F, 0xFF, 0xFF, 0xFE,
			 0x60, 0x00, 0x00, 0x06,
			 0x00, 0x00, 0x00, 0x00};
  TIFF *image;

  // Open the TIFF file
  if((image = TIFFOpen("output.tif", "w")) == NULL){
    printf("Could not open output.tif for writing\n");
    exit(42);
  }

  // We need to set some values for basic tags before we can add any data
  TIFFSetField(image, TIFFTAG_IMAGEWIDTH, 32);
  TIFFSetField(image, TIFFTAG_IMAGELENGTH, 9);
  TIFFSetField(image, TIFFTAG_BITSPERSAMPLE, 1);
  TIFFSetField(image, TIFFTAG_SAMPLESPERPIXEL, 1);
  TIFFSetField(image, TIFFTAG_ROWSPERSTRIP, 9);

  TIFFSetField(image, TIFFTAG_COMPRESSION, COMPRESSION_CCITTFAX4);
  TIFFSetField(image, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
  TIFFSetField(image, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
  TIFFSetField(image, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

  TIFFSetField(image, TIFFTAG_XRESOLUTION, 300.0);
  TIFFSetField(image, TIFFTAG_YRESOLUTION, 300.0);
  TIFFSetField(image, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);
  
  // Write the information to the file
  TIFFWriteEncodedStrip(image, 0, buffer, 4 * 9);

  // Close the file
  TIFFClose(image);
}
