#!/usr/local/bin/perl

use strict;
my(@args);

# This script runs through the specified docbook source, and applies the commands in % blocks

while(<STDIN>){
    if(/(.*)%([^ \t]*)%(.*)/){
	print $1;
	@args = split(/:/, $2);
	print `$args[0] < $args[1]`;
	print "$3";
    }
    else{
	print $_;
    }
}
