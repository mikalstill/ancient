#include <stdio.h>

int main(int argc, char *argv[])
{
  char input[255];
  
  while(fgets(input, 255, stdin))
    {
      if(strcmp(input, "Mikal\n") == 0)
	printf("Hi!\n");
      else
	printf("Hello %s", input);
    }

  printf("Bye\n");
}
