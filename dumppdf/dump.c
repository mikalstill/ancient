/*****************************************************************************
  A simple application that dumps a PDF (including things like control r's)
  because less, vi, and emacs on a linux machine just say DOS file, which
  makes debugging hard...

  Do no error checking, just a quick and dirty.

  Michael Still 12 July 2000
*****************************************************************************/

int main(int argc, char *argv[]){
  FILE    *input;
  int     c;

  input = fopen(input, "r");

  while((c = fgetc(input)) != EOF){
    switch(c){
    case '\r':
      printf("[\r]");
      break;

    default:
      printf("%c", c);
      break;
    }

    printf("[EOF]\n\n");
  }
