// Demonstrates how to use mmap
//
// Copyright (c) Michael Still 2002, released under the terms of the GNU GPL

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
  int fd;
  char *file;
  struct stat sb;

  if(argc < 2){
    fprintf(stderr, "Usage: %s <input>\n", argv[0]);
    exit(43);
  }

  // Map the input file into memory
  if ((fd = open (argv[1], O_RDONLY)) < 0)
    {
      perror("Could not open file");
      exit (43);
    }

  if(fstat(fd, &sb) < 0){
    perror("Could not stat file");
    exit(43);
  }

  if ((file =
       (char *) mmap (NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0)) == -1)
    {
      perror("Could not mmap file");
      exit (43);
    }

  // file is now a pointer to the entire contents of the file...
}
