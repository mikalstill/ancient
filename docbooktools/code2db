#!/usr/local/bin/perl

use strict;
my($suppress);

$suppress = 0;

# Make C code safe for display in docbook
print "<programlisting>\n";

while(<STDIN>){
    chomp;
    if(/^[ \t]*\/\*\*\*\*\*\*\*\*\*/){
	$suppress = 1;
    }
    elsif($suppress == 0){
	s/\r$//;
	
	s/&/&amp;/g;
	s/</&lt;/g;
	s/>/&gt;/g;
	
	print "$_\r\n";
    }

    if(($suppress == 1) && (/\*\*\*\*\*\*\*\*\*\/$/)){
	$suppress = 0;
    }
}

print "</programlisting>\n";
print "<para><emphasis>Code: $ARGV[0]</emphasis></para>\n";
