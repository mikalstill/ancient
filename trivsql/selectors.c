/******************************************************************************
  Copyright (C) Michael Still 2002
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
******************************************************************************/

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

int trivsql_selor(int left, int right){
  if(left == SELTRUE) return SELTRUE;
  if(right == SELTRUE) return SELTRUE;
  return SELFALSE;
}

int trivsql_seland(int left, int right){
  if(left == SELFALSE) return SELFALSE;
  if(right == SELFALSE) return SELFALSE;
  return SELTRUE;
}
