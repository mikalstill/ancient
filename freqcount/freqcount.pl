#!/usr/bin/perl

# This little ditty gives a frequency count of the letters in 
# /usr/share/dict/words. This is useful for cryptography...
#
# Copyright (c) Michael Still 2002, released under the GNU GPL

use strict;
my($WORDS, %alphabet, $letter);

open WORDS, "< /usr/share/dict/words";
while($_ = getc(WORDS)){
    lc;
    $alphabet{$_}++ if(/[a-z]/);
}

foreach $letter (sort(keys %alphabet)){
    print $alphabet{$letter}.": $letter\n";
}
