all:	short lj

short: output.sgml output.ps output.pdf
	striptext -c < output.sgml | wc -w

output.sgml: source.sgml
	builddb eps < source.sgml > output.sgml

output.ps: output.sgml
	db2ps output.sgml

output.pdf: output.ps
	ps2pdf output.ps

clean:
	rm output* gen-* *~

lj: output.sgml
	./format.pl < source.sgml > lj.txt
	wc -w *.txt sidebar-*