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

#define LINEWIDTH 140

int side_min (int, int);

int
main (int argc, char *argv[])
{
  char temp[2][100], fmt[255], cmd[2][1024], *file[2], color[2][10],
    line[LINEWIDTH];
  int fd[2], cnt[2], done;
  struct stat sb[2];
  off_t inset[2];
  int count[2], currfile;

  // Check the arguements
  if (argc < 4)
    {
      fprintf (stderr, "Usage: %s <command> <file1> <file2>\n", argv[0]);
      exit (1);
    }

  // Setup the color control codes. This should be integrated with getopt 
  // one day...
  snprintf (color[0], 10, "%c[34m", 27);	// blue
  snprintf (color[1], 10, "%c[31m", 27);	// red

  // Filter the files into temporary locations -- open to format string 
  // attacks, as we as temporary file substitution attacks
  fprintf (stderr,
	   "Warning, do not use this command in a secure environment...\n");
  snprintf (fmt, 255, "%s > %%s", argv[1]);
  for (currfile = 0; currfile < 2; currfile++)
    {
      snprintf (temp[currfile], 100, "/tmp/side.XXXXXX");
      if (fd[currfile] = mkstemp (temp[currfile]) < 0)
	{
	  perror ("Could not make temporary file one");
	  exit (1);
	}
      close (fd[currfile]);
      snprintf (cmd[currfile], 1024, fmt, argv[2 + currfile], temp[currfile]);
      printf ("Executing: %s\n", cmd[currfile]);
      system (cmd[currfile]);
      if (fd[currfile] = open (temp[currfile], NULL) < 0)
	{
	  perror ("Error opening temporary file one");
	  exit (1);
	}

      // Do some memory mapping
      if (fstat (fd[currfile], &sb[currfile]) < 0)
	{
	  perror ("Could not stat file");
	  exit (1);
	}

      if ((file[currfile] =
	   (char *) mmap (NULL, sb[currfile].st_size, PROT_READ, MAP_SHARED,
			  fd[currfile], 0)) == -1)
	{
	  perror ("Could not mmap file");
	  exit (1);
	}

      count[currfile] = 0;
      inset[currfile] = 0;
    }

  // Display the files
  // Color control codes stolen from http://www.termsys.demon.co.uk/vtansi.htm
  memset (line, ' ', LINEWIDTH);
  snprintf (line, LINEWIDTH / 2, "%s", cmd[0]);
  line[side_min (strlen (cmd[0]), LINEWIDTH / 2 - 1)] = ' ';
  snprintf (line + (LINEWIDTH / 2) + 1, LINEWIDTH / 2, "%s", cmd[1]);
  line[LINEWIDTH / 2] = '|';
  printf ("  %s\n", line);

  memset (line, '-', LINEWIDTH);
  line[LINEWIDTH] = '\0';
  line[LINEWIDTH - 1] = '\n';
  line[LINEWIDTH / 2] = '|';
  printf ("  %s", line);

  while ((inset[0] < sb[0].st_size) || (inset[1] < sb[1].st_size))
    {
      memset (line, ' ', LINEWIDTH);
      line[LINEWIDTH] = '\0';
      line[LINEWIDTH - 1] = '\n';
      line[LINEWIDTH / 2] = '|';

      for (currfile = 0; currfile < 2; currfile++)
	{
	  if (inset[currfile] >= sb[currfile].st_size)
	    {
	      line[1 + (currfile * LINEWIDTH / 2)] = '*';
	    }
	  else
	    {
	      while ((count[currfile]++ < (LINEWIDTH / 2) - 1) &&
		     (file[currfile][inset[currfile]] != '\n'))
		{
		  line[count[currfile] + (currfile * LINEWIDTH / 2) +
		       currfile] = file[currfile][inset[currfile]++];
		  if (file[currfile][inset[currfile] - 1] == '\t')
		    count[currfile] += 7;
		}
	    }

	  count[currfile] = 0;
	  inset[currfile]++;
	}

      if (strncmp (line, line + (LINEWIDTH / 2) + 1, (LINEWIDTH / 2) - 2) ==
	  0)
	printf ("%s  ", color[0]);
      else
	printf ("%s++", color[1]);
      printf ("%s", line);
    }

  // Cleanup
  printf ("%c[0m", 27);
  for (currfile = 0; currfile < 2; currfile++)
    {
      if (munmap (file[currfile], sb[currfile].st_size) < 0)
	{
	  perror ("Could not unmap memory one");
	  exit (1);
	}
      close (fd[currfile]);
    }
}

int
side_min (int a, int b)
{
  if (a < b)
    return a;
  return b;
}
