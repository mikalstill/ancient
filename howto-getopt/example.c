#include <unistd.h>
#include <stdio.h>

int
main (int argc, char *argv[])
{
  char *inputFile = NULL, *outputFile = NULL;
  int doGronk = 0, optchar;

  while ((optchar = getopt (argc, argv, "i:o:g")) != -1)
    {
      switch (optchar)
	{
	case 'i':
	  inputFile = (char *) strdup (optarg);
	  break;

	case 'o':
	  outputFile = (char *) strdup (optarg);
	  break;

	case 'g':
	  doGronk = 1;

	default:
	case '?':
	  printf ("Unknown command line option...\n");
	  printf ("Try: %s [-i input] [-o output] [-g]\n", argv[0]);
	  exit (0);
	  break;
	}
    }

  printf ("Command line parsed to: input = %s, output = %s, gronk = %d\n",
	  inputFile, outputFile, doGronk);
  return 1;
}
