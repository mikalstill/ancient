#include "trivsql.h"
#include <string.h>

int trivsql_selequal(char *arg1, char *arg2){
  if(strcmp(arg1, arg2) == 0) return SELTRUE;
  return SELFALSE;
}

// todo: implement %'s
int trivsql_sellike(char *arg1, char *arg2){
  int front = 0, back = 0, ret;
  char *sel, *str;

  sel = strdup(arg2);

  if(arg2[0] == '%'){
    front = 1;
    sel++;
  }

  if(arg2[strlen(arg2) - 1] == '%'){
    back = 1;
    sel[strlen(sel) - 1] = '\0';
  }

  printf("%d %d (%s)\n", front, back, sel);

  if((str = strstr(arg1, sel)) != NULL){
    if((front == 1) && (str == arg1))
      ret = SELFALSE;
    else
      ret = SELTRUE;
  }
  else ret = SELFALSE;

  //trivsql_xfree(sel);
  return ret;
}

int trivsql_selor(trivsql_seltreenode *left, trivsql_seltreenode *right){
  if(left->selector(left->selArgOne, left->selArgTwo) == TRIVSQL_TRUE)
    return TRIVSQL_TRUE;
  if(right->selector(right->selArgOne, right->selArgTwo) == TRIVSQL_TRUE)
    return TRIVSQL_TRUE;
  return TRIVSQL_FALSE;
}

int trivsql_seland(trivsql_seltreenode *left, trivsql_seltreenode *right){
  if(left->selector(left->selArgOne, left->selArgTwo) == TRIVSQL_FALSE)
    return TRIVSQL_FALSE;
  if(right->selector(right->selArgOne, right->selArgTwo) == TRIVSQL_FALSE)
    return TRIVSQL_FALSE;
  return TRIVSQL_TRUE;
}
