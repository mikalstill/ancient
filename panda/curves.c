/******************************************************************************
  curves.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      18012001

  Purpose:
    This file looks after the calls to draw lines and curves onto a PDF page.
    It also deals with things like the line ending styles and good stuff like
    that. Many of these calls are internal, and are abstracted for real use
    later in this file...
******************************************************************************/

#include <panda/constants.h>
#include <panda/functions.h>

// Set the start point of a line on the page
void
panda_setlinestart (panda_page * target, int x, int y)
{
  panda_entergraphicsmode(target);
  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream,
			"%d %d m\n", x, target->height - y);
}

// Add a point to the line that we are drawing (a straight line segment)
void
panda_addlinesegment (panda_page * target, int x, int y)
{
  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream,
			"%d %d l\n", x, target->height - y);
}

// Add a curved segment to the line that we are building
void
panda_addcubiccurvesegment (panda_page * target, int x, int y, int cx1,
			    int cy1, int cx2, int cy2)
{
  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream,
			"%d %d %d %d %d %d v\n", cx1, target->height - cy1, 
			cx2, target->height - cy2, x, target->height - y);
}

// Add a different type of curved segment
void
panda_addquadraticcurvesegmentone (panda_page * target, int x, int y, int cx1,
				 int cy1)
{
  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream,
			"%d %d %d %d v\n", cx1, target->height - cy1, 
			x, target->height - y);
}

// Add a different type of curved segment
void
panda_addquadraticcurvesegmenttwo (panda_page * target, int x, int y, int cx1,
				 int cy1)
{
  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream,
			"%d %d %d %d y\n", cx1, target->height - cy1, 
			x, target->height - y);
}

// Return to the start point of the line to make some
void
panda_closeline (panda_page * target)
{
  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream, "h\n");
}

// Add a rectangle from the current cursor point
void
panda_rectangle (panda_page * target, int top, int left, int bottom, 
		 int right)
{
  target->contents->layoutstream = 
    panda_streamprintf (target->contents->layoutstream,
			"%d %d %d %d re\n", left, target->height - top,
			right - left, top - bottom);
}

// Make the line be drawn by the viewer
void
panda_endline (panda_page * target)
{
  panda_exitgraphicsmode(target);
}

// Stroke the path that we have just described
// You say "you've never had a mum and nobody needs you"; So cry; So cry
//                                                    -- Vanessa Amarosi, Shine
void
panda_strokeline (panda_page * target)
{
  // There is also an option to close and stoke, but I see no reason to
  // implement it...
  target->contents->layoutstream = 
    panda_streamprintf (target->contents->layoutstream,
			"S\n");
}  

// Stroke the path that we have just described
// Now that we are through, I realise that I want you
//                                              -- Vanessa Amarosi, Have a Look
void
panda_fillline (panda_page * target)
{
  // There is another instance of this command, but the spec recommends not
  // implementing it...
  target->contents->layoutstream = 
    panda_streamprintf (target->contents->layoutstream,
			"f\n");
}  
