#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

#include "fileutil.h"

#define CHUNKSIZE 3000

char *memopad_xsnprintf(char *, ...);
void memopad_xfree(void *);
void *memopad_xrealloc(void *, size_t);
int memopad_min(int, int);
void *memopad_xmalloc(size_t);
char *memopad_strlimit(char *, int);

int main(int argc, char *argv[]){
  FILE *output;
  int rcnt, fd, chunkcnt = 0;
  char *contents, *data;
  unsigned long offset;
  struct stat statbuf;

  srand(time(NULL));

  if((output = fopen("memopad.dat", "w")) == NULL){
    fprintf(stderr, "Could not create memopad.dat\n");
    exit(42);
  }

  // Magic number, file path, and custom show header
  fprintf(output, "%c%c%c%c", 0x0, 0x1, 0x50, 0x4d);
  memopad_insertstring(output, "C:\\Palm\\mikal\\memopad\\memopad.dat");
  memopad_insertstring(output, "0 1 0 0 1 0X");
  
  // Category information (this is just a std set)
  memopad_insertinteger(output, 131);
  memopad_insertinteger(output, 3);

  memopad_insertinteger(output, 1);
  memopad_insertinteger(output, 1);
  memopad_insertinteger(output, 0);
  memopad_insertstring(output, "Business");
  memopad_insertstring(output, "Business");

  memopad_insertinteger(output, 2);
  memopad_insertinteger(output, 2);
  memopad_insertinteger(output, 0);
  memopad_insertstring(output, "Personal");
  memopad_insertstring(output, "Personal");

  memopad_insertinteger(output, 3);
  memopad_insertinteger(output, 130);
  memopad_insertinteger(output, 0);
  memopad_insertstring(output, "Tutorial");
  memopad_insertstring(output, "Tutorial");

  // Schema information (never changes)
  memopad_insertinteger(output, 64);
  memopad_insertinteger(output, 6);
  memopad_insertinteger(output, 0);
  memopad_insertinteger(output, 1);
  memopad_insertinteger(output, 2);
  memopad_insertshort(output, 6);

  // Schema field information (never changes)
  memopad_insertshort(output, 1);
  memopad_insertshort(output, 1);
  memopad_insertshort(output, 1);
  memopad_insertshort(output, 5);
  memopad_insertshort(output, 6);
  memopad_insertshort(output, 1);

  // Determine how many records we are going to create
  // For each record
  for(rcnt = 0 ; rcnt < argc - 1; rcnt++){
    // Open the data file
    if ((fd = open (argv[rcnt + 1], O_RDONLY)) < 0)
      {
	fprintf (stderr, "Could not open the data file: %s\n", argv[rcnt + 1]);
	exit (0);
      }
    
    fstat(fd, &statbuf);
    if(statbuf.st_size > 4095){
      printf("%s is too big for a single chunk and will be split\n",
	     argv[rcnt + 1]);
      chunkcnt += statbuf.st_size / CHUNKSIZE;
      chunkcnt += (statbuf.st_size % CHUNKSIZE == 0) ? 0 : 1;
    }
    else{
      chunkcnt++;
    }
    close(fd);
  }

  // Record count * field count
  printf("Chunk count is %d\n", chunkcnt);
  memopad_insertinteger(output, chunkcnt * 6);

  // For each record
  for(rcnt = 0 ; rcnt < argc - 1; rcnt++){
    // Open the data file
    printf("Processing %s: ", argv[rcnt + 1]);

    if ((fd = open (argv[rcnt + 1], O_RDONLY)) < 0)
      {
	fprintf (stderr, "Could not open the data file: %s\n", argv[rcnt + 1]);
	exit (0);
      }
    
    fstat(fd, &statbuf);
    offset = 0;

    if ((data = (char *) 
	 mmap (NULL, statbuf.st_size, 
	       PROT_READ, MAP_SHARED, fd, 0)) == -1)
      {
	fprintf (stderr, "Could not mmap data file: %s\n", argv[rcnt + 1]);
	exit (0);
      }
    
    while(offset < statbuf.st_size){
      printf(".");

      // Record id
      memopad_insertinteger(output, 1);
      memopad_insertinteger(output, 15466507 - 
			    (1 + (int) (15466507.0 * rand() / 
					(RAND_MAX + 1.0))));
      
      // Status
      memopad_insertinteger(output, 1);
      memopad_insertinteger(output, 0);
      
      // Position
      memopad_insertinteger(output, 1);
      memopad_insertinteger(output, 1);
      
      // Memo contents -- we add the name of the file as the first line of
      // the memo entry so we know what file we are editting...
      memopad_insertinteger(output, 5);
      memopad_insertinteger(output, 0);
      
      contents = memopad_xsnprintf("File: %s [%03d]\r\n%s", argv[rcnt + 1], 
				   offset / CHUNKSIZE,
				   memopad_strlimit(data + offset, CHUNKSIZE));
      memopad_insertstring(output, contents);
      memopad_xfree(contents);
      
      // Private
      memopad_insertinteger(output, 6);
      memopad_insertinteger(output, 0);
      
      // Category
      memopad_insertinteger(output, 1);
      memopad_insertinteger(output, 0);

      offset += CHUNKSIZE;
    }

    printf("\n");
    munmap(data, statbuf.st_size);
    close(fd);
  }
}

char *memopad_xsnprintf (char *format, ...)
{
  char *output = NULL;
  int size, result;
  va_list ap;

  /* We start with the size of the format string as a guess */
  size = strlen (format);
  va_start (ap, format);

  while (1)
    {
      output = (char *) memopad_xrealloc (output, size);
      result = vsnprintf (output, size, format, ap);

      if (result == -1)
        {
          /* Up to glibc 2.0.6 and Microsoft's implementation */
          size += 100;
        }
      else
        {
          /* Check if we are done */
          if (result < size)
            break;

          /* Glibc from now on */
          size = result + 1;
        }
    }

  va_end (ap);
  return output;
}

void
memopad_xfree (void *memory)
{
  if (memory != NULL)
    free(memory);
}

void *
memopad_xrealloc (void *memory, size_t size)
{
  void *buffer;

  if ((buffer = realloc (memory, size)) == NULL)
    {
      fprintf(stderr, "Realloc of memory failed");
      exit(42);
    }

  return buffer;
}

int memopad_min(int one, int two)
{
  if(one < two) return one;
  return two;
}

void *memopad_xmalloc (size_t size)
{
  void *buffer;

  if ((buffer = malloc (size)) == NULL)
    {
      fprintf(stderr, "memopad memory allocation error");
      exit(42);
    }

  return buffer;
}

char *memopad_strlimit(char *str, int maxlen){
  char *newptr;
  int count;

  if(strlen(str) < maxlen) return str;
  
  newptr = memopad_xmalloc(sizeof(char) * maxlen);
  for(count = 0; count < maxlen; count++){
    newptr[count] = str[count];
  }
  newptr[count] = 0;
  return newptr;
}
