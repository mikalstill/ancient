%{
  #include <stdio.h>
%}



%token BEGIN VCARD END
%token NAME STRING
%token NUMBER

%%

vcard    : BEGIN ':' VCARD { printf("\nvcard header\n"); }
             cardlines
             END ':' VCARD { printf("\nvcard footer\n"); }
         ;

cardlines: NAME ':' value { printf("\n%s %s\n", $1, $3); } cardlines
         |
         ;

value    : NUMBER value
         | NAME value
         | STRING value
         |
         ;

%%

int yyerror(char *s){
  printf("vcard parsing error: %s\n", s);
  exit(42);
}

// The main routine for the engine
int main(int argc, char *argv[]){
  yyparse();

  return 0;
}
