#!/usr/bin/perl

use strict;
my($INPUT, $one, $two, $three, $fc);

open INPUT, "< output.sgml" or die "Couldn't open output.sgml\n";

while(<INPUT>){
  while(/(.*)<emphasis>([^<]+)<\/emphasis>(.*)/){
    $one = $1;
    $two = $2;
    $three = $3;

    $two =~ s/ //g;
    $two = lc($two);
    $fc = substr($two, 0, 1);
    if ( ! -f "entry-$fc-$two.sgml" ){
      print "entry-$fc-$two.sgml\n";
    }

    $_ = $one.$three;
    }
  }
