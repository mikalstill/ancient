%{
  #include "lexinterface.h"
  #include "samples.h"
  #include "pandalex.h"

  #include <stdarg.h>

  #define YYMAXDEPTH 50000
  #define YYERROR_VERBOSE 1

  // The callbacks
  pandalex_callback_type pandalex_callbacks[pandalex_event_max];
%}

          /* Define the possible yylval values */
%union {
  int        intVal;

  struct streamVal{
    char *data;
    int len;
  } sval;
}

%token <sval> VERSION
%token <sval> NAME
%token <sval> STRING
%token <sval> OBJREF <sval> OBJ <sval> ENDOBJ 
%token <intVal> INT
%token <sval> FP
%token <sval> DBLLT <sval> DBLGT
%token <sval> STREAM <sval> ENDSTREAM
%token <sval> ARRAY <sval> ENDARRAY <sval> ENDARRAYDBLGT
%token <sval> PDFEOF XREF TRAILER
%token <sval> ANYTHING

%type <sval> binary
%type <sval> header
%type <sval> objref

%type <intVal> dictionary
%type <intVal> subdictionary

%%

          /*********************************************************
            It would appear that $$ already includes the value of $1,
	    so we only need to append the value of $2, $3, $4 et al
          *********************************************************/
// completely implemented
pdf       : { pandalex_callback(pandalex_event_begindocument, ""); } 
            header { pandalex_callback(pandalex_event_entireheader, $2.data); } 
            object linear objects xref trailer endcrap
          ;

// completely implemented
header    : VERSION { pandalex_callback(pandalex_event_specver, $1.data); }
            binary { $$.data = pandalex_strmcat($1.data, $1.len, $3.data, $3.len); $$.len = $1.len + $3.len + 1; }
          ;

linear    : xref trailer { }
          |
          ;

// Clibpdf sometimes puts some binary crap at the end of the file (pointer
// problems?)
endcrap   : binary { }
          |
          ;

// completely implemented
objects   : object objects
          | 
          ;

// todo_mikal: might need a .data here
object    : INT INT OBJ { pandalex_callback(pandalex_event_objstart, $1, $2); } 
            dictionary { if($5 != -1) pandalex_callback(pandalex_event_dictint, $1, $2, $5); } 
            stream ENDOBJ {}
          ;

dictionary: DBLLT dict DBLGT { $$ = -1; }
          | INT { $$ = $1; }
          | ARRAY arrayvals ENDARRAY { $$ = -1; }
          | objref { $$ = -1; }
          | NAME { $$ = -1; }
          | STRING { $$ = -1 };
          | { $$ = -1; }
          ;

subdictionary: DBLLT dict DBLGT { $$ = -1 };

dict      : NAME STRING { pandalex_callback(pandalex_event_dictitem_string, $1.data, $2.data); } dict
          | NAME NAME { pandalex_callback(pandalex_event_dictitem_name, $1.data, $2.data); } dict
          | NAME ARRAY arrayvals ENDARRAY { pandalex_callback(pandalex_event_dictitem_array, $1.data, $2.data); } dict
          | NAME objref { pandalex_callback(pandalex_event_dictitem_object, $1.data, $2.data); } dict
          | NAME { pandalex_callback(pandalex_event_dictitem_dict, $1.data); } 
              subdictionary { pandalex_callback(pandalex_event_dictitem_dictend, $1.data); } dict
          | NAME INT { pandalex_callback(pandalex_event_dictitem_int, $1.data, $2); } dict
          | NAME FP {} dict
          | 
          ;

arrayvals : objref arrayvals {}
          | INT arrayvals {}
          | NAME arrayvals {}
          | STRING arrayvals {}
          | ARRAY arrayvals {}
          | DBLLT dict DBLGT arrayvals {}
          |
          ;

objref    : INT INT OBJREF { if(($$.data = (char *) malloc((pandalex_intlen($1) + pandalex_intlen($2) + 5) * sizeof(char))) == NULL){
			       fprintf(stderr, "Could not allocate enough space for objref\n");
			       exit(42);
                               }
			     
			     sprintf($$.data, "%d %d R", $1, $2);
			     $$.len = strlen($$.data) + 1;
			                       }
          ;

stream    : STREAM binary ENDSTREAM { free($2); }
          |
          ;

binary    : ANYTHING binary { $$.data = pandalex_strmcat($1.data, $1.len, $2.data, $2.len); $$.len = $1.len + $2.len; free($2); }
          | STRING binary { $$.data = pandalex_strmcpy($1.data, -1); $$.len = strlen($1.data); }
          | { $$.data = pandalex_strmcpy("", -1); $$.len = 0; }
          ;

xref      : XREF INT INT xrefitems {}
          ;

xrefitems : INT INT STRING xrefitems {}
          |
          ;

trailer   : TRAILER DBLLT dict DBLGT STRING INT PDFEOF {}
          ;

%%

void pandalex_init(){
  int i;

  // Make sure that the callbacks default to nothing
  for(i = 0; i < pandalex_event_max; ++i){
    pandalex_callbacks[i] = NULL;
  }
}

void pandalex_setupcallback(int callback, pandalex_callback_type functoid){
  pandalex_callbacks[callback] = functoid;
}

// Here we call the callbacks. This includes converting to the types that the
// callback function expects.
void pandalex_callback(int event, ...){
  va_list argptr;

  // Start accessing the arguements from the end
  va_start(argptr, event);
  
  // If no event handler is setup, then we ignore the event
  if(pandalex_callbacks[event] != NULL){
    pandalex_callbacks[event] (event, argptr);
  }
  
  // Stop with the arguements
  va_end(argptr);
}

int pandalex_parse(){
  // We are not looking into a stream at the moment
  yyparse();
}

int yyerror(char *s){
  fprintf(stderr, "\n---------------------------------------------------------------\n");
  fprintf(stderr, "PandaLex parser error (%s):\n", s);
  fprintf(stderr, "  Please send this error text, along with a copy of your PDF\n");
  fprintf(stderr, "  document (if possible) to mikal@stillhq.com, so that this can\n");
  fprintf(stderr, "  be fixed for the next release...\n\n");
  fprintf(stderr, "version = 0.4\n");
  fprintf(stderr, "last token = \"%s\" (%d) or %d\n", yylval.sval.data, yylval.sval.len, yylval.intVal);
  fprintf(stderr, "\n---------------------------------------------------------------\n");

  exit(42);
}

// Buffer overrun safe strcat
char *pandalex_strmcat(char *dest, int destLen, char *append, int appendLen){
  char *new;
  int count, len;

  // What length do we need?
  if((new = (char *) malloc(sizeof(char) * 
			    (((destLen == -1) ? strlen(dest) : destLen) + 
			    ((appendLen == -1) ? strlen(append) : appendLen) + 
			    2))) == NULL){
    fprintf(stderr, "Could not malloc enough space\n");
    exit(42);
  }
  
  if((destLen == -1) && (appendLen == -1))
    sprintf(new, "%s%s", dest, append);
  else{
    // We need to copy characters the hard way -- change this to a memcpy
    count = 0;

    for(len = 0; len < ((destLen == -1) ? strlen(dest) : destLen); len++){
      new[count] = dest[len];
      count++;
    }

    for(len = 0; len < ((appendLen == -1) ? strlen(append) : appendLen); len++){
      new[count] = append[len];
      count++;
    }

    new[count] = '\0';
  }
  return new;
}

// Buffer overrun safe strcpy
char *pandalex_strmcpy(char *data, int len){
  return pandalex_strmcat(data, len, "", 0);
}

int pandalex_intlen(int number){
  int length = 0;

  while(number > 0){
    length ++;
    number /= 10;
  }

  return number;
}
