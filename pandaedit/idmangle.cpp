// Turn 24 bit object id numbers into colours which are hopefully distinct

#include <math.h>
#include "idmangle.h"

void mangle(int in, unsigned char& out1, unsigned char& out2, 
	      unsigned char& out3)
{
  unsigned char mangled[3];
  mangled[0] = 0;
  mangled[1] = 0;
  mangled[2] = 0;

  for(int count = 0; count < 24; count++)
    {
      int byte = count % 3;
      int bit = count / 3;
      
      if(in & (int) pow(2, count))
	{
	  mangled[byte] += (int) pow(2, bit);
	}
    }

  out1 = mangled[0];
  out2 = mangled[1];
  out3 = mangled[2];
}

int demangle(unsigned char in1, unsigned char in2, unsigned char in3)
{
  int demangled = 0;
  unsigned char mangled[3];
  mangled[0] = in1;
  mangled[1] = in2;
  mangled[2] = in3;

  for(int count = 0; count < 24; count++)
    {
      int byte = count % 3;
      int bit = count / 3;
      
      if(mangled[byte] & (int) pow(2, bit))
	{
	  demangled += (int) pow(2, count);
	}
    }

  return demangled;
}
