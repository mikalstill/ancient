#include "objectmodel.h"
#include "utility.h"
#include "decompressor.h"
#include "flate.h"
#include "verbosity.h"

#include "zlib.h"

void
flate::reinit ()
{}

char *
flate::decompress (char *input, unsigned long length, unsigned long &newlength)
{
  char *decomp;
  unsigned long decompSize = length;
  
  debug(dlTrace, string("Input stream: ") + binaryToString(input, length));

  while(decompSize < 1000000){
    if((decomp = (char *) malloc(decompSize)) == NULL){
      debug(dlError, "Could not allocate memory whilst decompressing "
	    "flate stream");
      goto errorout;
    }
    
    debug(dlTrace, string("Attempting decompression with a buffer size of ") +
	  toString((long) decompSize));
    int err = uncompress((Bytef *) decomp, &decompSize, (Bytef *) input, 
			 length);
    debug(dlTrace, string("After compression buffer size is ") +
	  toString((long) decompSize));

    switch(err){
    case Z_OK:
      debug(dlTrace, "Decompression ok");
      newlength = decompSize;
      return decomp;
      
    case Z_MEM_ERROR:
      debug(dlError, "Memory error within flate decompression");
      goto errorout;
      
    case Z_DATA_ERROR:
      debug(dlError, string("Corrupt flate data: ") +
	    binaryToString(input, 10) + string(" (first 10 bytes)"));
      goto errorout;
      break;

    case Z_BUF_ERROR:
      debug(dlTrace, "zlib buffer too small");
      decompSize *= 2;
      free(decomp);
    }
  }
  debug(dlError, "zlib buffer maximum size reached");
  
 errorout:
  newlength = 0;
  return NULL;
}
