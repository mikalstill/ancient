#!/usr/local/bin/perl

use strict;

# Make C code safe for display in docbook

while(<STDIN>){
    chomp;
    s/\r$//;

    s/&/&amp;/g;
    s/</&lt;/g;
    s/>/&gt;/g;

    print "$_\n";
}
