#!/usr/bin/perl -w
#
# random.pl - Return a set number of random links from an HTML file
# ~~~~~~~~~   (c) Andrew Flegg 1999. Released under the Artistic Licence.

use strict;
my $NUMBER = 5;   # Number of links to return

my @lines;
while (<>) { push @lines, $_ if /<a /i; }

my %links;
my $pos;
my $max = (scalar(@lines) < $NUMBER ? scalar(@lines) : $NUMBER);

while (scalar(keys(%links)) < $max) {
    $pos = int((rand() * scalar(@lines)) + 1);
    $lines[$pos] =~ m!<a [^>]*href="([^"]*?)"[^>]*>(.*?)</a>!i;
    if (defined($1) && defined($2) && ($2 !~ m/\(P\)/)) { $links{$1} = $2; }
}

my $i;
foreach $i (keys(%links)) {
    print "&nbsp;<b>*</b>&nbsp;<a href=\"$i\">$links{$i}</a> - <small>[&nbsp;$i&nbsp;]</small><br>\n";
}

exit;
