.\"
.\" ############ NOTES
.\"		Add registers/strings for QP/QS/QE
.\"		"XX" at start of comment says: don't insert space ahead of comment
.\"		Print example continuation line from pg@top (ET diversion)
.\"		Implement levels in save_nr
.\"
.\" ############ BOOK-SPECIFIC components of this macro package:
.\"		macros: book_setup_*, PT, BT, make_chap_toc
.\"		also: the macro invocation, ".book_setup_*"
.\"
.\" ############ ENVIRONMENTS:
.\"		0	normal text
.\"		pt	page top and bottom
.\"		cm	cutmarks
.\"		bt	bleeding tabs
.\"       tc	chapter tocs
.\"		cw	examples, screen displays
.\"		??	footnotes (check tmac.gs, which uses fn environment)
.\"
.\" ############ DEFAULTS AND MISC VARIABLES
.\" The following strings are set in accord with the Makefile
.ds TMPDIR /var/tmp
.ds BINDIR /usr/local/bin
.ds gmat_gmacroff /usr/local/bin/gmacroff
.\" DEFINE THE FOLLOWING paths for your local environment!
.ds gmat_echo /bin/echo
.ds gmat_sed /usr/bin/sed
.ds gmat_rm /bin/rm
.ds gmat_mv /bin/mv
.\" The following strings set variables to subdirectories of the book
.\" directories; you can leave them as is; they're for generating index
.\" and tocs for individual books 
.ds INDEXDIR ./fmt/index
.ds TOCDIR ./fmt/toc
.ds FIGDIR ./fmt/figlist
.ds TABDIR ./fmt/tablist
.ds EXDIR ./fmt/exlist
.\" END OF SYSTEM STUFF; variables for languages follow;English by def.
.ds Table Table
.ds Figure Figure
.ds Chapter Chapter
.ds CHAPTER CHAPTER
.ds Appendix Appendix
.ds APPENDIX APPENDIX
.ds Example Example
.ds toc Table of Contents
.ds lof List of Figures 
.ds lot List of Tables
.ds loe List of Examples
.ds Foreword Foreword
.ds Preface Preface
.ds continued continued
.ds cont'd cont'd
.ds CAUTION CAUTION
.ds EXAMPLE EXAMPLE
.ds WARNING WARNING
.ds NOTE NOTE
.de set_name_strings
.\" for future work
.if "\\*[lang]"D" \{\
.ds Table Tabelle
.ds Figure Abbildung
.ds Chapter Kapitel
.ds CHAPTER KAPITEL
.ds Example Beispiel
.ds continued Fortsetzung
.ds cont'd Forts.
.ds toc Inhalt
.ds lof Verzeichnis der Abbildungen 
.ds lot Verzeichnis der Tabellen 
.ds loe Verzeichnis der Beispiele
.ds Foreword Vorwort 
.ds Preface Vorwort 
.ds Appendix Anhang
.ds APPENDIX ANHANG
.\".ds Preface Einf\(:uhrung 
.ds CAUTION VORSICHT
.ds EXAMPLE BEISPEIL
.ds WARNING WARNUNG
.ds NOTE HINWEIS
.NU\}
.if "\\*[lang]"F" \{\
.ds Figure Figure
.ds Chapter Chapitre
.ds Example Exemple
.ds toc Table des Mati\(e`res
.ds lof Liste d'illustrations
.ds lot Liste de tables 
.ds loe Liste d'exemples
.ds Foreword Avant-Propos 
.ds Preface Pr\(e'face
.ds continued Suite
.ds CAUTION ATTENTION!
.ds EXAMPLE EXEMPLE
.ds WARNING AVERTISSEMENT
.ds NOTE NOTE
.NU\}
..
.nr writefile_open 0	\" we haven't opened an index file yet!
.nr newchapflag 0		\" flag: we're on first page of a new chapter (for PT)
.nr bT 0				\" no bleeding tabs (until Pt macro)
.nr cM 1				\" print cutmarks
.nr fig_counter 0		\" initialize figure counter
.nr table_counter 0		\" initialize table counter
.ds lastelem 0
.nr counting_space 0	\" used in space-control mechanism
.nr accum_space 0		\" ditto
.nr keepingfig 0		\" we're not keeping a fig for next page, to start with
.nr doingkeptfig 0		\" are we printing a kept figure now?
.if !r hD .nr hD 2		\" level of head we're recognizing for toc
.if !r B .nr B 0		\" book type; 0=generic; 1=Nutshell; 2=X Windows...
.ie r N .nr hD_num \nN	\" level of head for which we use para nums (if nH=0)
.el .nr hD_num 2		\"		2 = Bheads & higher
.\"Next 2 lines lower footnote symbols/superscripts from default
.ds par@sup-start \v'-.7m\s'\En[.s]*7u/10u'+.7m'
.ds par@sup-end \v'-.7m\s0+.7m'
.ds f1 \f(TR*\fP\"			XX first footnote on page is asterisk
.ds f2 \(dg\"			XX second footnote on page is dagger
.ds f3 \(dd\"			XX third footnote on page is double dagger
.ds * \\*[par@sup-start]\E*(f\\n+[fn*text-num]\\*[par@sup-end]\"XX pick one of the footnote symbols
.\".ds * \\*[par@sup-start]\\\\*(f\\n+[fn*text-num]\\*[par@sup-end]\"XX pick one of the footnote symbols
.if !r F .nr F 0		\" print PostScript figures
.\".if !r # .nr # 9999		\" id for this format job--normally provided by fmt
.\" the value of # is now set in gmat to the pid
.if !r d .nr d 0		\" d != 0 means we're in debug mode
.nr gtroff_res 1i
.nr red 0				\"this and next for two-color book
.nr black 0
.\"
.\" ############ EMPTY MACRO DEFINITIONS (these defs prevent gtroff warnings)
.if !"\\nQ"55" .warn 260351 
.de NU		\" null macro
..
.de yy		\" end of .ig block
..
.de yy_Fs
..
.de yy_Fe
..
.de yy_make_chap_toc
..
.de keptfig	\" we append to this macro for "keeping" figs across pages
..
.\"
.\" ############ DEBUGGING
.de SFILE
.if !"\\*[source_filename]"" .tm File \\*[source_filename], pages \\*[starting_page] to \\*[NN]
.ds source_filename \\$*
.nr seen_an_sfile 1
..
.de FILE	\" get filename and restart line counter
.ie !\\n[seen_an_sfile] \{\
.if !"\\*[source_filename]"" .tm File \\*[source_filename], pages \\*[starting_page] to \\*[NN]
.ds source_filename \\$*
.NU\}
.el .nr seen_an_sfile 0
.if \\n[writefile_open] \{\
.	close index
.	close toc
.	close extoc
.	close figtoc
.	close tabtoc
.	if !\\n[pagerange] \{\
.		sy mv \\*[INDEXDIR]/\\*[filename].\\n# \\*[INDEXDIR]/\\*[filename]
.		sy mv \\*[TOCDIR]/\\*[filename].\\n# \\*[TOCDIR]/\\*[filename]
.		sy mv \\*[FIGDIR]/\\*[filename].\\n# \\*[FIGDIR]/\\*[filename]
.		sy mv \\*[TABDIR]/\\*[filename].\\n# \\*[TABDIR]/\\*[filename]
.		sy mv \\*[EXDIR]/\\*[filename].\\n# \\*[EXDIR]/\\*[filename]
.	.NU\}
.NU\}
.ds filename \\$1
.nr c. 1
.open toc \\*[TOCDIR]/\\*[filename].\\n#
.open figtoc \\*[FIGDIR]/\\*[filename].\\n#
.open tabtoc \\*[TABDIR]/\\*[filename].\\n#
.open extoc \\*[EXDIR]/\\*[filename].\\n#
.open index \\*[INDEXDIR]/\\*[filename].\\n#
.sy \\*[gmat_rm] -f \\*[TMPDIR]/date.*
.sy \\*[gmat_echo] ".ds tD `date +%H:%M`" > \\*[TMPDIR]/date.\n#
.so \\*[TMPDIR]/date.\n#
.nr writefile_open 1
..
.de aB
.ab file \\*[filename]; line \\n[c.]; \\$*
..
.\"
.\" ############ GENERIC FORMAT measuring b/b
.if \nB>0 .ig yy
.de book_setup_0
.nr no_cutmarks 0  \"print cutmarks; if 1, don't print them 
.nr nH 0	\" no numbered paragraphs
.hy 14	\" don't hyphenate last line of pg; don't split last 2 chars of a word
.nr HY 14 \" need to reset this reg if you want to change hyphenation rules
.hlm 2	\" don't hyphenate more than two lines in a row
.nr FF 4	\" use symbols (asterisk, dagger, etc) for footnotes
.		\" following lines establish footnote symbols
.ad b
.nr AD \\n[.j]
.\"
.\" ############ DEFINITIONS FOR FOLLOWING
.\"		"ext" measures are from edge of physical sheet on which troff prints
.\"		"int" measures are from cutmarks
.\"		"body" refers to main text body rectangle, exclusive of header/footer
.\"
.\" ############ PRIMARY PAGE PARAMETERS
.nr pg_length_ext 11i	\" length of troff page, top to bottom
.nr pg_width_ext 8.5i	\" width of troff page, left to right
.nr top_off_ext 1.0i	\" from top edge of troff page to top cutmark
.nr bot_off_ext 1.0i	\" from bottom edge of troff page to bottom cutmark
.nr left_off_ext 1.25i	\" from left edge of troff page to left cutmark
.nr right_off_ext 1.25i	\" from right edge of troff page to right cutmark
.nr top_off_int 1.1i	\" from top cutmark to baseline of first body text
.nr bot_off_int 0.9i	\" from bottom cutmark to baseline of lowest body text
.nr left_off_int 0.75i	\" from left cutmark to left edge of body text
.nr body_width 4.6i		\" width of body text
.nr hdr_vertoff_int 0.5i	\" from top cutmark to baseline of main header text
.nr ftr_vertoff_int 0.3i	\" from bottom cutmark to baseline of main footer text
.nr hdr_leftoff 0	\" offset relative to left body margin (positive to right)
.nr hdr_rightoff 0	\" offset relative to right body margin (positive to right)
.nr ftr_leftoff 0			\" ditto
.nr ftr_rightoff 0			\" ditto
.nr chap_num_off 1.18i		\" from header text to chapter number (baselines)
.nr chap_hdr_off 0.41i		\" from chapter number to chapter head (baselines)
.nr chap_hdr_vs 26.5p		\" leading for multi-line chapter headings
.nr chap_toc_vert_off 0.3i	\" from baseline of header text to top of toc box
.nr chap_text_prespace 0.6i	\" from chap title to 1st text
.\"
.\" ############ MISCELLANEOUS SPACING PARAMETERS
.\" unless otherwise indicated, spaces are baseline-to-baseline
.nr PD 1.5v				\" space between paragraphs
.nr VS 13p				\" text spacing, baseline to baseline
.nr list_prespace 0	  		\" additional space before first item of list
.nr list_itemspace 1.5v		\" space preceding list items
.nr list_postspace 1.5v		\" space following last list item
.nr list_paraspace 4p  \"space between paras within a list item (old version)
.nr varlist_PD 1.25v		\"space between paras within a list
.nr varlist_indent .5i		\" indent for hanging label list
.nr varlist_indent_outer .25i\"indent from current left margin to varlist item 
.nr orlist_indent_outer .25i\"indent from current left margin to varlist item 
.nr orlist_indent_inner 5n\"indent from outer indent to text after # 
.nr itemlist_indent_inner 3n\"indent from outer indent to text after # 
.nr Ah_need 6				\" for .ne cmd preceding Ahead
.nr Bh_need 5				\" for .ne cmd preceding Bhead
.nr Ch_need 5				\" for .ne cmd preceding Chead
.nr Ah_prespace 0.45i		\" space preceding Ahead
.nr Bh_prespace 0.4i		\" space preceding Bhead
.nr Ch_prespace 0.35i		\" space preceding Chead
.nr Ah_linelength \\n[body_width]-.5i \"default line length of Ahead 
.nr Bh_linelength \\n[body_width]-.5i \"default line length of Bhead
.nr Ch_linelength \\n[body_width]-.5i \"default line length of Chead
.nr Ah_postspace 0.25i		\" space following Ahead
.nr Bh_postspace 0.25i		\" space following Bhead
.nr Ch_postspace 0.25i		\" space following Chead
.nr Fs_prespace 2v			\" space preceding a figure
.nr Fc_prespace 2v			\" space preceding a figure caption
.nr Fe_postspace 2v			\" space following a figure
.nr Ns_prespace 2v			\" space preceding a note
.nr Nc_postspace 1.5v		\" space following note heading
.nr Ne_postspace 2v			\" space following a note
.nr Ns_need 5				\" for .ne cmd preceding note
.nr Ps_prespace 1.5v		\" space preceding a screen display
.nr Pe_postspace 1.5v		\" space following a screen display
.nr DDS_prespace 1.5v		\" space preceding a non-screen display
.nr DDE_postspace 1.5v		\" space following a non-screen display
.nr QS_prespace .25i		\" space preceding a blockquote 
.nr QE_postspace .223i		\" space following a blockquote 
.nr Ts_prespace 1.5v		\" space preceding a table
.nr Tc_postspace 1.1v		\" space following a table caption
.nr Te_postspace 2.0v		\" space following a table
.nr Xs_prespace 1.5v		\" space preceding an example
.nr Xc_postspace 1.2v		\" space following an example caption
.nr Xe_postspace 1.5v		\" space following an example
.nr Bx_prespace 1.5v		\"space before boxed text
.nr Bx_postspace 1.5v		\"space after boxed text
.\" the standard spacings given immediately above may need to be modified for
.\" particular combinations of format elements.  To define a spacing for a
.\" particular element combination, just assign the desired spacing to a number
.\" register whose name consists of <comma><1st element>-<comma><2nd element>
.nr ,Fe-,P 2v				\" space between figure end and new paragraph
.\"
.\" ############ INDENTATION PARAMETERS
.nr PI 0	\" indent for first line of paragraph; default indent for RS, IP
.nr Nh_space 2n			\" indent from para number to paragraph heading
.nr Ns_indent 15p			\" indent for note
.nr list_indent_outer 0\"	XX indent from current left margin to bullet/num
.nr list_indent_inner 3n		\" indent from bullet/number to list item text
.nr varlist_indent 15n		\" indent for hanging label list
.nr Ps_indent 15p			\" indent for screen displays
.nr DDS_indent 15p			\" indent for non-screen displays
.nr Xs_indent 0u			\" indent for examples
.nr Ts_indent 0u			\" indent for tables
.\"
.\" ############ FONT PARAMETERS
.nr PS 11				\" font size for main text
.nr hdr_ps 9			\" font size for page header
.ds hdr_ft GBI\"		XX font for page header
.nr footer_ps 10		\" font size for page footer
.ds footer_ft GBI\"		XX font for page footer
.ds chap_num_ft R\"		XX font for chapter number on title page
.nr chap_num_ps 96		\" font size of chapter number on title page
.ds chap_hdr_ft GBI\"	XX font for chapter header on title page
.nr chap_hdr_ps 24		\" font size for chapter on title page
.ds tocbox_ft I\"		XX font for text of tocbox on chapter title page
.nr tocbox_ps 10		\" font size for text of tocbox
.nr tocbox_vs 12p		\" baseline-to-baseline spacing of text in tocbox
.ds Ah_ft GBI\"		XX font for Aheads
.nr Ah_ps 18			\" font size for Aheads
.nr Ah_vs 20p			\" space between baselines of multiline Ahead
.ds Bh_ft GBI\"		XX font for Bheads
.nr Bh_ps 14			\" font size for Bheads
.nr Bh_vs 16p			\" space between baselines of multiline Bhead
.ds Ch_ft GBI\"		XX font for Cheads
.nr Ch_ps 11			\" font size for Cheads
.nr Ch_vs 13p			\" space between baselines of multiline Chead
.ds Fe_ft I\"			XX font for figure captions (used also for examples)
.ds Fe_num_ft I\"			XX font for figure captions (used also for examples)
.nr Fe_ps 9			\" font size for figure captions
.nr Fe_vs 11p			\" font size for figure captions
.ds Xs_ft I\"			XX font for example captions (used also for examples)
.nr Xs_ps 9			\" font size for example captions 
.nr Xs_vs 11p			\" space between baselines for multiline example captions
.ds QP_ft I\"			XX font for quoted paragraphs
.nr QS_ps 9			\" font size for quoted sections
.nr QS_vs 11p			\" space between baselines of quoted sections
.ds QS_ft I\"			XX font for quoted sections
.nr QI 5n				\"indent for QS/QE blockquotes
.ds Ta_ft I\"			XX font for table captions and text
.nr Ta_ps 10			\" font size for table captions and text
.nr Ta_vs 12p			\" space between baselines for tables
.ds Ta_caption_ft I\"			XX font for table captions
.nr Ta_caption_ps 10		\"font size for table captions
.nr Ps_ps 8			\" font size for screen display text
.nr Ps_vs 10p			\" space between baselines of display text
.nr DDS_ps 10			\" font size for screen display text
.nr DDS_vs 12p			\" space between baselines of display text
.ds FAM G\"	XX font family for main text
.ds cw_font_fam C\" 		XX constant width font family:  Courier
.ds special_font_fam ZD\" 	XX special font family:  Zapf-Dingbats
.\"
.\" ############ TOC PARAMETERS 
.nr B 1\"gtop_toc requires number of local book_setup macro
.nr toc_header_margin 1.5i
.nr toc_title_margin 1.5i
.ds toc_font_fam T 
.ds toc_header_font HB 
.ds toc_body_font R 
.ds toc_chapter_font I
.nr toc_header_fontsize 26
.nr toc_chapter_fontsize 13 
.nr toc_body_fontsize 11 
.nr toc_header_to_entry_space 3v 
.nr toc_chapter_prespace 15p
.nr toc_body_prespace 3p
.nr toc_bhead_indent 3n
.nr toc_chead_indent 6n
.nr toc_dhead_indent 6n
.\" ############ OTHER PARAMETERS
.\" following string for use in complex numbers in figure captions, etc.
.ds dot_or_hyphen \-
.nr tocbox_shade 80			\" percent shading for box
.nr tocbox_PD 1v+2p			\" paragraph spacing inside box
.nr tocbox_width 1.47i		\" width of box
.ds btab_font Times-Roman\"PostScript name of font in bleeding tab
.nr btab_pointsize 10\"pointsize of font in bleeding tab
.nr btab_number_of 5	\"number of bl.tabs on page
.nr btab_long_dimension 1.5i	\"long dimension of bl.tab 
.nr btab_short_dimension .625i	\"short dimension of bl.tab
.nr btab_first_tab_offset_from_header .625i
.nr btab_intertab_space .125i\"space between the tabs
.nr btab_center 0
.\"
.\" ############ DERIVATIVE PARAMETERS
.nr LL \\n[body_width]u	\" line length for body text
.nr LT \\n[LL]			\" length for .tl use (not necessarily used in hdrs)
.nr HM \\n[top_off_ext]+\\n[top_off_int]		\" header margin
.nr FM \\n[bot_off_ext]+\\n[bot_off_int]+1		\" footer margin
.nr PO \\n[left_off_ext]+\\n[left_off_int]		\" page offset
.ds NN \\\\n[PN]
.nr hdr_vertoff \\n[top_off_ext]u+\\n[hdr_vertoff_int]u
.cs CR 19\" XX set character spacings for constant width
.cs CI 19
.cs CB 19
.cs CBI 19
.cs YR 19
.cs YI 19
.cs YB 19
.cs CWR 19
.cs CWI 19
.cs CWB 19
.nr b2 \\n[btab_center]
.nr b4 \\n[btab_number_of]
.nr b5 \\n[btab_long_dimension]
.nr b6 \\n[btab_short_dimension]
.nr b7 \\n[btab_first_tab_offset_from_header]
.nr b8 \\n[btab_intertab_space]
.nr b9 \\n[btab_pointsize]
..
.yy
.de set_fmt_params \" set secondary parameters, based on general format type
.in 0
.ps \\n[PS]z
.vs \\n[VS]u
.po \\n[PO]u
.ll \\n[LL]u
.lt \\n[LT]u
.ad \\n[AD]
.fam \\*[FAM] \"body text in 0 environment
.\" now set parameters for different environments
.set_env_font fg \\*[FAM]
.set_env_font fn \\*[FAM]
.set_env_font dd \\*[FAM]
.set_env_font ta \\*[FAM]
.set_env_font cw \\*[cw_font_fam]
.set_env_font sp \\*[special_font_fam]
.set_env_font tc \\*[FAM]
.set_env_font VL \\*[FAM]
.set_env_font OL \\*[FAM]
.set_env_font IL \\*[FAM]
..
.de ev_tc_init	\" initialize chapter toc environment
.EV tc
.par*env-init
.ft \\*[tocbox_ft] 
.ps \\n[tocbox_ps]z 
.vs \\n[tocbox_vs]u
.po \\n[PO]u
.ll \\n[tocbox_width]u
.in 0
.nh
.fi
.na
..
.de ev_cw_init	\" initialize constant-width environment (screen displays)
.EV cw
.ft R\"always reset font to Roman; don't keep stack across ev's
.ie !"\\$2"" .ps \\$2z
.el .ps \\n[Ps_ps]z				\" set point size
.vs \\n[Ps_vs]u			\" set vertical line spacing
.po \\n[PO]u				\" set page offset
.in (u;0\\$1)				\" no indent unless argument given
.ll \\n[LL]u				\" set line length
'nf
..
.de ev_dd_init	\" initialize display no-fill environment (non-screen displays)
.EV dd
.ft R\"always reset font to Roman; don't keep font stack across ev's
.ps \\n[DDS_ps]z			\" set point size
.vs \\n[DDS_vs]u			\" set vertical line spacing
.po \\n[PO]u				\" set page offset
.in (u;0\\$1)	\" no indent unless argument given
.ll \\n[LL]u				\" set line length
.nf
..
.de ev_fg_init	\" initialize figure environment (graphics)
.EV fg\" enter environment for figures/examples
.ft \\*[Fe_ft]				\" set font
.ps \\n[Fe_ps]z				\" set point size
.vs \\n[Fe_vs]u				\" set vertical line spacing
.po \\n[PO]u				\" set page offset
.in (u;0\\$1)	\" no indent unless argument given
.ll \\n[LL]u				\" set line length
'nf						\" no fill mode
..
.de ev_ta_init	\" initialize figure environment (tables)
.EV ta\" enter environment for tables
.ft \\*[Ta_ft]				\" set font
.ps \\n[Ta_ps]z				\" set point size
.vs \\n[Ta_vs]u			\" set vertical line spacing
.po \\n[PO]u				\" set page offset
.in (u;0\\$1)				\" no indent unless argument given
.ll \\n[LL]u				\" set line length
.hy 0
..
.\"
.\" ############ HEADER AND FOOTER MACROS
.de CM	\" cut marks
.nr cm_width \\n[pg_width_ext]-\\n[left_off_ext]-\\n[right_off_ext]
.vpt 0
.po \\n[left_off_ext]u
.ev cm
.nf
.if "\\$1"pagetop" \{\
.	SP_ABS \\n[top_off_ext]u
.if !\\n[no_cutmarks] \&\h'-0.5i'\D'l 0.35i 0'\h'.15i'\v'-.15i'\D'l 0 -0.35i'\h'\\n[cm_width]u'\D'l 0 0.35i'\v'.15i'\h'.15i'\D'l 0.35i 0'
.NU\}
.if "\\$1"pagebottom" \{\
.	SP_ABS \\n[pg_length_ext]u-\\n[bot_off_ext]u
.if !\\n[no_cutmarks] \&\h'-0.5i'\D'l 0.35i 0'\h'.15i'\v'.15i'\D'l 0 0.35i'\h'\\n[cm_width]u'\D'l 0 -0.35i'\v'-.15i'\h'.15i'\D'l 0.35i 0'
.	SP_ABS \\n[pg_length_ext]u-\\n[bot_off_ext]u+0.5i
.if !\\n[no_cutmarks] \s6\fR\0\\*[DY] \\*[tD]\fP\s0
.NU\}
.fl
.ev
.po \\n[PO]u
.vpt 1
..
.de PT
.ev pt
.po \\n(POu+\\n[hdr_leftoff]u+\\n[hdr_rightoff]u
.in 0
.ft \\*[hdr_ft]
.ps \\n[hdr_ps]z
.lt \\n[LT]u
.SP_ABS \\n[top_off_ext]u+\\n[hdr_vertoff_int]u
.if !\\nB=2 \{\
.ie o .if !\\n[newchapflag] .tl \\*[RH]
.el .tl \\*[LH]
.nr newchapflag 0
.ft R
.ps 15	\" so following \D line will be a hairline
.SP_ABS \\n[top_off_ext]u+\\n[hdr_vertoff_int]u+4p
\D'l \\n[LT]u 0'
.fl
.ps
.NU\}
.po
.ev
.if \\n[cM] .CM pagetop
.als \\$0 PT_real
..
.de BT	\" invoked from footer trap (see pg@top & pg*footer macros in tmac.gs)
.ev pt
.vpt 0
.if \\n[bT] .if o .b0	\" invoke bleeding tabs, if any
.vpt 1
.po \\n(POu+\\n[hdr_leftoff]u+\\n[hdr_rightoff]u
.in 0
.ft \\*[footer_ft]
.ps \\n[footer_ps]z
.lt \\n[LT]u
.SP_ABS \\n[pg_length_ext]u-\\n[bot_off_ext]u-\\n[ftr_vertoff_int]u
.if \\nB<2 \{\
.	ie o .tl \\*[NN]
.	el .tl \\*[NN]
.NU\}
.if \\nB=2 \{\
.	ie o .tl \\*[RH]\\*[NN]
.	el .tl \\*[NN]\\*[LH]
.NU\}
.fl
.po
.ev
.if \\n[cM] .CM pagebottom
.als \\$0 PT_real
..
.de St
.ds RH \\$1
..
.de Ti
.ds LH \\$1
.ds book_title \\$1
..
.\".am pg@super-eject	\" end-macro defined in tmac.gs
.\".sy \\*[gmat_rm] -f /tmp/date.\n#
.\"..
.\"
.\" ############ HEADINGS
.de Nh
.ie \\$1 .nr nH 1
.el .nr nH 0
.ds saved_number_head_value \\$1
..
.de Hd_start
.par@finish
.lg 0
.hy 0
.na
.nD \\$2
.vs \\$5u
.ps \\$4z
.ft \\$6
.SP? \\$3u \\$1
.if !"\\$7"" \{\
.	ll \\$7u
.	nr reset_header_ll 1
.NU\}
..
.de Hd_end
.RT
.SP? \\$2u \\$1
.lg
.ad \\n[AD]
.\".if \\n[reset_header_ll] \{
.\".	ll \\n[body_width]u
.\".	nr reset_header_ll 0
.\".\}
.ns
..
.de Ah	\" Ahead; $1 = head; $2 = second line of long Ahead
.if !r Ah_linelength .nr Ah_linelength \\n[body_width]-.5i
.Hd_start Ah \\n[Ah_need] \\n[Ah_prespace] \\n[Ah_ps] \\n[Ah_vs] \\*[Ah_ft] \\n[Ah_linelength]
.if \\n[nH] .NHx 1
.if r sE .if \\n[nH] .if \w'\\*[SNx]\0\\$1'>\\n[Ah_linelength] \{\
.	in \w'\\*[SNx]\0'u 
.	ti -\w'\\*[SNx]\0'u
.NU\}
.if r sE .if \\n[nH] \\*[SNx]\0\c
\&\\$1
.br
.if !\\$2 \{\
.ie \\n[nH] \h\w'\\*[SNx]\0'u\&\\$2
.el \&\\$2
.NU\}
.Hd_end Ah \\n[Ah_postspace]
.if \\n[hD] \{\
.	ds head_tmp
.	if \\n[nH] .ds head_tmp \\*[SNx]\0\0
.	as head_tmp \\$1
.	if !\\$2 .as head_tmp \& \\$2
.	dT .tC Ah "\\*[head_tmp]"
.NU\} 
..
.de Bh  \" Bhead; $1 = head; $2 = second line of long Bhead
.if !r Bh_linelength .nr Bh_linelength \\n[body_width]-.5i
.Hd_start Bh \\n[Bh_need] \\n[Bh_prespace] \\n[Bh_ps] \\n[Bh_vs] \\*[Bh_ft] \\n[Bh_linelength]
.if \\n[nH] .NHx 2
.if r sE .if \\n[nH] .if \w'\\*[SNx]\0\\$1'>\\n[Bh_linelength] \{\
.	in \w'\\*[SNx]\0'u 
.	ti -\w'\\*[SNx]\0'u
.NU\}
.if \\n[nH] .if \\n[hD_num]>1 \\*[SNx]\0\c
\&\\$1
.br
.if !\\$2 \{\
.ie \\n[nH] \h\w'\\*[SNx]\0'u\&\\$2
.el \&\\$2
.NU\}
.Hd_end Bh \\n[Bh_postspace]
.if \\n[hD]>1 \{\
.    ds head_tmp
.    if \\n[nH] .if \\n[hD_num]>1 .ds head_tmp \\*[SNx]\0\0
.    as head_tmp \\$1
.    if !\\$2 .as head_tmp \& \\$2
.    dT .tC Bh "\\*[head_tmp]"
.NU\}
..
.de Ch  \" Chead; $1 = head; $2 = second line of long Chead
.if !r Ch_linelength .nr Ch_linelength \\n[body_width]-.5i
.Hd_start Ch \\n[Ch_need] \\n[Ch_prespace] \\n[Ch_ps] \\n[Ch_vs] \\*[Ch_ft] \\n[Ch_linelength]
.if \\n[nH] .NHx 3
.if r sE .if \\n[nH] .if \w'\\*[SNx]\0\\$1'>\\n[Ch_linelength] \{\
.	in \w'\\*[SNx]\0'u 
.	ti -\w'\\*[SNx]\0'u
.NU\}
.if \\n[nH] \\*[SNx]\0\c
\&\\$1
.br
.if !\\$2 \{\
.ie \\n[nH] \h\w'\\*[SNx]\0'u\&\\$2
.el \&\\$2
.NU\}
.Hd_end Ch \\n[Ch_postspace]
.if \\n[hD]-2 .dT .tC Ch "\\$1"
..
.de NHx  \" redefined from NH macro of -gs; 'x' differentiates the variables
.nr NSx \\$1					\" track the level of head we're dealing with
.if r sE .if !"\\g(sE"" .nr NSx +1	\" if in chapter, Ah (e.g.) -> level 2
.nr H\\n[NSx]x +1				\" increment the count for current level
.if !\\n[NSx]-4 .nr H5x 0		\" zero out everything below this level
.if !\\n[NSx]-3 .nr H4x 0
.if !\\n[NSx]-2 .nr H3x 0
.if !\\n[NSx]-1 .nr H2x 0
.if r sE .if !"\\g(sE"" .if !"\\g(sE"A" \{\
.nr H1x \\n[sE]
.ds SNx \\n[H1x]
.NU\}
.if r sE .if "\\g(sE"A" .ds SNx \\n[sE]
.ie \\n[NSx]-1 .as SNx .\\n[H2x]
.el .as SNx .
.if \\n[NSx]-2 .as SNx .\\n[H3x]
.if \\n[NSx]-3 .as SNx .\\n[H4x]
.if \\n[NSx]-4 .as SNx .\\n[H5x]
..
.\"
.\" ############ FOOTNOTES
.de par*fp!4
.@PP
\&\\*[f\\$1]\ \c
..
.de par*fp!4-no
.@PP
\&\\$1\ \c
..
.\" redef from tmac.gs
.am par@init \" because FL=LL in fn defaults to 5/6th of LL
.nr FL \\n[body_width]u
.nr FPS 9
.nr FVS \\n[FPS]+1.5
.nr FI 0
..
.\"
.\" ############ TOC, INDEX
.\" get rid of -mgs toc stuff
.\".rm @XS
.\".rm XS
.rm XE
.rm XA
.rm TC
.rm PX
.de tC	\" create raw toc entries
.if "\\$1"C" .write toc \\$* \\*[NN]
.if "\\$1"A" .write toc \\$* \\*[NN]
.if "\\$1"Ah" .write toc \\$* \\*[NN]
.if "\\$1"Bh" .write toc \\$* \\*[NN]
.if "\\$1"Ch" .write toc \\$* \\*[NN]
.if "\\$1"P" .write toc \\$*
.if "\\$1"F" .write figtoc \\$* \\*[NN]
.if "\\$1"T" .write tabtoc \\$* \\*[NN]
.if "\\$1"X" .write extoc \\$* \\*[NN]
..
.de XX
.dT .write index "X\t\\$1\\$2\\$3\\$4\t\\*[NN]"
..
.de XB
.dT .write index "B\t\\$1\\$2\\$3\\$4\t\\*[NN]"
..
.de @XS \"ACTUALLY XS; Gnu tmac.gs .cov*ab-init aliases it. 
.dT .write index "S\t\\$1\\$2\\$3\\$4\t\\*[NN]"
..
.de XE
.dT .write index "E\t\\$1\\$2\\$3\\$4\t\\*[NN]"
..
.de XN
.dT .write index "N\t\\$1\\$2\\$3\\$4\t"
..
.de XNP
.dT .write index "N1\t\\$1\\$2\\$3\\$4\t\\*[NN]"
..
.de XC
.dT .write index "C\\$1\t\\$2\\$3"
..
.de X1 \"start span entry with BOLD page numbers
.dT .write index "B1\t\\$1\\$2\\$3\\$4\t\\*[NN]"
..
.de X2 \"end span entry with BOLD page numbers
.dT .write index "B2\t\\$1\\$2\\$3\\$4\t\\*[NN]"
..
.\"
.\" ############ GRAPHICS MACROS
.de Fs	\" start figure; $1=float figure if this much space not available
.		\" $2=optional indent
.if \\n[sgml_on] \{\.endwcd
.nr wcon_off 1\}
.ie \\n[keepingfig] \{\
.	am keptfig
.	Fs \\$@
\\..
.NU\}
.el \{\
.if \\n[nl]>\\n[HM] .if (i;\\$1)+\\n[Fs_prespace]+\\n[Fc_prespace]+\\n[VS]+\\n[Fe_postspace]>\\n[.t] \{\
.	nr keepingfig 1
.	de keptfig
.	Fs \\$@
\\..
.	NU\}
.if \\n[keepingfig]=1 .ig yy_Fs
.if !\\n[doingkeptfig] .br
.ev_fg_init \\$2
.SP? \\n[Fs_prespace]u Fs
.yy_Fs
.NU\} \"ends else
..
.de Fe  \" Figure end; $1 = title
.if \\n[keepingfig]=1 \{\
.	am keptfig
.	Fe \\$@
\\..
.NU\}
.if \\n[keepingfig]=1 .ig yy_Fe
.nr fG +1
.ds fG
.SP_reset	\" there may have been no text output since .Fs--only PostScript
.if !"\\$1"" \{\
.if r sE .if !"\\g(sE"" .ds fG \\n[sE]\\*[dot_or_hyphen]
.if "\\n[sE]"0" .ds fG
.as fG \\n[fG]
.SP? \\n[Fc_prespace]u
.if \\n[center_fig_caption] .ce 5
.dT .tC F "\\*[fG]" "\\$*"
.ie "\\*[Fe_num_ft]"" \\*[Figure] \\*[fG]:\h'1n'\\$*\fP
.el \f[\\*[Fe_num_ft]]Figure \\*[fG]:\h'1n'\fP\\$*
.if \\n[center_fig_caption] .ce 0
.NU\}
.SP? \\n[Fe_postspace]u Fe
.fl
.EV\"	XX  pop environment we set in .ev_fg_init
.if \\n[sgml_on] .if \\n[wcon_off] .nr wcon_off 0
.yy_Fe
..
.de Pf    \"   $1 = filename; $2 = width (in inches); $3 = height;
.\"  $4 = horiz offset; $5 = vert offset.  Figure will be scaled until
.\"  it meets width or height, whichever occurs first.  If $4 is
.\"  "center", figure will be centered in current text column.
.\"  $6 = noscale means don't scale the figure. If F register is
.\"  bigger than 1, no PostScript is output but space for figure is 
.\"  IF FIGURE IS SCALED, YOU *MUST* GIVE WIDTH AND HEIGHT ARGS
.\"  MAKE irrelevant ONE OUT OF BOUNDS; scale will be according to first
.\"  one it hits correctly. DON'T SPECIFY UNITS, but keep them inches!! 
.if \\n[keepingfig]=1 \{\
.	am keptfig
.	Pf \\$@
\\..
.NU\}
.if \\n[keepingfig]=1 .ig yy
.Pf_raw \\$@
.yy
..
.de Pf_raw    \"   $1 = filename; $2 = width (in inches); $3 = height;
.\"  $4 = horiz offset; $5 = vert offset.  Figure will be scaled until
.\"  it meets width or height, whichever occurs first.  If $4 is
.\"  "center", figure will be centered in current text column.
.\"  $6 = noscale means don't scale the figure. If F register is
.\"  bigger than 1, no PostScript is output but space for figure is 
.nr tp 0
.if "\\$2"" .nr tp 1
.if "\\$2"0" .nr tp 1
.if "\\$3"" .nr tp 1
.if "\\$3"0" .nr tp 1
.if !"\\$6"noscale" .if \\n[tp]=1 .tm Warning: missing width or height specification in Pf macro
.nr Pf +1	\" only used for numbering tmp files
.nr pf_offset 0
.ds ctr
.if !"\\$4"" \{\
.    ie "\\$4"center" \{\
.		nr tp \\n[.l]-\\n[.i]
.		ds ctr -c \\n[tp]
.	NU\}
.    el .nr pf_offset \\$4i
.NU\}
.ds scale
.if "\\$6"noscale" .ds scale -noscale
.ie \\n[hiresfigs] \{\
.if "\\$2"" .if "\\$3"".sy \\*[gmat_gmacroff] \\$1 -h \\*[ctr] \\*[scale] -p \\n#.\\n[Pf]
.if !"\\$2"" .if "\\$3"".sy \\*[gmat_gmacroff] \\$1 -h -x \\$2 \\*[ctr] \\*[scale] -p \\n#.\\n[Pf]
.if "\\$2"" .if !"\\$3"".sy \\*[gmat_gmacroff] \\$1 -h -y \\$3 \\*[ctr] \\*[scale] -p \\n#.\\n[Pf]
.if !"\\$2"" .if !"\\$3"".sy \\*[gmat_gmacroff] \\$1 -h -x \\$2 -y \\$3 \\*[ctr] \\*[scale] -p \\n#.\\n[Pf]
.NU\}
.el \{\
.if "\\$2"" .if "\\$3"" .sy \\*[gmat_gmacroff] \\$1  \\*[ctr] \\*[scale] -p \\n#.\\n[Pf]
.if !"\\$2"" .if "\\$3"" .sy \\*[gmat_gmacroff] \\$1 -x \\$2 \\*[ctr] \\*[scale] -p \\n#.\\n[Pf]
.if "\\$2"" .if !"\\$3"" .sy \\*[gmat_gmacroff] \\$1 -y \\$3 \\*[ctr] \\*[scale] -p \\n#.\\n[Pf]
.if !"\\$2"" .if !"\\$3"" .sy \\*[gmat_gmacroff] \\$1 -x \\$2 -y \\$3 \\*[ctr] \\*[scale] -p \\n#.\\n[Pf]
.NU\}
.\" single quote instead of . in next line is to fool soelim
'so \\*[TMPDIR]/Pf.size.\\n#.\\n[Pf]
.nr tmp 0
.if !"\\$5"" .nr tmp \\$5i
.rs
.if \\n[nl]>\\n[HM] .ne \\n[tmp]u+\\n[pf_height]u
.ie \\nF .sp \\n[tmp]u+\\n[pf_height]u+1v
.el .sp \\n[tmp]u+\\n[pf_height]u
.if \\n[pf_offset]  .in +\\n[pf_offset]u
.if !\\nF \\X'ps: import \\*[TMPDIR]/Pf.fig.\\n#.\\n[Pf] \\n[llx] \\n[lly] \\n[urx] \\n[ury] \\n[pf_width] \\n[pf_height]'
.if \\n[pf_offset] .in
.fl
.if \\nd=0 .sy \\*[gmat_rm] \\*[TMPDIR]/Pf.size.\\n#.\\n[Pf]
..
.\" bT       \" number of times Pt has been invoked so far
.\" b1       \" same as bT--or $3 (if given)
.\".nr pg_width_ext 8.5i	\" width of troff page, left to right
.\".nr right_off_ext .75i	\" from right edge of troff page to right cutmark
.de Pt \" Print tab on edge of page. If you specify values to replace
.	  \" defaults, the new values remain in effect for subsequent
.	  \" invocations of Pt. To turn off bleeding tabs, set bT to 0.
.ie \\n[btab_align] .nr pM \\n[btab_align]
.el .nr pM \\n[pg_width_ext]-\\n[right_off_ext]
.\" $1 = text line 1
.\" $2 = text line 2
.\" $3 = n, as in nth tab of book.  (Defaults to # of invocations of Pt so far.)
.\" $4 = number of tab positions.  (Defaults to 5.)
.\" $5 = long dimension of tab.  (Defaults to 1.5 inches.)
.\" $6 = narrow dimension of tab.  (Defaults to 5/8 inch.)
.\" $7 = offset of first tab from HM.  (Defaults to .25 inch.)
.\" $8 = inter-tab spacing.  (Defaults to .1 inch.)
.\" $9 = pointsize for tab text
.ds pT \\$1
.if !"\\$2"" .ds pU \\$2
.\".nr bT +1
.ie "\\$3"" .nr b1 \\n(bT
.el .nr b1 \\$3
.if !"\\$4"" .nr b4 \\$4
.if !"\\$5"" .nr b5 \\$5
.if !"\\$6"" .nr b6 \\$6
.if !"\\$7"" .nr b7 \\$7
.if !"\\$8"" .nr b8 \\$8
.if !"\\$9"" .nr b9 \\$9 
..
.de b0
.\" ==========================================================================
.\" First, define PS subroutines for invocation below.  bT_position changes
.\" the coordinate space and orientation so that (0,0) is the upper-left
.\" corner of the tab (when the page is viewed in portrait orientation) and
.\" the text runs down the page (in landscape fashion).
.\"
\\X'ps: exec /bT_position { \\n(pM u \\n(b6 u sub \\n(HM u \\n(b7 u add \
    \\n(b1 1 sub \\n(b4 mod \\n(b5 u \\n(b8 u add mul add translate 90 \
    rotate newpath 0 0 moveto } def'
.\"
.\" ==========================================================================
.\" gfont is the font to use for the tab.  This has to be expressed using
.\" grops 'SF' macro or the text will be backwards.
.\"
\\X'ps: exec /gfont { /F0 \\n(b9/\\*[btab_font]@0 SF } def'
.\"
.\" ==========================================================================
.\" width1 is the width of the first line
.\" width2 is the width of the second line (defined below, if there are two
.\" lines).
.\" width is the width of the longest line (only used if not centering)
.\"
\\X'ps: exec gfont /width1 (\\*[pT]) stringwidth pop def'
.\"
.\" ==========================================================================
.\" draw black box
.\"
\\X'ps: exec /-save0- save def bT_position 0 \\n(b6 u 125 add neg rlineto \
    \\n(b5 u 0 rlineto 0 \\n(b6 u 125 add rlineto closepath 0 setgray fill \
    -save0- restore'
.\"
.\" ==========================================================================
.\" insert text (if there is only one line)
.\"
.if \\*[pU] \{\
.\" in each line, increase number before "moveto" to move text toward 
.\" the edge of the page.
.\"
\\X'ps: exec /-save0- save def bT_position gfont 1 setgray \\n(b5 u width1 \
    sub 2 div 15 neg moveto (\\*[pT]) show \
    -save0- restore'
.NU\}
.\"
.\" ==========================================================================
.\" insert text (if there are two lines)
.\"
.if !\\*[pU] \{\
.\"
.\" define width2, set width, redefine width1 and width2 if centering
.\"
\\X'ps: exec gfont /width2 (\\*[pU]) stringwidth pop def width1 width2 gt \
    {/width width1 def} {/width width2 def} ifelse'
.if !\\n(b2 \\X'ps: exec /width2 width def /width1 width def'
.\"
.\" each moveto in ff line positions a line of tab text.  Increasing the last
.\" number before each moveto shifts a text line toward outer edge of page
.\"
\\X'ps: exec /-save0- save def gfont 1 setgray bT_position \
    \\n(b5 u 2 div \
    width1 2 div sub \
    22 neg moveto \
    (\\*[pT]) show \
    \\n(b5 u 2 div \
    width2 2 div sub \
    10 neg moveto (\\*[pU]) show \
    -save0- restore'
.NU\}
..
.\"
.\" ############ DISPLAYS (EXAMPLES, TABLES, SCREENS)
.de Ps	\" start screen display; $1 = indent (other than default)
.br
.ie !"\\$1"" .nr tmp_Ps (n;\\$1) 
.el .nr tmp_Ps \\n[Ps_indent]+\\n[.i]
.ev_cw_init \\n[tmp_Ps]u
.ne 3
.SP? \\n[Ps_prespace]u Ps
.ns
.lg 0
..
.de Pe  \" screen display end
.lg
.fl
.EV
.SP? \\n[Pe_postspace]u Pe
..
.de DDS	\" start non-screen display; $1 = indent (other than default)
.br
.ie !"\\$1"" .nr tmp_Ds (n;\\$1)
.el .nr tmp_Ds \\n[DDS_indent]
.\".ie !"\\n[tmp_Ds]"0" .ev_dd_init \\n[tmp_Ds]u
.\".el .ev_dd_init
.ev_dd_init \\n[tmp_Ds]u
.ne 3
.SP? \\n[DDS_prespace]u DDS
.ns
..
.de DDE  \" screen display end
.fl
.EV
.SP? \\n[DDE_postspace]u DDE
..
.de Xs   \" start of example $1 = title; $2 = optional nonstandard indent
.ie \\n[Xs_need] .Nd \\n[Xs_need]
.el .br
.ie !"\\$2"" .nr tmp_Xs (n;\\$2)
.el .nr tmp_Xs \\n[Xs_indent]
.ev_fg_init \\n[tmp_Xs]u
.SP? \\n[Xs_prespace]u Xs
.nr xG +1
.ds xG
.\".if r sE .if !"\\g(sE"" .ds xG \\n[sE]\\*[dot_or_hyphen]
.if !"\\n[sE]"0" .ds xG \\n[sE]\\*[dot_or_hyphen]
.as xG \\n[xG]
.if \\n[tmp_Xs] .in 0
\\*[Example] \\*[xG]:  \\$1
.dT .tC X "\\*[xG]" "\\$1"
.di ET 
\f[\\*[Fe_ft]]\\*[Example] \\*[xG]:  \\$1  (\\*[continued])\fP
.nr et_on 1
.nr exp*header-ht \\n[dn]
.dT .SP? \\n[Xc_postspace]u
.di
.if \\n[tmp_Xs] .in \\n[tmp_Xs]u
.fl
.EV\"	XX pop environment we set in .ev_fg_init
.Ps \\n[tmp_Xs]u\" this puts us in cw environment
.rs
.SP? \\n[Xc_postspace]u Xc
.nr ex_in_prog 1
.nf
..
.de Xe  \" Example end
.nr et_on 0
.nr ex_in_prog 0
.Pe
.\".SP? \\n[Xe_postspace]u Xe
..
.de Ts  \" start a table.  $1=cutline; $2=optional nonstandard indent
.\" $3=nonstandard need
.br
.ie !"\\$2"" .nr tmp_Ts (n;\\$2)
.el .nr tmp_Ts \\n[Ts_indent]
.ev_ta_init \\n[tmp_Ts]u
.SP? \\n[Ts_prespace]u Ts
.if \\n[i\\n[l0]] .in +\\n[i\\n[l0]]u 
.nr tB +1
.ds tB
.ps \\n[Ta_caption_ps]z 
.\".if r sE .if !"\\g(sE"" .ds tB \\n[sE]\\*[dot_or_hyphen]
.if !"\\n[sE]"0" .ds tB \\n[sE]\\*[dot_or_hyphen]
.as tB \\n[tB]
.ie "\\$3"" .ne 1i
.el .ne \\$3
\f[\\*[Ta_caption_ft]]\\*[Table] \\*[tB]:  \\$1\fP
.dT .tC T "\\*(tB" "\\$1"
.ds tB \f[\\*[Ta_caption_ft]]\\*[Table] \\*[tB]:  \\$1  (\\*[continued])\fP
.ps \\n[Ta_ps]z
.nr tB_on 1
.SP? \\n[Tc_postspace]u Tc
.ne 1i
.ns
..
.de Te  \" table end--no space follows if $1 is 0 
.nr tB_on 0
.hy 14
.fl
.if \\n[i\\n[l0]] .in
.EV
.if !"\\$1"0" .ie "\\n[TE_page]"\\n%" .SP? \\n[Te_postspace]u-\\n[DD]u Te
.el .SP? \\n[Te_postspace]u Te
..
.de TS
.\" The break is necessary in the case where the first page has not yet begun.
.br
.sp \\n[DD]u
.nr tbl*on 1
.if '\\$1'H' .di tbl*header-div
..
.de TE \"redefine it from tmac.gs to avoid extra space
.ie '\\n(.z'tbl*header-div' .@error-recover .TS H but no .TH before .TE
.el \{\
.   nr tbl*on 0
.	nr tbl*have-header 0
.	nr TE_page \\n%             \"testing pageno for space problem
.	sp \\n[DD]u
.\}
.\" reset tabs
.TA
..
.\" redefined from tmac.gs to output continued lines in tbl-heads
.\" will do so if there is a tbl-head or not, and if there is a
.\" caption or not
.de fig@top-hook
.if \\n[keepingfig]=1 \{\
.	nr keepingfig 0
.	nr doingkeptfig 1
.	keptfig
.	nr doingkeptfig 0
.NU\}
..
.de tbl@top-hook
.ie !\\n[keepingfig] \{\
.ie \\n[tB_on] \{\
.if \\n[tbl*have-header] \{\
.	ie \\n[.t]-\\n[tbl*header-ht]-1v \{\
.			nr save_ind \\n[.i]
.			in 0
.if \\n[Ta_caption_ps] .ps \\n[Ta_caption_ps]z
\&\\*(tB
.if \\n[Ta_caption_ps] .ps
.			SP? \\n[Tc_postspace]u
.			in \\n[save_ind]u
.		tbl*print-header
.		NU\}
.	el .sp \\n[.t]u
.NU\}
.if !\\n[tbl*have-header] \{\
.			nr save_ind \\n[.i]
.			in 0
\&\\*(tB
.			SP? \\n[Tc_postspace]u
.			in \\n[save_ind]u
.NU\}
.NU\}
.el \{\
.if \\n[tbl*have-header] \{\
.	ie \\n[.t]-\\n[tbl*header-ht]-1v .tbl*print-header
.	el .sp \\n[.t]u
.\}
.NU\}
.			NU\}
.el \{\
.am keptfig
.ie \\n[tB_on] \{\
.if \\n[tbl*have-header] \{\
.	ie \\n[.t]-\\n[tbl*header-ht]-1v \{\
.			nr save_ind \\n[.i]
.			in 0
.if \\n[Ta_caption_ps] .ps \\n[Ta_caption_ps]z
\&\\*(tB
.if \\n[Ta_caption_ps] .ps
.			SP? \\n[Tc_postspace]u
.			in \\n[save_ind]u
.		tbl*print-header
.		NU\}
.	el .sp \\n[.t]u
.NU\}
.if !\\n[tbl*have-header] \{\
.			nr save_ind \\n[.i]
.			in 0
\&\\*(tB
.			SP? \\n[Tc_postspace]u
.			in \\n[save_ind]u
.NU\}
.NU\}
.el \{\
.if \\n[tbl*have-header] \{\
.	ie \\n[.t]-\\n[tbl*header-ht]-1v .tbl*print-header
.	el .sp \\n[.t]u
.\}
.NU\}
\\..
.NU\}
..
.de exp@top-hook
.if \\n[et_on] \{\
.	ie \\n[.t]-\\n[exp*header-ht]-1v \{\
.			nr save_ind \\n[.i]
.			in 0 
\\*[ET]\&
.sp -1v
.nr two_line_ET 0
.			in \\n[save_ind]u
.		NU\}
.	el .sp \\n[.t]u
.\}
..
.de kp*do-top
.\" If the first keep won't fit, only force it out if we haven't had a footnote
.\" and we're at the top of the page.
.nr kp*force \\n[pg@fn-flag]=0&(\\n[nl]<=\\n[pg@header-bottom])
.nr kp*fits 1
.while \\n[kp@tail]>\\n[kp@head]&\\n[kp*fits] \{\
.	ie \\n[.t]>\\n[kp*ht!\\n[kp@head]]:\\n[kp*force] \{\
.		nr kp*force 0
.		\" It's important to advance kp@head before bringing
.		\" back the keep, so that if the last line of the
.		\" last keep springs the bottom of page trap, a new
.		\" page will not be started unnecessarily.
.		rn kp*div!\\n[kp@head] kp*temp
.		nr kp@head +1
.		ev nf
\&
.sp -\\n[VS]u
.		kp*temp
.		ev
.		rm kp*temp
.	\}
.	el .nr kp*fits 0
.\}
..
.de pg*start-col
.\" Handle footnote overflow before floating keeps, because the keep
.\" might contain an embedded footnote.
.if \\n[wcon_off] .nr wcon_off 0
.ns
.fn@top-hook
.fig@top-hook
.kp@top-hook
.tbl@top-hook
.exp@top-hook
.ns
..
.de Ve	\" vertical ellipsis.  $1=optional indent; $2=optional pointsize
.if !"\\$1"" .in \\$1
.if !"\\$2"" .ps \\$2z
.vs 4
.sp
\&.
\&.
\&.
.if !"\\$2"" .ps
.vs
.if !"\\$1"" .in
..
.de Ns \" "note/caution/warning"; $1 = type N, C, W, R
.SP? \\n[Ns_prespace]u
.ne \\n[Ns_need]u
.nr note_context 1
.ds tmp_Ns \\*[NOTE]
.if !"\\$1"E" .if !"\\$1"N" .if !"\\$1"C" .if !"\\$1"W" .if !"\\$1"R" \
.	tm "Need N, C, W, R E, or P as argument for Ns macro--using N"
.if "\\$1"C" .ds tmp_Ns \\*[CAUTION]
.if "\\$1"E" .ds tmp_Ns \\*[EXAMPLE]
.if "\\$1"W" .ds tmp_Ns \\*[WARNING]
.if "\\$1"R" .ds tmp_Ns NOTE TO REVIEWERS
.if "\\$1"R" .nr rN 1	\" flag for note end
.ft R
.ns
.nr save_indent_Ns \\n[.i]
.fi
.di nT   \" divert following text to nT ("Note Text")
.\".in +\\n[Ns_indent]u
.nr \\n[.ev]:li \\n[.i]u+\\n[Ns_indent]u
.nr \\n[.ev]:ri \\n[.i]u+\\n[Ns_indent]u
.\".nr LL -(\\n[Ns_indent]u+\\n[Ns_indent]u)
.nr LL -(\\n[Ns_indent]u)
.par@reset
.ad b
.ll \\n(LLu
..
.de Ne \" "note end"; no args
.br
.di
.ce
.ie "\\*[Ns_ft]"" \fB\\*[tmp_Ns]\fP
.el \f[\\*[Ns_ft]]\\*[tmp_Ns]\fP
.SP? \\n[Nc_postspace]u Nc
.ns
.in 0
.nf
.nT    \" output note text
.if \\n(rN \{\
.	da rN  \" divert to Review Note macro for processing by EM
\!.IP "\(sq Page \\*[NN]:" 12n 
.	nT  \" output saved text again
.	br
.	di
.NU\}
.nr rN 0 \" clear flag
.\".in \\n[save_indent_Ns]u
.\".nr LL +(\\n[Ns_indent]u+\\n[Ns_indent]u)
.nr LL +(\\n[Ns_indent]u)
.ll \\n(LLu
.nr \\n[.ev]:ri -\\n[Ns_indent]u
.nr \\n[.ev]:li -\\n[Ns_indent]u
.par@finish
.LP
.SP? \\n[Ne_postspace]
.fi
.nr note_context 0
..
.\"
.\" ############ LIST MACROS
.de Ls
.\" list start; $1 = A(LPHA), a(alpha), B(ullet), N(umeric), 
.\"                  R(oman), r(oman);  $2 = indent
.br
.if !"\\$1"D" .if !"\\$1"Z" .if !"\\$1"P" .if !"\\$1"A" .if !"\\$1"B" .if !"\\$1"N" .if !"\\$1"R" \
.									if !"\\$1"r" .if !"\\$1"a" \
.	tm "Ls: missing argument or invalid argument: \\$1 Defaulting to N"
.nr l\\n+[l0] 0 1
.if rnumber_list_indent_inner .nr list_indent_inner \\n[number_list_indent_inner]u
.if rnumber_list_indent_outer .nr list_indent_outer \\n[number_list_indent_outer]u 
.if "\\$1"A" .af l\\n[l0] A
.if "\\$1"a" .af l\\n[l0] a
.if "\\$1"B"\{\
.	if "\\$3"" .ds l\\n[l0] \(bu
.	if !"\\$3"" .ds l\\n[l0] \\$3
.	nr l\\n[l0] 0-1
.	if rbullet_list_indent_inner .nr list_indent_inner \\n[bullet_list_indent_inner]
.	if rbullet_list_indent_outer .nr list_indent_outer \\n[bullet_list_indent_outer]u 
.NU\}
.if "\\$1"R" .af l\\n[l0] I
.if "\\$1"r" .af l\\n[l0] i
.if "\\$1"D"\{\
.	ds l\\n(l0 \-
.	nr l\\n(l0 0-1
.NU\}
.ie !"\\$2"" .nr i\\n[l0] (n;\\$2)   \" list indent
.el .nr i\\n[l0] \\n[list_indent_inner]
.if "\\$1"Z" \{\
.	nr \\n[l0]co_seq 1 
.	nr \\n[l0]Zz 1
.   ds l\\n[l0] \E[dnum\\\\n+[\\\\n[l0]co_seq]]
.NU\}
.nr co_sq 1
.nr \\n[.ev]:ai \\n[\\n[.ev]:pli]
.if \\n[note_context] .nr \\n[.ev]:ai +\\n[Ns_indent]u
.SP? \\n[list_prespace]u Ls
.\".SP_reset
.RS
.if r list_indent_outer .nr \\n[.ev]:li +\\n[list_indent_outer]u
.if \\n[newmplayout] \{\
.nr newmpindent_saved \\n[.i]
.nr \\n[.ev]:li +\\n[OLin]u
.NU\}
.ns
..
.nr l0 0 1      \" initialize list nesting level counter
.de Li  \" list item;  $1 = 0 means no blank line before item
.if !"\\$1"" .if !"\\$1"0" .aB "Li: invalid argument: \\$1"
.br
.ne 2.1v
.if !"\\$1"0" .SP? \\n[list_itemspace]u Li
.ns \"may be causing problems
.ie !\\n[\\n[l0]Zz] \{\
.ie "\\n[l\\n[l0]]"-1" .IP "\\*[l\\n[l0]]" "\\n[i\\n[l0]]u"
.el \{\
.	nr l\\n[l0] +1
.ie \\n[align_on_dot] .nip "\\n[l\\n[l0]]." "\\n[i\\n[l0]]u" 
.el .IP "\\n[l\\n[l0]]." "\\n[i\\n[l0]]u"
.NU\}
.			NU\}
.el \{\
.IP "\\*[l\\n[l0]]" "\\n[i\\n[l0]]u"
.nr \\n[l0]co_seq +1 
.NU\}
..
.de Le  \" list end;  $1 = 0 means no extra blank line following last item
.if !"\\$1"" .if !"\\$1"0" .aB "Le: invalid argument: \\$1"
.br
.if rlist_indent_outer .nr \\n[.ev]:li -\\n[list_indent_outer]u
.rr l\\n[l0]
.rr i\\n[l0]
.rm l\\n[l0]
.rr \\n[l0]Zz
.rr \\n[l0]co_seq
.rr co_sq 
.nr l0 -1
.RE
.if !\\n[l0] .RT
.if \\n[note_context] .in +\\n[Ns_indent]u
.if !"\\$1"0" .SP? \\n[list_postspace]u Le
.\".if \\n[newmplayout] .in \\n[newmpindent_saved]u
.if \\n[newmplayout] .in .25i 
..
.de nip \"number IP macro: modified to align numbers for numlists on decimal point
.if \\n[.$]>1 .nr \\n[.ev]:ai (n;\\$2)
.par*start \\n[\\n[.ev]:ai] 0
.if !'\\$1'' \{\
.	\" Divert the label so as to freeze any spaces.
.	di par*label
.	par*push-tag-env
.	ta \w'\0\0.'uR
\&\t\\$1
.	par*pop-tag-env
.	di
.	chop par*label
.	ti -\\n[\\n[.ev]:ai]u
.	ie \\n[dl]+1n<=\\n[\\n[.ev]:ai] \\*[par*label]\h'|\\n[\\n[.ev]:ai]u'\c
.	el \{\
\\*[par*label]
.	br
.	\}
.	rm par*label
.\}
..
.\"
.\" ############ SPACING CONTROL
.\" .SP? says: give us whatever additional space (over and above any cumu-
.\" lative total) may be necessary to achieve the amount specified in arg1.
.\" We may want to pre-process the user's .sp commands into .SP? commands.
.\" Important:  this mechanism should not try to fiddle with no-space mode,
.\" which needs to be available for normal use.
.\" Invoke SP? _after_ a line break, and after any .vs change.  Also, use
.\" .EV rather than .ev when doing space control across environments.
.de SP?	\" $1=total space we want to achieve
.		\" $2=non-null: look for total space in nr \\*[lastelem-\\$2]
.nr tmp (u;\\$1)
.if !"\\$2"" .if r ,\\*[lastelem]-,\\$2 .nr tmp \\n[,\\*[lastelem]-,\\$2]
.nr tmp1 \\n[.d]
.nr tmp2 \\n%
.sp (u;\\n[tmp])-\\n[.v]u-\\n[accum_space]u
.ie \\n[tmp2]=\\n% .nr accum_space +(\\n[.d]-\\n[tmp1])
.el .nr accum_space 0
.it 1 SP_reset
.nr counting_space 1
.ds lastelem \\$2
..
.de SP!	\" sneak some space outside of space-control mechanism; $1=space
.\" $2 allows you to identify the element if you wish
.nr tmp \\n%
.sp (u;\\$1)
.if !\\n[tmp]=\\n% .nr accum_space 0
.if !"\\$2"" .ds lastelem \\$2
..
.\" space to absolute position on page regardless of current vertical spacing
.\" this is independent of the space-tracking system of the macros above
.de SP_ABS	\" $1=absolute position (no vert bar)
.sp |((v;\\$1)-(\\n[.v]u-\\n[VS]u))
..
.de MK_ABS	\" mark absolute position on page; $1=number register to assign
.			\" $2=optional increment (eg to back up over a line break)
.ie "\\$2"" .nr \\$1 \\n[nl]+(\\n[.v]-\\n[VS])
.el         .nr \\$1 \\n[nl]+(\\n[.v]-\\n[VS]+(\\$2))
..
.de SP_reset	\" reset space counting
.nr accum_space 0
.nr counting_space 0
.ds lastelem 0
..
.de EV
.ev \\$1
.it 1 SP_reset
..
.\"
.\" ############ SOME MISCELLANEOUS "TOOL" MACROS
.de dT	\" pass through a macro invocation if we're in a diversion
.ds cmd \\$1
.shift
.ie "\\n[.z]"" \\*[cmd] \\$*
.el \\!.dT \\*[cmd] \\$@
..
.de save_nr	\" save number register ($1) and give it a temporary value ($2)
.while \\n[.$] \{\
.	nr save\\$1 \\n[\\$1]
.	nr \\$1 \\$2
.	shift 2
.NU\}
..
.de restore_nr	\" restore saved value to number registers
.while \\n[.$] \{\
.	nr \\$1 \\n[save\\$1]
.	shift
.NU\}
..
.de set_env_font	\" set font family for an environment
.ev \\$1
.fam \\$2
.ev
..
.de aton
.if "\\$1"A" \{\
.af \\$2 A
.nr \\$2 1
.NU\}
.if "\\$1"B" \{\
.af \\$2 A
.nr \\$2 2
.NU\}
.if "\\$1"C" \{\
.af \\$2 A
.nr \\$2 3
.NU\}
.if "\\$1"D" \{\
.af \\$2 A
.nr \\$2 4
.NU\}
.if "\\$1"E" \{\
.af \\$2 A
.nr \\$2 5
.NU\}
.if "\\$1"F" \{\
.af \\$2 A
.nr \\$2 6
.NU\}
.if "\\$1"G" \{\
.af \\$2 A
.nr \\$2 7
.NU\}
.if "\\$1"H" \{\
.af \\$2 A
.nr \\$2 8
.NU\}
.if "\\$1"I" \{\
.af \\$2 A
.nr \\$2 9
.NU\}
.if "\\$1"J" \{\
.af \\$2 A
.nr \\$2 10
.NU\}
.if "\\$1"K" \{\
.af \\$2 A
.nr \\$2 11
.NU\}
.if "\\$1"L" \{\
.af \\$2 A
.nr \\$2 12
.NU\}
.if "\\$1"M" \{\
.af \\$2 A
.nr \\$2 13
.NU\}
.if "\\$1"N" \{\
.af \\$2 A
.nr \\$2 14
.NU\}
.if "\\$1"O" \{\
.af \\$2 A
.nr \\$2 15
.NU\}
.if "\\$1"P" \{\
.af \\$2 A
.nr \\$2 16
.NU\}
.if "\\$1"Q" \{\
.af \\$2 A
.nr \\$2 17
.NU\}
.if "\\$1"R" \{\
.af \\$2 A
.nr \\$2 18
.NU\}
.if "\\$1"S" \{\
.af \\$2 A
.nr \\$2 19
.NU\}
.if "\\$1"T" \{\
.af \\$2 A
.nr \\$2 20
.NU\}
.if "\\$1"U" \{\
.af \\$2 A
.nr \\$2 21
.NU\}
.if "\\$1"V" \{\
.af \\$2 A
.nr \\$2 22 
.NU\}
.if "\\$1"W" \{\
.af \\$2 A
.nr \\$2 23 
.NU\}
.if "\\$1"X" \{\
.af \\$2 A
.nr \\$2 24 
.NU\}
.if "\\$1"Y" \{\
.af \\$2 A
.nr \\$2 25 
.NU\}
.if "\\$1"Z" \{\
.af \\$2 A
.nr \\$2 26 
.NU\}
..
.de shaded_box	\" draw shaded box. $1=width; $2=height; $3=percent shade
.nr boxwidth (u;\\$1);
.nr boxheight (u;\\$2);
\X'ps: exec gsave \\$3 100 div setgray \\n[boxwidth] u 0 rlineto 0 \\n[boxheight] u rlineto -\\n[boxwidth] u 0 rlineto closepath fill grestore'
.rr boxheight
.rr boxwidth
..
.de rV	\" draw reverse video box around word. $1=word, $2=anything if
.\" words on next line should be on same line as this 
.if "\\n[.ev]"cw" .ft CB
.if "\\n[.ev]"fg" .ft CB
.if !\\n[.u] .nr nofill 1
.if !\\n[.u] .fi
.nr boxwidth (\w'\\$1'u+1n)
.nr boxheight 0
.nr boxheight -1v
\v'3p'\\X'ps: exec gsave 0 100 div setgray \\n[boxwidth] u 0 rlineto 0 \\n[boxheight] u rlineto -\\n[boxwidth] u 0 rlineto closepath fill grestore'\v'-3p'
\\X'ps: exec gsave 1 setgray'
.nr boxtextwidth \w'\\$1'
\&\h'-1.5n'\\$1\\X'ps: exec grestore'
.rr boxwidth
.rr boxheight
.if \\n[nofill] .if "\\$2"" \{.nf
.nr nofill 0\}
.if "\\n[.ev]"cw" .ft
.if "\\n[.ev]"fg" .ft
..
.de sT	\" draw shaded box around word. $1=word 
.if !\\n[.u] .nr nofill 1
.if !\\n[.u] .fi
.nr boxwidth (\w'\\$1'u+1n)
.nr boxheight -1v
\v'3p'\\X'ps: exec gsave .75 setgray \\n[boxwidth] u 0 rlineto 0 \\n[boxheight] u rlineto -\\n[boxwidth] u 0 rlineto closepath fill grestore'\v'-3p'
\\X'ps: exec gsave 0 setgray'
.nr boxtextwidth \w'\\$1'
\&\h'-1.5n'\\$1
\\X'ps: exec grestore'
.rr boxwidth
.rr boxheight
.if \\n[nofill] .nf
..
.de shaded_textbox
.\" .shaded_textbox width height percent_shade "text" "fudgefactor"
.\"
.\" This macro draws a shaded box of size "width x height" by inserting
.\" PostScript code directly into the output stream.  The text argument
.\" is centered in the box by troff.   The text is printed in white letters,
.\" but this could be changed by adjusting the percentage in front of
.\" the 'setgray' that immediately precedes the text.  Do not remove the
.\" following 'grestore' or all subsequent text will also be white!
.\"
.\" After this macro executes, the cursor is returned to a point on the
.\" page immediately above the box and at the left margin.
.\"
.\" Example: .shaded_box 6.5i .5i 50 "\f[CP1]Some Text\fR"
.\"
.\"   Centers the text "Some Text" in the Copperplate font in a 50% shaded
.\"   gray box 6.5 inches wide and 0.5 inches tall.
.\"
.\" 13 Jun 94: hacked together by Norm and Lenny and Steve.
.\"
.nr boxwidth (u;\\$1)
.nr boxheight (u;\\$2)
.nr boxhalf (\\n[boxheight]/2+\\$5)
\\X'ps: exec gsave \\$3 100 div setgray \\n[boxwidth] u 0 rlineto 0 \\n[boxheight] u rlineto -\\n[boxwidth] u 0 rlineto closepath fill grestore'
\\X'ps: exec gsave 1 setgray'
.mk R&
.nr boxtextwidth \w'\\$4'
.nr boxtextshift (\\n[boxwidth]-\\n[boxtextwidth])/2
\v'\\n[boxhalf]u'\h'\\n[boxtextshift]u'\\$4
.sp |\\n(R&u
\\X'ps: exec grestore'
..
.\" CURSORS OF ONE SORT OR ANOTHER (block or underline)
.ds Cu \R'boxheight -1v'\R'boxwidth +\wnu'\v'3p'\\X'ps: exec gsave 0 100 div setgray \\n[boxwidth] u 0 rlineto 0 \\n[boxheight] u rlineto -\\n[boxwidth] u 0 rlineto closepath fill grestore'\v'-3p'\\X'ps: exec gsave 1 setgray'\h'(\\n[boxwidth]-\wMiu)/2-.5n'
.ds Uc \X'ps: exec grestore'\R'boxheight 0'\R'boxwidth 0'\|\|
.de uL
.\".ie !\\n[.u] \Z'\\$1'\v'.25m'\D'l \w'\\$1'u 0'\v'-.25m'\\$2
.\".el \Z'\\$1'\v'.25m'\D'l \w'\\$1'u 0'\v'-.25m'\\$2\c
\Z'\\$1'\v'.25m'\D'l \w'\\$1'u 0'\v'-.25m'\\$2\c
..
.\" ############ MISC REDEFS FROM GTROFF'S tmac.gs
.rm toc*end-entry
.de @error-recover
.\" changes:
.\"    add .if test to following line so we can invoke .@error-recover from .Se
.if !"\\$1"" .@error \\$@ (recovering)
.nr *pop-count 0
.while !'\\n(.z'' \{\
.	@warning automatically terminating diversion \\n(.z
.	ie d @div-end!\\n(.z .@div-end!\\n(.z
.	el .*div-end-default
.	nr *pop-count +1
.	\" ensure that we don't loop forever
.	if \\n[*pop-count]>20 .@fatal recovery failed
.\}
.while !'\\n[.ev]'0' .ev
.par@reset-env
.par@reset
..
.am pg@top
.SP_reset
.if !\\n[consec_footnote_nums] \{\
.	if \\n[fn*text-num]=\\n[fn*note-num] \{\
.nr fn*text-num 0 1
.nr fn*note-num 0 1
.	NU\}
.NU\}
.if \\n(xF>0 \{\
.nr save_ind \\n[.i]
.in 0
.ie !\\n[new_contd_line] \{\
.ie \\n(xF=1 \&\\*(xF (\\*[cont'd])
.el \{\
.sy \\*[gmat_echo] "\\*(xf" | \\*[gmat_sed] -f /usr/local/txtools/bin/xfsed > /tmp/,xf\n#
'so /tmp/,xf\n#
.sy \\*[gmat_rm] -f /tmp/,xf\n#
\\*(xF, \\*(xf (\\*[cont'd])
.NU\}
.NU\}
.el \{\
.\".if e \{\
.\".ie \\n(xF=1 \&\\*(xF (\\*[cont'd])
.\".el \{\
.\".sy \\*[gmat_echo] "\\*(xf" | \\*[gmat_sed] -f /usr/local/txtools/bin/xfsed > /tmp/,xf\n#
.\"'so /tmp/,xf\n#
.\".sy \\*[gmat_rm] -f /tmp/,xf\n#
.\"\\*(xF, \\*(xf (\\*[cont'd])
.\".NU\}
.\".NU\}
.NU\}
.in \\n[save_ind]u
.NU\}
..
.am QP
.ps -1
.ft \\*[QP_ft]
..
.am @QS
.save_nr PS \\n[QS_ps] VS \\n[QS_vs]
.sty 1 \\*[QS_ft]
.if \\n[QS_PD] .nr PD \\n[QS_PD]u
.LP
.SP? \\n[QS_prespace]u QS
..
.de QQE \"can't add to QE because it is aliased as RE
.br
.ie \\n[\\n[.ev]:il] \{\
.	nr \\n[.ev]:il -1
.	nr \\n[.ev]:ai \\n[\\n[.ev]:ai!\\n[\\n[.ev]:il]]
.	nr \\n[.ev]:li \\n[\\n[.ev]:li!\\n[\\n[.ev]:il]]
.	nr \\n[.ev]:ri \\n[\\n[.ev]:ri!\\n[\\n[.ev]:il]]
.	nr \\n[.ev]:pli \\n[\\n[.ev]:pli!\\n[\\n[.ev]:il]]
.	nr \\n[.ev]:pri \\n[\\n[.ev]:pri!\\n[\\n[.ev]:il]]
.\}
.el .@error unbalanced \\$0
.if \\n[QS_PD] .nr PD \\n[body_text_PD]u
.par@reset
.restore_nr PS VS
.sty 1 R
.LP
.SP? \\n[QE_postspace]u QQE
..
.als Qs @QS
.als Qe QQE
.de QE
.br
.ie \\n[\\n[.ev]:il] \{\
.	nr \\n[.ev]:il -1
.	nr \\n[.ev]:ai \\n[\\n[.ev]:ai!\\n[\\n[.ev]:il]]
.	nr \\n[.ev]:li \\n[\\n[.ev]:li!\\n[\\n[.ev]:il]]
.	nr \\n[.ev]:ri \\n[\\n[.ev]:ri!\\n[\\n[.ev]:il]]
.	nr \\n[.ev]:pli \\n[\\n[.ev]:pli!\\n[\\n[.ev]:il]]
.	nr \\n[.ev]:pri \\n[\\n[.ev]:pri!\\n[\\n[.ev]:il]]
.\}
.el .@error unbalanced \\$0
.if \\n[QS_PD] .nr PD \\n[body_text_PD]u
.par@reset
..
.\" following macro invoked for every different paragraph type
.de par*start
.ds@auto-end
.nr \\n[.ev]:pli \\$1
.nr \\n[.ev]:pri \\$2
.par@reset
.SP? \\n[\\n[.ev]:PD]u P
.ne 1v+\\n(.Vu
..
.\" box a word:redef for punctuation after it
.de BX
.if !\\n[.u] .nr nofill 1
.if !\\n[.u] 'fi
.nr par*bxw \w'\\$1'+.4m
.ie "\\$2"" \{\
\Z'\v'.25m'\D'l 0 -1.1m'\D'l \\n[par*bxw]u 0'\D'l 0 1.1m'\D'l -\\n[par*bxw]u 0''\Z'\h'.2m'\\$1'\h'\\n[par*bxw]u'\|
.NU\}
.el \{\
\Z'\v'.25m'\D'l 0 -1.1m'\D'l \\n[par*bxw]u 0'\D'l 0 1.1m'\D'l -\\n[par*bxw]u 0''\
\Z'\h'.2m'\\$1'\
\h'\\n[par*bxw]u'\^\\$2\|
.NU\}
.if \\n[nofill] \{'nf
.nr nofill 0\}
..
.\" redef to allow setting indent of box and text ($1) and also space
.\" between text and vertical sides of box ($2); default is 1n 
.de B1
.ie !"\\$1"" .nr boxindent \\$1
.el .nr boxindent 1n
.ie !"\\$2"" .nr boxtextindent \\$2
.el .nr boxtextindent 1n
.br
.di par*box-div
.nr \\n[.ev]:li \\n[boxindent]u+\\n[boxtextindent]
.nr \\n[.ev]:ri \\n[boxindent]u+\\n[boxtextindent]
.par@reset
..
.de B2
.ie '\\n(.z'par*box-div' \{\
.	br
.	di
.	ds@need \\n[dn]
.	par*box-mark-top
.	ev nf
.	par*box-div
.	ev
.	nr \\n[.ev]:ri -(\\n[boxindent]u+\\n[boxtextindent])
.	nr \\n[.ev]:li -(\\n[boxindent]u+\\n[boxtextindent])
.	par@finish
.ie !"\\n[boxindent]"5000" .par*box-draw \\n[boxindent]u \\n[.l]u-\\n[boxindent]u
.el .par*box-draw \\n[.i]u \\n[.l]u
.\}
.el .@error B2 without B1
..
.de pg@end-text	\" end macro
.if \\n[keepingfig] \{\
\&\c
'	bp
.	pg@top
.NU\}
.br
.if "\\g(PN"i" .ds numtmp numisrom
.if "\\g(PN"i" .af PN 1
.tm File \\*[source_filename], pages \\*[starting_page] to \\*[NN]
.if "\\*[numtmp]"numisrom" .af PN i
.ds numtmp
.if !\\n[pagerange] \{\
.	sy mv \\*[INDEXDIR]/\\*[filename].\\n# \\*[INDEXDIR]/\\*[filename]
.	sy mv \\*[TOCDIR]/\\*[filename].\\n# \\*[TOCDIR]/\\*[filename]
.	sy mv \\*[FIGDIR]/\\*[filename].\\n# \\*[FIGDIR]/\\*[filename]
.	sy mv \\*[TABDIR]/\\*[filename].\\n# \\*[TABDIR]/\\*[filename]
.	sy mv \\*[EXDIR]/\\*[filename].\\n# \\*[EXDIR]/\\*[filename]
.NU\}
.\".close index
.nr pg@text-ended 1
.pg@super-eject
..
.de pg@super-eject
.br
.if !'\\n(.z'' .@error-recover diversion open while ejecting page
.\" Make sure we stay in the end macro while there is still footnote overflow
.\" left, or floating keeps.
.while \\n[kp@tail]>\\n[kp@head]:\\n[pg@fn-flag] \{\
.	if \\n[kp@tail]>\\n[kp@head] \{\
.	da kp*div!\\n[kp@head]
.	nr fudge_pageno \\n[PN]+1
.	dT .tm File \\*[source_filename], pages \\*[starting_page] to \\n[fudge_pageno]
.	nr fudge_pageno 0
.	da
.	NU\}
.	rs
.	bp
.\}
.bp
..
.\"
.\" ############ ODDS AND ENDS
.de Nd \"to push out lines with a break even after a floating fig
.br
.ie \\n[keepingfig] \{\
.		am keptfig
.		Nd \\$1
\\..
.NU\}
.el .if !"\\$1"" .ne \\$1
..
.de nD \"to push out lines *before* a floating figure
.br 
.if !"\\$1"" .ne \\$1
..
.de nE \"to push out lines *after* a floating figure
.ie \\n[keepingfig] \{\
.am keptfig
.ne \\$1
\\..
.NU\}
.el .ne \\$1
..
.\" redef to make need 2v! tmac.gs has only one
.de fn*do-FS
.if \\n[fn*open] .@error-recover nested FS
.nr fn*open 1
.if \\n[.u] \{\
.	\" Ensure that the first line of the footnote is on the same page
.	\" as the reference.  I think this is minimal.
.	ev fn
.	nr fn*need 2v
.	ev
.	ie \\n[pg@fn-flag] .nr fn*need +\\n[fn:PD]
.	el .nr fn*need +\\n[fn@sep-dist]
.	ne \\n[fn*need]u+\\n[.V]u>?0
.\}
.ev fn
.par@reset-env
.fn*start-div
.par@reset
.ie \\n[.$] .FP \\$@
.el .@LP
..
.\" In-chapter cross reference macro
.de Ref
.nr dX 1
.	\" Cross refs.  $1: e=Example, f=Figure, t=Table
.       \" $2= text/punctuation to follow the ref
.       \" $3= p or n, reference to prev or next object (default is next + 1)
.       \" $4 = a number specifying how previous or next
.ie !"\\$3"" \{\
.       ie "\\$3"p" \{\
.               nr dX \\$4-1 \" set decrement to arg 4 -1 (0 is prev fig)
.               nr eX \\n[xG]-\\n[dX]  \"Define values for PREVIOUS reference
.ie \\n[keepingfig] .nr fX \\n[fG]-\\n[dX]+1  \" (augment value of counter variables)
.el .nr fX \\n[fG]-\\n[dX]  \" (current value of counter variables)
.               nr tX \\n[tB]-\\n[dX]\}
.       el \{\
.if !"\\$4"" .nr dX \\$4 \" set increment to value of arg 4
.               nr eX \\n[xG]+\\n[dX]  \"Define values for PREVIOUS reference
.ie \\n[keepingfig] .nr fX \\n[fG]+\\n[dX]+1 \" (augment value of counter variables)
.el .nr fX \\n[fG]+\\n[dX]  \" (current value of counter variables)
.               nr tX \\n[tB]+\\n[dX]\}
.       \}
.el \{\
.       nr eX \\n[xG]+1  \"Define values for NEXT (+1) reference
.ie \\n[keepingfig] .nr fX \\n[fG]+2
.el .nr fX \\n[fG]+1
.       nr tX \\n[tB]+1\}
.ds enext \\n[sE]\\*[dot_or_hyphen]\\n[eX]\"Define strings that use values
.ds fnext \\n[sE]\\*[dot_or_hyphen]\\n[fX]
.ds tnext \\n[sE]\\*[dot_or_hyphen]\\n[tX]
.if "\\$1"e" \\*[Example] \\*[enext]\\$2
.if "\\$1"f" \\*[Figure] \\*[fnext]\\$2
.if "\\$1"t" \\*[Table] \\*[tnext]\\$2
..
.de BLANK \"outputs LAST BLANK PAGE ONLY, without headers/footers
.if \\n[keepingfig] \{\
.br
\&\c
'	bp
.	pg@top
.NU\}
.de PT
.if \\n[cM] .CM pagetop
\\..
.bp
\&
.de BT
.if \\n[cM] .CM pagebottom
\\..
..
.\" Prints toc entries 
.de TC
.if "\\$1"0" \{\
.ft \\*[toc_chapter_font]
.ps \\n[toc_chapter_fontsize]z
.sp \\n[toc_chapter_prespace]u
.lc
.ie "\\$2"Preface" \\$2 \a \\$3
.el Chapter \\$2:  \\$3 \a \\$4
.sp \\n[toc_chapter_prespace]u-\\n[toc_body_prespace]u
.lc .
.NU\}
.if "\\$1"A" \{\
.ft \\*[toc_chapter_font]
.ps \\n[toc_chapter_fontsize]z
.sp \\n[toc_chapter_prespace]u
.lc
Appendix \\$2:  \\$3 \a \\$4
.sp \\n[toc_chapter_prespace]u-\\n[toc_body_prespace]u
.lc .
.NU\}
.if "\\$1"1" .nr toc_body_entry 1
.if "\\$1"2" .nr toc_body_entry 1
.if "\\$1"3" .nr toc_body_entry 1
.if "\\$1"4" .nr toc_body_entry 1
.if \\n[toc_body_entry] \{\
.	ft \\*[toc_body_font]
.	ps \\n[toc_body_fontsize]z
.	sp \\n[toc_body_prespace]u
.	if "\\$1"1" \{\
.	ie "\\$3"" \\$2
.	el \\$2 \a \\$3
.	NU\}
.	if "\\$1"2" \{\
.	ie "\\$3"" \\$2
.	el \h'\\n[toc_bhead_indent]u'\\$2 \a \\$3
.	NU\}
.	if "\\$1"3" \{\
.	ie "\\$3"" \\$2
.	el \h'\\n[toc_chead_indent]u'\\$2 \a \\$3
.	NU\}
.	if "\\$1"4" \{\
.	ie "\\$3"" \\$2
.	el \h'\\n[toc_bhead_indent]u'\\$2 \a \\$3
.	NU\}
.NU\}
.if "\\$1"F" .nr other_tocentry 1
.if "\\$1"X" .nr other_tocentry 1
.if "\\$1"E" .nr other_tocentry 1 
.if "\\$1"T" .nr other_tocentry 1 
.if \\n[other_tocentry] \{\
.	ft \\*[toc_body_font]
.	ps \\n[toc_body_fontsize]z
.	sp \\n[toc_body_prespace]u
.	ie "\\$3"" \\$2
.	el \\$2 \a \\$3
.NU\}
.rr other_tocentry 
.rr toc_sect_entry
.br
..
.\" ############ LET'S INITIALIZE AND GO
.AM	\" get "improved accent marks"
.\" ############ SOME USEFUL DEVELOPMENT NOTES
.\" Everything hereafter will probably go in a separate file eventually.
.\" Font families on ruby:
.\"		A 	Avant Garde
.\"		BM 	Bookman
.\"		C 	Courier
.\"		G 	Garamond
.\"		GB 	GaramondBook
.\"		GC 	Garamond
.\"		H 	Helvetica;  
.\"		HN 	Helvetica Narrow
.\"		BK	NewBaskerville
.\"		MDR Madrone
.\"		P 	Palatino
.\"		T 	Times (Roman);
.\"		WDO Woodtype Ornaments
.\"		ZD 	Zapf Dingbats
.de ND \"need for tables, which are processed in diversions in gtbl
.	ie '\\n(.z'' \{
.		br
.		ne \\$1
.	NU\}
.	el \!.ND \\$1
..
.\"##########TABLE FOOTNOTE MACROS
.ds j2 \\f(TR*\\fP
.ds j3 \\(dg
.ds j4 \\(dd
.ds j5 \\(sc
.ds = \\*[par@sup-start]\E*(j\\n+[stfct]\\*[par@sup-end]\"XX pick one of the footnote symbols
.ds - \\*[par@sup-start]\E*(j\\n[stfct]\\*[par@sup-end]\"XX pick one of the footnote symbols
.ds w0 \f(TR*\fP
.ds w1 \(dg
.ds w2 \(dd
.ds w3 \(sc
.de TFS
.ie !"\\$1"" .Nd \\$1 
.el .Nd 3
.rs
.ie !\\n[tfct] .sp -5p
.el .sp -1P
.ps 8
.vs 10
.ft 1
.ad b
.fi
\\*(w\\n[tfct]
..
.de TFE
.br
.ps \\n(PSz
.vs \\n(VSu
.nr tfct +1
.ft 1 
.sp 1P
.ns
..
.am Ts
.nr tfct 0 1
.nr stfct 0 1
..
.\"###########################################
.\" Fix for compatibility from version 1.08 to 1.09
.de @init
.\" a non-empty environment
.ev ne
\c
.ev
.ev nf
'nf
.ev
..
.\"#############MISCELLANEOUS POSSIBLY USEFUL STRING DEFS
.ds TeX T\h'-.2m'\v'.3n'E\v'-.3n'\h'-.2n'X
.ds LaTeX L\s-2\h'-.6n'\v'-.3n'A\v'.3n'\h'-.2n'\s0T\h'-.2m'\v'.3n'E\v'-.3n'\h'-.2n'X
.ds BIBTeX B\s-2IB\s0\h'-.2n'T\h'-.2m'\v'.3n'E\v'-.3n'\h'-.2n'X
.ds log_2_e log\s-2\Z'\v'0.4m'2\v'-0.4m'\s+2'\h'1n'\fIe\fP
.ds log_2_10 log\s-2\Z'\v'0.4m'2\v'-0.4m'\s+2'\h'1n'10
.ds sqrt_2 \(sr2
.ds co_num \[dnum\\n[co_sq]]\R'co_sq +1'
.\" Zapf dingbat sans serif reverse video encircled numbers for lists
.char \[dnum1] \s10\f[ZD]\N'202'\fP\s0
.char \[dnum2] \s10\f[ZD]\N'203'\fP\s0
.char \[dnum3] \s10\f[ZD]\N'204'\fP\s0
.char \[dnum4] \s10\f[ZD]\N'205'\fP\s0
.char \[dnum5] \s10\f[ZD]\N'206'\fP\s0
.char \[dnum6] \s10\f[ZD]\N'207'\fP\s0
.char \[dnum7] \s10\f[ZD]\N'208'\fP\s0
.char \[dnum8] \s10\f[ZD]\N'209'\fP\s0
.char \[dnum9] \s10\f[ZD]\N'210'\fP\s0
.char \[dnum10] \s10\f[ZD]\N'211'\fP\s0
.char \[check] \s10\f[ZD]\N'51'\fP\s0
.char \(bb \N'166'
.char \[sdq] ""
.\" next char is 2/4 fraction
.char \[24] \s-4\v'-.3m'2\s+4\v'.3m'\(f/\h'-.5p'\s-44\s+4
.\" next 2 lines fool groff to hyphenate around em dash with \^'s around it
.hcode \(em -
.char \(em \^\(em\^
.de wcon_off
.nr wcon_off 1
..
.\"ALTERNATIVE TO IP that takes two args, followed by hanging indent 
.de YP
.if \\n[.$]>1 .nr \\n[.ev]:ai (n;\\$2)
.par*start \\n[\\n[.ev]:ai] 0
.if !'\\$1'' \{\
.	\" Divert the label so as to freeze any spaces.
.ad l \"to make labels rr; remove this and ad b below if you want adj 
.	di par*label
.	par*push-tag-env
\&\\$1
.	par*pop-tag-env
.	di
.	chop par*label
.	nr label_length \w'\\$1'
.	ie \\n[label_length]>\\n[body_width] .in -\\n[\\n[.ev]:ai]u
.	el .ti -\\n[\\n[.ev]:ai]u
.	ie \\n[dl]+1n<=\\n[\\n[.ev]:ai] \\*[par*label]\h'|\\n[\\n[.ev]:ai]u'\c
.	el \{\
.	ie \\n[label_length]>\\n[body_width] \\$1 
.	el \{\
\\*[par*label]
.	NU\}
.	br
.ad b
.	\}
.	rm par*label
.	if \\n[label_length]>\\n[body_width] \{\
.	in \\n[varlist_indent]u
.	nr \\n[.ev]:ai \\n[varlist_indent]u
.	NU\}
.\}
..
.de IPS 
.if !"\\n[.z]"" .SP_reset
.ne 2.1v
.ns
.if "\\n[.$]"1" \{\
.if \w'\\$1'>\\n[body_width] .sp -1v
.YP "\\$1" \\n[varlist_indent]u
.NU\}
.if "\\n[.$]"2" \{\
.if \w'\\$1'>\\n[body_width] .sp -1v
.YP "\&\\$1"
.sp -.5v
.YP "\\$2" \\n[varlist_indent]u
.NU\}
.if \\n[.$]>=3 \{\
.if \w'\\$1'>\\n[body_width] .sp -1v
.while \\n[.$]>1 \{\
.YP "\\$1" \\n[varlist_indent]u
.sp -.5v
.shift
.NU\}
.if \\n[.$]=1 .YP "\\$1" \\n[varlist_indent]u
.NU\}
..
.de lphyph
.\" sets slash to permit line to break after it
.cflags 4 /
..
.\" .Ac accented-char accent char
.de Ac
.char \\$1 \\$3\
\k[acc]\
\h'(u;-\w'\\$2'-\w'\\$3'/2+\\\\n[skw]+(\w'x'*0)-\\\\n[skw])'\
\v'(u;\w'x'*0+\\\\n[rst]+(\w'\\$3'*0)-\\\\n[rst])'\
\\$2\
\v'(u;\w'x'*0-\\\\n[rst]+(\w'\\$3'*0)+\\\\n[rst])'\
\h'|\\\\n[acc]u'
.hcode \\$1\\$3
..
.Ac \[xdot] \(a. \fIx\fR
.Ac \[xDot] \(ad \fIx\fR
.Ac \[xhat] \(a^ \fIx\fR
.Ac \[xtilde] \(a~ \fIx\fR
.Ac \[xbar] \(a- \fIx\fR
.char \[arar] \v'-0.5m'\s-2\(->\s+2\v'0.5m' 
.char \[alrar] \v'-0.5m'\s-2\(<>\s+2\v'0.5m' 
.char \[ubar] \h'-1p'\v'0.1m'_\v'-0.1m'
.Ac \[xrarr] \[arar] \fIx\fR
.Ac \[xlrarr] \[alrar] \fIx\fR
.Ac \[xubar] \[ubar] \fIx\fR
.ds sub \s-2\v'+.4m'
.ds /sub \v'-.4m'\s+2
.char \[quotedbl] \N'034'
.char \[NS] \v'1.5p'\X'ps: import /work/nutshell/webnut.win32/figs/HTML_dingbat_N.eps 0 0 9 10 10000'\h'.13i'\v'-1.5p'
.char \[IE] \v'1.5p'\X'ps: import /work/nutshell/webnut.win32/figs/HTML_dingbat_I.eps 0 0 10 11 10000'\h'.13i'\v'-1.5p'
.char \[NA] \v'1.5p'\X'ps: import /work/nutshell/webnut.win32/figs/HTML_dingbat_NA.eps 0 0 10 10 10000'\h'.13i'\v'-1.5p'
.char \[ns] \v'1.5p'\X'ps: import /work/nutshell/webnut.win32/figs/HTML_dingbat_N.eps 0 0 9 10 7500'\h'.1i'\v'-1.5p'
.char \[ie] \v'1.5p'\X'ps: import /work/nutshell/webnut.win32/figs/HTML_dingbat_I.eps 0 0 10 11 7500'\h'.1i'\v'-1.5p'
