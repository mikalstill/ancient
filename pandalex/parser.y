%{
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
  
#include <pandalex.h>

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
  
#define YYMAXDEPTH 50000
#define YYERROR_VERBOSE 1
  
  // The callbacks
  pandalex_callback_type pandalex_callbacks[pandalex_event_max];

  // Globals, these should go away
  int fd;
  char *file;
  struct stat sb;
  unsigned int gInset;

  int pandalex_gettext(char *buffer, int maxlen){
    int size;
    
    // Determine the maximum size to return
    size = pandalex_min(maxlen, sb.st_size - gInset);
    
    if(size > 0){
      memcpy(buffer, file + gInset, size);
      gInset += size;
    }
    
    return size;
  }
  
  int pandalex_min(int a, int b){
    if(a < b)
      return a;
    return b;
  }
%}

/* Define the possible yylval values */
%union {
  int        intVal;

  struct streamVal{
    char *data;
    int len;
  } sval;
}

%token <sval> PDFVER
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
%type <sval> arrayvals

%type <intVal> dictionary
%type <intVal> subdictionary

%%

          /*********************************************************
            It would appear that $$ already includes the value of $1,
	    so we only need to append the value of $2, $3, $4 et al
          *********************************************************/
// completely implemented
pdf       : header { pandalex_callback(pandalex_event_entireheader, $1.data); } 
            object linear endpart endcrap
          ;

// Acrobat Distiller 4.0 for Windows needs this
// completely implemented
endpart   : objects xref trailer endpart
          |
          ;

// completely implemented
header    : PDFVER { pandalex_callback(pandalex_event_specver, $1.data); }
            binary { $$.data = pandalex_strmcat($1.data, $1.len, $3.data, $3.len); $$.len = $1.len + $3.len + 1; }
          ;

linear    : xref trailer { }
          |
          ;

// Clibpdf sometimes puts some binary crap at the end of the file (pointer
// problems?)
// completely implemented
endcrap   : { beginBinary(); } binary { endBinary(); }
          ;

// completely implemented
objects   : object objects
          | 
          ;

// todo_mikal: might need a .data here
object    : INT INT OBJ { pandalex_callback(pandalex_event_objstart, $1, $2); } 
            dictionary { if($5 != -1) pandalex_callback(pandalex_event_dictint, $1, $2, $5); } 
            stream ENDOBJ { pandalex_callback(pandalex_event_objend, $1, $2); }
          ;

// todo_mikal: I don't think these array things work, as they are matched by bracketted string rules
dictionary: DBLLT dict DBLGT { $$ = -1; }
          | INT { $$ = $1; }
          | ARRAY { pandalex_callback(pandalex_event_dictitem_arraystart, "STREAM"); } arrayvals ENDARRAY 
               { pandalex_callback(pandalex_event_dictitem_arrayend, "STREAM"); $$ = -1; }
          | objref { $$ = -1; }
          | NAME { $$ = -1; }
          | STRING { pandalex_callback(pandalex_event_dictitem_string, "STREAM", $1.data); $$ = -1; }
          | { $$ = -1; }
          ;

subdictionary: DBLLT dict DBLGT { $$ = -1; }
          ;

dict      : NAME STRING { pandalex_callback(pandalex_event_dictitem_string, $1.data, $2.data); } dict
          | NAME NAME { pandalex_callback(pandalex_event_dictitem_name, $1.data, $2.data); } dict
          | NAME ARRAY { pandalex_callback(pandalex_event_dictitem_arraystart, $1.data); } 
              arrayvals ENDARRAY { pandalex_callback(pandalex_event_dictitem_arrayend, $1.data); } dict
          | NAME objref { pandalex_callback(pandalex_event_dictitem_object, $1.data, $2.data); } dict
          | NAME { pandalex_callback(pandalex_event_dictitem_dict, $1.data); } 
              subdictionary { pandalex_callback(pandalex_event_dictitem_dictend, $1.data); } dict
          | NAME INT { pandalex_callback(pandalex_event_dictitem_int, $1.data, $2); } dict
          | NAME FP {} dict
          | 
          ;

arrayvals : objref { pandalex_callback(pandalex_event_dictitem_arrayitem, $1.data); } arrayvals 
          | INT { /*todo*/ } arrayvals
          | NAME { pandalex_callback(pandalex_event_dictitem_arrayitem, $1.data); } arrayvals
          | STRING { pandalex_callback(pandalex_event_dictitem_arrayitem, $1.data); } arrayvals
          | ARRAY { pandalex_callback(pandalex_event_dictitem_arrayitem, $1.data); } arrayvals
          | DBLLT { pandalex_callback(pandalex_event_dictitem_dict, "array-dictitem"); } 
              dict DBLGT { pandalex_callback(pandalex_event_dictitem_dictend, "array-dictitem"); } arrayvals
          | {}
          ;

// completely implemented
objref    : INT INT OBJREF { if(($$.data = (char *) malloc((pandalex_intlen($1) + pandalex_intlen($2) + 5) * sizeof(char))) == NULL){
			       fprintf(stderr, "Could not allocate enough space for objref\n");
			       exit(42);
                               }
			     
			     sprintf($$.data, "%d %d R", $1, $2);
			     $$.len = strlen($$.data) + 1;
			                       }
          ;

// completely implemented
stream    : STREAM binary ENDSTREAM {   int inset = 0;
                                        while(($2.data[inset] == '\n') || 
                                              ($2.data[inset] == '\r'))
                                          inset ++;

                                        pandalex_callback(pandalex_event_stream, 
							  $2.data + inset, 
							  $2.len - 1 - inset); 
                                        free($2.data);
                                    }
          |
          ;

// completely implemented: callbacks are handled in the callers to this
binary    : ANYTHING binary { $$.data = pandalex_strmcat($1.data, $1.len, $2.data, $2.len); $$.len = $1.len + $2.len; free($1.data); free($2.data); }
          | STRING binary { $$.data = pandalex_strmcat($1.data, $1.len, $2.data, $2.len); $$.len = $1.len + $2.len; free($1.data); free($2.data); }
          | { $$.data = pandalex_strmcpy("", -1); $$.len = 0; }
          ;

// completely implemented
xref      : XREF INT INT { pandalex_callback(pandalex_event_xrefstart); }
              xrefitems {}
          ;

// Made recursive for the benefit of 000357
// completely implemented
xrefitems : INT INT STRING { pandalex_callback(pandalex_event_xrefitem, $1, $2, $3); }
              xrefitems
	  | INT INT xrefitems {}
          | { pandalex_callback(pandalex_event_xrefend); }
          ;

// completely implemented
trailer   : TRAILER { pandalex_callback(pandalex_event_trailerstart); } 
              DBLLT dict DBLGT STRING INT { pandalex_callback(pandalex_event_trailerend, $6, $7); } 
              PDFEOF { pandalex_callback(pandalex_event_enddocument); }
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
#if defined DEBUG
  printf("Defining a callback\n");
#endif

  pandalex_callbacks[callback] = functoid;
}

// Here we call the callbacks. This includes converting to the types that the
// callback function expects.
void pandalex_callback(int event, ...){
  va_list argptr;

#if defined DEBUG
  printf("Attempting to call callback\n");
#endif

  // Start accessing the arguements from the end
  va_start(argptr, event);
  
  // If no event handler is setup, then we ignore the event
  if(pandalex_callbacks[event] != NULL){
    pandalex_callbacks[event] (event, argptr);
  }
#if defined DEBUG
  else printf("No callback defined\n");
#endif
  
  // Stop with the arguements
  va_end(argptr);
}

int pandalex_parse(char *filename){
  // Map the input file into memory
  if ((fd = open (filename, O_RDONLY)) < 0)
    {
      perror("Could not open file");
      return(-1);
    }

  if(fstat(fd, &sb) < 0){
    perror("Could not stat file");
    return(-1);
  }

  if ((file =
       (char *) mmap (NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0)) == -1)
    {
      perror("Could not mmap file");
      return(-1);
    }

  // We are not looking into a stream at the moment
  pandalex_callback(pandalex_event_begindocument, filename);
  yyparse();
}

int pandalex_endparse(){
  if(munmap(file, sb.st_size) < 0){
    perror("Could not unmap memory");
    return(-1);
  }
  
  close(fd);
}

int yyerror(char *s){
  int i;

  fprintf(stderr, "\n---------------------------------------------------------------\n");
  fprintf(stderr, "PandaLex parser error (%s):\n", s);
  fprintf(stderr, "  Please send this error text, along with a copy of your PDF\n");
  fprintf(stderr, "  document (if possible) to mikal@stillhq.com, so that this can\n");
  fprintf(stderr, "  be fixed for the next release...\n\n");
  fprintf(stderr, "version = %s\n", VERSION);

  // This only works if the matched token was a SVAL, which is all of the except for
  // INT. If the last token was an INT, then a segmentation fault occured before I made
  // this pretty...
  fprintf(stderr, "yychar = %d (INT is %d)\n", yychar, INT);

  fprintf(stderr, "Last token matched was: ");
  if(yychar == INT){
    printf("%d\n", yylval.intVal);
  }
  else if(yychar < 0){
    printf("N/A\n");
  }
  else{
    printf("  \"");
    for(i = 0; i < ((yylval.sval.len == -1) ? strlen(yylval.sval.data) : yylval.sval.len); 
	i++){
      if(yylval.sval.data[i] == '\n') fprintf(stderr, " \\n " );
      else if(yylval.sval.data[i] == '\t') fprintf(stderr, " \\t ");
      else if(yylval.sval.data[i] == '\r') fprintf(stderr, " \\r ");
      else if(yylval.sval.data[i] == ' ') fprintf(stderr, " sp ");
      else if(isprint(yylval.sval.data[i])) fprintf(stderr, "%c", yylval.sval.data[i]);
      else fprintf(stderr, " \\%d ", (unsigned char) yylval.sval.data[i]);
    }
    fprintf(stderr, "\"\n");
  }

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

void pandalex_xfree(void *ptr){
  if(ptr != NULL)
    free(ptr);
}
