/******************************************************************************
 A surprising useful little program! (tridge@samba.org) 
 Extended by mikal@stillhq.com

 This code is licensed under the terms of the GNU GPL
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

static void
print_one (unsigned long byteCount, unsigned long long v)
{
  char buffer[10];
  char *formats[]={"%llu", "0x%llX", "0%llo", NULL};
  int i = 0;
  
  printf("%-10d", byteCount);
  
  while(formats[i] != NULL){
    snprintf(buffer, 10, formats[i++], v);
    printf("%11s", buffer);
  }

  if (v < 256 && isprint (v))
    printf ("       '%c'", (unsigned char) v);
  printf ("\n");
}

int
main (int argc, char *argv[])
{
  int i;
  unsigned long byteCount = 0;

  // Behaviour one: command line
  if(strcmp(argv[1], "-") != 0){
    for (i = 1; i < argc; i++)
      {
	unsigned char *p;
	unsigned long long v = strtoll (argv[i], (char **) &p, 0);
	if (p == (unsigned char *) argv[i])
	  for (; *p; p++)
	    print_one (byteCount++, *p);
	else
	  print_one (byteCount++, v);
      }
  }
  // Behaviour two: stdin
  else{
    int p;
    while((p = fgetc(stdin)) != EOF)
      print_one(byteCount++, p);
  }

  return 0; 
}
