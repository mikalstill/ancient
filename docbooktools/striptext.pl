#!/usr/bin/perl

use strict;
my($INPUT);

open INPUT, "< $ARGV[0]";
while(<INPUT>){
    s/<[^<>]*>//g;
    s/[^>]*>//g;
    s/<[^<]*//g;

    printf $_;
}
