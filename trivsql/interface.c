#include "trivsql.h"

char *gTrivData = NULL;
int gTrivInset;

// Interface to the SQL database
trivsql_state *trivsql_opendb(char *path){
  return trivsql_init(path);
}

trivsql_recordset *trivsql_execute(trivsql_state *state, char *sql){
  trivsql_xfree(gTrivData);
  gTrivData = trivsql_xsnprintf("%s", sql);
  gTrivInset = 0;
}

int trivsql_gettext(char *buffer, int maxlen){
  

}
