#!/usr/local/bin/perl

use strict;
my(@args);

# Make sure local scripts can run
$ENV{PATH} = $ENV{PATH} . ":.";

# This script runs through the specified docbook source, and applies the commands in % blocks

while(<STDIN>){
    if(/(.*)%([^ \t]*)%(.*)/){
	print $1;
	@args = split(/:/, $2);
	if($args[0] ne "todo"){
	    print `$args[0] < $args[1]`;
	}
	else{
	    print "<figure>\n";
	    print "<title>$args[1]</title>\n";
	    print "<graphic format=\"EPS\" fileref=\"todo.eps\"></graphic>\n";
	    print "</figure>\n";
	}
	print "$3";
    }
    else{
	print $_;
    }
}
