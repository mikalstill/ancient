#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

#define __USE_LARGEFILE64
#define __USE_GNU
#include <fcntl.h>

#include <syscall.h>
#include <sys/stat.h>
#include <errno.h>

_syscall3(int, getdents, uint, fd, struct dirent *, dirp, uint, count);

int main(int argc, char *argv[])
{
/*   DIR *d; */
/*   struct dirent *de; */
  unsigned long fd;
  struct stat64 sb;
  char dea[4096];
  int dec, i, off;

/*   if((d = opendir(".")) == NULL){ */
/*     perror("Couldn't open the current directory: "); */
/*     exit(42); */
/*   } */

  fd = open(".", O_RDONLY|O_NONBLOCK|O_LARGEFILE|O_DIRECTORY);
  printf("Open returned the file descriptor %d\n", fd);

  if(fstat64(fd, &sb) < 0){
    perror("fstat64 failed: ");
    exit(42);
  }

  // This should be fcntl64...
  if(fcntl(fd, F_SETFD, FD_CLOEXEC) < 0){
    perror("fcntl64 failed: ");
    exit(42);
  }

/*   while((de = readdir(d)) != NULL){ */
/*     printf("%s\n", de->d_name); */
/*   } */

  // Should be getdents64
  while((dec = getdents(fd, (struct dirent *) &dea, 4096)) > 0){
    off = 0;
    while(off < dec){
      struct dirent *dp = (struct dirent *) (dea + off);
      printf("%s\n", dp->d_name - 1);
      off += dp->d_reclen;
    }
  }

/*   closedir(d); */
  close(fd);
}
