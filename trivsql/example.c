#include <stdio.h>
#include "trivsql.h"

int main(int argc, char *argv[]){
  trivsql_state *ourState;
  trivsql_recordset *rs;
  char cmd[1000];

  if(argc != 2){
    fprintf(stderr, "Please specify a db file\n");
    exit(42);
  }

  ourState = trivsql_opendb(argv[1]);

  while(fgets(cmd, 1000, stdin) != NULL){
    rs = trivsql_execute(ourState, cmd);
    if(rs != NULL) trivsql_displayrs(rs);
    else printf("NULL recordset\n");
    trivsql_xfree(rs);
  }
}
