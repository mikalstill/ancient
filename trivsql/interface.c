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

  // Was there an error?
  switch(rs->errno){
  case TRIVSQL_FALSE:
    printf("This statement produced no results.\n");
    return;

  case TRIVSQL_TRUE:
    break;

  default:
    printf("There was an error processing this statement (%d).\n",
	   rs->errno);
    return;
  }

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
  trivsql_rsmovefirst(rs);
  while(trivsql_rseof(rs) != TRIVSQL_TRUE){
    for(i = 0; i < rs->numCols; i++)
      printf("| %-12s ", trivsql_rsfield(rs, i));
    printf("|\n-");
    for(i = 0; i < rs->numCols; i++){
      printf("---------------");
    }
    printf("\n");

    trivsql_rsmovenext(rs);
  }

  printf("\n\n");
  printf("Select returned %d rows of %d columns\n\n", 
	 rs->numRows, rs->numCols);
}

void trivsql_rsmovefirst(trivsql_recordset *rs){
  rs->currentRow = rs->rows;
}

void trivsql_rsmovenext(trivsql_recordset *rs){
  if(rs->currentRow->next != NULL)
    rs->currentRow = rs->currentRow->next;
}

int trivsql_rseof(trivsql_recordset *rs){
  if(rs->errno != TRIVSQL_TRUE) return TRIVSQL_TRUE;
  return rs->currentRow->next == NULL ? TRIVSQL_TRUE : TRIVSQL_FALSE;
}

int trivsql_rsbof(trivsql_recordset *rs){
  return rs->currentRow == rs->rows ? TRIVSQL_TRUE : TRIVSQL_FALSE;
}

char *trivsql_rsfield(trivsql_recordset *rs, int colnum){
  int count;
  trivsql_col *theCol;

  count = 0;
  theCol = rs->currentRow->cols;
  while((theCol->next != NULL) && (count < colnum)){
    theCol = theCol->next;
    count++;
  }

  return theCol->val;
}
