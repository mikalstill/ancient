%{
  #include <stdio.h>
%}

%token BEGIN VCARD END LINEFOLD
%token FN N NICKNAME PHOTO BDAY
%token STRING

%%

vcard    : BEGIN ':' VCARD { printf("\nvcard header\n"); }
             cardlines
             END ':' VCARD { printf("\nvcard footer\n"); }
         ;

cardlines: names ':' value cardlines
         |
         ;

names    : name
         | name ';' names
         |
         ;

name     : FN
         | N
         | NICKNAME
         | PHOTO
         | BDAY
         ;

value    : STRING value
         | 
         ;

%%

int yyerror(char *s){
  printf("\n\nvcard parsing error: %s\n", s);
  exit(42);
}

// The main routine for the engine
int main(int argc, char *argv[]){
  yyparse();

  return 0;
}
