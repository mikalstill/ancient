#!/usr/bin/perl

# Correlate radlast output with squid logs

use strict;
my($RADIUS, $SQUID, @cols, %ips, %unreg, $temp, $rec, $linecount, 
   $found, $toomany, $toofew, $justright, %users, @hitrec, $line);

# Turn off output buffering
select((select(STDOUT), $| = 1)[0]);

# Read the radius log
open RADIUS, "< radlast";
while(<RADIUS>){
    s/[ \t]+/ /g;
    @cols = split(/ /, $_);

    $ips{$cols[2]} = "$cols[3] $cols[4] $cols[5] $cols[6]~$cols[8]~$cols[0];".$ips{$cols[2]};
}
print "Finished reading the radius log\n";

# Read the squid log
$toomany = 0;
$toofew = 0;
$justright = 0;
$linecount = 0;
open SQUID, "< access.log";
while(<SQUID>){
    chomp;
    s/[ \t]+/ /g;
    $line = $_;
    @cols = split(/ /, $_);

    # Lookup the IP in the hash of connected users
    if($ips{$cols[2]} eq ""){
#	print "Unregistered: @cols\n";
	$unreg{$cols[2]}++;
    }
    else{
#	$line =~ s/^\d+\.\d+/localtime $&/e;
	@cols = split(/ /, $line);	
	$temp = "$cols[0] $cols[1] $cols[2] $cols[3]";
	$temp =~ s/:[^:]*//;

	$found = 0;
	foreach $rec (split(/;/, $ips{$cols[6]})){
#	    $found += inrange($rec, $temp);
	    if($found == 1){
		@hitrec = split(/~/, $rec);
	    }
	    elsif($found == 2){
		# Exit the loop (for efficiency)
		last;
	    }
	}
	
	if($found == 1){
	    $users{$hitrec[2]} = "$cols[8] $cols[9];".$users{$hitrec[2]};
	    $justright++;
	}
	elsif($found > 1){
#	    print "Too many: @cols\n";
	    $toomany++;
	}
	else{
#	    print "Too few: @cols\n";
	    $toofew++;
	}
    }

    $linecount++;
    if($linecount % 1000 == 0){
	print ".";
    }
}
print "\n\n";



# Print out some simple statistics
print scalar(keys %ips)." unique IP addresses seen in the radius log\n";
print "$linecount lines of squid log processed\n";

print scalar(keys %unreg)." unique unregistered IP addresses seen\n";
print "$toofew squid log entries which didn't match a logged on user\n";
print "$toomany squid log entries matched more than one user\n";
print "$justright squid log entries matched just one user\n";


#############################################################################

# Is this number in the range specified
sub inrange(){
    my($range, $sample) = @_;
    my(@rcol, @dtime, $startmin, $stopmin, $checkmin);

    @rcol = split(/ /, $range);
    @dtime = split(/ /, $sample);
    if("$rcol[0] $rcol[1] $rcol[2]" eq "@dtime[0] $dtime[1] $dtime[2]"){
	@rcol = split(/~/, $rcol[3]);
	
	# Convert the times into minutes
	@dtime = split(/:/, $rcol[0]);
	$startmin = $dtime[0] * 60 + $dtime[1];
	
	@dtime = split(/:/, $rcol[1]);
	$stopmin = $dtime[0] * 60 + $dtime[1];
	
	@dtime = split(/ /, $sample);
	@dtime = split(/:/, $dtime[3]);
	$checkmin = $dtime[0] * 60 + $dtime[1];
	
	# Now we can check the range
	if(($checkmin >= $startmin) && ($checkmin <= $stopmin)){
	    return 1;
	}
    }

    return 0;
}
