#include "trivsql.h"
#include <string.h>

int trivsql_selequal(char *arg1, char *arg2){
  if(strcmp(arg1, arg2) == 0) return SELTRUE;
  return SELFALSE;
}

// todo: implement %'s
int trivsql_sellike(char *arg1, char *arg2){
  int front = 0, back = 0, ret;
  char *sel;

  sel = strdup(arg2);

  if(arg2[0] == '%'){
    front = 1;
    sel++;
  }

  if(arg2[strlen(arg2) - 1] == '%'){
    back = 1;
    sel[strlen(sel) - 1] = '\0';
  }

  printf("%d %d\n", front, back);

  if(strstr(arg1, sel) != NULL) ret = SELTRUE;
  ret = SELFALSE;

  trivsql_xfree(sel);
  return ret;
}
