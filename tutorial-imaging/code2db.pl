#!/usr/local/bin/perl

use strict;

# Make C code safe for display in docbook
print "<programlisting>\n";

while(<STDIN>){
    chomp;
    s/\r$//;

    s/&/&amp;/g;
    s/</&lt;/g;
    s/>/&gt;/g;

    print "$_\n";
}

print "</programlisting>\n";
print "<para><emphasis>Code: $ARGV[0]</emphasis></para>\n";
