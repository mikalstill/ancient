#
# Map DocBook tags for Texinfo
#
# $Id: dbtotexi.tag,v 1.1.1.1 2002-07-15 21:33:31 root Exp $
#
#
######################################################################

PARA                 (NOP)
/PARA                (NOP)
ABSTRACT             (NOP)
APPENDIX             @appendix
ACKNO                (NOP)
ARTHEADER            (NOP)
ARTICLE              (NOP)
CAUTION              @quotation\n@strong{CAUTION:} @emph{
/CAUTION             }\n@end quotation
IMPORTANT            @quotation\n@strong{IMPORTANT:} @emph{
/IMPORTANT           }\n@end quotation
NOTE                 @quotation\n@strong{NOTE:} @emph{
/NOTE		     }\n@end quotation
TIP                  @quotation\n@strong{TIP:} @emph{
/TIP		     }\n@end quotation
WARNING              @quotation\n@strong{WARNING:} @emph{
/WARNING             }\n@end quotation
ADDRESS              (NOP)
AUTHORBLURB          (NOP)
CORPAUTHOR           (NOP)
AUTHORGROUP          (NOP)
AUTHORINITIALS       (NOP)
AUTHOR               (NOP)
BIBLIOGRAPHY         (NOP)
BIBLIODIV            (NOP)
BIBLIOENTRY          (NOP)
BOOK                 (NOP)
/BOOK                @bye
BOOKBIBLIO           (NOP)
BOOKINFO             (NOP)
SUBTITLE             (NOP)
HIGHLIGHTS           (NOP)
CHAPTER              @chapter
CLASSNAME            (NOP)
CONSTANT             (NOP)
HOLDER               (NOP)
COPYRIGHT            (NOP)
CORPNAME             (NOP)
DATE                 (NOP)
EDITION              (NOP)
EDITOR               (NOP)
EPIGRAPH             (NOP)
EQUATION             (NOP)
ERRORNAME            (NOP)
ERRORTYPE            (NOP)
EVENTSTRUCTURE       (NOP)
EVENTTYPE            (NOP)
EXAMPLE              (NOP)
FIGURE               (NOP)
FOOTNOTE             (NOP)
GLOSSARY	     (NOP)
GLOSSSEE             (NOP)
GLOSSDEF             (NOP)
GLOSSENTRY           (NOP)
GLOSSSEEALSO         (NOP)
GLOSSTERM            (NOP)
GLOSSDIV             (NOP)
GRAPHIC              (NOP)
HONORIFIC            (NOP)
INFORMALTABLE        (NOP)
DOCINFO              (NOP)
KEYCODE              (NOP)
KEYCAP               (NOP)
OTHERCREDIT          (NOP)
LEGALNOTICE          (NOP)
LISTITEM             @item
/LISTITEM            (NOP)
SUBSTEPS             (NOP)
STEP                 (NOP)
TERM                 (NOP)
VARLISTENTRY         @item
PROGRAMLISTING       @example
/PROGRAMLISTING      @end example
LITERALLAYOUT        @example
/LITERALLAYOUT       @end example
EXTERNALLINK         (NOP)
LOTENTRY             (NOP)
LOT                  (NOP)
SIMPLELIST           (NOP)
ITEMIZEDLIST         @itemize @bullet
/ITEMIZEDLIST        @end itemize
ORDEREDLIST          @enumerate
/ORDEREDLIST         @end enumerate
PROCEDURE            (NOP)
VARIABLELIST         @table @asis
/VARIABLELIST        @end table
SEGLISTITEM          (NOP)
SEGMENTEDLIST        (NOP)
SEG                  (NOP)
SEGTITLE             (NOP)
METAINFO             (NOP)
MSGENTRY             (NOP)
MSGSET               (NOP)
OOPS                 (NOP)
FORMALPARA	     (NOP)
RESOURCE             (NOP)
PREFACE              (NOP)
PARTINTRO            (NOP)
PART                 (NOP)
PUBLISHER            (NOP)
BLOCKQUOTE           @quotation
/BLOCKQUOTE          @end quotation
QUOTE                ``
/QUOTE               ''
COMMENT              (NOP)
REFSECT1             @section
REFSECT2             @subsection
REFSECT3             @subsubsection
REFNAMEDIV           (NOP)
REFERENCE            (NOP)
REFCLASS             (NOP)
REFDESCRIPTOR        (NOP)
REFENTRY             (NOP)
REFFILENAME          (NOP)
REFMISCINFO          (NOP)
REFMETA              (NOP)
REFNAME              (NOP)
REFPURPOSE           (NOP)
REFENTRYTITLE        (NOP)
MANVOLNUM            (NOP)
REFSYNOPSISDIV       (NOP)
GROUP                (NOP)
CMDSYNOPSIS          (NOP)
RETURNVALUE          (NOP)
SCREENINFO           (NOP)
SCREEN               @example
/SCREEN              @end example
SCREENSHOT           (NOP)
SET                  (NOP)
SECT1                @section
SECT2                @subsection
SECT3                @subsubsection
SECT4                @subsubsection
SECT5                @subsubsection
SIDEBAR              @cartouche
BRIDGEHEAD           (NOP)
KEY                  (NOP)
STRUCTURE            (NOP)
STRUCTNAME           (NOP)
SYNOPSIS             (NOP)
TABLE		     (NOP)
TOC                  (NOP)
TOCCHAP		     (NOP)
TOCLEVEL1	     (NOP)
TOCLEVEL2	     (NOP)
TOCLEVEL3	     (NOP)
TOCLEVEL4	     (NOP)
TOCLEVEL5	     (NOP)
TITLEABBREV          (NOP)
TITLE                (NOP)
INDEXAS              (NOP)
INDEXDIV             (NOP)
INDEXTERM            (NOP)
PRIMARY              (NOP)
SECONDARY            (NOP)
TERTIARY             (NOP)
PRIMARYIE            (NOP)
SECONDARYIE          (NOP)
TERTIARYIE           (NOP)
SEEALSOIE            (NOP)
INDEXENTRY           (NOP)
SEEIE                (NOP)
INDEX                (NOP)
XMP                  (NOP)
SEEALSO              (NOP)
SEE                  (NOP)
#
OOPSCHAR             (NOP)
ABBREV               (NOP)
ACRONYM              (NOP)
JOBTITLE             (NOP)
ARTPAGENUMS          (NOP)
FIRSTNAME            (NOP)
AFFILIATION          (NOP)
CITY                 (NOP)
OTHERNAME            (NOP)
SURNAME              (NOP)
ANCHOR               (NOP)
BIBLIOMISC           (NOP)
COMMAND              @emph{
OPTION               (NOP)
CHARSET              (NOP)
CHARACTER            (NOP)
CITATION             @emph{
CITETITLE            @cite{
PROMPT               (NOP)
FIRSTTERMENTRY       (NOP)
EMAIL                @t{
EMPHASIS             @emph{
SYMBOL		     (NOP)
INLINEEQUATION       (NOP)
ACTION               (NOP)
FAX                  (NOP)
FOREIGNPHRASE        @emph{
FILENAME             @file{
FONT                 (NOP)
VOID                 (NOP)
PARAMDEF             (NOP)
FIRSTTERM            (NOP)
FUNCTION             (NOP)
FUNCTIONPARAM        (NOP)
VARARGS              (NOP)
GLYPH                (NOP)
INLINEGRAPHIC        (NOP)
ISBN                 (NOP)
ISSN                 (NOP)
KEYSYM               (NOP)
MEMBER               (NOP)
LINEANNOTATION       (NOP)
USERINPUT            (NOP)
WORDASWORD           (NOP)
MARKUP               (NOP)
COMPUTEROUTPUT       (NOP)
PROTOCOLREQUEST      (NOP)
LITERAL              @samp{
MSGAUD               (NOP)
MSGEXPLAN            (NOP)
MSGINFO              (NOP)
MSGLEVEL             (NOP)
MSGMAIN              (NOP)
MSGORIG              (NOP)
MSGREL               (NOP)
MSGSUB               (NOP)
MSGTEXT              (NOP)
MSG                  (NOP)
OPTIONAL             (NOP)
OSNAME               (NOP)
BEGINPAGE            (NOP)
APPLICATION          @emph{
PRODUCTNAME          (NOP)
PARAMETER            (NOP)
FUNCDEF              (NOP)
FUNCSYNOPSISINFO     (NOP)
FUNCPARAMS           (NOP)
FUNCSYNOPSIS         (NOP)
REPLACEABLE          @var{
SYNOPFRAGMENT        (NOP)
SYNOPFRAGMENTREF     (NOP)
SGMLTAG              (NOP)
STRUCTFIELD          (NOP)
SUBSCRIPT            (NOP)
SUPERSCRIPT          (NOP)
SYSTEMITEM	     (NOP)
ARG                  (NOP)
TRADEMARK            (NOP)
INTERFACEDEFINITION  (NOP)
INTERFACE            (NOP)
VARPARAM             (NOP)
XREF                 (NOP)
FOOTNOTEREF          (NOP)
#
# These tags are never output, they are just placeholders so that the
# class routines will get called w/o complaint (every element must have
# a tag rule).
#
COLSPEC		(NOP)
THEAD		(NOP)
/THEAD		(NOP)
TBODY		(NOP)
/TBODY		(NOP)
TFOOT		(NOP)
TGROUP		(NOP)
ROW		(NOP)
ENTRY		(NOP)
SPANSPEC	(NOP)
TOCENTRY        (NOP)
