#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include "fileutil.h"

int main(int argc, char *argv[]){
  int fd;
  char *file;
  int count, maxchunk, chunksize, waste, read, density;
  long long offset, length, filep;
  struct stat sb;

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
  for(count = 0; count < maxchunk; count++){
    printf("\nChunk %d\n", count);
    printf("Magic bytes: ");
    for(count = 0; count < 4; count++)
      printf("%c", file[filep++]);
    printf("\n");
    
    waste = fileutil_displayinteger(file, "Wasted (or quickref) chunk space: ", &filep); printf("\n");
    fileutil_displayinteger(file, "Always zero: ", &filep); printf("\n");
    fileutil_displayinteger(file, "Previous chunk number: ", &filep); printf("\n");
    fileutil_displayinteger(file, "Next chunk number: ", &filep); printf("\n");
    offset = chunksize - 4 - (4 * 4);

    while(offset > 0){
      // Filename
      printf("\n  Remaining chunk length: %d\n", offset);
      length = fileutil_displaybyte(file, "  Name length: ", &filep); printf("\n");
      offset--;

      if(length == 0)
	break;

      printf("  Filename: ");
      for(count = 0; count < length; count++){
	printf("%c", file[filep++]);
	offset--;
      }
      printf("\n");
      
      fileutil_displayencinteger(file, "  Content section: ", &read, &filep); printf("\n");
      offset -= read;
      fileutil_displayencinteger(file, "  Offset: ", &read, &filep); printf("\n");
      offset -= read;
      fileutil_displayencinteger(file, "  Length: ", &read, &filep); printf("\n");
      offset -= read;
    }

    // Display the quickrefs
    for(count = 0; count < (1 + (1 << density)); count++){
      fileutil_displayinteger(file, "  Quick reference entry: ", &filep); printf("\n");
      offset -= 4;
    }

    offset -= 2;
    printf("\n  Skipping %d bytes: ", offset);
    for(count = 0; count < offset; count++){
      printf("%02x ", file[filep++]);
    }
    printf("\n");

    fileutil_displayshort(file, "  Number of entries in this chunk: ", &filep); printf("\n");
  }


  // It's polite to cleanup
  munmap(file, sb.st_size);
  close(fd);
}
