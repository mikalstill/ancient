#!/usr/bin/perl

use strict;

sub rmkdir{
  my($tpath) = @_;
  my($dir, $accum);

  foreach $dir (split(/\//, $tpath)){
    $accum = "$accum$dir/";
    if($dir ne ""){
      if(! -d "$accum"){
	mkdir $accum;
      }
    }
  }
}

