#include <tiffio.h>

int main(int argc, char *argv[]){
  TIFF *image, *output;
  uint16 photo;
  tsize_t stripSize;
  unsigned long imageOffset, result;
  int stripMax, stripCount;
  char *stripBuffer;
  unsigned long bufferSize;

  // Open the TIFF image
  if((image = TIFFOpen(argv[1], "r")) == NULL){
    fprintf(stderr, "Could not open incoming image\n");
    exit(42);
  }

  // Read in the possibly multile strips
  stripSize = TIFFStripSize (image);
  stripMax = TIFFNumberOfStrips (image);
  imageOffset = 0;
  
  bufferSize = TIFFNumberOfStrips (image) * stripSize;
  if((stripBuffer = (char *) malloc(bufferSize)) == NULL){
    fprintf(stderr, "Could not allocate enough memory for the uncompressed image\n");
    exit(42);
  }
  
  for (stripCount = 0; stripCount < stripMax; stripCount++){
    if((result = TIFFReadEncodedStrip (image, stripCount,
				      stripBuffer + imageOffset,
				      stripSize)) == -1){
      fprintf(stderr, "Read error on input strip number %d\n", stripCount);
      exit(42);
    }

    imageOffset += result;
  }  

  // This affects whether we have to twiddle bits
  TIFFGetField(image, TIFFTAG_PHOTOMETRIC, &photo);

  TIFFClose(image);
}
