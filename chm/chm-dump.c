#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include "fileutil.h"

typedef struct chmdump_internal_filelist
{
  char *name;
  int cs;
  int offset;
  int length;

  struct chmdump_internal_filelist *next;
} chmdump_filelist;

int main(int argc, char *argv[]){
  int fd;
  char *file, chunktype[4], filename[1024];
  int count, maxchunk, chunksize, waste, read, density, chunkhits, chunkent, thischunk;
  long long offset, length, filep, chunkp;
  struct stat sb;
  chmdump_filelist *fl, *flcurrent;

  if(argc < 2){
    fprintf(stderr, "Usage: %s <input>\n", argv[0]);
    exit(43);
  }

  // Map the input file into memory
  if ((fd = open (argv[1], O_RDONLY)) < 0)
    {
      fprintf (stderr, "Could not open the data file\n");
      exit (43);
    }

  if(fstat(fd, &sb) < 0){
    fprintf(stderr, "File stat error\n");
    exit(43);
  }

  if ((file =
       (char *) mmap (NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0)) == -1)
    {
      fprintf (stderr, "Could not mmap data file\n");
      exit (43);
    }
  filep = 0;

  // Setup the filelist
  if((fl = (chmdump_filelist *) malloc(sizeof(chmdump_filelist))) == NULL){
    fprintf(stderr, "Could not allocate space for file list\n");
    exit(43);
  }
  fl->next = NULL;
  flcurrent = fl;

  /////////////////////////////////////////////////////////////////////////////
  // File header
  /////////////////////////////////////////////////////////////////////////////
  printf("File header\n");
  printf("Magic bytes: ");
  for(count = 0; count < 4; count++)
    printf("%c", file[filep++]);
  printf("\n");

  fileutil_displayinteger(file, "Version: ", &filep); printf("\n");
  fileutil_displayinteger(file, "Header length: ", &filep); printf("\n");
  fileutil_displayinteger(file, "Unknown (should be 1): ", &filep); printf("\n");
  fileutil_displayinteger(file, "Timestamp: ", &filep); printf("\n");
  fileutil_displaywindowslanguage(file, "Language: ", &filep); printf("\n");
  fileutil_displayguid(file, "GUID 1: ", &filep); printf("\n");
  fileutil_displayguid(file, "GUID 2: ", &filep); printf("\n");

  offset = fileutil_displaylong(file, "Section start offset: ", &filep); printf("\n");
  length = fileutil_displaylong(file, "Section length: ", &filep); printf("\n");
  
  // We need to be able to jump over some unwanted bytes here
  // fileutil_displaylong(file, "Offset to content section 0: ", &filep); printf("\n"); // v2 lacks this
  offset -= 4 + (4 * 4) + 4 + (16 * 2) + (8 * 2);
  printf("Skipping %d bytes: ", offset);
  for(count = 0; count < offset; count++){
    printf("%02x ", file[filep++]);
  }
  printf("\n");

  /////////////////////////////////////////////////////////////////////////////
  // Header section table
  /////////////////////////////////////////////////////////////////////////////
  printf("\nHeader section table\n");
  fileutil_displayinteger(file, "Unknown (should be 510): ", &filep); printf("\n");
  fileutil_displayinteger(file, "Unknown (should be 0): ", &filep); printf("\n");
  fileutil_displaylong(file, "File length: ", &filep); printf("\n");
  fileutil_displayinteger(file, "Unknown (should be 0): ", &filep); printf("\n");
  fileutil_displayinteger(file, "Unknown (should be 0): ", &filep); printf("\n");
  
  /////////////////////////////////////////////////////////////////////////////
  // Directory listing header
  /////////////////////////////////////////////////////////////////////////////
  printf("\nDirectory listing header\n");
  printf("Magic bytes: ");
  for(count = 0; count < 4; count++)
    printf("%c", file[filep++]);
  printf("\n");

  fileutil_displayinteger(file, "Version (should be 1): ", &filep); printf("\n");
  fileutil_displayinteger(file, "Length of header: ", &filep); printf("\n");
  fileutil_displayinteger(file, "Unknown (should be 0): ", &filep); printf("\n");
  chunksize = fileutil_displayinteger(file, "Chunk size: ", &filep); printf("\n");
  density = fileutil_displayinteger(file, "Density of quickref (usually 2): ", &filep); printf("\n");
  fileutil_displayinteger(file, "Depth of index tree (1 = none):", &filep); printf("\n");
  fileutil_displayinteger(file, "Chunk number for root index chunk: ", &filep); printf("\n");
  fileutil_displayinteger(file, "Chunk number for root listing chunk: ", &filep); printf("\n");
  fileutil_displayinteger(file, "Final listing chunk: ", &filep); printf("\n");
  fileutil_displayinteger(file, "Unknown (should be -1): ", &filep); printf("\n");
  maxchunk = fileutil_displayinteger(file, "Number of directory chunks: ", &filep); printf("\n");
  fileutil_displaywindowslanguage(file, "Language: ", &filep); printf("\n");
  fileutil_displayguid(file, "GUID (5D02926A-212E-11D0-9DF9-00A0C922E6EC): ", &filep); printf("\n");
  fileutil_displayinteger(file, "Length (should be 84): ", &filep); printf("\n");
  fileutil_displayinteger(file, "Unknown (should be -1): ", &filep); printf("\n");
  fileutil_displayinteger(file, "Unknown (should be -1): ", &filep); printf("\n");
  fileutil_displayinteger(file, "Unknown (should be -1): ", &filep); printf("\n");

  /////////////////////////////////////////////////////////////////////////////
  // Chunks
  /////////////////////////////////////////////////////////////////////////////
  for(thischunk = 0; thischunk < maxchunk; thischunk++){
    chunkp = filep;

    printf("\nChunk %d\n", thischunk);
    printf("Magic bytes: ");
    for(count = 0; count < 4; count++){
      chunktype[count] = file[filep];
      printf("%c", file[filep++]);
    }
    printf("\n");
    chunktype[count] = '\0';

    waste = fileutil_displayinteger(file, "Wasted (or quickref) chunk space: ", &filep); printf("\n");

    // Listing chunks
    if(strcmp("PMGL", chunktype) == 0){
      fileutil_displayinteger(file, "Always zero: ", &filep); printf("\n");
      fileutil_displayinteger(file, "Previous chunk number: ", &filep); printf("\n");
      fileutil_displayinteger(file, "Next chunk number: ", &filep); printf("\n");
      
      // For each of the hits
      chunkp += chunksize - 2;
      chunkhits = fileutil_displayshort(file, "Number of entries in chunk: ", &chunkp); printf("\n");
      for(chunkent = 0; chunkent < chunkhits; chunkent++){
	printf("  Chunk entry %d\n", chunkent + 1);

	length = fileutil_displaybyte(file, "  Name length: ", &filep); printf("\n");	
	printf("  Filename: ");
	for(count = 0; count < length; count++){
	  if(count < 1023){
	    filename[count] = file[filep];
	  }
	  printf("%c", file[filep++]);
	}
	printf("\n");
	filename[count] = '\0';
	
	// We need to store information about this file
	if((flcurrent->next = (chmdump_filelist *) malloc(sizeof(chmdump_filelist))) == NULL){
	  fprintf(stderr, "Could not append to file list\n");
	  exit(43);
	}

	flcurrent->next->next = NULL;
	flcurrent->name = (char *) strdup(filename);
	flcurrent->cs = fileutil_displayencinteger(file, "  Content section: ", &read, &filep); printf("\n");
	flcurrent->offset = fileutil_displayencinteger(file, "  Offset: ", &read, &filep); printf("\n");
	flcurrent->length = fileutil_displayencinteger(file, "  Length: ", &read, &filep); printf("\n");
	flcurrent = flcurrent->next;
	printf("\n");
      }
      
      // Display the quickrefs
      printf("  ...Skipping %d bytes...\n", (chunkp - 2 - (1 + (1 << density)) * 4) - filep);
      filep = chunkp - 2 - (1 + (1 << density)) * 4;
      for(count = 0; count < (1 + (1 << density)); count++){
	fileutil_displayinteger(file, "  Quick reference entry: ", &filep); printf("\n");
      }
      
      fileutil_displayshort(file, "  Number of entries in this chunk: ", &filep); printf("\n");
    }
    else if(strcmp("PMGI", chunktype) == 0){
      chunkp += chunksize - waste;
      while(filep < chunkp){
	length = fileutil_displaybyte(file, "  Name length: ", &filep); printf("\n");	
	printf("  Name: ");
	for(count = 0; count < length; count++)
	  printf("%c", file[filep++]);
	printf("\n");
	
	fileutil_displayencinteger(file, "  Content section: ", &read, &filep); printf("\n");
	printf("\n");
      }

      printf("  ...Skipping %d bytes...\n", waste);
      filep += waste;
    }
    else{
      printf("Unknown chunk type %s\n", chunktype);
      return 1;
    }
  }
  printf("\n");

  /////////////////////////////////////////////////////////////////////////////
  // Content section 0
  /////////////////////////////////////////////////////////////////////////////
  chunkp = filep;

  printf("Start byte for content section 0: %d\n", chunkp);
  // Go through and extract all the files in this section
  flcurrent = fl;
  while(flcurrent->next != NULL){
    if(flcurrent->cs == 0){
      printf("%s [start %d, end %d]\n", flcurrent->name, 
	     flcurrent->offset, flcurrent->offset + flcurrent->length);

      if(flcurrent->length > 0){
	char *tempname;
	int tempfd;

	printf("  File\n");
	tempname = (char *) strdup("output/");
	strcat(tempname, flcurrent->name);
	printf("  Opening %s\n", tempname);
	
	mkpath(tempname);
	if((tempfd = open(tempname, O_WRONLY | O_CREAT)) < 0){
	  fprintf(stderr, "Could not open the output file %s\n", tempname, tempfd);
	  fprintf(stderr, "%d: %s\n", errno, strerror(errno));
	  exit(43);
	}
	write(fd, file + filep + flcurrent->offset, flcurrent->length);
	close(fd);

	free(tempname);	
      }
      else{
	char *tempname;

	printf("  Directory\n\n");
	tempname = (char *) strdup("output");
	strcat(tempname, flcurrent->name);
	mkdir(tempname, 0);
	free(tempname);
      }
    }

    flcurrent = flcurrent->next;
  }






  // It's polite to cleanup
  munmap(file, sb.st_size);
  close(fd);
  return 0;
}
