#ifndef DECOMPRESSOR_H
#define DECOMPRESSOR_H

class decompressor
{
public:
  virtual void reinit () = 0;
  virtual char *decompress (char *output, unsigned long length,
			    unsigned long &newlength) = 0;

};

#endif
