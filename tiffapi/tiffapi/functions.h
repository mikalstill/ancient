/******************************************************************************
  Functions.h

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      29012001

  Purpose:
    Prototypes for all internal function calls...

******************************************************************************/

#ifndef TIFFAPI_FUNCTIONS_H
#define TIFFAPI_FUNCTIONS_H 1

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>

#if defined HAVE_LIBDMALLOC
#include <dmalloc.h>
#endif

/******************************************************************************
  Error.c
******************************************************************************/

  void tiffapi_error (char *);

/******************************************************************************
  Memory.c
******************************************************************************/

  void *tiffapi_xmalloc (size_t);

/******************************************************************************
  Tiffapi.c
******************************************************************************/

  char *tiffapi_readencodedstrips(tiff *);

#ifdef __cplusplus
}
#endif

#endif				/* TIFFAPI_FUNCTIONS_H */
