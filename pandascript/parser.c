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

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 1 "parser.y"

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


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 19 "parser.y"
typedef union YYSTYPE {
  int      intVal;
  char     *textVal;
  double   fltVal;
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 215 "parser.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 227 "parser.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  150
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   255

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  59
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  53
/* YYNRULES -- Number of rules. */
#define YYNRULES  104
/* YYNRULES -- Number of states. */
#define YYNSTATES  293

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   313

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
{
       0,     0,     3,     6,     9,    12,    15,    18,    21,    24,
      27,    30,    33,    36,    39,    42,    45,    48,    51,    54,
      57,    60,    63,    66,    69,    72,    75,    78,    81,    84,
      87,    90,    93,    96,    99,   102,   105,   108,   111,   114,
     117,   120,   123,   126,   129,   132,   135,   138,   141,   144,
     147,   150,   153,   156,   157,   161,   175,   187,   203,   214,
     218,   222,   230,   236,   242,   244,   250,   252,   254,   256,
     259,   262,   265,   270,   275,   280,   288,   297,   304,   312,
     322,   325,   328,   331,   334,   337,   340,   343,   346,   349,
     352,   355,   358,   361,   364,   366,   369,   372,   375,   378,
     381,   384,   387,   390,   393
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      60,     0,    -1,    61,    60,    -1,    62,    60,    -1,    63,
      60,    -1,    64,    60,    -1,    65,    60,    -1,    66,    60,
      -1,    67,    60,    -1,    68,    60,    -1,    69,    60,    -1,
      70,    60,    -1,    71,    60,    -1,    72,    60,    -1,    73,
      60,    -1,    74,    60,    -1,    75,    60,    -1,    76,    60,
      -1,    77,    60,    -1,    78,    60,    -1,    79,    60,    -1,
      80,    60,    -1,    81,    60,    -1,    82,    60,    -1,    83,
      60,    -1,    84,    60,    -1,    85,    60,    -1,    86,    60,
      -1,    87,    60,    -1,    88,    60,    -1,    89,    60,    -1,
      90,    60,    -1,    91,    60,    -1,    92,    60,    -1,    93,
      60,    -1,    94,    60,    -1,    95,    60,    -1,    96,    60,
      -1,    97,    60,    -1,    98,    60,    -1,    99,    60,    -1,
     100,    60,    -1,   101,    60,    -1,   102,    60,    -1,   103,
      60,    -1,   104,    60,    -1,   105,    60,    -1,   106,    60,
      -1,   107,    60,    -1,   108,    60,    -1,   109,    60,    -1,
     110,    60,    -1,   111,    60,    -1,    -1,     3,    55,    55,
      -1,     4,    58,    55,    55,    58,    58,    58,    58,    54,
      54,    54,    58,    58,    -1,     5,    55,    55,    58,    58,
      58,    58,    54,    54,    54,    58,    -1,     6,    55,    55,
      58,    58,    58,    58,    58,    58,    58,    58,    54,    54,
      54,    58,    -1,     7,    55,    58,    58,    58,    58,    54,
      54,    54,    58,    -1,     8,    58,    58,    -1,     9,    58,
      58,    -1,    10,    58,    58,    58,    58,    58,    58,    -1,
      11,    58,    58,    58,    58,    -1,    12,    58,    58,    58,
      58,    -1,    13,    -1,    14,    58,    58,    58,    58,    -1,
      15,    -1,    16,    -1,    17,    -1,    18,    58,    -1,    19,
      58,    -1,    20,    58,    -1,    21,    58,    58,    58,    -1,
      22,    58,    58,    58,    -1,    23,    58,    58,    58,    -1,
      24,    58,    58,    58,    58,    55,    58,    -1,    25,    58,
      58,    58,    58,    54,    55,    58,    -1,    26,    58,    58,
      58,    58,    55,    -1,    27,    58,    58,    58,    58,    54,
      55,    -1,    28,    58,    58,    58,    58,    54,    58,    58,
      55,    -1,    29,    58,    -1,    30,    54,    -1,    31,    58,
      -1,    32,    55,    -1,    33,    55,    -1,    34,    55,    -1,
      35,    55,    -1,    36,    55,    -1,    37,    58,    -1,    38,
      58,    -1,    39,    54,    -1,    40,    54,    -1,    41,    54,
      -1,    42,    54,    -1,    43,    -1,    44,    55,    -1,    45,
      55,    -1,    46,    58,    -1,    47,    58,    -1,    48,    58,
      -1,    49,    58,    -1,    50,    58,    -1,    51,    58,    -1,
      52,    58,    -1,    53,    58,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    97,   104,   112,   120,   128,   136,
     144,   152,   160,   168,   176,   184,   192,   200,   208,   216,
     224,   232,   240,   248,   256,   264,   272,   280,   288,   296,
     304,   312,   320,   328,   336,   344,   352,   360,   368,   376,
     384,   392,   400,   408,   416,   424,   432,   440,   448,   456,
     464,   472,   480,   488,   496
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "PANDA_OPEN", "PANDA_TEXTANNOTATION", 
  "PANDA_FREETEXTANNOTATION", "PANDA_LINEANNOTATION", 
  "PANDA_INSERTANNOTATION", "PANDA_SETLINESTART", "PANDA_ADDLINESEGMENT", 
  "PANDA_ADDCUBICCURVESEGMENT", "PANDA_ADDQUADRATICCURVESEGMENTONE", 
  "PANDA_ADDQUADRATICCURVESEGMENTTWO", "PANDA_CLOSELINE", 
  "PANDA_RECTANGLE", "PANDA_ENDLINE", "PANDA_STROKELINE", 
  "PANDA_FILLLINE", "PANDA_SETLINEWIDTH", "PANDA_SETLINECAP", 
  "PANDA_SETLINEJOIN", "PANDA_SETLINEDASH", "PANDA_SETFILLCOLOR", 
  "PANDA_SETLINECOLOR", "PANDA_IMAGEBOX", "PANDA_IMAGEBOXROT", 
  "PANDA_TEXTBOX", "PANDA_TEXTBOXROT", "PANDA_TEXTBOXROTALIGN", 
  "PANDA_PAGEDURATION", "PANDA_TRANSDURATION", "PANDA_TRANSSTYLE", 
  "PANDA_SETAUTHOR", "PANDA_SETCREATOR", "PANDA_SETTITLE", 
  "PANDA_SETSUBJECT", "PANDA_SETKEYWORDS", "PANDA_SETFONTSIZE", 
  "PANDA_SETFONTMODE", "PANDA_SETCHARACTERSPACING", 
  "PANDA_SETWORDSPACING", "PANDA_SETHORIZONTALSCALING", 
  "PANDA_SETLEADING", "PANDA_CLOSE", "PANDA_NEWPAGE", "PANDA_NEWTEMPLATE", 
  "PANDA_HIDETOOLBAR", "PANDA_HIDEMENUBAR", "PANDA_HIDEWINDOWUI", 
  "PANDA_FITWINDOW", "PANDA_CENTERWINDOW", "PANDA_NFSPAGEMODE", 
  "PANDA_FULLSCREEN", "PANDA_TEXTDIRECTION", "FLOAT", "QUOTESTR", 
  "ANYTHING", "IMGTYPE", "INTEGER", "$accept", "commands", "panda_open", 
  "panda_textannotation", "panda_freetextannotation", 
  "panda_lineannotation", "panda_insertannotation", "panda_setlinestart", 
  "panda_addlinesegment", "panda_addcubiccurvesegment", 
  "panda_addquadraticcurvesegmentone", 
  "panda_addquadraticcurvesegmenttwo", "panda_closeline", 
  "panda_rectangle", "panda_endline", "panda_strokeline", 
  "panda_fillline", "panda_setlinewidth", "panda_setlinecap", 
  "panda_setlinejoin", "panda_setlinedash", "panda_setfillcolor", 
  "panda_setlinecolor", "panda_imagebox", "panda_imageboxrot", 
  "panda_textbox", "panda_textboxrot", "panda_textboxrotalign", 
  "panda_pageduration", "panda_transduration", "panda_transstyle", 
  "panda_setauthor", "panda_setcreator", "panda_settitle", 
  "panda_setsubject", "panda_setkeywords", "panda_setfontsize", 
  "panda_setfontmode", "panda_setcharacterspacing", 
  "panda_setwordspacing", "panda_sethorizontalscaling", 
  "panda_setleading", "panda_close", "panda_newpage", "panda_newtemplate", 
  "panda_hidetoolbar", "panda_hidemenubar", "panda_hidewindowui", 
  "panda_fitwindow", "panda_centerwindow", "panda_nfspagemode", 
  "panda_fullscreen", "panda_textdirection", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    59,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     0,     3,    13,    11,    15,    10,     3,
       3,     7,     5,     5,     1,     5,     1,     1,     1,     2,
       2,     2,     4,     4,     4,     7,     8,     6,     7,     9,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     1,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
      53,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,    66,    67,    68,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    53,    53,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,     0,     0,
       0,     0,     0,     0,     0,     0,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
       1,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    54,     0,     0,     0,     0,    59,    60,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,    63,    65,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    77,     0,     0,
       0,     0,     0,     0,    61,    75,     0,    78,     0,     0,
       0,     0,     0,    76,     0,     0,     0,     0,     0,    79,
       0,     0,     0,    58,     0,    56,     0,     0,     0,    55,
       0,     0,    57
};

/* YYDEFGOTO[NTERM-NUM]. */
static const yysigned_char yydefgoto[] =
{
      -1,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -4
static const short yypact[] =
{
      -3,    47,    45,    49,    50,    51,    52,    53,    54,    55,
      56,    -4,    57,    -4,    -4,    -4,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    74,    71,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    86,
      87,    -4,    88,    89,    84,    90,    91,    92,    93,    94,
      95,    96,   107,    -3,    -3,    -3,    -3,    -3,    -3,    -3,
      -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,
      -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,
      -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,
      -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,
      -3,    -3,    -3,    -3,   100,   101,   102,   103,   104,   105,
     106,   108,   109,   110,   111,    -4,    -4,    -4,   112,   113,
     114,   115,   116,   117,   118,   119,    -4,    -4,    -4,    -4,
      -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,
      -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,
      -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,
      -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,
      -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,
      -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,
      -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,
      -4,    -4,    -4,   123,   121,   122,   124,    -4,    -4,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,    -4,
      -4,    -4,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,    -4,    -4,    -4,   158,   160,   161,   163,   164,
     157,   162,   165,   167,   166,   168,   170,    -4,   172,   171,
     173,   174,   175,   176,    -4,    -4,   177,    -4,   178,   180,
     183,   181,   184,    -4,   185,   187,   188,   186,   189,    -4,
     191,   190,   192,    -4,   193,    -4,   195,   194,   199,    -4,
     200,   197,    -4
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
      -4,    -2,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,
      -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,
      -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,
      -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,
      -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,
      -4,    -4,    -4
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const unsigned short yytable[] =
{
       1,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   104,   105,   106,   107,   108,   150,     0,     0,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,     0,   136,   137,   134,   135,
     138,   139,   142,   140,   141,     0,     0,     0,   143,   144,
     145,   146,   147,   148,   149,   202,   203,   204,   205,     0,
       0,     0,   206,   207,   208,     0,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,     0,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   260,   257,   258,   259,     0,
     261,   263,     0,   262,   264,   266,   265,   267,   270,   268,
     272,   269,     0,   271,   275,   273,   274,   276,   278,   277,
     279,   280,   281,     0,   282,   284,     0,   283,   285,   288,
     286,   287,   289,   290,   291,   292
};

static const yysigned_char yycheck[] =
{
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,    55,    58,    55,    55,    55,     0,    -1,    -1,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    54,    58,
      55,    55,    55,    55,    55,    -1,    54,    54,    58,    58,
      54,    54,    58,    55,    55,    -1,    -1,    -1,    58,    58,
      58,    58,    58,    58,    58,    55,    55,    55,    55,    -1,
      -1,    -1,    58,    58,    58,    -1,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    55,    58,
      58,    -1,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    55,    54,    58,    55,    54,    54,    -1,
      58,    54,    -1,    58,    58,    55,    58,    55,    54,    58,
      54,    58,    -1,    58,    54,    58,    58,    54,    54,    58,
      55,    54,    54,    -1,    58,    54,    -1,    58,    58,    54,
      58,    58,    58,    54,    54,    58
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    55,    58,    55,    55,    55,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    54,    58,    55,
      55,    55,    55,    55,    58,    58,    54,    54,    54,    54,
      55,    55,    58,    58,    58,    58,    58,    58,    58,    58,
       0,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    55,    55,    55,    55,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    55,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    55,    54,    55,    54,    54,
      58,    58,    58,    54,    58,    58,    55,    55,    58,    58,
      54,    58,    54,    58,    58,    54,    54,    58,    54,    55,
      54,    54,    58,    58,    54,    58,    58,    58,    54,    58,
      54,    54,    58
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylineno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylineno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 54:
#line 98 "parser.y"
    {
	// Class pointer is panda
	m_panda_pdf = panda_open( yyvsp[-1].textVal, yyvsp[0].textVal);
printf("100 panda_open executed\n");
}
    break;

  case 55:
#line 105 "parser.y"
    {
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_textannotation(m_panda_pdf, m_panda_page, yyvsp[-11].intVal, yyvsp[-10].textVal, yyvsp[-9].textVal, yyvsp[-8].intVal, yyvsp[-7].intVal, yyvsp[-6].intVal, yyvsp[-5].intVal, yyvsp[-4].fltVal, yyvsp[-3].fltVal, yyvsp[-2].fltVal, yyvsp[-1].intVal, yyvsp[0].intVal);
printf("100 panda_textannotation executed\n");
}
    break;

  case 56:
#line 113 "parser.y"
    {
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_freetextannotation(m_panda_pdf, m_panda_page, yyvsp[-9].textVal, yyvsp[-8].textVal, yyvsp[-7].intVal, yyvsp[-6].intVal, yyvsp[-5].intVal, yyvsp[-4].intVal, yyvsp[-3].fltVal, yyvsp[-2].fltVal, yyvsp[-1].fltVal, yyvsp[0].intVal);
printf("100 panda_freetextannotation executed\n");
}
    break;

  case 57:
#line 121 "parser.y"
    {
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_lineannotation(m_panda_pdf, m_panda_page, yyvsp[-13].textVal, yyvsp[-12].textVal, yyvsp[-11].intVal, yyvsp[-10].intVal, yyvsp[-9].intVal, yyvsp[-8].intVal, yyvsp[-7].intVal, yyvsp[-6].intVal, yyvsp[-5].intVal, yyvsp[-4].intVal, yyvsp[-3].fltVal, yyvsp[-2].fltVal, yyvsp[-1].fltVal, yyvsp[0].intVal);
printf("100 panda_lineannotation executed\n");
}
    break;

  case 58:
#line 129 "parser.y"
    {
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	m_panda_object = panda_insertannotation(m_panda_pdf, m_panda_page, yyvsp[-8].textVal, yyvsp[-7].intVal, yyvsp[-6].intVal, yyvsp[-5].intVal, yyvsp[-4].intVal, yyvsp[-3].fltVal, yyvsp[-2].fltVal, yyvsp[-1].fltVal, yyvsp[0].intVal);
printf("100 panda_insertannotation executed\n");
}
    break;

  case 59:
#line 137 "parser.y"
    {
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_setlinestart(m_panda_page, yyvsp[-1].intVal, yyvsp[0].intVal);
printf("100 panda_setlinestart executed\n");
}
    break;

  case 60:
#line 145 "parser.y"
    {
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_addlinesegment(m_panda_page, yyvsp[-1].intVal, yyvsp[0].intVal);
printf("100 panda_addlinesegment executed\n");
}
    break;

  case 61:
#line 153 "parser.y"
    {
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_addcubiccurvesegment(m_panda_page, yyvsp[-5].intVal, yyvsp[-4].intVal, yyvsp[-3].intVal, yyvsp[-2].intVal, yyvsp[-1].intVal, yyvsp[0].intVal);
printf("100 panda_addcubiccurvesegment executed\n");
}
    break;

  case 62:
#line 161 "parser.y"
    {
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_addquadraticcurvesegmentone(m_panda_page, yyvsp[-3].intVal, yyvsp[-2].intVal, yyvsp[-1].intVal, yyvsp[0].intVal);
printf("100 panda_addquadraticcurvesegmentone executed\n");
}
    break;

  case 63:
#line 169 "parser.y"
    {
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_addquadraticcurvesegmenttwo(m_panda_page, yyvsp[-3].intVal, yyvsp[-2].intVal, yyvsp[-1].intVal, yyvsp[0].intVal);
printf("100 panda_addquadraticcurvesegmenttwo executed\n");
}
    break;

  case 64:
#line 177 "parser.y"
    {
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_closeline(m_panda_page);
printf("100 panda_closeline executed\n");
}
    break;

  case 65:
#line 185 "parser.y"
    {
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_rectangle(m_panda_page, yyvsp[-3].intVal, yyvsp[-2].intVal, yyvsp[-1].intVal, yyvsp[0].intVal);
printf("100 panda_rectangle executed\n");
}
    break;

  case 66:
#line 193 "parser.y"
    {
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_endline(m_panda_page);
printf("100 panda_endline executed\n");
}
    break;

  case 67:
#line 201 "parser.y"
    {
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_strokeline(m_panda_page);
printf("100 panda_strokeline executed\n");
}
    break;

  case 68:
#line 209 "parser.y"
    {
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_fillline(m_panda_page);
printf("100 panda_fillline executed\n");
}
    break;

  case 69:
#line 217 "parser.y"
    {
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_setlinewidth(m_panda_page, yyvsp[0].intVal);
printf("100 panda_setlinewidth executed\n");
}
    break;

  case 70:
#line 225 "parser.y"
    {
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_setlinecap(m_panda_page, yyvsp[0].intVal);
printf("100 panda_setlinecap executed\n");
}
    break;

  case 71:
#line 233 "parser.y"
    {
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_setlinejoin(m_panda_page, yyvsp[0].intVal);
printf("100 panda_setlinejoin executed\n");
}
    break;

  case 72:
#line 241 "parser.y"
    {
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_setlinedash(m_panda_page, yyvsp[-2].intVal, yyvsp[-1].intVal, yyvsp[0].intVal);
printf("100 panda_setlinedash executed\n");
}
    break;

  case 73:
#line 249 "parser.y"
    {
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_setfillcolor(m_panda_page, yyvsp[-2].intVal, yyvsp[-1].intVal, yyvsp[0].intVal);
printf("100 panda_setfillcolor executed\n");
}
    break;

  case 74:
#line 257 "parser.y"
    {
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_setlinecolor(m_panda_page, yyvsp[-2].intVal, yyvsp[-1].intVal, yyvsp[0].intVal);
printf("100 panda_setlinecolor executed\n");
}
    break;

  case 75:
#line 265 "parser.y"
    {
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_imagebox(m_panda_pdf, m_panda_page, yyvsp[-5].intVal, yyvsp[-4].intVal, yyvsp[-3].intVal, yyvsp[-2].intVal, yyvsp[-1].textVal, yyvsp[0].intVal);
printf("100 panda_imagebox executed\n");
}
    break;

  case 76:
#line 273 "parser.y"
    {
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_imageboxrot(m_panda_pdf, m_panda_page, yyvsp[-6].intVal, yyvsp[-5].intVal, yyvsp[-4].intVal, yyvsp[-3].intVal, yyvsp[-2].fltVal, yyvsp[-1].textVal, yyvsp[0].intVal);
printf("100 panda_imageboxrot executed\n");
}
    break;

  case 77:
#line 281 "parser.y"
    {
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_textbox(m_panda_pdf, m_panda_page, yyvsp[-4].intVal, yyvsp[-3].intVal, yyvsp[-2].intVal, yyvsp[-1].intVal, yyvsp[0].textVal);
printf("100 panda_textbox executed\n");
}
    break;

  case 78:
#line 289 "parser.y"
    {
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_textboxrot(m_panda_pdf, m_panda_page, yyvsp[-5].intVal, yyvsp[-4].intVal, yyvsp[-3].intVal, yyvsp[-2].intVal, yyvsp[-1].fltVal, yyvsp[0].textVal);
printf("100 panda_textboxrot executed\n");
}
    break;

  case 79:
#line 297 "parser.y"
    {
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_textboxrotalign(m_panda_pdf, m_panda_page, yyvsp[-7].intVal, yyvsp[-6].intVal, yyvsp[-5].intVal, yyvsp[-4].intVal, yyvsp[-3].fltVal, yyvsp[-2].intVal, yyvsp[-1].intVal, yyvsp[0].textVal);
printf("100 panda_textboxrotalign executed\n");
}
    break;

  case 80:
#line 305 "parser.y"
    {
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_pageduration(m_panda_pdf, m_panda_page, yyvsp[0].intVal);
printf("100 panda_pageduration executed\n");
}
    break;

  case 81:
#line 313 "parser.y"
    {
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_transduration(m_panda_pdf, m_panda_page, yyvsp[0].fltVal);
printf("100 panda_transduration executed\n");
}
    break;

  case 82:
#line 321 "parser.y"
    {
	// Class pointer is panda_page
	// Parent pointer is panda_pdf
	panda_transstyle(m_panda_pdf, m_panda_page, yyvsp[0].intVal);
printf("100 panda_transstyle executed\n");
}
    break;

  case 83:
#line 329 "parser.y"
    {
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_setauthor(m_panda_pdf, yyvsp[0].textVal);
printf("100 panda_setauthor executed\n");
}
    break;

  case 84:
#line 337 "parser.y"
    {
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_setcreator(m_panda_pdf, yyvsp[0].textVal);
printf("100 panda_setcreator executed\n");
}
    break;

  case 85:
#line 345 "parser.y"
    {
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_settitle(m_panda_pdf, yyvsp[0].textVal);
printf("100 panda_settitle executed\n");
}
    break;

  case 86:
#line 353 "parser.y"
    {
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_setsubject(m_panda_pdf, yyvsp[0].textVal);
printf("100 panda_setsubject executed\n");
}
    break;

  case 87:
#line 361 "parser.y"
    {
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_setkeywords(m_panda_pdf, yyvsp[0].textVal);
printf("100 panda_setkeywords executed\n");
}
    break;

  case 88:
#line 369 "parser.y"
    {
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_setfontsize(m_panda_pdf, yyvsp[0].intVal);
printf("100 panda_setfontsize executed\n");
}
    break;

  case 89:
#line 377 "parser.y"
    {
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_setfontmode(m_panda_pdf, yyvsp[0].intVal);
printf("100 panda_setfontmode executed\n");
}
    break;

  case 90:
#line 385 "parser.y"
    {
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_setcharacterspacing(m_panda_pdf, yyvsp[0].fltVal);
printf("100 panda_setcharacterspacing executed\n");
}
    break;

  case 91:
#line 393 "parser.y"
    {
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_setwordspacing(m_panda_pdf, yyvsp[0].fltVal);
printf("100 panda_setwordspacing executed\n");
}
    break;

  case 92:
#line 401 "parser.y"
    {
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_sethorizontalscaling(m_panda_pdf, yyvsp[0].fltVal);
printf("100 panda_sethorizontalscaling executed\n");
}
    break;

  case 93:
#line 409 "parser.y"
    {
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_setleading(m_panda_pdf, yyvsp[0].fltVal);
printf("100 panda_setleading executed\n");
}
    break;

  case 94:
#line 417 "parser.y"
    {
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_close(m_panda_pdf);
printf("100 panda_close executed\n");
}
    break;

  case 95:
#line 425 "parser.y"
    {
	// Class pointer is panda_pdf
	// Parent pointer is panda
	m_panda_page = panda_newpage(m_panda_pdf, yyvsp[0].textVal);
printf("100 panda_newpage executed\n");
}
    break;

  case 96:
#line 433 "parser.y"
    {
	// Class pointer is panda_pdf
	// Parent pointer is panda
	m_panda_page = panda_newtemplate(m_panda_pdf, yyvsp[0].textVal);
printf("100 panda_newtemplate executed\n");
}
    break;

  case 97:
#line 441 "parser.y"
    {
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_hidetoolbar(m_panda_pdf, yyvsp[0].intVal);
printf("100 panda_hidetoolbar executed\n");
}
    break;

  case 98:
#line 449 "parser.y"
    {
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_hidemenubar(m_panda_pdf, yyvsp[0].intVal);
printf("100 panda_hidemenubar executed\n");
}
    break;

  case 99:
#line 457 "parser.y"
    {
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_hidewindowui(m_panda_pdf, yyvsp[0].intVal);
printf("100 panda_hidewindowui executed\n");
}
    break;

  case 100:
#line 465 "parser.y"
    {
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_fitwindow(m_panda_pdf, yyvsp[0].intVal);
printf("100 panda_fitwindow executed\n");
}
    break;

  case 101:
#line 473 "parser.y"
    {
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_centerwindow(m_panda_pdf, yyvsp[0].intVal);
printf("100 panda_centerwindow executed\n");
}
    break;

  case 102:
#line 481 "parser.y"
    {
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_nfspagemode(m_panda_pdf, yyvsp[0].intVal);
printf("100 panda_nfspagemode executed\n");
}
    break;

  case 103:
#line 489 "parser.y"
    {
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_fullscreen(m_panda_pdf, yyvsp[0].intVal);
printf("100 panda_fullscreen executed\n");
}
    break;

  case 104:
#line 497 "parser.y"
    {
	// Class pointer is panda_pdf
	// Parent pointer is panda
	panda_textdirection(m_panda_pdf, yyvsp[0].intVal);
printf("100 panda_textdirection executed\n");
}
    break;


    }

/* Line 999 of yacc.c.  */
#line 1871 "parser.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("syntax error, unexpected ") + 1;
	  yysize += yystrlen (yytname[yytype]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyss < yyssp)
	    {
	      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
	      yydestruct (yystos[*yyssp], yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
      yydestruct (yytoken, &yylval);
      yychar = YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      yyvsp--;
      yystate = *--yyssp;

      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 508 "parser.y"


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

