%{
  #include "lexinterface.h"
  #include "samples.h"
  #include "pandalex.h"

  #include <stdarg.h>

  int    binaryMode;

  // The callbacks
  pandalex_callback_type pandalex_callbacks[gpandalex_callback_max];
%}

          /* Define the possible yylval values */
%union {
  int      intVal;
  char     *textVal;
}

%token <textVal> VERSION
%token <textVal> NAME <textVal> STRING
%token <textVal> OBJREF <textVal> OBJ <textVal> ENDOBJ <intVal> INT 
%token <textVal> FP <textVal> DBLLT <textVal> DBLGT 
%token <textVal> STREAM <textVal> ENDSTREAM
%token <textVal> ARRAY <textVal> ENDARRAY
%token <textVal> PDFEOF
%token <textVal> ANYTHING

%type <textVal> binary
%type <textVal> header

%%

          /*********************************************************
            It would appear that $$ already includes the vale of $1,
	    so we only need to append the value of $2, $3, $4 et al
          *********************************************************/
pdf       : header { 
                    pandalex_callback(gpandalex_callback_entireheader, 
				      gpandalex_callback_type_string, $1); } 
            objects xref trailer
          ;

header    :  VERSION { 
	            pandalex_callback(gpandalex_callback_specver, 
				      gpandalex_callback_type_string, $1); }
             binary { 
                    strmcat($$, $3); }
          ;

objects   : object objects
          |
          ;

object    : INT INT OBJ { 
                    pandalex_callback(gpandalex_callback_objstart, 
				      gpandalex_callback_type_object, 
				      $1, $2); } 
            DBLLT dict DBLGT stream ENDOBJ {}
          ;

dict      : NAME STRING dict { 
                    pandalex_callback(gpandalex_callback_dictitem_string, 
				      gpandalex_callback_type_string, 
				      $1, $2); }
          | NAME NAME dict { 
                    pandalex_callback(gpandalex_callback_dictitem_name, 
				      gpandalex_callback_type_string, 
				      $1, $2); }
          | NAME ARRAY arrayvals ENDARRAY dict { 
	            pandalex_callback(gpandalex_callback_dictitem_array, 
				      gpandalex_callback_type_string, 
				      $1, $2); }
          | NAME OBJREF dict { 
                    pandalex_callback(gpandalex_callback_dictitem_object, 
				      gpandalex_callback_type_string, 
				      $1, $2); }
          | NAME DBLLT dict DBLGT dict { 
                    pandalex_callback(gpandalex_callback_dictitem_dict, 
				      gpandalex_callback_type_string, 
				      $1, $2); }
          | NAME INT dict { 
                    pandalex_callback(gpandalex_callback_dictitem_int, 
				      gpandalex_callback_type_string, 
				      $1, $2); }
          |
          ;

arrayvals : OBJREF arrayvals {}
          | INT arrayvals {}
          |
          ;

stream    : STREAM { binaryMode = 1; } binary { binaryMode  } DBLLT dict DBLGT stream ENDOBJ {}
          ;

dict      : NAME STRING dict {}
          | NAME NAME dict {}
          | NAME ARRAY arrayvals ENDARRAY dict {}
          | NAME OBJREF dict {}
          | NAME DBLLT dict DBLGT dict {}
          | NAME INT dict {}
          |
          ;

arrayvals : OBJREF arrayvals {}
          | INT arrayvals {}
          |
          ;

stream    : STREAM { binaryMode = 1; } binary { binaryMode = 0; } ENDSTREAM 
          |
          ;

binary    : ANYTHING binary { strmcat($$, $2); }
          | { strmcpy($$, ""); }
          ;

xref      : STRING INT INT xrefitems {}
          ;

xrefitems : INT INT STRING xrefitems {}
          |
          ;

trailer   : STRING DBLLT dict DBLGT STRING INT PDFEOF {}
          ;

%%

void pandalex_init(){
  int i;

  // Make sure that the callbacks default to nothing
  for(i = 0; i < gpandalex_callback_max; ++i){
    pandalex_callbacks[i] = NULL;
  }
}

void pandalex_setupcallback(int callback, pandalex_callback_type functoid){
  pandalex_callbacks[callback] = functoid;
}

// Here we call the callbacks. This includes converting to the types that the
// callback function expects.
void pandalex_callback(int event, int type, ...){
  va_list argptr;

  // Start accessing the arguements from the end
  va_start(argptr, type);
  
  // If no event handler is setup, then we ignore the event
  if(pandalex_callbacks[event] != NULL){
    pandalex_callbacks[event] (event, type, argptr);
  }
  
  // Stop with the arguements
  va_end(argptr);
}

int pandalex_parse(){
  // We are not looking into a stream at the moment
  binaryMode = 0;

  yyparse();
}

int yyerror(char *s){
  fprintf(stderr, "Error is %s\n", s);
  exit(42);
}

// Buffer overrun safe strcat
char *strmcat(char *dest, char *append){
  char *new;

  // What length do we need?
  if((new = (char *) realloc(dest, sizeof(char) * 
			     (strlen(dest) + strlen(append) + 2))) == NULL){
    fprintf(stderr, "Could not realloc enough space\n");
    exit(42);
  }

  strcpy(new, dest);
  strcat(new, append);

  return new;
}

// Buffer overrun safe strcpy
char *strmcpy(char *dest, char *data){
  char *new;

  // What length do we need?
  if((new = (char *) malloc(sizeof(char) * (strlen(data) + 1))) == NULL){
    fprintf(stderr, "Could not malloc enough space\n");
    exit(42);
  }

  strcpy(new, data);

  return new;
}




// Some demo code for how to use PandaLex
int main(int argc, char *argv[]){
  pandalex_init();

  // Setup the callbacks
  pandalex_setupcallback(gpandalex_callback_specver, 
			 pandalex_sample_specversion);
  pandalex_setupcallback(gpandalex_callback_entireheader, 
			 pandalex_sample_entireheader);
  pandalex_setupcallback(gpandalex_callback_objstart,
			 pandalex_sample_objstart);

  pandalex_setupcallback(gpandalex_callback_dictitem_string,
			 pandalex_sample_dictitem_string);
  pandalex_setupcallback(gpandalex_callback_dictitem_name,
			 pandalex_sample_dictitem_name);
  pandalex_setupcallback(gpandalex_callback_dictitem_array,
			 pandalex_sample_dictitem_array);
  pandalex_setupcallback(gpandalex_callback_dictitem_object,
			 pandalex_sample_dictitem_object);
  pandalex_setupcallback(gpandalex_callback_dictitem_dict,
			 pandalex_sample_dictitem_dict);
  pandalex_setupcallback(gpandalex_callback_dictitem_int,
			 pandalex_sample_dictitem_int);

  pandalex_parse();

  return 0;
}
