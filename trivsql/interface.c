#include "trivsql.h"

char *gTrivData = NULL;
int gTrivInset;

// Interface to the SQL database
trivsql_state *trivsql_opendb(char *path){
  return trivsql_init(path);
}

trivsql_recordset *trivsql_execute(trivsql_state *state, char *sql){
  printf("[Executing %s]\n", sql);

  trivsql_xfree(gTrivData);
  gTrivData = trivsql_xsnprintf("%s", sql);
  gTrivInset = 0;
  
  yyparse();
}

int trivsql_gettext(char *buffer, int maxlen){
  int size;

  printf("[Starting read]\n");

  // Determine the maximum size to return
  size = trivsql_min(maxlen, strlen(gTrivData) - gTrivInset);

  if(size > 0){
    memcpy(buffer, gTrivData + gTrivInset, size);
    gTrivInset += size;
  }

  printf("[Read %d chars]\n", size);
  return size;
}
