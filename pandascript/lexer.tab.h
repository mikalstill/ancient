typedef union {
  int      intVal;
  char     *textVal;
  double   fltVal;
} YYSTYPE;
#define	IMAGEBOX	257
#define	IMAGEBOXROT	258
#define	TEXTBOX	259
#define	OPENDOC	260
#define	NEWPAGE	261
#define	CLOSE	262
#define	SETFONTMODE	263
#define	SETCHARSPACING	264
#define	SETWORDSPACING	265
#define	SETHORIZSCALING	266
#define	SETLEADING	267
#define	SETFONT	268
#define	INT	269
#define	FLOAT	270
#define	QUOTESTR	271
#define	ANYTHING	272
#define	IMGTYPE	273


extern YYSTYPE yylval;
