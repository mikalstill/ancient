// Ignore bulk data, as that's probably the device doing something, and we're interested in
// patterns

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
  char dat[255];

  while(fgets(dat, 255, stdin) != NULL)
    {
      if(strncmp(dat, ".", 1) == 0)
	{
	  printf(".\n");
	}
      else
	{
	  printf("Ignored\n");
	}

      fflush(stdout);
    }

  return 0;
}
