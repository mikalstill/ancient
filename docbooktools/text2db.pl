#!/usr/local/bin/perl

use strict;
my($line, $prevline, $listmode, $paramode, $ctr_l);

# Make random textfiles safe for display in docbook (outside a programlisting
# block)

$paramode = 1;
$listmode = 0;

$ctr_l = chr(12);
print "<para>";

while(<STDIN>){
    chomp;

    s/&/&amp;/g;
    s/</&lt;/g;
    s/>/&gt;/g;
    s/\t//g;
    s/ +/ /g;
    s/^ //;

    $line = $_;
    $prevline = "NOSUCHLINEFOOFOOFOO";

    while($line ne $prevline){
	$_ = $line;

	# Forcing a new page
	if(/$ctr_l/){
	    if($paramode == 1){
		$line =~ s/$ctr_l/<\/para><beginpage>/;
	    }
	    else{
		$line =~ s/$ctr_l/<beginpage>/;
	    }
	}

	# Blank lines end paragraphs
	elsif(length($line) < 2){
	    if($paramode == 1){
		$line = "</para>";
		$paramode = 0;
	    }
	    if($listmode == 1){
		$line = "$line</listitem>";
		$listmode = 0;
	    }
	}

	# We deal with lines starting with a number and then a fullstop as a 
        # list entry (only if we don't have a paragraph open)
	elsif(($paramode == 0) && (/^[0-9]\./)){
	    if($listmode == 0){
		$line = "<itemizedlist>$line";
	    }
	    
	    $line =~  s/[0-9]\./<listitem><para>/;
	    $listmode = 1;
	}

	# If this is a line with text, then we need to start a para sometimes
	elsif(($paramode == 0) && (/^[^<]/)){
	    $line = "<para>$line";
	    $paramode = 1;
	}

	$prevline = $line;
    }

    print "$line\n";
}
