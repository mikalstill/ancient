#!/usr/bin/perl

use strict;

while(<>){
  s/\n//g;
  s/\r//g;
  if(/^[ \t]*$/){
  }
  else{
    print "$_\n";
  }
}
