#!/usr/local/bin/perl

use strict;
my($line, $prevline, $listmode, $listitemmode, $paramode, $ctr_l, $subst, $sectnum, $firstsectnum, $temp, $codemode);

# Make random textfiles safe for display in docbook (outside a programlisting
# block)

$firstsectnum = $ARGV[1] - 1;
if($firstsectnum eq ""){
    $firstsectnum = 1;
}

$paramode = 1;
$listmode = 0;
$codemode = 0;
$listitemmode = 0;
$sectnum = $firstsectnum;

$ctr_l = chr(12);
print "<para>";

while(<STDIN>){
    chomp;

    s/&/&amp;/g;
    s/</&lt;/g;
    s/>/&gt;/g;
    s/\t//g;

    if(($ARGV[2] eq "y") && (/^    [^ ]/)){
	s/^    /%%%%/;
    }

#    s/ +/ /g;
#    s/^ //;

    $line = $_;
    $prevline = "NOSUCHLINEFOOFOOFOO";

    while($line ne $prevline){
	$_ = $line;

	# Forcing a new page
	if(/$ctr_l/){
	    $subst = "";
	    if($paramode == 1){
		$subst = "<\/para>";
		$paramode = 0;
	    }
	    if($listitemmode == 1){
		$subst = "$subst<\/listitem>";
		$listitemmode = 0;
	    }
	    if($listmode == 1){
		$subst = "$subst<\/itemizedlist>";
		$listmode = 0;
	    }

	    $line =~ s/$ctr_l/$subst<beginpage>/;
	}

	# Blank lines end paragraphs
	elsif(length($line) < 2){
	    if($paramode == 1){
		$line = "</para>";
		$paramode = 0;
	    }
	    elsif($codemode == 1){
		$line = "</programlisting>";
		$codemode = 0;
	    }
	}

	# The libpng.txt file uses I. FOO as a heading format, as does the OPL
	elsif(/^([IVX]+)\. (.*)/){
	    $temp = "\n<sect".($firstsectnum + 1)."><title>$1. $2</title>";
	    
	    if($paramode == 1){
		print "</para>\n";
		$paramode = 0;
	    }
	    
	    if($listitemmode == 1){
		print "</listitem>";
		$listitemmode = 0;
	    }
	    
	    if($listmode == 1){
		print "</itemizedlist>";
		$listmode = 0;
	    }
	    
	    if($sectnum != $firstsectnum){
		print "</sect$sectnum>\n";
	    }

	    $sectnum = $firstsectnum + 1;
	    print $temp;
	    $line = "";
	}

	# We deal with lines starting with a number and then a fullstop as a 
        # list entry (only if we don't have a paragraph open)
	elsif(/^[0-9A-Z]{1,3}[\.\)]/){
	    $line = "<listitem><para>$line";

	    if($listmode == 0){
		$line = "<itemizedlist>$line";
	    }

	    if($listitemmode == 1){
		$line = "</listitem>$line";
	    }
	    
	    if($paramode == 1){
		$line = "</para>$line";
	    }

	    $listmode = 1;
	    $listitemmode = 1;
	    $paramode = 1;
	}

	# If this is a line with text, then we need to start a para sometimes
	elsif(($paramode == 0) && (/^[^<%]/) && ($codemode == 0)){
	    $line = "<para>$line";
	    $paramode = 1;
	}
	elsif((/^%%%%/) && ($codemode == 0)){
	    $line =~ s/%%%%/<programlisting>\n    /;
	    $codemode = 1;
	}
	elsif(/^%%%%/){
	    $line =~ s/^%%%%/    /;
	}

	$prevline = $line;
    }

    if($codemode == 1){
	chomp $line;
	print "$line\r\n";
    }
    else{
	print "$line\n";
    }
}

if($paramode == 1){
    print "</para>\n";
}

if($listitemmode == 1){
    print "</listitem>";
}

if($listmode == 1){
    print "</itemizedlist>";
}
