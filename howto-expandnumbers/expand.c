#include <stdio.h>

// Expand numbers in strings so that numberic sorting is sexy

int main(int argc, char *argv[])
{
  char input[1024];
  int i, num, numvalid;

  // We read strings in from stdin, and assume that each string is less
  // than 1024 characters long. Expanded strings are written to stdout
  while(fgets(input, 1024, stdin) != NULL)
    {
      numvalid = 0;
      num = 0;
      for(i = 0; i < strlen(input); i++)
	{
	  if(!isdigit(input[i]))
	    {
	      if(numvalid != 0)
		{
		  printf("%08d", num);
		  numvalid = 0;
		  num = 0;
		}
	      printf("%c", input[i]);
	    }
	  else
	    {
	      num *= 10;
	      num += input[i] - '0';
	      numvalid = 1;
	    }
	}
      
      // We don't need to do any cleanup, as we know each line ends with a \n
      // if this wasn't the case, we'd include a test for num != 0 here...
    }
}
