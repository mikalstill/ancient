#include <stdio.h>

int main(int argc, char *argv[])
{
  int i;

  for(i = -5; i < 6; i++)
    {
      printf("%2d: ");
      if(i)
	printf("true");
      else
	printf("false");
      printf("\n");
    }
}
