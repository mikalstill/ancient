#!/usr/bin/perl

use strict;
my($stripcode, $codemode);

$stripcode = 1;
$codemode = 0;

while(<STDIN>){
    if($stripcode == 1){
	s/.*<programlisting>.*<\/programlisting>//g;
	if(/<programlisting>.*/){
	    s/<programlisting>.*//;
	    $codemode = 1;
	    
	    s/<[^<>]*>//g;
	    s/[^>]*>//g;
	    s/<[^<]*//g;
	    print "$_\n";
	}

	if(/.*<\/programlisting>.*/){
	    s/.*<\/programlisting>//;
	    $codemode = 0;
	}
    }

    s/<[^<>]*>//g;
    s/[^>]*>//g;
    s/<[^<]*//g;

    if($codemode == 0){
	print $_;
    }
}
