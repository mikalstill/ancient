all:	short lj

short: output.sgml output.ps output.pdf
	striptext -c < output.sgml | wc -w

output.sgml: built.sgml
	builddb eps < built.sgml > output.sgml

output.ps: output.sgml
	db2ps output.sgml

output.pdf: output.ps
	ps2pdf output.ps

clean:
	rm output* gen-* *~

lj: output.sgml
	./format.pl < built.sgml | sed -e 's/&lt;/</g' -e 's/&gt;/>/g' -e 's/&mdash;/--/g' > lj.txt
	./fixup.sh
	wc -w *.txt sidebar-*

built.sgml: source.xml dw2db
	./dw2db < source.xml > built.sgml