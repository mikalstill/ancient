.SUFFIXES : .sgml .ps .pdf

all: clean thesis

thesis: GDMSthesis.sgml GDMSthesis.ps GDMSthesis.pdf
	striptext -c < GDMSthesis.sgml | wc -w

GDMSthesis.sgml: thesis.sgml
	builddb eps < thesis.sgml > GDMSthesis.sgml 2> GDMSthesis.sgml.log

# General purpose rules
.sgml.ps:
	db2ps $< 2> $<.errs > $<.out

.ps.pdf:
	ps2pdf $<

clean:
	rm -f GDMSthesis* *.log *.errs *~
