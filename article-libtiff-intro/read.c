#include <stdio.h>
#include <string.h>
#include <tiffio.h>

char *readencodedstrips(TIFF *, unsigned long *);
void *xmalloc(size_t);

int main(int argc, char *argv[]){
  TIFF *image, *output;
  char *buffer, *outputBuffer;
  uint32 width, length, tempuint32;
  uint16 tempuint16;
  int tempint, invert, addwidth, i;
  unsigned long bufferSize, outputBufferOffset, bufferOffset;

  if(argc < 4){
    fprintf(stderr, "Usage: %s <input> <output> <additional width>\n", argv[0]);
    exit(1);
  }

  /* The first step is we open the TIFF images */

  if((image = TIFFOpen(argv[1], "r")) == NULL){
    fprintf(stderr, "Could not open incoming image\n");
    exit(42);
  }

  if((output = TIFFOpen(argv[2], "w")) == NULL){
    fprintf(stderr, "Could not open output image\n");
    exit(42);
  }

  buffer = readencodedstrips(image, &bufferSize);
  TIFFGetField(image, TIFFTAG_IMAGEWIDTH, &width);

  addwidth = atoi(argv[3]);

  /* Based on the photometric interpretation of the TIFF image, we may need
  create. */

  TIFFGetField(image, TIFFTAG_PHOTOMETRIC, &tempuint16);
  invert = tempuint16;


  /* Make the output buffer just that little bit wider and then move the
  image data into that buffer */

  TIFFGetField(image, TIFFTAG_IMAGELENGTH, &length);
  
  if((outputBuffer = (char *) malloc((((width + addwidth) / 8 ) + 1) *
				    length * sizeof(char))) == NULL){
    fprintf(stderr, "Could not grab enough memory for expanded image\n");
    exit(42);
  }

  /* Set the image to white */
  memset(outputBuffer, (invert == 0) ? 0 : 255, (((width + addwidth) / 8 ) + 1) *
				    length);
  
  outputBufferOffset = bufferOffset = 0;

  for(i = 0; i < length; i++){
    memcpy(outputBuffer + outputBufferOffset, buffer + bufferOffset, ((width % 8 == 0) ? (width / 8) : (width / 8) +1));
    bufferOffset += ((width % 8 == 0) ? (width / 8) : (width / 8) +1);
    outputBufferOffset += (((width + addwidth)% 8 == 0) ? ((width + addwidth) / 8) : ((width + addwidth) / 8) +1);
  }

  /* Write out the TIFF image and go away */

  /* We need to set some tags for the output file */
  TIFFSetField(output, TIFFTAG_IMAGELENGTH, length);
  TIFFSetField(output, TIFFTAG_ROWSPERSTRIP, length);
  
  TIFFSetField(output, TIFFTAG_IMAGEWIDTH, width + addwidth);
  
  TIFFGetField(image, TIFFTAG_BITSPERSAMPLE, &tempuint16);
  TIFFSetField(output, TIFFTAG_BITSPERSAMPLE, tempuint16);
  
  TIFFGetField(image, TIFFTAG_PLANARCONFIG, &tempuint16);
  TIFFSetField(output, TIFFTAG_PLANARCONFIG, tempuint16);
  
  TIFFGetField(image, TIFFTAG_COMPRESSION, &tempuint16);
  TIFFSetField(output, TIFFTAG_COMPRESSION, tempuint16);
  
  TIFFGetField(image, TIFFTAG_FAXMODE, &tempint);
  TIFFSetField(output, TIFFTAG_FAXMODE, tempint);
  
  TIFFGetField(image, TIFFTAG_SAMPLESPERPIXEL, &tempuint16);
  TIFFSetField(output, TIFFTAG_SAMPLESPERPIXEL, tempuint16);
  
  TIFFGetField(image, TIFFTAG_FILLORDER, &tempuint16);
  TIFFSetField(output, TIFFTAG_FILLORDER, tempuint16);
  
  TIFFGetField(image, TIFFTAG_PHOTOMETRIC, &tempuint16);
  TIFFSetField(output, TIFFTAG_PHOTOMETRIC, tempuint16);
  
  /* Some things are not copied from the image image */
  TIFFSetField(output, TIFFTAG_SOFTWARE, "Image widen [Michael Still, ESG2]");
  TIFFSetField(output, TIFFTAG_XRESOLUTION, 300.0);
  TIFFSetField(output, TIFFTAG_YRESOLUTION, 300.0);
  TIFFSetField(output, TIFFTAG_RESOLUTIONUNIT, 2);      /* Pixels per inch */

  TIFFWriteEncodedStrip(output, 0, outputBuffer, (((width + addwidth) / 8 ) + 1) * length);

  /* Clean up */
  TIFFClose(output);
  TIFFClose(image);

  exit(0);
}

char *readencodedstrips(TIFF *image, unsigned long *bufferSize){
  tsize_t stripSize;
  unsigned long imageOffset, result;
  int stripMax, stripCount;
  char *stripBuffer;

  stripSize = TIFFStripSize (image);
  stripMax = TIFFNumberOfStrips (image);
  imageOffset = 0;
  
  *bufferSize = TIFFNumberOfStrips (image) * stripSize;
  stripBuffer = xmalloc(*bufferSize);
  
  for (stripCount = 0; stripCount < stripMax; stripCount++){
    if((result = TIFFReadEncodedStrip (image, stripCount,
				      stripBuffer + imageOffset,
				      stripSize)) == -1){
      fprintf(stderr, "Read error on input strip number %d\n",
	      stripCount);
      exit(42);
    }

    imageOffset += result;
  }
  
  return stripBuffer;
}

void *
xmalloc(size_t size)
{
  void *buffer;

  if ((buffer = (void *) malloc (size)) == NULL)
    {
     fprintf(stderr, "xmalloc failed to allocate memory\n");
     exit(42);
    }

  return buffer;
}
