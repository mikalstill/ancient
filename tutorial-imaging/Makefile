tutorial: output.sgml output.ps
	striptext.pl -c < output.sgml | wc -w

output.sgml: concept-antialias.sgml concept-ctm.sgml concept-encryption.sgml concept-grayscaleconvert.sgml concept-pixel.sgml concept-raster.sgml concepts.sgml concept-vector.sgml concept-theory.sgml conclusion.sgml gif-coding.sgml gif.sgml imagemagick.sgml introduction.sgml jpeg.sgml pdf.sgml pdf-type-boolean.sgml pdf-type-name.sgml pdf-type-numbers.sgml pdf-type-string.sgml pdf-type-template.sgml png.sgml svg.sgml template-figure.sgml template-install.sgml template-list.sgml template.sgml template-table.sgml tiff.sgml tutorial.sgml
	builddb.pl < tutorial.sgml > output.sgml

output.ps: output.sgml
	db2ps output.sgml 2> errs

output.pdf: output.ps
	ps2pdf output.ps
