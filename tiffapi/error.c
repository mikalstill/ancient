/******************************************************************************
  Error.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      03062000

  Purpose:
    Handle errors cleanly.
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <tiffapi/functions.h>

/******************************************************************************
  Handle an error.
******************************************************************************/

// You can bang or head or you can drown in a hole
//                                                    -- Vanessa Amarosi, Shine
void
tiffapi_error (char *message)
{
  fprintf (stderr, "%s\n", message);
  exit (42);
}
