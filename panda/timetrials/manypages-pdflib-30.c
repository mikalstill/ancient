/* hello.c
 * Copyright (C) 1997-2000 Thomas Merz. All rights reserved.
 * PDFlib client: hello example in C
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "pdflib.h"

int
main(void)
{
    PDF *p;
    int font, count;

    p = PDF_new();

    /* open new PDF file */
    if (PDF_open_file(p, "output.pdf") == -1) {
	fprintf(stderr, "Error: cannot open PDF file hello_c.pdf.\n");
	exit(2);
    }

    PDF_set_info(p, "Creator", "hello.c");
    PDF_set_info(p, "Author", "Thomas Merz");
    PDF_set_info(p, "Title", "Hello, world (C)!");

    font = PDF_findfont(p, "Helvetica-Bold", "host", 0);

    for(count = 0; count < 10000; count++){
      PDF_begin_page(p, a4_width, a4_height);	/* start a new page	*/
      PDF_setfont(p, font, 24);
      PDF_set_text_pos(p, 10, 10);
      PDF_show(p, "x");
      PDF_end_page(p);				/* close page		*/
    }

    PDF_close(p);				/* close PDF document	*/

    exit(0);
}
