.SUFFIXES : .sgml .ps .pdf .txt .html

all: clean agenda
	striptext < output.sgml | wc -w

agenda: output.sgml output.ps output.pdf output.txt output.html

output.sgml: agenda.sgml
	builddb eps < agenda.sgml > output.sgml 2> log

# General purpose rules
.sgml.txt:
	docbook2txt $< 2> $<.errs > $<.out

.sgml.html:
	docbook2html --nochunks $< 2> $<.errs > $<.out

.sgml.ps:
	db2ps $< 2> $<.errs > $<.out

.ps.pdf:
	ps2pdf $<

clean:
	rm -f output* *.log *.errs *~ agenda.sgml.out
