/******************************************************************************
  Exercise

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      07012002
                     ... Code inserted as needed to test new functionality ...

  Purpose:
    This code is based on that found in main.c... It's main use is to help me
    find what area of the code certain kinds of memory leaks are in... This is
    done by changing the number of times areas of the code are exercised -- 
    thus hopefully changing the size of the leak.
******************************************************************************/

#include  <panda/functions.h>
#include  <panda/constants.h>

int
main (int argc, char *argv[])
{
  panda_pdf *demo;
  panda_page *currPage, *templatePage;
  int lineDepth, blockend = 0;
  char tempString[1024], *tempPtr;

  printf ("Welcome to the Panda 0.4 sample application...\n");

  // Initialise the library
  panda_init ();

  // Open our demo PDF
  if ((demo = panda_open ("output.pdf", "w")) == NULL)
    panda_error (panda_true, "demo: could not open output.pdf to write to.");

  ///////////////////////////////////////////////////////////////////////////
  // Demonstrate the supported lines and curve thingies -- note that no
  // graphics state is held from the previous set of lines, so you'll need
  // to rebuild it each time.
  ///////////////////////////////////////////////////////////////////////////

  currPage = panda_newpage (demo, panda_pagesize_a4);

  // Now some curves -- stroked
  panda_setlinestart (currPage, 200, 200);
  panda_addcubiccurvesegment (currPage, 300, 200, 225, 300, 275, 300);
  panda_strokeline (currPage);
  panda_endline (currPage);

  panda_setlinestart (currPage, 200, 200);
  panda_addquadraticcurvesegmentone (currPage, 300, 200, 200, 300);
  panda_strokeline (currPage);
  panda_endline (currPage);

  panda_setlinestart (currPage, 200, 200);
  panda_addquadraticcurvesegmenttwo (currPage, 300, 200, 300, 300);
  panda_strokeline (currPage);
  panda_endline (currPage);

  panda_setlinestart(currPage, 200, 200);
  panda_addlinesegment(currPage, 225, 300);
  panda_addlinesegment(currPage, 275, 300);
  panda_addlinesegment(currPage, 300, 200);
  panda_strokeline(currPage);
  panda_endline(currPage);

  panda_close (demo);

  // We should return a value here
  return 0;
}
