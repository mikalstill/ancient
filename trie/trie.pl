#!/usr/local/bin/perl

# A simple example of a hash of hashes -- so I know how to do it for
# objectify... This implements a trie...

use strict;

my(%phash, %chash, %nhash, $INFILE, $lines, $line, $letter, $ref);

#$chash{"foo"} = "bar";
#$phash{"wibble"} = \%chash;

%phash = ();
$phash{"END"} = "aval";
dumphash(%phash);
exit;

# Go through the system dictionary
open INFILE, "< /usr/share/dict/words";
print "Reading in the system dictionary...\n";
$lines = 0;
while(<INFILE>){
    $lines++;
    chomp;
    $line = lc;
    %chash = %phash;

    # We go through each character in the word, and build it's item in the trie
    while(length($line) > 0){
	$letter = substr($line, 0, 1);

	# Get me a reference to the hash I am adding to
	if(exists $chash{$letter}){
	    $ref = $chash{$letter};	
	}
	else{
	    $chash{$letter}{"banana"} = "foo";
	}
	
	if(!exists $chash{$letter}){
	    die "Could not add sub hash\n";
	}

	# If the length is one, then add a terminal
	    

	# Move on to the next letter
	$ref = $chash{$letter};
	%chash = %$ref;
	$line = substr($line, 1);
    }
}
close INFILE;
print "Read $lines words\n";
dumphash(%phash);
exit;

sub dumphash{
    my(%hash, $depth) = @_;
    my($key, $ref, $count);
    
    foreach $key (keys %hash){
	for($count = 0; $count < $depth; $count++){
	    print " ";
	}

	print "$key: ";
	if($key ne "END"){
	    $ref = $hash{$key};
	    dumphash(%$ref, $depth + 1);
	}
	else{
	    print $hash{$key};
	}
	print "\n";
    }
}
