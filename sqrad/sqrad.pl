#!/usr/bin/perl

# Correlate radlast output with squid logs

use strict;
use Time::Local;

# todo: determine which of these are spare...
my($RADIUS, $SQUID, @cols, %ips, %unreg, $temp, $rec, @recarray, $linecount, $i, $key, 
   $found, %users, @hitrec, $line, @hm, @months, $month,
   %radtime, %raduser);

# Turn off output buffering
select((select(STDOUT), $| = 1)[0]);

$months[0] = "Jan";
$months[1] = "Feb";
$months[2] = "Mar";
$months[3] = "Apr";
$months[4] = "May";
$months[5] = "Jun";
$months[6] = "Jul";
$months[7] = "Aug";
$months[8] = "Sep";
$months[9] = "Oct";
$months[10] = "Nov";
$months[11] = "Dec";

# Read the radius log
print "Reading radius log:\n";
$linecount = 0;
open RADIUS, "< radlast";
while(<RADIUS>){
    s/[ \t]+/ /g;
    print STDERR "Radius: $_" if $ARGV[0] eq "-v";
    @cols = split(/ /, $_);

    # Determine which month (as a number) we are talking about here
    for ($i = 0; $i < @months; $i++) {
	if ($months[$i] eq $cols[4]) {
	    $month = $i;
	    last;
	}
    }
    print STDERR "  Month: $month\n" if $ARGV[0] eq "-v";

    # Convert the radius times into something compatible with squid
    @hm = split(/:/, $cols[6]);
    $temp = timelocal(0, $hm[1], $hm[0], $cols[5], $month, 2002);
    print STDERR "  Initial time ".$cols[5]."-".$month."-2002 ".$cols[6]." Munged time: $temp\n" if $ARGV[0] eq "-v";

    # If we have a user for this IP already, then they must have logged off
    if($raduser{$cols[2]} ne ""){
	# Format is ontime~offtime~user
	$ips{$cols[2]} = $radtime{$cols[2]}."~$temp~".$raduser{$cols[2]}.";".$ips{$cols[2]};
	print STDERR "  Logged off a user for this IP address\n" if $ARGV[0] eq "-v";
    }

    # Save this for next time
    $raduser{$cols[2]} = $cols[0];
    $radtime{$cols[2]} = $temp;

    $linecount++;
    if($linecount > 1000){
	print ".";
	$linecount = 0;
    }
}

# Add the people who never logged off
$temp = time();
foreach $key (keys %radtime){
    $ips{$cols[2]} = $radtime{$cols[2]}."~$temp~".$raduser{$cols[2]}.";".$ips{$cols[2]};
}

# Read the squid log
print "\nReading squid log\n";
$linecount = 0;
open SQUID, "< access.log";
while(<SQUID>){
    chomp;
    s/[ \t]+/ /g;
    @cols = split(/ /, $_);

    # Lookup the IP in the hash of connected users
    if($ips{$cols[2]} eq ""){
#	print "Unregistered: @cols\n";
	$unreg{$cols[2]}++;
    }
    else{
	$found = 0;
	foreach $rec (split(/;/, $ips{$cols[2]})){
	    @recarray = split(/~/, $rec);

	    if(($cols[0] >= $recarray[0]) && ($cols[0] < $recarray[1])){
		last;
	    }
	}
    }

    $linecount++;
    if($linecount > 1000){
	print ".";
	$linecount = 0;
    }
}
print "\n\n";



# Print out some simple statistics
print scalar(keys %ips)." unique IP addresses seen in the radius log\n";
print "$linecount lines of squid log processed\n";

print scalar(keys %unreg)." unique unregistered IP addresses seen\n";

