#ifndef LZW_H
#define LZW_H

#include "decompressor.h"

// The basic U.S. patent expires in June, 2003. Variants on the basic LZW patent run for about 20 
// years and further U.S. applications are pending. Note that there is no LZW patent in Australia
// which is where I am based...
#warning "This program may breach the LZW patent held by Unisys. There is too much FUD to be able to tell."

class lzwTable
{
public:
  vector < unsigned char >getValue (int code);
  bool exists (vector < unsigned char >seq);
  bool pushValue (vector < unsigned char >seq);
  void clear ();
  unsigned int size ();

private:
    vector < vector < unsigned char > >m_table;
};

class lzw:public decompressor
{
public:
  void reinit ();
  char *decompress (char *input, unsigned long length,
		    unsigned long &newlength);

private:
    lzwTable m_table;
};

#endif
