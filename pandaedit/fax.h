#ifndef FAX_H
#define FAX_H

#include "decompressor.h"

class fax:public decompressor
{
public:
  void reinit ();
  char *decompress (char *output, unsigned long length,
		    unsigned long &newlength);

  // Special functions needed to build the fake tiff
  void setWidth(long width);
  void setLength(long length);
  void setK(int k);

private:
  long m_width, m_length;
  int m_k;
};

#endif
