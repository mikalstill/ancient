%{
  #include "lexinterface.h"
  #include "samples.h"
  #include "pandalex.h"

  #include <stdarg.h>

  int    binaryMode;

  // The callbacks
  pandalex_callback_type pandalex_callbacks[gpandalex_callback_max];

  // This information is needed for the stream callback
  // streamLength is the length of the stream unless it is stored
  // in a referred object, at which time streamLength = -2, and
  // streamLengthObjRef stores the details of the object reference
  // streamLength = -1 means undefined
  int    streamLength;
  char   *streamLengthObjRef;
  char   *streamFilter;

  // Sample code specific
  extern pandalex_sample_dictint_list *dictint_list;
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
%token <textVal> PDFEOF XREF TRAILER
%token <textVal> ANYTHING

%type <textVal> binary
%type <textVal> header
%type <textVal> objref

%%

          /*********************************************************
            It would appear that $$ already includes the vale of $1,
	    so we only need to append the value of $2, $3, $4 et al
          *********************************************************/
pdf       : header { 
                    pandalex_callback(gpandalex_callback_entireheader, $1); } 
            object linear objects xref trailer
          ;

header    : VERSION { 
	            pandalex_callback(gpandalex_callback_specver, $1); }
            binary { 
                    $$ = $3; }
          ;

linear    : xref trailer { }
          |
          ;

objects   : object objects
          | 
          ;

object    : INT INT OBJ { 
                    pandalex_callback(gpandalex_callback_objstart, $1, $2); 
#if defined DEBUG
  fprintf(stdout, "object %d %d\n", $1, $2);
#endif
                          } 
            dictionary stream ENDOBJ {}
          ;

dictionary: DBLLT dict DBLGT {}
          | INT { pandalex_callback(gpandalex_callback_dictint, $1); }
          | ARRAY dict ENDARRAY {}
          |
          ;

dict      : NAME STRING dict { 
                    pandalex_callback(gpandalex_callback_dictitem_string, $1, $2); }
          | NAME NAME dict { 
                    pandalex_callback(gpandalex_callback_dictitem_name, $1, $2); 

		    // If this is for a stream, then we save the
		    // filter name for convenience reasons
		    if(strcmp($1, "Filter") == 0)
		      streamFilter = strmcpy($2);
                                               }
          | NAME ARRAY arrayvals ENDARRAY dict { 
	    // This one needs work...
                    pandalex_callback(gpandalex_callback_dictitem_array, $1, $2); }
          | NAME objref dict { 
	    // pandalex_callback(gpandalex_callback_dictitem_object, 
	    //		      gpandalex_callback_type_string, 
	    //	      $1, $2); 

                    // If this is for a stream, then we need to save
		    // this info for convenience reasons
	            if(strcmp($1, "Length") == 0){
		      streamLength = -2;
		      streamLengthObjRef = strmcpy($2);
		    }
	                                       }

          | NAME DBLLT dict DBLGT dict { 
                    pandalex_callback(gpandalex_callback_dictitem_dict, $1, $2); }
          | NAME INT dict { 
                    pandalex_callback(gpandalex_callback_dictitem_int, $1, $2); 

                    // If this is for a stream, then we need to save
		    // this info for convenience reasons
	            if(strcmp($1, "Length") == 0){
		      streamLength = $2;
                    }
                                               }
          |
          ;

arrayvals : objref arrayvals {}
          | INT arrayvals {}
          |
          ;

objref    : INT INT OBJREF { if(($$ = (char *) malloc((intlen($1) + intlen($2) + 5) * sizeof(char))) == NULL){
			       fprintf(stderr, "Could not allocate enough space for objref\n");
			       exit(42);
                               }
			     
			     sprintf($$, "%d %d R", $1, $2);
			                       }
          ;

stream    : STREAM { binaryMode = 1; } binary ENDSTREAM { printf("filter = %s, length = %d, lengthObj = %s\n", streamFilter, streamLength, streamLengthObjRef); pandalex_callback(gpandalex_callback_stream, streamFilter, streamLength, streamLengthObjRef, $3); free($3); }
          |
          ;

binary    : ANYTHING binary { $$ = strmcat($1, $2); free($2); }
          | { $$ = strmcpy(""); }
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
  for(i = 0; i < gpandalex_callback_max; ++i){
    pandalex_callbacks[i] = NULL;
  }

  // Initialise our few globals
  streamFilter = NULL;
  streamLengthObjRef = NULL;
  streamLength = -1;
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
  if((new = (char *) malloc(sizeof(char) * 
    (strlen(dest) + strlen(append) + 2))) == NULL){
    fprintf(stderr, "Could not malloc enough space\n");
    exit(42);
  }
 
  sprintf(new, "%s%s", dest, append);
  return new;
}

// Buffer overrun safe strcpy
char *strmcpy(char *data){
  char *new;
  
  // What length do we need?
  if((new = (char *) malloc(sizeof(char) * (strlen(data) + 1))) == NULL){
    fprintf(stderr, "Could not malloc enough space\n");
    exit(42);
  }
  
  strcpy(new, data);
  return new;
}

int intlen(int number){
  int length = 0;

  while(number > 0){
    length ++;
    number /= 10;
  }

  return number;
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

  pandalex_setupcallback(gpandalex_callback_stream,
			 pandalex_sample_stream);
  pandalex_setupcallback(gpandalex_callback_dictint,
			 pandalex_sample_dictint);
  
  // Initialise the dictint_list structure;
  if((dictint_list = (pandalex_sample_dictint_list *)
      malloc(sizeof(pandalex_sample_dictint_list))) == NULL){
    fprintf(stderr, "Could not initialise the dictint list\n");
    exit(42);
  }

  dictint_list->next = NULL;

  // Start parsing
  pandalex_parse();

  return 0;
}
