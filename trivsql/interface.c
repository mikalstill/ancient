#include "trivsql.h"

char *gTrivData = NULL;
int gTrivInset;

extern trivsql_state *gState;

// Interface to the SQL database
trivsql_state *trivsql_opendb(char *path){
  return trivsql_init(path);
}

trivsql_recordset *trivsql_execute(trivsql_state *state, char *sql){
  trivsql_xfree(gTrivData);
  gTrivData = trivsql_xsnprintf("%s", sql);
  gTrivInset = 0;

  trivsql_xfree(state->rs);
  state->rs = NULL;
  gState = state; 
  yyparse();
  return gState->rs;
}

int trivsql_gettext(char *buffer, int maxlen){
  int size;

  // Determine the maximum size to return
  size = trivsql_min(maxlen, strlen(gTrivData) - gTrivInset - 1);

  if(size > 0){
    memcpy(buffer, gTrivData + gTrivInset, size);
    gTrivInset += size;
  }

  return size;
}

void trivsql_displayrs(trivsql_recordset *rs){
  int i, col;
  char *t, *u, *c, *localCols;
  trivsql_row *theRow;
  trivsql_col *theCol;

  // Print the header line
  printf("\n=");
  for(i = 0; i < rs->numCols; i++){
    printf("===============");
  }
  printf("\n|");

  // Print out the column names
  localCols = trivsql_xsnprintf("%s", rs->cols);
  c = strtok(localCols, ";");
  while(c != NULL){
    printf(" %-12s |", c);
    c = strtok(NULL, ";");
  }
  
  printf("\n=");
  for(i = 0; i < rs->numCols; i++){
    printf("===============");
  }
  printf("\n");

  // Print out the values we have found
  theRow = rs->rows;
  while(theRow->next != NULL){
    printf("|");
    theCol = theRow->cols;
    while(theCol->next != NULL){
      printf(" %-12s |", theCol->val);
      theCol = theCol->next;
    }

    printf("\n-");
    for(i = 0; i < rs->numCols; i++){
      printf("---------------");
    }
    printf("\n");
    theRow = theRow->next;
  }

  printf("\n\n");
  printf("Select returned %d rows of %d columns\n\n", 
	 rs->numRows, rs->numCols);
}
