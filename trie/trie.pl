#!/usr/local/bin/perl

# A simple example of a hash of hashes -- so I know how to do it for
# objectify... This implements a trie...

use strict;

my(%phash, $INFILE, $lines, $line, $letter, $ref);

%phash = ();

# Go through the system dictionary
open INFILE, "< words";    # /usr/share/dict/words";
print "Reading in the system dictionary...\n";
$lines = 0;
while(<INFILE>){
    $lines++;
    chomp;
    $line = lc;
    addword($line, %phash);
}
close INFILE;
print "Read $lines words\n";
dumphash(%phash);
exit;

sub addword{
    my($line, %hash) = @_;

    print "\n\"$line\" -- ";

    if(length($line) == 0){
	print "[Terminal]";
	$hash{"END"} = "END";
    }
    else{
	print "Adding ".substr($line, 0, 1);
	if(!exists $hash{substr($line, 0, 1)}){
	    print " [New]";
	    $hash{substr($line, 0, 1)} = ();
	}

	addword(substr($line, 1), $hash{substr($line, 0, 1)});
    }
}

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
