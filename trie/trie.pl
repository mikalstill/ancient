#!/usr/bin/perl

use strict;
my($hashref, $INFILE, $lines, $line);

# Read in all the words
open INFILE, "< /usr/share/dict/words";
print "Reading in the system dictionary...\n";
$lines = 0;
while(<INFILE>){
    $lines++;
    chomp;
    $line = lc;

    $hashref = addword($hashref, $line);
    print "\n";
}
close INFILE;
print "Read $lines words\n";

dumphash($hashref, 0, "");


##########################
sub addword()
{
    my($hash_ref, $word) = @_;
    my($char, $subword);

    $char = substr($word, 0, 1);
    $subword = substr($word, 1, length($word));
    print "[$char, $subword]";
    
    if(length($char) == 0){
	$hash_ref->{"END"} = "END";
	print "[END]";
    }
    else{
	$hash_ref->{$char} = addword($hash_ref->{$char}, $subword);
    }

    return $hash_ref;
}

sub dumphash()
{
    my($hash_ref, $indent, $pre) = @_;
    my($key, $count);

    foreach $key (keys %$hash_ref){
	for($count = 0; $count < $indent; $count++){
	    print " ";
	}

	print "$key [$pre$key]\n";
	if($key ne "END"){
	    dumphash($hash_ref->{$key}, $indent + 1, "$pre$key");
	}
    }
}
