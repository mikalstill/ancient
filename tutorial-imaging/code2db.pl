#!/usr/local/bin/perl

use strict;

# Make C code safe for display in docbook

while(<STDIN>){
    s/&/&amp;/g;
    s/</&lt;/g;
    s/>/&gt;/g;

    print $_;
}
