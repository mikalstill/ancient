.SUFFIXES : .sgml .ps .pdf

all: thesis

thesis: GDMSthesis.sgml GDMSthesis.ps GDMSthesis.pdf
	striptext -c < techmanual.sgml | wc -w

GDMSthesis.sgml: thesis.sgml
	builddb eps < thesis.sgml > GDMSthesis.sgml 2> GDMSthesis.log

# General purpose rules
.sgml.ps:
	db2ps $< 2> $<.errs

.ps.pdf:
	ps2pdf $<

clean:
	rm -f GDMSthese* *.log *.errs *~
