#!/usr/local/bin/perl

use Data::Dumper;
use strict;

my(@array, %hash, $scalar);

$array[0] = "foo";
$array[1] = "bar";

$hash{"bob"} = "banana";
$hash{"carl"} = "chicken";
$hash{"walter"} = "wibble";

$scalar = 42;

# We could eval this output to get it back...
print Dumper([@array], {%hash}, $scalar);
