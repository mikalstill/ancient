// A simple example of how to use the POSIX regular expression matching
// available in glibc

#include <stdio.h>
#include <regex.h>

char *urls[] = {"wibble.mp3", "foobar.mp4", "banana.jpg", "whatever", NULL};

int main(int argc, char *argv[]){
  regex_t compre;
  int count;

  if(regcomp(&compre, ".*mp3", 0) != 0){
    fprintf(stderr, "Regular expression compilation failled\n");
    exit(1);
  }
  
  count = 0;
  while(urls[count] != NULL){
    printf("%s: ", urls[count]);
    if(regexec(&compre, urls[count], 0, NULL, 0) == 0){
      printf("match\n");
    }
    else{
      printf("miss\n");
    }
    count++;
  }

  regfree(&compre);
  return 0;
}
