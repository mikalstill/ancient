// Demonstrates how to use mmap
//
// Copyright (c) Michael Still 2004, released under the terms of the GNU GPL

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
  int realfd, backingfd;
  char *realfile, *backingfile;
  struct stat sb;

  if(argc < 3){
    fprintf(stderr, "Usage: %s <input> <output>\n", argv[0]);
    exit(43);
  }

  if ((realfd = open (argv[1], O_RDONLY)) < 0)
    {
      printf("Backing store: couldn't open the real file\n");
      return;
    }
  
  if ((backingfd = open (argv[2], O_CREAT | O_RDWR, S_IRWXU)) < 0)
    {
      printf("Backing store: couldn't open the backing file\n");
      close(realfd);
      return;
    }
  
  if(fstat(realfd, &sb) < 0)
    {
      printf("Backing store: couldn't stat the real file\n");
      close(realfd);
      close(backingfd);
      return;
    }
  
  if(ftruncate(backingfd, sb.st_size) < 0)
    {
      printf("Backing store: couldn't allocate space for backing data\n");
      close(realfd);
      close(backingfd);
      return;
    }
  
  if ((realfile =
       (char *) mmap (NULL, sb.st_size, PROT_READ, 
		      MAP_SHARED, realfd, 0)) == -1)
    {
      printf("Backing store: couldn't mmap the real file\n");
      close(realfd);
      close(backingfd);
      return;
    }
  
  if ((backingfile =
       (char *) mmap (NULL, sb.st_size, PROT_WRITE, 
		      MAP_SHARED, backingfd, 0)) == -1)
    {
      printf("Backing store: couldn't mmap the backing file\n");
      close(realfd);
      close(backingfd);
      return;
    }
  
  if(memcpy(backingfile, realfile, sb.st_size) < 0)
    {
      printf("Backing store: copy failed\n");
      close(realfd);
      close(backingfd);
      return;
    }
  
  if(munmap(realfile, sb.st_size) < 0)
    {
      printf("Backing store: couldn't munmap the real file\n");
      close(realfd);
      close(backingfd);
      return;
    }
  
  if(munmap(backingfile, sb.st_size) < 0)
    {
      printf("Backing store: couldn't munmap the backing file\n");
      close(realfd);
      close(backingfd);
      return;
    }
  
  close(realfd);
  close(backingfd);
}
