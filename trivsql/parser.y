%{
  #include <stdio.h>
  #include "trivsql.h"

  #define YYERROR_VERBOSE 1

  trivsql_state *gState;
%}

%token CREATE TABLE 
%token INSERT VALUES INTO
%token SELECT FROM STRING WHERE LIKE
%token ALTER ADD COLUMN
%token UPDATE SET

%%

sql      : create sql | insert sql | sel sql | alt sql | upd sql
         |
         ;



create   : CREATE TABLE STRING '(' colvalspec ')' ';' 
{ gState->rs = trivsql_makers((char *) $3); trivsql_docreate((char *) $3, (char *) $5); } 
         ;

insert   : INSERT INTO STRING '(' colvalspec ')' VALUES '(' colvalspec ')' ';'
{ gState->rs = trivsql_makers((char *) $3); gState->rs->errno = trivsql_checktable((char *) $3); if(gState->rs->errno == TRIVSQL_FALSE){trivsql_doinsert((char *) $3, (char *) $5, (char *) $9);}}
         ;

sel      : SELECT cvsaster FROM STRING selector ';'
{ gState->rs = trivsql_makers((char *) $4); gState->rs->errno = trivsql_checktable((char *) $4); if(gState->rs->errno == TRIVSQL_FALSE){trivsql_doselect((char *) $4, (char *) $2);}}
         ;

alt      : ALTER STRING ADD COLUMN STRING ';'
{ gState->rs = trivsql_makers((char *) $2); gState->rs->errno = trivsql_checktable((char *) $2); if(gState->rs->errno == TRIVSQL_FALSE){trivsql_doalter((char *) $2, (char *) $5);}}
         ;

upd      : UPDATE STRING SET STRING '=' str selector ';'
{gState->rs = trivsql_makers((char *) $2); gState->rs->errno = trivsql_checktable((char *) $2); if(gState->rs->errno == TRIVSQL_FALSE){trivsql_doselect((char *) $2, (char *) $4); trivsql_updaters(gState, gState->rs, (char *) $4, (char *) $6);}}
         ;



cvsaster : colvalspec { $$ = trivsql_xsnprintf("%s", (char *) $1); }
         | '*' { $$ = trivsql_xsnprintf("*"); }
         ;

colvalspec : str ',' colvalspec { $$ = trivsql_xsnprintf("%s;%s", (char *) $1, (char *) $3); } 
         | str { $$ = trivsql_xsnprintf("%s", (char *) $1); }
         ;

selector : WHERE str '=' str { gState->selector = trivsql_selequal; gState->selArgOne = $2; gState->selArgTwo = $4 }
         | WHERE str LIKE str { gState->selector = trivsql_sellike; gState->selArgOne = $2; gState->selArgTwo = $4 }
	 | { gState->selector = NULL }
         ;

str      : STRING { $$ = $1 } 
         | '\'' STRING '\'' { $$ = $2 }
         ; 

%%

int yyerror(char *s){
  printf("\nsql parsing error: %s\n", s);
  exit(42);
}
