#!/usr/local/bin/perl

use strict;
my($temp, $listmode);

# Make random textfiles safe for display in docbook (outside a programlisting
# block)

$listmode = "";
while(<STDIN>){
    s/&/&amp;/g;
    s/</&lt;/g;
    s/>/&gt;/g;

    # Wierd characters
    $temp = chr(12);            # ^L
    s/$temp/<\/para><para>/;
    
    # Blank lines and headings
    if(/^[ \t]+[0-9A-Za-z][\)\]\.]/){
	if($listmode eq ""){
	    print "<orderedlist>\n";
	    $listmode = 1;
	}
	else{
	    print "</para></listitem>";
	}
	
	s/[0-9A-Za-z][\)\]\.]/<listitem><para>/;
    }
    
    s/^[ \t]+/<\/para><para>/;
    s/^[ \t]*$/<\/para><para>/;

    print $_;
}
