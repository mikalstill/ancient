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
  if(trivsql_initok(ourState) != TRIVSQL_TRUE){
    fprintf(stderr, "Database open failed\n");
    exit(42);
  }

  while(fgets(cmd, 1000, stdin) != NULL){
    rs = trivsql_execute(ourState, cmd);
    if(rs != NULL) trivsql_displayrs(rs);
    else printf("NULL recordset\n");
    trivsql_xfree(rs);
  }
}
