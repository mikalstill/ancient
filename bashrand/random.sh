#!/bin/bash

# Generate a random number. Copyright (c) Michael Still 2002
# Released under the terms of the GNU GPL
#
# (Is it possible to copyright a single line of code?)

# To quote from the rand manpage as to why we bound the random number this way:
#
#       In Numerical Recipes in C: The Art of Scientific Computing
#       (William  H.  Press, Brian P. Flannery, Saul A. Teukolsky,
#       William T.  Vetterling;  New  York:  Cambridge  University
#       Press, 1992 (2nd ed., p. 277)), the following comments are
#       made:
#              "If you want to generate a random integer between 1
#              and 10, you should always do it by using high-order
#              bits, as in
#
#                     j=1+(int) (10.0*rand()/(RAND_MAX+1.0));
#
#              and never by anything resembling
#
#                     j=1+(rand() % 10);
#
#              (which uses lower-order bits)."

# To seed the random number generator, set RANDOM to a value... We can see
# that the bash code (2.05a in this case) already does some seeding for us...
#
# brand ()
# {
#  rseed = rseed * 1103515245 + 12345;
#  return ((unsigned int)((rseed >> 16) & 32767));       /* was % 32768 */
# }
#
# Here I have an example using the current time, which wont work well with
# multiple calls per second
#RANDOM=`date | tr -d ":" | cut -f 4 -d " "`

# These variables just make the equation easier to read, and are not needed
LOBOUND=1
HIBOUND=10
RANDMAX=32767
BINUMBER=$(( $LOBOUND + ($HIBOUND * $RANDOM) / ($RANDMAX + 1) ))

echo $BINUMBER
