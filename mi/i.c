/******************************************************************************
 A surprising useful little program! (tridge@samba.org) 
 Extended by mikal@stillhq.com

 This code is licensed under the terms of the GNU GPL
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

unsigned long long lastv;
int lastvcnt;

static int
print_one (unsigned long byteCount, unsigned long long v, int contextual)
{
  char buffer[10];
  char *formats[]={"%llu", "0x%02llX", "0%llo", NULL};
  int i = 0;
  
  if(!contextual){
    if(lastv == v){
      lastvcnt++;
      return 1;
    }
    else if(lastvcnt > 0){
      printf("  repeated %d times\n", lastvcnt);
      lastvcnt = 0;
    }
    lastv = v;

    printf("%-10d", byteCount);
    
    while(formats[i] != NULL){
      snprintf(buffer, 10, formats[i++], v);
      printf("%11s", buffer);
    }

    printf("       ");
  }

  if ((contextual || !isspace(v)) && v < 256 && isprint (v))
    printf ("%c", (unsigned char) v);
  else
    printf (".");

  return 0;
}

int
main (int argc, char *argv[])
{
  int i;
  unsigned long byteCount = 0;
  
  if(argc == 1)
    return 0;

  lastv = -1;
  lastvcnt = 0;

  // Behaviour one: command line
  if(strcmp(argv[1], "-") != 0){
    for (i = 1; i < argc; i++)
      {
	unsigned char *p;
	unsigned long long v = strtoll (argv[i], (char **) &p, 0);
	for (; *p; p++){
	  print_one(byteCount++, *p, 0);
	  printf("\n");
	}
      }
  }
  // Behaviour two: stdin
  else{
    int p[10], bufcount = 0, bufshift;

    while((p[10] = fgetc(stdin)) != EOF){
      bufcount++;
      
      if(bufcount > 10){
	if(!print_one(byteCount++, p[0], 0)){
	  printf(" ");
	  
	  for(bufshift = 0; bufshift < 10; bufshift++)
	    print_one(0, p[bufshift], 1);
	  printf("\n");
	}
      }

      for(bufshift = 0; bufshift < 10; bufshift++){
	p[bufshift] = p[bufshift + 1];
      }
    }
  }

  return 0; 
}
