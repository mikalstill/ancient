#include <stdio.h>
#include "trivsql.h"

int main(int argc, char *argv[]){
  trivsql_state *ourState;
  trivsql_recordset *rs;
  char cmd[1000];

  ourState = trivsql_opendb("foo.tdb");

  while(fgets(cmd, 1000, stdin) != NULL){
    rs = trivsql_execute(ourState, cmd);
    if(rs != NULL) trivsql_displayrs(rs);
    else printf("NULL recordset\n");
    trivsql_xfree(rs);
  }
}
