%{
  #include <panda/functions.h>
  #include <panda/constants.h>

  char *strmcpy(char *);
  char *strmcat(char *, char *);

  pdf  *ourdoc = NULL;
  page *ourpage = NULL;
%}

          /* Define the possible yylval values */
%union {
  int      intVal;
  char     *textVal;
  double   fltVal;
}

%token <textVal> IMAGEBOX
%token <textVal> IMAGEBOXROT
%token <textVal> TEXTBOX

%token <textVal> OPENDOC
%token <textVal> NEWPAGE
%token <textVal> CLOSE

%token <textVal> SETFONTMODE
%token <textVal> SETCHARSPACING
%token <textVal> SETWORDSPACING
%token <textVal> SETHORIZSCALING
%token <textVal> SETLEADING
%token <textVal> SETFONT

%token <intVal> INT
%token <fltVal> FLOAT

%token <textVal> QUOTESTR
%token <textVal> ANYTHING

%token <intVal> IMGTYPE

%%

          /*********************************************************
            It would appear that $$ already includes the vale of $1,
	    so we only need to append the value of $2, $3, $4 et al
          *********************************************************/
commands  : OPENDOC QUOTESTR {
                      if(ourdoc != NULL)
			printf("103 You already have an output location\n");
		      else ourdoc = pdfopen($2, "w");
                      } commands
	  | NEWPAGE {
                      ourpage = pdfpage(ourdoc, gPageSizeA4);
                      } commands
	  | CLOSE {
		      if(ourdoc == NULL)
                        printf("104 It's tradition to open a document first\n");
                      else{
			pdfclose(ourdoc);
			exit(0);
		        }
                      } commands
          | IMAGEBOX INT INT INT INT QUOTESTR IMGTYPE { 
                      if(ourdoc == NULL)
                        printf("101 You need to name the output document first\n");
                      else if(ourpage == NULL)
                        printf("102 You need to create a page in the pdf first\n");
                      else
                        imagebox(ourdoc, ourpage, $2, $3, $4, $5, $6, $7);
                      } commands
          | IMAGEBOXROT INT INT INT INT FLOAT QUOTESTR IMGTYPE {
                      if(ourdoc == NULL)
                        printf("105 You need to name the output document first\n");
                      else if(ourpage == NULL)
                        printf("106 You need to create a page in the pdf first\n");
                      else
                        imageboxrot(ourdoc, ourpage, $2, $3, $4, $5, $6, $7, $8);
                      } commands
          | TEXTBOX INT INT INT INT QUOTESTR {
                      if(ourdoc == NULL)
                        printf("107 You need to name the output document first\n");
                      else if(ourpage == NULL)
                        printf("108 You need to create a page in the pdf first\n");
                      else
                        textbox(ourdoc, ourpage, $2, $3, $4, $5, $6);
        	      } commands
          | SETFONTMODE INT {
                      if(ourdoc == NULL)
                        printf("109 You need to name the output document first\n");
                      else if(ourpage == NULL)
                        printf("110 You need to create a page in the pdf first\n");
                      else
                        setfontmode(ourdoc, $2);
                      } commands
          | SETCHARSPACING FLOAT {
                      if(ourdoc == NULL)
                        printf("111 You need to name the output document first\n");
                      else if(ourpage == NULL)
                        printf("112 You need to create a page in the pdf first\n");
                      else
                        setcharacterspacing(ourdoc, $2);
                      } commands
          | SETWORDSPACING FLOAT {
                      if(ourdoc == NULL)
                        printf("113 You need to name the output document first\n");
                      else if(ourpage == NULL)
                        printf("114 You need to create a page in the pdf first\n");
                      else
                        setwordspacing(ourdoc, $2);
                      } commands
          | SETHORIZSCALING FLOAT {
                      if(ourdoc == NULL)
                        printf("115 You need to name the output document first\n");
                      else if(ourpage == NULL)
                        printf("116 You need to create a page in the pdf first\n");
                      else
                        sethorizontalscaling(ourdoc, $2);
                      } commands
          | SETLEADING FLOAT {
                      if(ourdoc == NULL)
                        printf("117 You need to name the output document first\n");
                      else if(ourpage == NULL)
                        printf("118 You need to create a page in the pdf first\n");
                      else
                        setleading(ourdoc, $2);
                      } commands
          | SETFONT QUOTESTR {
                      if(ourdoc == NULL)
                        printf("119 You need to name the output document first\n");
                      else if(ourpage == NULL)
                        printf("120 You need to create a page in the pdf first\n");
                      else
                        setfont(ourdoc, createfont(ourdoc, $2, 1, "MacRomanEncoding"));
                      } commands
		      //	  | error stuff {
		      // yyerror("Bad line");
                      // yyerrok;
		      // }
          |
          ;

stuff     : ANYTHING stuff {}
          |
          ;

%%

int yyerror(char *s){
  printf("200 Unknown command. Try help for more command information...\n");
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

  strcat(new, append);

#if defined DEBUG
  printf("New length of string is %d\n", strlen(new));
#endif

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

// The main routine for the engine
int main(int argc, char *argv[]){
  initpanda();
  yyparse();

  return 0;
}
