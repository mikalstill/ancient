#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
  DIR *d;
  struct dirent *de;

  if((d = opendir(".")) == NULL){
    perror("Couldn't open the current directory: ");
    exit(42);
  }

  while((de = readdir(d)) != NULL){
    printf("%s\n", de->d_name);
  }

  closedir(d);
}
