#
# Map DocBook tags to gtroff
#
# $Id: xdbto3l.tag,v 1.1.1.1 2002-07-15 21:33:31 root Exp $
#
#
######################################################################
#
# The <SYMBOL> tag is a real hack!
#
PARA                 .P
ABSTRACT             .ABS
APPENDIX             .APX
ACKNO                .ARA
ARTHEADER            .ARH
ARTICLE              .ART
ATTRIBUTION          .ATR
CAUTION              .ATT C
IMPORTANT            .ATT I
NOTE                 .ATT N
TIP                  .ATT T
WARNING              .ATT W
ADDRESS              .AUA
AUTHORBLURB          (NOP)
CORPAUTHOR           .AUC
AUTHORGROUP          .AUG
AUTHORINITIALS       .AUI
AUTHOR               .AUN
BIBLIOGRAPHY         .BIB
BIBLIODIV            .BID
BIBLIOENTRY          .BIE
BOOK                 .BK
BOOKBIBLIO           .BKB
BOOKINFO             .BKI
SUBTITLE             .BKS
HIGHLIGHTS           .CHH
CHAPTER              .CHP
CLASSNAME            .CLA
CLASSGROUP           .CLG
CLASSREF             .CLR
CLASSSYNOPSIS        .CLS
CONSTANT             .CNS
HOLDER               .CPH
CO                   .CO
COPYRIGHT            .CPR
CORPNAME             .CRP
DATE                 .DAT
EDITION              .EDN
EDITOR               .EDR
EPIGRAPH             .EPI
EQUATION             .EQN
ERRORNAME            .ERR
ERRORTYPE            .ERT
EVENTSTRUCTURE       .EVS
EVENTTYPE            .EVT
EXAMPLE              .EXP
EXTENDS              .EXT
FUNCDEF              .FDF
FIGURE               .FIG
FOOTNOTE             .FNT
PARAMDEF             .FPD
FUNCPROTOTYPE        .FPT
GLOSSARY	     .GLS
GLOSSSEE             .GLC
GLOSSDEF             .GLD
GLOSSENTRY           .GLE
GLOSSSEEALSO         .GLK
GLOSSTERM            (NOP)
GLOSSDIV             .GLV
GRAPHIC              .GRA
HONORIFIC            .HON
IMPLEMENTS           .IMP
INFORMALTABLE        .ITB
DOCINFO              .INF
KEYCODE              .KCD
KEYCAP               .BX
OTHERCREDIT          .KUD
LEGALNOTICE          .LGN
LISTITEM             .LI
SUBSTEPS             .LIP
STEP                 .LIS
TERM                 (NOP)
CALLOUT              .LSV
CALLOUTLIST          .CL
VARLISTENTRY         .LIV
PROGRAMLISTING       .LLP
PROGRAMREFERENCE     .PRF
LITERALLAYOUT        .LLT
EXTERNALLINK         .LNX
LINK                 (NOP)
LOTENTRY             .LOE
LOT                  .LOT
SIMPLELIST           .LS
ITEMIZEDLIST         .LSI
ORDEREDLIST          .LSO
PROCEDURE            .LSP
VARIABLELIST         .LSV
SEGLISTITEM          .LXI
SEGMENTEDLIST        .LXL
SEG                  .LXS
SEGTITLE             .LXT
MEMBERGROUP          .MRG
MEMBERS              .MRS
METAINFO             .MET
MODIFIERS            .MOD
MSGENTRY             .MSG
MSGSET               .MSZ
OOPS                 .OOPS
NUTENTRY             .NTE
NUTENTRYBODY         .NEB
NUTLIST              .NTL
NUTSYNOPSIS          .NTS
NUTLISTTITLE         .NTT
FORMALPARA	     .FP
RESOURCE             .PGR
PREFACE              .PRE
PARTINTRO            .PRI
PART                 .PRT
PUBLISHER            .PUB
PUBLISHERNAME        .PUN
PUBDATE              .PUD
BLOCKQUOTE           .QBL
QUOTE                \*[QUO]
COMMENT              .REM
REFSECT1             .RF1
REFSECT2             .RF2
REFSECT3             .RF3
REFSECT4             .RF4
REFNAMEDIV           .RFA
REFERENCE            .RFB
REFCLASS             .RFC
REFDESCRIPTOR        .RFD
REFENTRY             .RFE
REFFILENAME          .RFF
REFMISCINFO          .RFI
REFMETA              .RFM
REFNAME              .RFN
REFPURPOSE           .RFP
REFENTRYTITLE        .RFT
MANVOLNUM            .RFV
REFSYNOPSISDIV       .RFX
GROUP                .RGP
CMDSYNOPSIS          .RSY
RETURNVALUE          .RTV
SCREENINFO           .SCI
SCREEN               .SCR
SCREENSHOT           .SCS
SET                  .SET
SECT1                .SH1
SECT2                .SH2
SECT3                .SH3
SECT4                .SH4
SECT5                .SH5
SIDEBAR              .SID
BRIDGEHEAD           .SIH
KEY                  .SKE
STRUCTURE            .STE
STRUCTNAME           .STN
SYNOPSIS             .SYN
TABLE		     .GTB
THROWS               .THR
TASK                 .TSK
TASKINFO             .TSI
TOC                  .TOC
TOCCHAP			.TC 0
TOCLEVEL1		.TC 1
TOCLEVEL2		.TC 2
TOCLEVEL3		.TC 3
TOCLEVEL4		.TC 4
TOCLEVEL5		.TC 5
TITLEABBREV          .TLA
TITLE                (NOP)
INDEXAS              .XAS
INDEXDIV             .XDV
INDEXTERM            .XET
PRIMARY              .XE1
SECONDARY            .XE2
TERTIARY             .XE3
PRIMARYIE            .XI1
SECONDARYIE          .XI2
TERTIARYIE           .XI3
SEEALSOIE            .XIA
INDEXENTRY           .XIE
SEEIE                .XIS
INDEX                .XLS
XMP                  .XMP
SEEALSO              .XSA
SEE                  .XSE
#
OOPSCHAR             \*[-]
ABBREV               \*[ABV]
ACRONYM              \*[ACR]
JOBTITLE             \*[AJT]
ARTPAGENUMS          \*[ARP]
FIRSTNAME            (NOP)
AFFILIATION          \*[AUF]
CITY                 \*[AUL]
STATE                \*[ACR]
OTHERNAME            \*[AUO]
HONORIFIC            (NOP)
SURNAME              (NOP)
ANCHOR               \*[A]
BIBLIOMISC           \*[BIM]
COMMAND              \*[CMD]
OPTION               \*[CMF]
CHARSET              \*[CRS]
CHARACTER            \*[CRX]
CITEBOOK             \*[CTB]
CITECHAP             \*[CTC]
CITEREFENTRY         \*[CTE]
CITATION             \*[CTN]
CITESECT             \*[CTS]
CITETITLE            \*[CTT]
CURSOR               \*[CUR]
PROMPT               \*[CUE]
FIRSTTERMENTRY       \*[DFE]
EMAIL                \*[EMA]
EMPHASIS             \*[EM]
ENVAR                \*[ENV]
TYPE                 \*[TYP]
SYMBOL		     \*[EM]
INLINEEQUATION       \*[EQI]
ACTION               \*[EVA]
FAX                  \*[FAX]
FOREIGNPHRASE        \*[FA]
FILENAME             \*[FLN]
FONT                 \*[FON]
VOID                 \*[FOO]
FIRSTTERM            \*[FST]
FUNCTION             \*[FUN]
FUNCTIONPARAM        \*[FUP]
VARARGS              \*[FVA]
GLYPH                \*[GLF]
INLINEGRAPHIC        \*[GRI]
ISBN                 \*[ISB]
ISSN                 \*[ISS]
KEYSYM               \*[KSY]
MEMBER               \*[LIM]
LINEANNOTATION       \*[LLA]
USERINPUT            \*[LLI]
WORDASWORD           \*[LLL]
MARKUP               \*[LLM]
COMPUTEROUTPUT       \*[LLO]
PROTOCOLREQUEST      \*[LLR]
LITERAL              \*[LLS]
MSGAUD               .MSA
MSGEXPLAN            .MSE
MSGINFO              .MSI
MSGLEVEL             .MSL
MSGMAIN              .MSM
MSGORIG              \*[MSO]
MSGREL               .MSR
MSGSUB               .MSS
MSGTEXT              .MST
MSG                  .MSX
OPTIONAL             \*[OPT]
OSNAME               \*[OSN]
BEGINPAGE            \*[PAB]
APPLICATION          \*[PGN]
PRODUCTNAME          \*[PN]
PARAMETER            \*[PRM]
# changed to .FDF
# FUNCDEF              \*[RFD]
FUNCSYNOPSISINFO     \*[RSI]
FUNCPARAMS           \*[RFP]
FUNCSYNOPSIS         \*[RFS]
REPLACEABLE          \*[RPL]
SYNOPFRAGMENT        \*[RSF]
SYNOPFRAGMENTREF     \*[RSR]
SGMLTAG              \*[SGM]
STRUCTFIELD          \*[STF]
SUBSCRIPT            \*[SUB]
SUPERSCRIPT          \*[SUP]
SYSTEMITEM	     \*[SIT]
ARG                  \*[SYA]
TRADEMARK            \*[TM]
INTERFACEDEFINITION  \*[UID]
INTERFACE            \*[UI]
VARPARAM             \*[VAR]
VARNAME             \*[VAR]
XREF                 \*[XRF]
#
FOOTNOTEREF          \**
/FOOTNOTEREF         (NOP)
#
# These tags are never output, they are just placeholders so that the
# class routines will get called w/o complaint (every element must have
# a tag rule).
#
COLSPEC		.dummy_COLSPEC
THEAD		.TS H
/THEAD		.TH
TBODY		.TS H
/TBODY		.TE
TFOOT		.dummy_TFOOT
TGROUP		.dummy_TGROUP
ROW		.dummy_ROW
ENTRY		(NOP)
SPANSPEC	.dummy_SPANSPEC
TOCENTRY        .dummy_TOCENTRY
