#!/usr/bin/perl

use strict;

while(<>){
  if(/<timestamp>.*<\/timestamp>/){
  }
  elsif(/<urb sequence=/){
    print "<urb>\n";
  }
  else{
    print $_;
  }
}
