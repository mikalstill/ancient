#ifndef FAX_H
#define FAX_H

#include "decompressor.h"

class fax:public decompressor
{
public:
  void reinit ();
  char *decompress (char *output, unsigned long length,
		    unsigned long &newlength);

private:
};

#endif
