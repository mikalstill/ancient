.SUFFIXES : .sgml .ps .pdf

all: user tech

user: usermanual.sgml usermanual.ps usermanual.pdf
	striptext -c < usermanual.sgml | wc -w

usermanual.sgml: user.sgml quickstart.sgml install.sgml running.sgml uiconfig.sgml maths.sgml quickref.sgml
	builddb eps < user.sgml > usermanual.sgml 2> usermanual.log

tech: techmanual.sgml techmanual.ps techmanual.pdf
	striptext -c < techmanual.sgml | wc -w

techmanual.sgml: tech.sgml design.sgml classes.sgml testing.sgml source.sgml
	builddb eps < tech.sgml > techmanual.sgml 2> techmanual.log

# General purpose rules
.sgml.ps:
	db2ps $< 2> $<.errs

.ps.pdf:
	ps2pdf $<

clean:
	rm -f usermanual* techmanual* *.log *.errs *~