#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include "fileutil.h"

int undeleting = 1;

int main(int argc, char *argv[]){
  int fd, npackets, otag, urbCount, function, psize;
  char *file;
  long long filep;
  struct stat sb;

  if(argc < 2){
    fprintf(stderr, "Usage: %s <input>\n", argv[0]);
    exit(43);
  }

  // Map the input file into memory
  if ((fd = open (argv[1], O_RDWR)) < 0)
    {
      fprintf (stderr, "Could not open the data file\n");
      exit (43);
    }
  
  if(fstat(fd, &sb) < 0){
    fprintf(stderr, "File stat error\n");
    exit(43);
  }

  if ((file =
       (char *) mmap (NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == -1)
    {
      fprintf (stderr, "Could not mmap data file\n");
      exit (43);
    }
  filep = 0;

  // Details of the dump
  npackets = fileutil_displaynumber(file, "Number of packets: ", &filep); printf("\n");
  printf("%d URB pointers skipped\n", npackets);
  filep += npackets * 4;
  fileutil_displayunumber(file, "Plugin timestamp: ", &filep); printf("\n");
  fileutil_displayunumber(file, "Timestamp is relative (1 == true): ", &filep); printf("\n");
  printf("\n");

  // Details of the URB
  for(urbCount = 0; urbCount < npackets; urbCount++){
    printf("-----------------------------------------------\n");
    printf("URB %d starts at %d within file\n", urbCount, filep);
    otag = fileutil_displayushort(file, "Object tag: ", &filep); printf("\n");
    if(otag == 0x7FFF){
      otag = fileutil_displayuinteger(file, "Big object tag: ", &filep); printf("\n");
    }
    else
      otag = ((otag & 0x8000) | (otag & -0x8000));
    printf("Final object tag: %u\n", otag);
    printf("\n");
    
    fileutil_displayushort(file, "Sequence: ", &filep); printf("\n");
    fileutil_displaystring(file, "SnoopyPro URB object name: ", &filep); printf("\n");
    fileutil_displayinteger(file, "Unknown: ", &filep); printf("\n");
    fileutil_displayushort(file, "Function: ", &filep); printf("\n");
    fileutil_displayuinteger(file, "Time: ", &filep); printf("\n");
    fileutil_displaynumber(file, "Endpoint: ", &filep); printf("\n");
    fileutil_displayuinteger(file, "Pipe handle: ", &filep); printf("\n");
    fileutil_displayuinteger(file, "Flags: ", &filep); printf("\n");
    fileutil_displayinteger(file, "Status: ", &filep); printf("\n");
    fileutil_displayuinteger(file, "Link: ", &filep); printf("\n");
    printf("\n");
    
    fileutil_displayuinteger(file, "Length: ", &filep); printf("\n");
    fileutil_displayinteger(file, "Direction (0 to, 1 from): ", &filep); printf("\n");
    fileutil_displayuinteger(file, "Sequence number: ", &filep); printf("\n");
    fileutil_displayuinteger(file, "Timestamp: ", &filep); printf("\n");
    fileutil_displayushort(file, "Length: ", &filep); printf("\n");
    function = fileutil_displayushort(file, "Function: ", &filep); printf("\n");
    fileutil_displayuinteger(file, "Status: ", &filep); printf("\n");
    printf("Skipped device handle pointer\n");
    filep += 4;
    fileutil_displayuinteger(file, "Flags: ", &filep); printf("\n");
    printf("\n");
    
    fileutil_displayinteger(file, "Expanded in SnoopyPro view: ", &filep); printf("\n");
    printf("\n");

    // Do something with the function
    switch(function){
    case 8:
      psize = fileutil_displayuinteger(file, "Transfer size: ", &filep); printf("\n");
      filep += psize;
      break;

    case 11:
      break;

    default:
      printf("***Unknown function***\n");
    }
  }

  // It's polite to cleanup
  munmap(file, sb.st_size);
  close(fd);
  return 0;
}

