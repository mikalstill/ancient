#include <stdio.h>

int main(int argc, char *argv[]){
  trivsql_state *ourState;
  char cmd[1000];

  ourState = trivsql_opendb("foo.tdb");

  printf(">> ");
  fflush(stdout);

  while(fgets(cmd, 1000, stdin) != NULL){
    trivsql_execute(ourState, cmd);

    printf(">> ");
    fflush(stdout);
  }
}
