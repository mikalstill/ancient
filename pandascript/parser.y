%{
  #include <panda/functions.h>
  #include <panda/constants.h>

  #define YYERROR_VERBOSE 1

  // Uncomment this line to run dmalloc for memory checking
  // #include <dmalloc.h>

  char *strmcpy(char *);
  char *strmcat(char *, char *);

  panda_page *m_panda_page;
  panda_pdf *m_panda_pdf;
  panda_object *m_panda_object;
%}

          /* Define the possible yylval values */
%union {
  int      intVal;
  char     *textVal;
  double   fltVal;
}


%token <textVal> PANDA_OPEN
%token <textVal> PANDA_TEXTANNOTATION
%token <textVal> PANDA_FREETEXTANNOTATION
%token <textVal> PANDA_LINEANNOTATION
%token <textVal> PANDA_INSERTANNOTATION
%token <textVal> PANDA_SETLINESTART
%token <textVal> PANDA_ADDLINESEGMENT
%token <textVal> PANDA_ADDCUBICCURVESEGMENT
%token <textVal> PANDA_ADDQUADRATICCURVESEGMENTONE
%token <textVal> PANDA_ADDQUADRATICCURVESEGMENTTWO
%token <textVal> PANDA_CLOSELINE
%token <textVal> PANDA_RECTANGLE
%token <textVal> PANDA_ENDLINE
%token <textVal> PANDA_STROKELINE
%token <textVal> PANDA_FILLLINE
%token <textVal> PANDA_SETLINEWIDTH
%token <textVal> PANDA_SETLINECAP
%token <textVal> PANDA_SETLINEJOIN
%token <textVal> PANDA_SETLINEDASH
%token <textVal> PANDA_SETFILLCOLOR
%token <textVal> PANDA_SETLINECOLOR
%token <textVal> PANDA_IMAGEBOX
%token <textVal> PANDA_IMAGEBOXROT
%token <textVal> PANDA_TEXTBOX
%token <textVal> PANDA_TEXTBOXROT
%token <textVal> PANDA_TEXTBOXROTALIGN
%token <textVal> PANDA_PAGEDURATION
%token <textVal> PANDA_TRANSDURATION
%token <textVal> PANDA_TRANSSTYLE
%token <textVal> PANDA_SETAUTHOR
%token <textVal> PANDA_SETCREATOR
%token <textVal> PANDA_SETTITLE
%token <textVal> PANDA_SETSUBJECT
%token <textVal> PANDA_SETKEYWORDS
%token <textVal> PANDA_SETFONTSIZE
%token <textVal> PANDA_SETFONTMODE
%token <textVal> PANDA_SETCHARACTERSPACING
%token <textVal> PANDA_SETWORDSPACING
%token <textVal> PANDA_SETHORIZONTALSCALING
%token <textVal> PANDA_SETLEADING
%token <textVal> PANDA_CLOSE
%token <textVal> PANDA_NEWPAGE
%token <textVal> PANDA_NEWTEMPLATE
%token <textVal> PANDA_HIDETOOLBAR
%token <textVal> PANDA_HIDEMENUBAR
%token <textVal> PANDA_HIDEWINDOWUI
%token <textVal> PANDA_FITWINDOW
%token <textVal> PANDA_CENTERWINDOW
%token <textVal> PANDA_NFSPAGEMODE
%token <textVal> PANDA_FULLSCREEN
%token <textVal> PANDA_TEXTDIRECTION

%token <fltVal> FLOAT

%token <textVal> QUOTESTR
%token <textVal> ANYTHING

%token <intVal> IMGTYPE
%token <intVal> INTEGER

%%

          /*********************************************************
            It would appear that $$ already includes the vale of $1,
	    so we only need to append the value of $2, $3, $4 et al
          *********************************************************/

commands  : panda_open commands | panda_textannotation commands | panda_freetextannotation commands | panda_lineannotation commands | panda_insertannotation commands | panda_setlinestart commands | panda_addlinesegment commands | panda_addcubiccurvesegment commands | panda_addquadraticcurvesegmentone commands | panda_addquadraticcurvesegmenttwo commands | panda_closeline commands | panda_rectangle commands | panda_endline commands | panda_strokeline commands | panda_fillline commands | panda_setlinewidth commands | panda_setlinecap commands | panda_setlinejoin commands | panda_setlinedash commands | panda_setfillcolor commands | panda_setlinecolor commands | panda_imagebox commands | panda_imageboxrot commands | panda_textbox commands | panda_textboxrot commands | panda_textboxrotalign commands | panda_pageduration commands | panda_transduration commands | panda_transstyle commands | panda_setauthor commands | panda_setcreator commands | panda_settitle commands | panda_setsubject commands | panda_setkeywords commands | panda_setfontsize commands | panda_setfontmode commands | panda_setcharacterspacing commands | panda_setwordspacing commands | panda_sethorizontalscaling commands | panda_setleading commands | panda_close commands | panda_newpage commands | panda_newtemplate commands | panda_hidetoolbar commands | panda_hidemenubar commands | panda_hidewindowui commands | panda_fitwindow commands | panda_centerwindow commands | panda_nfspagemode commands | panda_fullscreen commands | panda_textdirection commands | 
          ;


panda_open: PANDA_OPEN QUOTESTR QUOTESTR 
{
	// Class pointer is panda
	m_panda_pdf = panda_open( $2, $3);
printf("100 panda_open executed\n");
}

panda_textannotation: PANDA_TEXTANNOTATION INTEGER QUOTESTR QUOTESTR INTEGER INTEGER INTEGER INTEGER FLOAT FLOAT FLOAT INTEGER INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_textannotation(m_panda_pdf, m_panda_page, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13);
printf("100 panda_textannotation executed\n");
}

panda_freetextannotation: PANDA_FREETEXTANNOTATION QUOTESTR QUOTESTR INTEGER INTEGER INTEGER INTEGER FLOAT FLOAT FLOAT INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_freetextannotation(m_panda_pdf, m_panda_page, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11);
printf("100 panda_freetextannotation executed\n");
}

panda_lineannotation: PANDA_LINEANNOTATION QUOTESTR QUOTESTR INTEGER INTEGER INTEGER INTEGER INTEGER INTEGER INTEGER INTEGER FLOAT FLOAT FLOAT INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_lineannotation(m_panda_pdf, m_panda_page, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15);
printf("100 panda_lineannotation executed\n");
}

panda_insertannotation: PANDA_INSERTANNOTATION QUOTESTR INTEGER INTEGER INTEGER INTEGER FLOAT FLOAT FLOAT INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	m_panda_object = panda_insertannotation(m_panda_pdf, m_panda_page, $2, $3, $4, $5, $6, $7, $8, $9, $10);
printf("100 panda_insertannotation executed\n");
}

panda_setlinestart: PANDA_SETLINESTART INTEGER INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_setlinestart(m_panda_page, $2, $3);
printf("100 panda_setlinestart executed\n");
}

panda_addlinesegment: PANDA_ADDLINESEGMENT INTEGER INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_addlinesegment(m_panda_page, $2, $3);
printf("100 panda_addlinesegment executed\n");
}

panda_addcubiccurvesegment: PANDA_ADDCUBICCURVESEGMENT INTEGER INTEGER INTEGER INTEGER INTEGER INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_addcubiccurvesegment(m_panda_page, $2, $3, $4, $5, $6, $7);
printf("100 panda_addcubiccurvesegment executed\n");
}

panda_addquadraticcurvesegmentone: PANDA_ADDQUADRATICCURVESEGMENTONE INTEGER INTEGER INTEGER INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_addquadraticcurvesegmentone(m_panda_page, $2, $3, $4, $5);
printf("100 panda_addquadraticcurvesegmentone executed\n");
}

panda_addquadraticcurvesegmenttwo: PANDA_ADDQUADRATICCURVESEGMENTTWO INTEGER INTEGER INTEGER INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_addquadraticcurvesegmenttwo(m_panda_page, $2, $3, $4, $5);
printf("100 panda_addquadraticcurvesegmenttwo executed\n");
}

panda_closeline: PANDA_CLOSELINE 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_closeline(m_panda_page);
printf("100 panda_closeline executed\n");
}

panda_rectangle: PANDA_RECTANGLE INTEGER INTEGER INTEGER INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_rectangle(m_panda_page, $2, $3, $4, $5);
printf("100 panda_rectangle executed\n");
}

panda_endline: PANDA_ENDLINE 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_endline(m_panda_page);
printf("100 panda_endline executed\n");
}

panda_strokeline: PANDA_STROKELINE 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_strokeline(m_panda_page);
printf("100 panda_strokeline executed\n");
}

panda_fillline: PANDA_FILLLINE 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_fillline(m_panda_page);
printf("100 panda_fillline executed\n");
}

panda_setlinewidth: PANDA_SETLINEWIDTH INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_setlinewidth(m_panda_page, $2);
printf("100 panda_setlinewidth executed\n");
}

panda_setlinecap: PANDA_SETLINECAP INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_setlinecap(m_panda_page, $2);
printf("100 panda_setlinecap executed\n");
}

panda_setlinejoin: PANDA_SETLINEJOIN INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_setlinejoin(m_panda_page, $2);
printf("100 panda_setlinejoin executed\n");
}

panda_setlinedash: PANDA_SETLINEDASH INTEGER INTEGER INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_setlinedash(m_panda_page, $2, $3, $4);
printf("100 panda_setlinedash executed\n");
}

panda_setfillcolor: PANDA_SETFILLCOLOR INTEGER INTEGER INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_setfillcolor(m_panda_page, $2, $3, $4);
printf("100 panda_setfillcolor executed\n");
}

panda_setlinecolor: PANDA_SETLINECOLOR INTEGER INTEGER INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_setlinecolor(m_panda_page, $2, $3, $4);
printf("100 panda_setlinecolor executed\n");
}

panda_imagebox: PANDA_IMAGEBOX INTEGER INTEGER INTEGER INTEGER QUOTESTR INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_imagebox(m_panda_pdf, m_panda_page, $2, $3, $4, $5, $6, $7);
printf("100 panda_imagebox executed\n");
}

panda_imageboxrot: PANDA_IMAGEBOXROT INTEGER INTEGER INTEGER INTEGER FLOAT QUOTESTR INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_imageboxrot(m_panda_pdf, m_panda_page, $2, $3, $4, $5, $6, $7, $8);
printf("100 panda_imageboxrot executed\n");
}

panda_textbox: PANDA_TEXTBOX INTEGER INTEGER INTEGER INTEGER QUOTESTR 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_textbox(m_panda_pdf, m_panda_page, $2, $3, $4, $5, $6);
printf("100 panda_textbox executed\n");
}

panda_textboxrot: PANDA_TEXTBOXROT INTEGER INTEGER INTEGER INTEGER FLOAT QUOTESTR 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_textboxrot(m_panda_pdf, m_panda_page, $2, $3, $4, $5, $6, $7);
printf("100 panda_textboxrot executed\n");
}

panda_textboxrotalign: PANDA_TEXTBOXROTALIGN INTEGER INTEGER INTEGER INTEGER FLOAT INTEGER INTEGER QUOTESTR 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_textboxrotalign(m_panda_pdf, m_panda_page, $2, $3, $4, $5, $6, $7, $8, $9);
printf("100 panda_textboxrotalign executed\n");
}

panda_pageduration: PANDA_PAGEDURATION INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_pageduration(m_panda_pdf, m_panda_page, $2);
printf("100 panda_pageduration executed\n");
}

panda_transduration: PANDA_TRANSDURATION FLOAT 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_transduration(m_panda_pdf, m_panda_page, $2);
printf("100 panda_transduration executed\n");
}

panda_transstyle: PANDA_TRANSSTYLE INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_transstyle(m_panda_pdf, m_panda_page, $2);
printf("100 panda_transstyle executed\n");
}

panda_setauthor: PANDA_SETAUTHOR QUOTESTR 
{
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_setauthor(m_panda_pdf, $2);
printf("100 panda_setauthor executed\n");
}

panda_setcreator: PANDA_SETCREATOR QUOTESTR 
{
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_setcreator(m_panda_pdf, $2);
printf("100 panda_setcreator executed\n");
}

panda_settitle: PANDA_SETTITLE QUOTESTR 
{
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_settitle(m_panda_pdf, $2);
printf("100 panda_settitle executed\n");
}

panda_setsubject: PANDA_SETSUBJECT QUOTESTR 
{
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_setsubject(m_panda_pdf, $2);
printf("100 panda_setsubject executed\n");
}

panda_setkeywords: PANDA_SETKEYWORDS QUOTESTR 
{
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_setkeywords(m_panda_pdf, $2);
printf("100 panda_setkeywords executed\n");
}

panda_setfontsize: PANDA_SETFONTSIZE INTEGER 
{
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_setfontsize(m_panda_pdf, $2);
printf("100 panda_setfontsize executed\n");
}

panda_setfontmode: PANDA_SETFONTMODE INTEGER 
{
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_setfontmode(m_panda_pdf, $2);
printf("100 panda_setfontmode executed\n");
}

panda_setcharacterspacing: PANDA_SETCHARACTERSPACING FLOAT 
{
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_setcharacterspacing(m_panda_pdf, $2);
printf("100 panda_setcharacterspacing executed\n");
}

panda_setwordspacing: PANDA_SETWORDSPACING FLOAT 
{
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_setwordspacing(m_panda_pdf, $2);
printf("100 panda_setwordspacing executed\n");
}

panda_sethorizontalscaling: PANDA_SETHORIZONTALSCALING FLOAT 
{
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_sethorizontalscaling(m_panda_pdf, $2);
printf("100 panda_sethorizontalscaling executed\n");
}

panda_setleading: PANDA_SETLEADING FLOAT 
{
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_setleading(m_panda_pdf, $2);
printf("100 panda_setleading executed\n");
}

panda_close: PANDA_CLOSE 
{
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_close(m_panda_pdf);
printf("100 panda_close executed\n");
}

panda_newpage: PANDA_NEWPAGE QUOTESTR 
{
	// Class pointer is panda_pdf
	// Parent pointer is panda
	m_panda_page = panda_newpage(m_panda_pdf, $2);
printf("100 panda_newpage executed\n");
}

panda_newtemplate: PANDA_NEWTEMPLATE QUOTESTR 
{
	// Class pointer is panda_pdf
	// Parent pointer is panda
	m_panda_page = panda_newtemplate(m_panda_pdf, $2);
printf("100 panda_newtemplate executed\n");
}

panda_hidetoolbar: PANDA_HIDETOOLBAR INTEGER 
{
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_hidetoolbar(m_panda_pdf, $2);
printf("100 panda_hidetoolbar executed\n");
}

panda_hidemenubar: PANDA_HIDEMENUBAR INTEGER 
{
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_hidemenubar(m_panda_pdf, $2);
printf("100 panda_hidemenubar executed\n");
}

panda_hidewindowui: PANDA_HIDEWINDOWUI INTEGER 
{
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_hidewindowui(m_panda_pdf, $2);
printf("100 panda_hidewindowui executed\n");
}

panda_fitwindow: PANDA_FITWINDOW INTEGER 
{
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_fitwindow(m_panda_pdf, $2);
printf("100 panda_fitwindow executed\n");
}

panda_centerwindow: PANDA_CENTERWINDOW INTEGER 
{
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_centerwindow(m_panda_pdf, $2);
printf("100 panda_centerwindow executed\n");
}

panda_nfspagemode: PANDA_NFSPAGEMODE INTEGER 
{
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_nfspagemode(m_panda_pdf, $2);
printf("100 panda_nfspagemode executed\n");
}

panda_fullscreen: PANDA_FULLSCREEN INTEGER 
{
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_fullscreen(m_panda_pdf, $2);
printf("100 panda_fullscreen executed\n");
}

panda_textdirection: PANDA_TEXTDIRECTION INTEGER 
{
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_textdirection(m_panda_pdf, $2);
printf("100 panda_textdirection executed\n");
}

stuff     : ANYTHING stuff {}
          |
          ;

%%

int yyerror(char *s){
  printf("200 Unknown command (%s). Try help for more command information...\n", s);
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
  panda_init();
  yyparse();

  return 0;
}
