#!/usr/local/bin/perl

use strict;
my($firstsectnum, $codemode);

$firstsectnum = $ARGV[1] - 1;
if($firstsectnum eq ""){
    $firstsectnum = 1;
}

$codemode = 0;

while(<STDIN>){
    if(/<!DOCTYPE/){
    }
    elsif(/<HTML>/){
    }
    elsif(/<\/HEAD>/){
    }
    elsif(/Section: /){
    }
    else{
	if(/<pre>/i){
	    s/<pre>/<programlisting>/i;
	    $codemode = 1;
	}
	if(/<\/pre>/i){
	    s/<\/pre>/<\/programlisting>/i;
	    $codemode = 0;
	}
	
	if($codemode == 0){
	    s/<b>/<command>/i;
	    s/<\/b>/<\/command>/i;
	    s/<i>/<emphasis>/i;
	    s/<\/i>/<\/emphasis>/i;
	    s/<p>/<\/para><para>/i;
	}
	else{
	    s/<b>//i;
	    s/<\/b>//i;
	    s/<i>//i;
	    s/<\/i>//i;
	    s/<p>/<\/para><para>/i;
	}
	
	s/<\/p>//i;
	s/<hr>//i;
	s/<br>//i;
	s/<a[^>]*>[^<]*<\/a>//i;
	s/<font[^>]*>//i;
	s/<\/font>//i;
	s/&nbsp;//i;
	
	if(/<h([0-9])>(.*)<\/h[0-9]>/i){
	    print "<sect";
	    print ($1 + $firstsectnum);
	    print "><title>$2</title>\n";
	}
	else{
	    print $_;
	}
    }
}
