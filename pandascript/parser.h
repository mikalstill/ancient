/* A Bison parser, made by GNU Bison 1.875a.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     PANDA_OPEN = 258,
     PANDA_TEXTANNOTATION = 259,
     PANDA_FREETEXTANNOTATION = 260,
     PANDA_LINEANNOTATION = 261,
     PANDA_INSERTANNOTATION = 262,
     PANDA_SETLINESTART = 263,
     PANDA_ADDLINESEGMENT = 264,
     PANDA_ADDCUBICCURVESEGMENT = 265,
     PANDA_ADDQUADRATICCURVESEGMENTONE = 266,
     PANDA_ADDQUADRATICCURVESEGMENTTWO = 267,
     PANDA_CLOSELINE = 268,
     PANDA_RECTANGLE = 269,
     PANDA_ENDLINE = 270,
     PANDA_STROKELINE = 271,
     PANDA_FILLLINE = 272,
     PANDA_SETLINEWIDTH = 273,
     PANDA_SETLINECAP = 274,
     PANDA_SETLINEJOIN = 275,
     PANDA_SETLINEDASH = 276,
     PANDA_SETFILLCOLOR = 277,
     PANDA_SETLINECOLOR = 278,
     PANDA_IMAGEBOX = 279,
     PANDA_IMAGEBOXROT = 280,
     PANDA_TEXTBOX = 281,
     PANDA_TEXTBOXROT = 282,
     PANDA_TEXTBOXROTALIGN = 283,
     PANDA_PAGEDURATION = 284,
     PANDA_TRANSDURATION = 285,
     PANDA_TRANSSTYLE = 286,
     PANDA_SETAUTHOR = 287,
     PANDA_SETCREATOR = 288,
     PANDA_SETTITLE = 289,
     PANDA_SETSUBJECT = 290,
     PANDA_SETKEYWORDS = 291,
     PANDA_SETFONTSIZE = 292,
     PANDA_SETFONTMODE = 293,
     PANDA_SETCHARACTERSPACING = 294,
     PANDA_SETWORDSPACING = 295,
     PANDA_SETHORIZONTALSCALING = 296,
     PANDA_SETLEADING = 297,
     PANDA_CLOSE = 298,
     PANDA_NEWPAGE = 299,
     PANDA_NEWTEMPLATE = 300,
     PANDA_HIDETOOLBAR = 301,
     PANDA_HIDEMENUBAR = 302,
     PANDA_HIDEWINDOWUI = 303,
     PANDA_FITWINDOW = 304,
     PANDA_CENTERWINDOW = 305,
     PANDA_NFSPAGEMODE = 306,
     PANDA_FULLSCREEN = 307,
     PANDA_TEXTDIRECTION = 308,
     FLOAT = 309,
     QUOTESTR = 310,
     ANYTHING = 311,
     IMGTYPE = 312,
     INTEGER = 313
   };
#endif
#define PANDA_OPEN 258
#define PANDA_TEXTANNOTATION 259
#define PANDA_FREETEXTANNOTATION 260
#define PANDA_LINEANNOTATION 261
#define PANDA_INSERTANNOTATION 262
#define PANDA_SETLINESTART 263
#define PANDA_ADDLINESEGMENT 264
#define PANDA_ADDCUBICCURVESEGMENT 265
#define PANDA_ADDQUADRATICCURVESEGMENTONE 266
#define PANDA_ADDQUADRATICCURVESEGMENTTWO 267
#define PANDA_CLOSELINE 268
#define PANDA_RECTANGLE 269
#define PANDA_ENDLINE 270
#define PANDA_STROKELINE 271
#define PANDA_FILLLINE 272
#define PANDA_SETLINEWIDTH 273
#define PANDA_SETLINECAP 274
#define PANDA_SETLINEJOIN 275
#define PANDA_SETLINEDASH 276
#define PANDA_SETFILLCOLOR 277
#define PANDA_SETLINECOLOR 278
#define PANDA_IMAGEBOX 279
#define PANDA_IMAGEBOXROT 280
#define PANDA_TEXTBOX 281
#define PANDA_TEXTBOXROT 282
#define PANDA_TEXTBOXROTALIGN 283
#define PANDA_PAGEDURATION 284
#define PANDA_TRANSDURATION 285
#define PANDA_TRANSSTYLE 286
#define PANDA_SETAUTHOR 287
#define PANDA_SETCREATOR 288
#define PANDA_SETTITLE 289
#define PANDA_SETSUBJECT 290
#define PANDA_SETKEYWORDS 291
#define PANDA_SETFONTSIZE 292
#define PANDA_SETFONTMODE 293
#define PANDA_SETCHARACTERSPACING 294
#define PANDA_SETWORDSPACING 295
#define PANDA_SETHORIZONTALSCALING 296
#define PANDA_SETLEADING 297
#define PANDA_CLOSE 298
#define PANDA_NEWPAGE 299
#define PANDA_NEWTEMPLATE 300
#define PANDA_HIDETOOLBAR 301
#define PANDA_HIDEMENUBAR 302
#define PANDA_HIDEWINDOWUI 303
#define PANDA_FITWINDOW 304
#define PANDA_CENTERWINDOW 305
#define PANDA_NFSPAGEMODE 306
#define PANDA_FULLSCREEN 307
#define PANDA_TEXTDIRECTION 308
#define FLOAT 309
#define QUOTESTR 310
#define ANYTHING 311
#define IMGTYPE 312
#define INTEGER 313




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 19 "parser.y"
typedef union YYSTYPE {
  int      intVal;
  char     *textVal;
  double   fltVal;
} YYSTYPE;
/* Line 1240 of yacc.c.  */
#line 159 "y.tab.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



