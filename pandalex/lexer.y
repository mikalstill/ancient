%{
  #include "lexinterface.h"
  #include "samples.h"
  #include "pandalex.h"

  #include <stdarg.h>

  #define YYMAXDEPTH 50000

  int    binaryMode;

  // The callbacks
  pandalex_callback_type pandalex_callbacks[pandalex_event_max];

  // This information is needed for the stream callback
  // streamLength is the length of the stream unless it is stored
  // in a referred object, at which time streamLength = -2, and
  // streamLengthObjRef stores the details of the object reference
  // streamLength = -1 means undefined
  int    streamLength;
  char   *streamLengthObjRef;
  char   *streamFilter;

  // Sample code specific
  extern pdfinfo_dictint_list *dictint_list;
%}

          /* Define the possible yylval values */
%union {
  int        intVal;
  char       *textVal;

  struct streamVal{
    char *data;
    int len;
  } sval;
}

%token <textVal> VERSION
%token <textVal> NAME <textVal> DBLLTNAME <textVal> STRING
%token <textVal> OBJREF <textVal> OBJ <textVal> ENDOBJ <intVal> INT 
%token <textVal> FP <textVal> DBLLT <textVal> DBLGT 
%token <textVal> STREAM <textVal> ENDSTREAM
%token <textVal> ARRAY <textVal> ENDARRAY
%token <textVal> PDFEOF XREF TRAILER
%token <sval> ANYTHING

%type <sval> binary
%type <textVal> header
%type <textVal> objref

%type <intVal> dictionary

%%

          /*********************************************************
            It would appear that $$ already includes the vale of $1,
	    so we only need to append the value of $2, $3, $4 et al
          *********************************************************/
pdf       : { pandalex_callback(pandalex_event_begindocument, ""); } header { 
                    pandalex_callback(pandalex_event_entireheader, $2); } 
            object linear objects xref trailer
          ;

header    : VERSION { 
	            pandalex_callback(pandalex_event_specver, $1); }
            binary { 
                    $$ = $3.data; }
          ;

linear    : xref trailer { }
          |
          ;

objects   : object objects
          | 
          ;

object    : INT INT OBJ { 
                    pandalex_callback(pandalex_event_objstart, $1, $2); 
#if defined DEBUG
  fprintf(stdout, "object %d %d\n", $1, $2);
#endif
                          } 
            dictionary { if($5 != -1) pandalex_callback(pandalex_event_dictint, $1, $2, $5); } stream ENDOBJ {}
          ;

// DBLLTNAME required for dodgy generators -- davince and typereader
// todo_mikal: fully implement
dictionary: DBLLT dict DBLGT { $$ = -1; }
          | DBLLTNAME STRING dict DBLGT { $$ = -1; }
          | DBLLTNAME NAME dict DBLGT { $$ = -1; }
          | DBLLTNAME ARRAY dict DBLGT { $$ = -1; }
          | DBLLTNAME objref dict DBLGT { $$ = -1; }
          | DBLLTNAME DBLLT dict DBLGT dict DBLGT { $$ = -1; }
          | DBLLTNAME INT dict DBLGT { $$ = -1; }
          | DBLLTNAME FP dict DBLGT { $$ = -1; }
          | INT { $$ = $1; }
          | ARRAY arrayvals ENDARRAY { $$ = -1; }
          | objref { $$ = -1; }
          | NAME { $$ = -1; }
          | { $$ = -1; }
          ;

dict      : NAME STRING dict { 
                    pandalex_callback(pandalex_event_dictitem_string, $1, $2); }
          | NAME NAME dict { 
                    pandalex_callback(pandalex_event_dictitem_name, $1, $2); 

		    // If this is for a stream, then we save the
		    // filter name for convenience reasons
		    if(strcmp($1, "Filter") == 0)
		      streamFilter = strmcpy($2, -1);
                                               }
          | NAME ARRAY arrayvals ENDARRAY dict { 
	    // This one needs work...
                    pandalex_callback(pandalex_event_dictitem_array, $1, $2); }
          | NAME objref dict { 
	    // pandalex_callback(pandalex_event_dictitem_object, 
	    //		      pandalex_event_type_string, 
	    //	      $1, $2); 

                    // If this is for a stream, then we need to save
		    // this info for convenience reasons
	            if(strcmp($1, "Length") == 0){
		      streamLength = -2;
		      streamLengthObjRef = strmcpy($2, -1);
		    }
	                                       }

          | NAME DBLLT dict DBLGT dict { 
                    pandalex_callback(pandalex_event_dictitem_dict, $1, $2); }
          | NAME INT dict { 
                    pandalex_callback(pandalex_event_dictitem_int, $1, $2); 

                    // If this is for a stream, then we need to save
		    // this info for convenience reasons
	            if(strcmp($1, "Length") == 0){
		      streamLength = $2;
                    }
                                               }
          | NAME FP dict {
	            // Need to add some stuff here
	                                       }
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

objref    : INT INT OBJREF { if(($$ = (char *) malloc((intlen($1) + intlen($2) + 5) * sizeof(char))) == NULL){
			       fprintf(stderr, "Could not allocate enough space for objref\n");
			       exit(42);
                               }
			     
			     sprintf($$, "%d %d R", $1, $2);
			                       }
          ;

stream    : STREAM { binaryMode = 1; } binary ENDSTREAM { printf("filter = %s, length = %d, lengthObj = %s\n", streamFilter, streamLength, streamLengthObjRef); pandalex_callback(pandalex_event_stream, streamFilter, streamLength, streamLengthObjRef, $3.data, $3.len); free($3); }
          |
          ;

binary    : ANYTHING binary { $$.data = strmcat($1.data, $1.len, $2.data, $2.len); $$.len = $1.len + $2.len; free($2); }
          | STRING binary { $$.data = strmcpy($1, -1); $$.len = strlen($1); }   /* New, may cause problems */
          | { $$.data = strmcpy("", -1); $$.len=0; }
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
char *strmcat(char *dest, int destLen, char *append, int appendLen){
  char *new;
  int count, len;

#if defined DEBUG
  printf("strmcat was passed %d, %d:\n", destLen, appendLen);
  debuglex(dest, destLen, "first arg", 0);
  debuglex(append, appendLen, "second arg", 0);
#endif

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
char *strmcpy(char *data, int len){
  return strmcat(data, len, "", 0);
}

int intlen(int number){
  int length = 0;

  while(number > 0){
    length ++;
    number /= 10;
  }

  return number;
}
