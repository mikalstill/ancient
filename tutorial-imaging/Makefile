tutorial: output.sgml output.ps
	striptext -c < output.sgml | wc -w

output.sgml: concept-antialias.sgml concept-ctm.sgml concept-encryption.sgml concept-grayscaleconvert.sgml concept-pixel.sgml concept-raster.sgml concepts.sgml concept-vector.sgml concept-theory.sgml conclusion.sgml gif-coding.sgml gif.sgml imagemagick.sgml introduction.sgml jpeg.sgml pdf.sgml pdf-type-boolean.sgml pdf-type-name.sgml pdf-type-numbers.sgml pdf-type-string.sgml png.sgml svg.sgml template-figure.sgml template-install.sgml template-list.sgml template.sgml template-table.sgml tiff.sgml tutorial.sgml
	builddb eps < tutorial.sgml > output.sgml 2> output.log

slides.sgml: concept-antialias.sgml concept-ctm.sgml concept-encryption.sgml concept-grayscaleconvert.sgml concept-pixel.sgml concept-raster.sgml concepts.sgml concept-vector.sgml concept-theory.sgml conclusion.sgml gif-coding.sgml gif.sgml imagemagick.sgml introduction.sgml jpeg.sgml pdf.sgml pdf-type-boolean.sgml pdf-type-name.sgml pdf-type-numbers.sgml pdf-type-string.sgml png.sgml svg.sgml template-figure.sgml template-install.sgml template-list.sgml template.sgml template-table.sgml tiff.sgml tutorial.sgml
	builddb jpg < tutorial.sgml > slides.sgml 2> slides.log

output.ps: output.sgml
	db2ps output.sgml 2> errs

output.pdf: output.ps
	ps2pdf output.ps

slides: slides.sgml
	rm content/*-*
	slides < slides.sgml 2> slides-gen.log
#	cp *.jpg content/
#	cp -R content /home/httpd/html/notes/presentations/
#	cp slides-list /home/httpd/html/notes/presentations/slides-list

proposal: proposal.sgml
	db2ps proposal.sgml 2> errs
	striptext -c < proposal.sgml | wc -w

proposal.pdf: proposal.ps
	ps2pdf proposal.ps