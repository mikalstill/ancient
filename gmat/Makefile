# Makefile for FreeBSD intro book
# Greg Lehey, LEMIS
#
# $Id: Makefile,v 1.1.1.1 2002-07-15 21:33:31 root Exp $
#
.SUFFIXES:	.sgml .ps .pdf

GMAT	=	/usr/local/share/gmat/bin/gmat
# Third edition, for importing.
F3	=	/home/Book/FreeBSD-3

# For converting PostScript to PDF
GS	=	gs

CHAPTERS = 	temp

CHAPS    =	${CHAPTERS:=.sgml}
POSTSCRIPT =	${CHAPTERS:=.ps}
PAPER	=	a4

# All files to be maintained by RCS
RCS-FILES = ${PREFACE} ${CHAPS} Makefile paths.make tidy.el howto.image README \
	BOOKFILES BOOKIDS convert.pl howto massageps.el

# Make the book in one swell foop.
all:
	${GMAT} --book --fopt -U -f

# Make individual chapters
chapters: ${POSTSCRIPT}

book book.ps:	${POSTSCRIPT}
	cat ${POSTSCRIPT} > book.ps
	emacs -batch book.ps -l massageps.el -f massageps -f exit

.sgml.ps:
	${GMAT} --fopt -U -f $<
	mv drv-grog-grog.ps $@

.ps.pdf:
	$(GS) -q -dNOPAUSE -dBATCH -sDEVICE=pdfwrite -sPAPERSIZE=$(PAPER) -sOutputFile=$@ -c save pop -f $<

# ${CHAPS} ${REVIEWS}: ${POSTSCRIPT}

Chapters:
	mkdir -p $*

# Check in with current date.
ci:
	ci -d  ${RCS-FILES} totidy

# Check out our files
co:
	-co -l -M -q ${RCS-FILES} totidy

import:
	for i in ${CHAPTERS:=.mm}; do \
		cp -p ${F3}/$$i .; \
		rcs -l $$i; \
		ci -m"Imported from 3rd edition" $$i;	\
		co $$i; \
	done
	./convert.pl
	
clean:
	@echo +++ $@
	rm -f *.toc *.ps *.ascii *~ core core.* *.even *.odd Contents Index make.log *.PS *.pdf
	rm -rf *.tar.gz
	rm -f  paths.make filelist
	rm -f ${RCS-FILES}  ${RCS-FILES:.sgml=.m} totidy massage*.el
	rm -f ktrace.out *.pp Make.log


