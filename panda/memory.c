/******************************************************************************
  Memory.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      09012001

  Purpose:
    Abstract the out of memory tests so that calling functions can asusme that
    memory was returned by malloc et al.
******************************************************************************/

#include <stdio.h>
#include <panda/functions.h>
#include <panda/constants.h>

/******************************************************************************
DOCBOOK START

FUNCTION panda_xmalloc
PURPOSE allocate some memory

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void *panda_xmalloc (size_t size);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. Allocate some memory, performing all of the error checking so we don't have to elsewhere.

RETURNS A void * to the memory allocated.

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

char *memory;

memory = (char *) panda_xmalloc(42);
EXAMPLE END
DOCBOOK END
******************************************************************************/

void *
panda_xmalloc (size_t size)
{
  void *buffer;

  if ((buffer = malloc (size)) == NULL)
    {
      panda_error ("panda_xmalloc failed to allocate memory");
    }

  return buffer;
}
