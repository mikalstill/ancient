// Implements a simple recursive mkdir
//
// Copyright (c) Michael Still 2004, released under the terms of the GNU GPL

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[]){
  char *path, *tok;
  int len;
  struct stat sb;

  len = strlen(argv[1]) + 1;
  path = malloc(len + 1);
  if(!path)
    {
      printf("Could not allocate memory\n");
      exit(1);
    }
  
  tok = strtok(argv[1], "/");
  snprintf(path, len, "%s", tok);
  while(1)
    {
      if(stat(path, &sb) < 0)
	{
	  // The path element didn't exist
	  mkdir(path, S_IRWXU);
	}
      
      if(!S_ISDIR(sb.st_mode))
	{
	  printf("Path element not a directory\n");
	  exit(3);
	}
      
      tok = strtok(NULL, "/");
      if(!tok)
	exit(0);
      strncat(path, "/", len);
      strncat(path, tok, len);
    }
}
