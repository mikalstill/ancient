#!/usr/local/bin/perl

use strict;

while(<STDIN>){
  while(/(.*)%(.+)%(.*)/){
    $_ = "$1" . $ENV{$2} . "$3\n";
    }

  print "$_";
  }
