#include  <panda/functions.h>
#include  <panda/constants.h>

int
main (int argc, char *argv[])
{
  panda_pdf *demo;
  panda_page *currPage;

  // Initialise the library
  panda_init ();

  // Open our demo PDF
  if ((demo = panda_open ("hello.pdf", "w")) == NULL)
    {
      fprintf (stderr, "Could not open hello.pdf\n");
      exit (1);
    }

  // Create a page
  currPage = panda_newpage (demo, panda_pagesize_a4);

  // Write some text to the page
  panda_setfont (demo, panda_createfont (demo, "Times-Roman", 1,
  					 "MacRomanEncoding"));
  panda_textbox (demo, currPage, 10, 10, 200, 200, "Hello world");

  // Finished all the demoing, close the PDF document
  panda_close (demo);
  return 0;
}
