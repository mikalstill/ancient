#!/usr/local/bin/perl

use strict;
my($cmd, $input, $arg);

# Make sure local scripts can run
$ENV{PATH} = $ENV{PATH} . ":.";

# This script runs through the specified docbook source, and applies the commands in % blocks

while(<STDIN>){
    if(/(.*)<execute>(.*)<\/execute>(.*)/){
	print STDERR "Executing $2...\n";
	print $1;
	$cmd = $2;
	$input = $2;
	$arg = $2;

	$cmd =~ s/.*<cmd>//;
	$cmd =~ s/<\/cmd>.*//;
	$input =~ s/.*<input>//;
	$input =~ s/<\/input>.*//;
	$arg =~ s/.*<args>//;
	$arg =~ s/<\/args>.*//;

	print STDERR "  Cmd: $cmd, Input: $input, Args: $arg\n";

	if($cmd ne "todo"){
#	    print `$cmd $input < $input`;
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
