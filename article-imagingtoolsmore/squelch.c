#include <stdio.h>

int main(int argc, char *argv[])
{
  int c, brcnt = 0;
  
  while((c = fgetc(stdin)) != EOF)
    {
      if(c == '\r'){}
      else if((c == '\n') && (brcnt == 0)) printf("%c", c);
      else if(c != '\n') printf("%c", c);

      if(c == '<') brcnt++;
      if(c == '>') brcnt--;
    }
}
