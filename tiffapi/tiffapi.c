/******************************************************************************
  tiffapi.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      29012001

  Purpose:
    An abstraction layer over libtiff to make using it a little bit easier.
******************************************************************************/

#include <tiffapi/functions.h>

char *tiffapi_readencodedstrips(tiff *image){
  tsize_t stripSize;
  unsigned long imageOffset;
  int stripMax;
  char *stripBuffer;

  stripSize = TIFFStripSize (image);
  stripMax = TIFFNumberOfStrips (image);
  imageOffset = 0;
  
  stripBuffer = tiffapi_xmalloc (TIFFNumberOfStrips (image) * stripSize);
  
  for (stripCount = 0; stripCount < stripMax; stripCount++)
    imageOffset += TIFFReadEncodedStrip (image, stripCount,
					 stripBuffer + imageOffset,
					 stripSize);
  
  return stripBuffer;
}
