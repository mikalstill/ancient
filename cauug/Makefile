.SUFFIXES : .sgml .ps .pdf

all: clean agenda

agenda: agenda-proc.sgml agenda.ps agenda.pdf

agenda-proc.sgml: agenda.sgml
	builddb eps < agenda.sgml > agenda-proc.sgml 2> log

# General purpose rules
.sgml.ps:
	db2ps $< 2> $<.errs > $<.out

.ps.pdf:
	ps2pdf $<

clean:
	rm -f agenda-proc* *.log *.errs *~
