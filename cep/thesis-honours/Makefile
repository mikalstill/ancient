.SUFFIXES : .sgml .ps .pdf

all: clean thesis

thesis: honoursThesis.sgml honoursThesis.ps honoursThesis.pdf
	striptext -c < honoursThesis.sgml | wc -w

honoursThesis.sgml: thesis.sgml
	builddb eps < thesis.sgml > honoursThesis.sgml 2> honoursThesis.sgml.log

# General purpose rules
.sgml.ps:
	db2ps $< 2> $<.errs > $<.out

.ps.pdf:
	ps2pdf $<

clean:
	rm -f honoursThesis* *.log *.errs *~
