// This is a simple example (well, a little more than bare minimal) for a decoder for
// usblogdump. The protocol is simple -- you're decoder gets input on stdin, and pushes output
// to stdout. usblogdump gives you data in the following form:
//
// <TYPE><SPACE><BINARY DATA>
// .
//
// For example:
// BULK 476987294867985249672769536783095685
// .
//
// The full stop (period for the Americans) means end of packet (as the binary data could contain 
// a newline). The full stop should appear on a line by itself. It's possible that that's valid
// data too, but it's quite unlikely. I hope.
//
// Output should be whatever you want to display for that packet, and then a full stop on a line
// by itself.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *to_binary(unsigned char number);

int main(int argc, char *argv[])
{
  char dat[255];

  while(fgets(dat, 255, stdin) != NULL)
    {
      if(strncmp(dat, "BULK ", 5) == 0)
	{
	  printf(to_binary((unsigned char) dat[5]));

	  printf(" (");
	  if(((unsigned char) dat[5]) & 0x01)
	    printf("Motor bank 1");
	  else if(((unsigned char) dat[5]) & 0x02)
	    printf("Motor bank 2");
	  else
	    printf("            ");
	  printf(") ");

	  printf(to_binary((unsigned char) dat[6]));
	  printf(" (");
	  if(((unsigned char) dat[6]) & 0x10)
	    printf("Motor 2 phase 1");
	  else if(((unsigned char) dat[6]) & 0x20)
	    printf("Motor 2 phase 2");
	  else if(((unsigned char) dat[6]) & 0x40)
	    printf("Motor 2 phase 3");
	  else if(((unsigned char) dat[6]) & 0x80)
	    printf("Motor 2 phase 4");
	  else if(((unsigned char) dat[6]) & 0x01)
	    printf("Motor 1 phase 1");
	  else if(((unsigned char) dat[6]) & 0x02)
	    printf("Motor 1 phase 2");
	  else if(((unsigned char) dat[6]) & 0x04)
	    printf("Motor 1 phase 3");
	  else if(((unsigned char) dat[6]) & 0x08)
	    printf("Motor 1 phase 4");
	  else
	    printf("               ");
	  printf(") ");

	  printf("\n");
	}
      else if(strncmp(dat, "CTRL ", 5) == 0)
	{
	  // This is here for completeness, as the USB stepper motor controller doesn't
	  // use control transfers
	  printf("No decoding implemented for control transfers\n");
	}
      else if(strncmp(dat, ".", 1) == 0)
	{
	  printf(".\n");
	}
      else
	{
	  printf("Huh (%s)\n.\n", dat);
	}

      fflush(stdout);
    }

  return 0;
}

char *to_binary(unsigned char number)
{
  char *retval;

  if((retval = malloc(sizeof(char) * 9)) == NULL)
    {
      fprintf(stderr, "Could not allocate memory\n");
      exit(42);
    }

  memset(retval, '0', 8);
  retval[8] = 0;

  if(number & 0x80) retval[0] = '1';
  if(number & 0x40) retval[1] = '1';
  if(number & 0x20) retval[2] = '1';
  if(number & 0x10) retval[3] = '1';
  if(number & 0x08) retval[4] = '1';
  if(number & 0x04) retval[5] = '1';
  if(number & 0x02) retval[6] = '1';
  if(number & 0x01) retval[7] = '1';
 
  return retval;
}
