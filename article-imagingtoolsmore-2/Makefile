short:  output.ps output.pdf
	striptext -c < output.sgml | wc -w

built.sgml: source.xml dw2db
	./squelch < source.xml | ./dw2db > built.sgml

output.sgml: built.sgml
	builddb eps < built.sgml > output.sgml

output.ps: output.sgml
	db2ps output.sgml

output.pdf: output.ps
	ps2pdf output.ps

clean:
	rm output* gen-* *~