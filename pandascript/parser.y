%{
  #include <panda/functions.h>
  #include <panda/constants.h>

  // Uncomment this line to run dmalloc for memory checking
  // #include <dmalloc.h>

  char *strmcpy(char *);
  char *strmcat(char *, char *);

  panda_fontmetric * m_panda_fontmetric;
  char * m_char;
  panda_page * m_panda_page;
  panda_pdf * m_panda_pdf;
  panda_object * m_panda_object;
%}

          /* Define the possible yylval values */
%union {
  int      intVal;
  char     *textVal;
  double   fltVal;
}


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
%token <textVal> PANDA_CREATEFONT
%token <textVal> PANDA_SETFONT
%token <textVal> PANDA_SETFONTSIZE
%token <textVal> PANDA_SETFONTMODE
%token <textVal> PANDA_SETCHARACTERSPACING
%token <textVal> PANDA_SETWORDSPACING
%token <textVal> PANDA_SETHORIZONTALSCALING
%token <textVal> PANDA_SETLEADING
%token <textVal> PANDA_GETFONTMETRIC
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

commands  : panda_textannotation | panda_freetextannotation | panda_lineannotation | panda_insertannotation | panda_setlinestart | panda_addlinesegment | panda_addcubiccurvesegment | panda_addquadraticcurvesegmentone | panda_addquadraticcurvesegmenttwo | panda_closeline | panda_rectangle | panda_endline | panda_strokeline | panda_fillline | panda_setlinewidth | panda_setlinecap | panda_setlinejoin | panda_setlinedash | panda_setfillcolor | panda_setlinecolor | panda_imagebox | panda_imageboxrot | panda_textbox | panda_textboxrot | panda_textboxrotalign | panda_pageduration | panda_transduration | panda_transstyle | panda_setauthor | panda_setcreator | panda_settitle | panda_setsubject | panda_setkeywords | panda_createfont | panda_setfont | panda_setfontsize | panda_setfontmode | panda_setcharacterspacing | panda_setwordspacing | panda_sethorizontalscaling | panda_setleading | panda_getfontmetric | panda_close | panda_newpage | panda_newtemplate | panda_hidetoolbar | panda_hidemenubar | panda_hidewindowui | panda_fitwindow | panda_centerwindow | panda_nfspagemode | panda_fullscreen | panda_textdirection | 
          ;


panda_textannotation: PANDA_TEXTANNOTATION INTEGER QUOTESTR QUOTESTR INTEGER INTEGER INTEGER INTEGER FLOAT FLOAT FLOAT INTEGER INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_textannotation(m_panda_pdf, m_panda_page, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13);
}

panda_freetextannotation: PANDA_FREETEXTANNOTATION QUOTESTR QUOTESTR INTEGER INTEGER INTEGER INTEGER FLOAT FLOAT FLOAT INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_freetextannotation(m_panda_pdf, m_panda_page, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11);
}

panda_lineannotation: PANDA_LINEANNOTATION QUOTESTR QUOTESTR INTEGER INTEGER INTEGER INTEGER INTEGER INTEGER INTEGER INTEGER FLOAT FLOAT FLOAT INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_lineannotation(m_panda_pdf, m_panda_page, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15);
}

panda_insertannotation: PANDA_INSERTANNOTATION QUOTESTR INTEGER INTEGER INTEGER INTEGER FLOAT FLOAT FLOAT INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	m_panda_object = panda_insertannotation(m_panda_pdf, m_panda_page, $2, $3, $4, $5, $6, $7, $8, $9, $10);
}

panda_setlinestart: PANDA_SETLINESTART INTEGER INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_setlinestart(m_panda_page, $2, $3);
}

panda_addlinesegment: PANDA_ADDLINESEGMENT INTEGER INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_addlinesegment(m_panda_page, $2, $3);
}

panda_addcubiccurvesegment: PANDA_ADDCUBICCURVESEGMENT INTEGER INTEGER INTEGER INTEGER INTEGER INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_addcubiccurvesegment(m_panda_page, $2, $3, $4, $5, $6, $7);
}

panda_addquadraticcurvesegmentone: PANDA_ADDQUADRATICCURVESEGMENTONE INTEGER INTEGER INTEGER INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_addquadraticcurvesegmentone(m_panda_page, $2, $3, $4, $5);
}

panda_addquadraticcurvesegmenttwo: PANDA_ADDQUADRATICCURVESEGMENTTWO INTEGER INTEGER INTEGER INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_addquadraticcurvesegmenttwo(m_panda_page, $2, $3, $4, $5);
}

panda_closeline: PANDA_CLOSELINE 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_closeline(m_panda_page);
}

panda_rectangle: PANDA_RECTANGLE INTEGER INTEGER INTEGER INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_rectangle(m_panda_page, $2, $3, $4, $5);
}

panda_endline: PANDA_ENDLINE 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_endline(m_panda_page);
}

panda_strokeline: PANDA_STROKELINE 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_strokeline(m_panda_page);
}

panda_fillline: PANDA_FILLLINE 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_fillline(m_panda_page);
}

panda_setlinewidth: PANDA_SETLINEWIDTH INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_setlinewidth(m_panda_page, $2);
}

panda_setlinecap: PANDA_SETLINECAP INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_setlinecap(m_panda_page, $2);
}

panda_setlinejoin: PANDA_SETLINEJOIN INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_setlinejoin(m_panda_page, $2);
}

panda_setlinedash: PANDA_SETLINEDASH INTEGER INTEGER INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_setlinedash(m_panda_page, $2, $3, $4);
}

panda_setfillcolor: PANDA_SETFILLCOLOR INTEGER INTEGER INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_setfillcolor(m_panda_page, $2, $3, $4);
}

panda_setlinecolor: PANDA_SETLINECOLOR INTEGER INTEGER INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_setlinecolor(m_panda_page, $2, $3, $4);
}

panda_imagebox: PANDA_IMAGEBOX INTEGER INTEGER INTEGER INTEGER QUOTESTR INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_imagebox(m_panda_pdf, m_panda_page, $2, $3, $4, $5, $6, $7);
}

panda_imageboxrot: PANDA_IMAGEBOXROT INTEGER INTEGER INTEGER INTEGER FLOAT QUOTESTR INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_imageboxrot(m_panda_pdf, m_panda_page, $2, $3, $4, $5, $6, $7, $8);
}

panda_textbox: PANDA_TEXTBOX INTEGER INTEGER INTEGER INTEGER QUOTESTR 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_textbox(m_panda_pdf, m_panda_page, $2, $3, $4, $5, $6);
}

panda_textboxrot: PANDA_TEXTBOXROT INTEGER INTEGER INTEGER INTEGER FLOAT QUOTESTR 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_textboxrot(m_panda_pdf, m_panda_page, $2, $3, $4, $5, $6, $7);
}

panda_textboxrotalign: PANDA_TEXTBOXROTALIGN INTEGER INTEGER INTEGER INTEGER FLOAT INTEGER INTEGER QUOTESTR 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_textboxrotalign(m_panda_pdf, m_panda_page, $2, $3, $4, $5, $6, $7, $8, $9);
}

panda_pageduration: PANDA_PAGEDURATION INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_pageduration(m_panda_pdf, m_panda_page, $2);
}

panda_transduration: PANDA_TRANSDURATION FLOAT 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_transduration(m_panda_pdf, m_panda_page, $2);
}

panda_transstyle: PANDA_TRANSSTYLE INTEGER 
{
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_transstyle(m_panda_pdf, m_panda_page, $2);
}

panda_setauthor: PANDA_SETAUTHOR QUOTESTR 
{
	// Class pointer is panda_pdf
	panda_setauthor(m_panda_pdf, $2);
}

panda_setcreator: PANDA_SETCREATOR QUOTESTR 
{
	// Class pointer is panda_pdf
	panda_setcreator(m_panda_pdf, $2);
}

panda_settitle: PANDA_SETTITLE QUOTESTR 
{
	// Class pointer is panda_pdf
	panda_settitle(m_panda_pdf, $2);
}

panda_setsubject: PANDA_SETSUBJECT QUOTESTR 
{
	// Class pointer is panda_pdf
	panda_setsubject(m_panda_pdf, $2);
}

panda_setkeywords: PANDA_SETKEYWORDS QUOTESTR 
{
	// Class pointer is panda_pdf
	panda_setkeywords(m_panda_pdf, $2);
}

panda_createfont: PANDA_CREATEFONT QUOTESTR INTEGER QUOTESTR 
{
	// Class pointer is panda_pdf
	m_char = panda_createfont(m_panda_pdf, $2, $3, $4);
}

panda_setfont: PANDA_SETFONT QUOTESTR 
{
	// Class pointer is panda_pdf
	panda_setfont(m_panda_pdf, $2);
}

panda_setfontsize: PANDA_SETFONTSIZE INTEGER 
{
	// Class pointer is panda_pdf
	panda_setfontsize(m_panda_pdf, $2);
}

panda_setfontmode: PANDA_SETFONTMODE INTEGER 
{
	// Class pointer is panda_pdf
	panda_setfontmode(m_panda_pdf, $2);
}

panda_setcharacterspacing: PANDA_SETCHARACTERSPACING FLOAT 
{
	// Class pointer is panda_pdf
	panda_setcharacterspacing(m_panda_pdf, $2);
}

panda_setwordspacing: PANDA_SETWORDSPACING FLOAT 
{
	// Class pointer is panda_pdf
	panda_setwordspacing(m_panda_pdf, $2);
}

panda_sethorizontalscaling: PANDA_SETHORIZONTALSCALING FLOAT 
{
	// Class pointer is panda_pdf
	panda_sethorizontalscaling(m_panda_pdf, $2);
}

panda_setleading: PANDA_SETLEADING FLOAT 
{
	// Class pointer is panda_pdf
	panda_setleading(m_panda_pdf, $2);
}

panda_getfontmetric: PANDA_GETFONTMETRIC 
{
	// Class pointer is panda_pdf
	m_panda_fontmetric = panda_getfontmetric(m_panda_pdf);
}

panda_close: PANDA_CLOSE 
{
	// Class pointer is panda_pdf
	panda_close(m_panda_pdf);
}

panda_newpage: PANDA_NEWPAGE QUOTESTR 
{
	// Class pointer is panda_pdf
	m_panda_page = panda_newpage(m_panda_pdf, $2);
}

panda_newtemplate: PANDA_NEWTEMPLATE QUOTESTR 
{
	// Class pointer is panda_pdf
	m_panda_page = panda_newtemplate(m_panda_pdf, $2);
}

panda_hidetoolbar: PANDA_HIDETOOLBAR INTEGER 
{
	// Class pointer is panda_pdf
	panda_hidetoolbar(m_panda_pdf, $2);
}

panda_hidemenubar: PANDA_HIDEMENUBAR INTEGER 
{
	// Class pointer is panda_pdf
	panda_hidemenubar(m_panda_pdf, $2);
}

panda_hidewindowui: PANDA_HIDEWINDOWUI INTEGER 
{
	// Class pointer is panda_pdf
	panda_hidewindowui(m_panda_pdf, $2);
}

panda_fitwindow: PANDA_FITWINDOW INTEGER 
{
	// Class pointer is panda_pdf
	panda_fitwindow(m_panda_pdf, $2);
}

panda_centerwindow: PANDA_CENTERWINDOW INTEGER 
{
	// Class pointer is panda_pdf
	panda_centerwindow(m_panda_pdf, $2);
}

panda_nfspagemode: PANDA_NFSPAGEMODE INTEGER 
{
	// Class pointer is panda_pdf
	panda_nfspagemode(m_panda_pdf, $2);
}

panda_fullscreen: PANDA_FULLSCREEN INTEGER 
{
	// Class pointer is panda_pdf
	panda_fullscreen(m_panda_pdf, $2);
}

panda_textdirection: PANDA_TEXTDIRECTION INTEGER 
{
	// Class pointer is panda_pdf
	panda_textdirection(m_panda_pdf, $2);
}

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
  panda_init();
  yyparse();

  return 0;
}
