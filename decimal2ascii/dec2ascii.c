#include <stdio.h>

int main(int argc, char *argv[]){
  char s[200];

  while(fgets(s, 200, stdin) != NULL){
    if(isprint(atoi(s)))
      printf("%c", atoi(s));
    else
      printf(" \\%d ", atoi(s));
  }
}
