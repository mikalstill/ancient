#include <stdio.h>
#include <stdlib.h>
#include <cpdflib.h>

int
main (int argc, char *argv[])
{
  CPDFdoc *pdf;

  // Open the document
  pdf = cpdf_open (0, NULL);
  cpdf_init (pdf);
  
  // Create a page
  cpdf_pageInit (pdf, 1, PORTRAIT, LETTER, LETTER);

  // Put some text on that page
  cpdf_beginText (pdf, 0);
  cpdf_setFont (pdf, "Times-Italic", "WinAnsiEncoding", 48.0);
  cpdf_text (pdf, 1.0, 8.0, 0.0, "Hello World");
  cpdf_endText (pdf);

  // Write the PDF out to disc
  cpdf_finalizeAll (pdf);
  cpdf_savePDFmemoryStreamToFile (pdf, "hello.pdf");
  cpdf_close (pdf);
  return 0;
}
