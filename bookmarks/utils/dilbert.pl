#!/usr/bin/perl

use LWP::Simple;

my $url  = "http://umweb2.unitedmedia.com/comics/dilbert/lotd/html/todays-lotd/lotd_1.html";
my $page = &get($url);

if ($page =~ m#^\s+\d+\s+(.*)</font></td>$#m) {
    print "<a href=\"$url\">$1</a>\n";
} else {
    print "Unable to fetch <a href=\"$url\">list</a>\n";
}
exit;
