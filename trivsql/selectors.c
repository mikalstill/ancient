#include "trivsql.h"
#include <string.h>

int trivsql_selequal(char *arg1, char *arg2){
  if(strcmp(arg1, arg2) == 0) return SELTRUE;
  return SELFALSE;
}

int trivsql_sellike(char *arg1, char *arg2){
  if(strstr(arg1, arg2) != NULL) return SELTRUE;
  return SELFALSE;
}
