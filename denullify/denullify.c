#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main (int argc, char *argv[])
{
  char *searchTerm = NULL, *searchFile = NULL;
  char **args;
  int optchar, caseInsensitive = 0, i, j;

  while ((optchar = getopt (argc, argv, "i")) != -1)
    {
      switch (optchar)
	{
	case 'i':
	  caseInsensitive = 1;
	  break;

	default:
	case '?':
	  fprintf (stderr, "Unknown command line option...\n");
	  fprintf (stderr, "This is a simple wrapper to protect grep from\n");
	  fprintf (stderr, "searchs of death.\n\n");
	  fprintf (stderr, "Try: [-i] <search term> <file>\n", argv[0]);
	  exit (1);
	  break;
	}
    }

  // Make sure we have enough remaining arguments for the search term
  // and the file to search
  if (argc < optind + 2)
    {
      fprintf (stderr, "Please specify search term and file to search\n");
      exit (1);
    }

  // Make sure the search term isn't an empty string
  if (strcmp (argv[optind], "") == 0)
    {
      fprintf (stderr, "Skipped null search of doom\n");
      exit (0);
    }

  // Args is an array of char *'s which point to the arguements. The first
  // arg is traditionally the name of the command
  if ((args =
       (char **) malloc ((1 + argc - optind) * sizeof (char *))) == NULL)
    {
      fprintf (stderr, "Couldn't allocate memory\n");
      exit (1);
    }

  // Build the argument array for the real command
  args[0] = strdup ("grep");
  if (caseInsensitive)
    {
      args[1] = strdup ("-i");
      i = 1;
    }
  else
    {
      i = 0;
    }

  for (j = optind; j < argc; j++)
    {
      args[1 + i + j - optind] = argv[j];
    }
  args[1 + i + j - optind] = NULL;

  // And now we can execute the grep command
  if (execvp ("/bin/grep", args) == -1)
    exit (2);
}
