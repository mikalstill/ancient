#include <tiffio.h>
#include <stdio.h>

int main(int argc, char *argv[]){
  TIFF *input, *output;
  uint32 width, height;
  char *raster;
  unsigned long rasterSize, inputOffset, result;
  tsize_t stripSize;
  int stripMax, stripCount;

  // Open the TIFF input
  if((input = TIFFOpen(argv[1], "r")) == NULL){
    fprintf(stderr, "Could not open incoming image\n");
    exit(42);
  }

  // Read in the possibly multile strips
  stripSize = TIFFStripSize (input);
  stripMax = TIFFNumberOfStrips (input);
  inputOffset = 0;
  
  rasterSize = TIFFNumberOfStrips (input) * stripSize;
  if((raster = (char *) malloc(rasterSize)) == NULL){
    fprintf(stderr, "Could not allocate enough memory for the "
	    "uncompressed input\n");
    exit(42);
  }
  
  printf("Reading %d strips of %d bytes\n", stripMax, stripSize);
  for (stripCount = 0; stripCount < stripMax; stripCount++){
    if((result = TIFFReadEncodedStrip (input, stripCount,
                                      raster + inputOffset,
                                      stripSize)) == -1){
      fprintf(stderr, "Read error on input strip number %d\n", stripCount);
      exit(42);
    }

    inputOffset += result;
  }

  // Open the output image
  if((output = TIFFOpen("output.tif", "w")) == NULL){
    fprintf(stderr, "Could not open outgoing image\n");
    exit(42);
  }

  // Write the tiff tags to the file
  if(TIFFGetField(input, TIFFTAG_IMAGEWIDTH, &width) != 0)
    TIFFSetField(output, TIFFTAG_IMAGEWIDTH, width);
  else
    printf("No image width defined\n");

  if(TIFFGetField(input, TIFFTAG_IMAGELENGTH, &height) != 0)
    TIFFSetField(output, TIFFTAG_IMAGELENGTH, height);
  else
    printf("No image height defined\n");

  TIFFSetField(output, TIFFTAG_COMPRESSION, COMPRESSION_DEFLATE);
  TIFFSetField(output, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  TIFFSetField(output, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

  // At the moment I'm assuming RGB
  TIFFSetField(output, TIFFTAG_BITSPERSAMPLE, 8);
  TIFFSetField(output, TIFFTAG_SAMPLESPERPIXEL, 3);

  // Actually write the image
  if(TIFFWriteEncodedStrip(output, 0, raster, width * height * 3) == 0){
    fprintf(stderr, "Could not write image\n");
    exit(42);
  }

  TIFFClose(input);
  TIFFClose(output);
}
