#ifndef FLATE_H
#define FLATE_H

#include "decompressor.h"
#include <zlib.h>

class flate:public decompressor
{
public:
  void reinit ();
  char *decompress (char *input, unsigned long length,
		    unsigned long &newlength);
};

#endif
