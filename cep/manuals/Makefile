.SUFFIXES : .sgml .ps .pdf

all: clean user

user: usermanual.sgml usermanual.ps usermanual.pdf
	striptext -c < usermanual.sgml | wc -w

usermanual.sgml: user.sgml 
	builddb eps < user.sgml > usermanual.sgml 2> usermanual.log

# General purpose rules
.sgml.ps:
	db2ps $< 2> $<.errs

.ps.pdf:
	ps2pdf $<

clean:
	rm -f usermanual* *.log *.errs *~
