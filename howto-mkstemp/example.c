// A simple example of how to use mkstemp

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
  char filename[] = "/tmp/exampleXXXXXX";
  int fd;

  printf("Before: %s\n", filename);
  fd = mkstemp(filename);
  printf("After: %s (fd = %d)\n", filename, fd);
  close(fd);
}
