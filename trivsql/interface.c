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
  char *t, *u, *c, *localCols, *tempCols;
  trivsql_row *theRow;
  trivsql_col *theCol;

  // Print the header line
  printf("Select returned %d rows of %d columns\n\n=", rs->numRows, rs->numCols);
  for(i = 0; i < rs->numCols; i++){
    printf("===============");
  }
  printf("\n|");

  // If the columns list is '*', substitute a list of all the columns
  if(strcmp(rs->cols, "*") == 0)
    localCols = trivsql_getallcolumns(rs->tname);
  else{
    localCols = rs->cols;
    tempCols = trivsql_xsnprintf("%s", rs->cols);

    // Determine that the named columns exist (unless we know they do)
    col = 0;
    c = strtok(tempCols, ";");
    while(c != NULL){
      i = 0;
      while(1){
	t = trivsql_xsnprintf("trivsql_%s_col%d", rs->tname, i);
	u = trivsql_dbread(gState, t);
	
	if(u == NULL){
	  trivsql_xfree(t);
	  fprintf(stderr, "%s is an unknown column\n", c);
	  return;
	}
	else if(strcmp(u, c) == 0){
	  trivsql_xfree(t);
	  trivsql_xfree(u);
	  break;
	}
	
	trivsql_xfree(t);
	trivsql_xfree(u);
	i++;
      }

    c = strtok(NULL, ";");
    }
  }

  // Print out the column names
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
}
