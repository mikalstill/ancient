%{
  #include <stdio.h>

  int parsemode;
%}

%token BEGIN VCARD END LINEFOLD
%token ENCODING
%token FN N NICKNAME PHOTO BDAY
%token ADR LABEL TEL EMAIL MAILER
%token VERSION URL
%token TZ GEO
%token TITLE ROLE LOGO AGENT ORG
%token CATEGORIES NOTE PRODID REV SORTSTRING SOUND UID
%token CLASS KEY
%token STRING

%%

vcard    : BEGIN ':' VCARD { printf("\nvcard header\n"); }
             cardlines
             END ':' VCARD { printf("\nvcard footer\n"); }
         ;

cardlines: names encoding ':' value cardlines
         |
         ;

names    : name
         | name ';' names
         |
         ;

name     : FN | N | NICKNAME | PHOTO | BDAY | VERSION | URL | ADR | LABEL | TEL | EMAIL | MAILER | TZ | GEO | TITLE | ROLE | LOGO | AGENT | ORG | CATEGORIES | NOTE | PRODID | REV | SORTSTRING | SOUND | UID | CLASS | KEY
         ;

encoding : ENCODING '=' STRING
         |
         ;

value    : { parsemode = 2; } STRING { parsemode = 1; } value 
         | 
         ;

%%

int yyerror(char *s){
  printf("\n\nvcard parsing error: %s\n", s);
  exit(42);
}

// The main routine for the engine
int main(int argc, char *argv[]){
  parsemode = 1;
  yyparse();

  return 0;
}
