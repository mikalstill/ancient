#!/usr/local/bin/perl

use strict;
my($cmd, $cmdstr, $input, $output, $arg);

# Make sure local scripts can run
$ENV{PATH} = $ENV{PATH} . ":.";

# This script runs through the specified docbook source, and applies the commands in % blocks

while(<STDIN>){
    if(/(.*)<execute>(.*)<\/execute>(.*)/){
	print $1;
	$cmd = $2;

	$_ = $cmd;
	if(/<input>/){
	    $input = $cmd;
	    $input =~ s/.*<input>//;
	    $input =~ s/<\/input>.*//;
	}
	else{
	    $input = "";
	}

	if(/<output>/){
	    $output = $cmd;
	    $output =~ s/.*<output>//;
	    $output =~ s/<\/output>.*//;
	}
	else{
	    $output = "";
	}

	if(/<args>/){
	    $arg = $cmd;
	    $arg =~ s/.*<args>//;
	    $arg =~ s/<\/args>.*//;
	}
	else{
	    $arg = "";
	}

	$cmd =~ s/.*<cmd>//;
	$cmd =~ s/<\/cmd>.*//;

	if($cmd ne "todo"){
	    $cmdstr = "$cmd $arg";

	    if($input ne ""){
		$cmdstr = "$cmdstr < $input";
	    }

	    if($output ne ""){
		$cmdstr = "$cmdstr > $output";
	    }

	    system($cmdstr);
	}
	else{
	    print "<figure>\n";
	    print "<title>$input</title>\n";
	    print "<graphic format=\"EPS\" fileref=\"todo.eps\"></graphic>\n";
	    print "</figure>\n";
	}
	print "$3";
    }
    else{
	print $_;
    }
}
