// Demonstrates how to use execvp
//
// Copyright (c) Michael Still 2003, released under the terms of the GNU GPL

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
  char **args;
  int idx;

  // Args is an array of char *'s which point to the arguements. The first
  // arg is traditionally the name of the command
  if((args = (char **) malloc(5 * sizeof(char *))) == NULL){
    printf("Couldn't allocate memory\n");
    exit(1);
  }

  args[0] = strdup("echo");
  args[1] = strdup("hello");
  args[2] = strdup("out");
  args[3] = strdup("there");
  args[4] = NULL;

  // Now determine how many entries we just created
  idx = 0;
  while(args[idx])
    idx++;
  idx++;
  printf("Size: %d\n", idx);

  
}
