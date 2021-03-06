#!/usr/local/bin/perl

use strict;
my($HISTORY, $filename, $date, %dateline, $day, $adate, @dates,
   $author, %cols, $newcolnum, @today, @html, $event, $file, $user, $i,
   $table, %authorcolor, @colors, $newauthnum, %heading, $color, $temp,
   $revision, $lines, $state, %authorcommits, $nowmonth, $prevmonth,
   @monthnames, $headerhtml, $loghtml, $nowyear);

open HISTORY, "< logout";

# We need some colors for authors -- I only bother to define 10 here
$colors[0] = "CC3399";
$colors[1] = "9900FF";
$colors[2] = "666699";
$colors[3] = "6699FF";
$colors[4] = "00FFCC";
$colors[5] = "66FF99";
$colors[6] = "33CC33";
$colors[7] = "99CC33";
$colors[8] = "663300";
$colors[9] = "FF6666";

# Some names for the months of the year
$monthnames[1] = "Jan";
$monthnames[2] = "Feb";
$monthnames[3] = "Mar";
$monthnames[4] = "Apr";
$monthnames[5] = "May";
$monthnames[6] = "Jun";
$monthnames[7] = "Jul";
$monthnames[8] = "Aug";
$monthnames[9] = "Sep";
$monthnames[10] = "Oct";
$monthnames[11] = "Nov";
$monthnames[12] = "Dec";

# Hoon through the CVS history
while(<HISTORY>){
    if(/^Working file: (.*)/){
	$filename = $1;
	chomp $filename;
    }
    elsif(/^date: ([0-9\/]+).*author: (.*);.*state: (.*);(.*)$/){
	$date = $1;
	$author = $2;
	$state = $3;
	$_ = $4;

	if(/.*lines: \+(.*) -(.*)/){
	    $lines = int($1) + int($2);
	    if($lines > 100){
		$lines = "*";
	    }
	    else{
		$lines = ".";
	    }
	}
	elsif($revision eq "1.1"){
	    $lines = "I";
	    $revision = "";
	    }
	else{
	    $lines = "&nbsp;";
	}

	if($state eq "dead"){
	    $lines = "D";
	}

	$date =~ s/\///g;
	$dateline{$date} = $dateline{$date} . "$filename!$author!$lines;";
    }
    elsif(/^revision (.*)/){
	$revision = $1;
    }
}

# Draw the graph
$newcolnum = 0;
$newauthnum = 0;
$table = "";
$prevmonth = "";

# For each date
@dates = sort keys %dateline;
foreach $adate (@dates) {
    for($i = 0; $i < $newcolnum; $i++){
	$html[$i] = "&nbsp;";
    }

    # If this is a new month, then we need to reset ourselves and draw a 
    # new header line
    $_ = $adate;
    /([0-9][0-9][0-9][0-9])([0-9][0-9])/;
    $nowmonth = $2;
    $nowyear = $1;
    if($nowmonth ne $prevmonth){
	if($prevmonth ne ""){
	    $table = $table . "<tr><td>&nbsp;</td></tr><tr><td><b>$monthnames[$prevmonth] $1</b></td>";
	    
	    # The funky header thingie
	    $headerhtml = "";
	    for($i = 0; $i < $newcolnum; $i++){
		$temp = $heading{$i};
		$temp =~ s/[\/\.]/_/g;
		if(! -e "colheader-$temp.jpg"){
		    `convert -font helvetica -pointsize 10 -draw "text 1,10 $heading{$i}" -rotate -90 blank.jpg colheader-$temp.jpg`;
		}
		$headerhtml = $headerhtml . 
		    "<td><img src=\"colheader-$temp.jpg\"></td>";
	    }
	    
	    $table = $table . $headerhtml . $loghtml;
	}
	$loghtml = "";
	$prevmonth = $nowmonth;
	$newcolnum = 0;
	%cols = ();
    }

    @today = split(/;/, $dateline{$adate});
    foreach $event (@today){
	$_ = $event;
	/(.*)!(.*)!(.*)/;
	$file = $1;
	$user = $2;
	$lines = $3;
	
	if($lines eq ""){
	    $lines = "&nbsp;";
	}

	# Do we need a new cell number?
	if($cols{$file} eq ""){
	    $cols{$file} = $newcolnum;
	    $heading{$newcolnum} = $file;
	    $newcolnum++;
	}

	# Do we need a new author color?
	if($authorcolor{$user} eq ""){
	    $authorcolor{$user} = $colors[$newauthnum];
	    $authorcommits{$user} = 0;
	    $newauthnum++;
	}

	# Increment the count of commits for this author
	$authorcommits{$user}++;

	# This could be cleaned up... Build the cell
	if($html[$cols{$file}] eq "&nbsp;"){
	    $html[$cols{$file}] = "";
	}

	if($html[$cols{$file}] eq ""){
	    $html[$cols{$file}] = 
		"<tr><td bgcolor=\"$authorcolor{$user}\">$lines</td></tr>";
	}
	else{
	    $html[$cols{$file}] = 
		"<tr><td bgcolor=\"$authorcolor{$user}\">$lines</td></tr>" .
		    $html[$cols{$file}];
	}
    }

    # This should be improved
    for($i = 0; $i < $newcolnum; $i++){
	$html[$i] = 
	    "<td valign=\"top\"><table width=\"100%\" cellspacing=0 cellpadding=0 bgcolor=\"000000\">" 
		. $html[$i] . "</table></td>";
    }

    $loghtml = $loghtml . "<tr><td valign=\"top\">$adate</td>@html</tr>\n";
}

# And the left over month
if($prevmonth ne ""){
    $table = $table . "<tr><td>&nbsp;</td></tr><tr><td><b>$monthnames[$prevmonth] $nowyear</b></td>";
    
    # The funky header thingie
    $headerhtml = "";
    for($i = 0; $i < $newcolnum; $i++){
	$temp = $heading{$i};
	$temp =~ s/[\/\.]/_/g;
	if(! -e "colheader-$temp.jpg"){
	    `convert -font helvetica -pointsize 10 -draw "text 1,10 $heading{$i}" -rotate -90 blank.jpg colheader-$temp.jpg`;
	}
	$headerhtml = $headerhtml . 
	    "<td><img src=\"colheader-$temp.jpg\"></td>";
    }
    
    $table = $table . $headerhtml . $loghtml;
}

print "<table cellspacing=0 cellpadding=0>";
print "$table</table>";
print "<BR><HR><BR>Authors: ";
print "<table><tr>";

while(($user, $color) = each %authorcolor){
    print "<td bgcolor=\"$color\">$user: ";
    print `grep $user authors | sed 's/^$user://'`;
    print " ($authorcommits{$user})";
    print "</td>";
}

print "</tr></table>";
print "<BR>Key: ";
print "<table><tr><td>I: Initial import of a file</td>";
print "<td>D: File deleted from repository</td>";
print "<td>.: Minor code change (less than 100 lines)</td>";
print "<td>*: Major code change (more than 100 lines)</td>";
print "</tr></table>";

print "<BR><BR>END OF REPORT";
