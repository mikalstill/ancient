// A simple little program which displays two files side by side, after
// executing the named filter on the files
//
// e.g. side "i - < %s" foo.one foo.two
//
// The %s will become the filename for each of the files
//
// Copyright (c) Michael Still (mikal@stillhq.com) 2002, released under the 
// GNU GPL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

#define COLWIDTH 70

int main(int argc, char *argv[]){
  char temp1[] = "/tmp/side.XXXXXX", temp2[] = "/tmp/side.XXXXXX", fmt[255], cmd1[1024], cmd2[1024],
    *file1, *file2;
  int fd1, fd2, cnt1, cnt2, done;
  struct stat sb1, sb2;
  off_t inset1, inset2;
  int count1, count2, currfile;

  // Check the arguements
  if(argc < 4){
    fprintf(stderr, "Usage: %s <command> <file1> <file2>\n", argv[0]);
    exit(1);
  }

  // Filter the files into temporary locations -- open to format string attacks
  fprintf(stderr, "Warning, do not use this command in a secure environment...\n");

  if(fd1 = mkstemp(temp1) < 0){
    perror("Could not make temporary file one");
    exit(1);
  }
  close(fd1);
  snprintf(fmt, 255, "%s > %%s", argv[1]);
  snprintf(cmd1, 1024, fmt, argv[2], temp1);
  system(cmd1);
  if(fd1 = open(temp1, NULL) < 0){
    perror("Error opening temporary file one");
    exit(1);
  }

  if(fd2 = mkstemp(temp2) < 0){
    perror("Could not make temporary file two");
    exit(1);
  }
  close(fd2);
  snprintf(cmd2, 1024, fmt, argv[2], temp2);
  system(cmd2);
  if(fd2 = open(temp2, NULL) < 0){
    perror("Error opening temporary file two");
    exit(1);
  }

  // Do some memory mapping
  if(fstat(fd1, &sb1) < 0){
    perror("Could not stat file one");
    exit(1);
  }
  if(fstat(fd2, &sb2) < 0){
    perror("Could not stat file two");
    exit(1);
  }

  if ((file1 =
       (char *) mmap (NULL, sb1.st_size, PROT_READ, MAP_SHARED, fd1, 0)) == -1)
    {
      perror("Could not mmap file one");
      exit (1);
    }

  if ((file2 =
       (char *) mmap (NULL, sb2.st_size, PROT_READ, MAP_SHARED, fd2, 0)) == -1)
    {
      perror("Could not mmap file two");
      exit (1);
    }

  // We now have filtered files we can display
  printf("%-70s    | %s\n", cmd1, cmd2);
  inset1 = inset2 = 0;
  count1 = count2 = 1;
  currfile = 0;
  while((inset1 < sb1.st_size) && (inset2 < sb2.st_size)){
    if(currfile == 0){
      if(file1[inset1] == '\t') count1 += 7;
      
      if(count1 > COLWIDTH){
	printf(" \\\\ | ");
	count1 = 1;
	currfile++;
      }
      
      if(file1[inset1] == '\n'){
	for(; count1< COLWIDTH + 1; count1++)
	  printf(" ");
	printf("    | ");
	count1 = 0;
	currfile++;
      }
      else{
	printf("%c", file1[inset1]);
      }
      
      inset1++;
      count1++;
    }
    else{
      if(file2[inset2] == '\t') count2 += 7;

      if(count2 > COLWIDTH){
	printf(" \\\\\n");
	count2 = 0;
	currfile--;
      }
      
      if(file2[inset2] == '\n'){
	printf("\n");
	count2 = 1;
	currfile--;
      }
      else{
	printf("%c", file2[inset2]);
      }
      
      inset2++;
      count2++;
    }
  }
  
  // Get to a new line and display end of file markers
  if((currfile == 1) || (count1 > 0)) printf("\n");
  if(inset1 > sb1.st_size - 1) printf("%-70s    | ", " [EOF]");
  else printf("%-70s    | ", " ");

  if(inset2 > sb2.st_size - 1) printf("%s", " [EOF]");
  printf("\n\n");
  
  // Cleanup
  if(munmap(file1, sb1.st_size) < 0){
    perror("Could not unmap memory one");
    exit(1);
  }

  if(munmap(file2, sb2.st_size) < 0){
    perror("Could not unmap memory two");
    exit(1);
  }
  
  close(fd1);
  close(fd2);
}
  
