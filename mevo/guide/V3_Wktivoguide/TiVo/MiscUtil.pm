#!/usr/bin/perl
# 
# MiscUtil.pm: Miscellaneous Perl routines used in Wktivoguide.
#
# Copyright (c) Warren Toomey, wkt@tuhs.org, 2002/2003/2004
#
#    $Revision: 1.1.1.1 $
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 0. This software must only be used in those parts of the world which
#    cannot obtain a program subscription service from TiVo, Inc.
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. All advertising materials mentioning features or use of this software
#    must display the following acknowledgement:
#      This product includes software developed by Warren Toomey.
# 4. Warren Toomey's name may not be used to endorse or promote products
#    derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY WARREN TOOMEY ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL WARREN TOOMEY BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
package TiVo::MiscUtil;
use strict;
use Time::Local;

BEGIN {
  use Exporter	 ();
  use vars	 qw($VERSION @ISA @EXPORT @EXPORT_OK %EXPORT_TAGS);

  $VERSION = do { my @r = (q$Revision: 1.1.1.1 $ =~ /\d+/g); sprintf "%d."."%02d" x $#r, @r }; # must be all one line, for MakeMaker

  @ISA	       = qw(Exporter);
  @EXPORT      = qw(Rehtml Calc_Off Get_Daynumber Get_Dayname
		    Get_Foxtel_Dayname
		    Open_Debug_File Debug LowercaseString
		    ZoneConvert Read_IntermediateFile
		    Write_IntermediateFile Strlensort
		    @ProgramData);
  %EXPORT_TAGS = ( );
  @EXPORT_OK	= qw(@ProgramData);
}

use vars	 @EXPORT_OK;
END { }	 # module clean-up code here (global destructor)


#####################################
###### Package-private Globals ######
#####################################

my ($DebuggingOn)=0;
my (@Month) = (	"January", "February", "March", "April", "May",
		"June", "July", "August", "September",
		"October", "November", "December" );
my (@Weekday) = ( "Sunday", "Monday", "Tuesday", "Wednesday",
		  "Thursday", "Friday", "Saturday" );

my (%Fileprefix) = (
	"description" => "Description",
	"episode" => "Episode",
	"year" => "Year",
	"genre" => "Genres",
	"episodic" => "Episodic",
	"advisory" => "Advisory",
	"actor" => "Actor",
	"director" => "Director",
	"bits" => "Bits",
	"starrating" => "StarRating",
	"tvrating" => "TvRating" );

######################
###### Routines ######
######################

# Procedure Rehtml:
# Take the given string, and rewrite it so that there is either 0 or 1
# html tags per line. Save as a temporary file named $out somewhere.
#
sub Rehtml($$) {
  my ($body, $out)= @_;
  my ($oldrs)= $/;

  open(REHOUT, ">$out") || die("Cannot create $out: $!\n");

  $oldrs= $/; undef $/; $_= $body;		# Read the entire file

  s/\r//g;					# Remove carriage returns
  s/\n/ /g;					# Newlines become spaces
  s/</\n</g;					# Put newlines before tags
  s/>/>\n/g;					# Put newlines after tags
  print(REHOUT);
  close(REHOUT); $/=$oldrs;
}


# Calculate the offset in seconds that we are from UTC.
# This code taken from the Time::Timezone Perl package,
# written by Graham Barr <bodg@tiuk.ti.com>,
# David Muir Sharnoff <muir@idiom.com> and
# Paul Foley <paul@ascent.com>
#
sub Calc_Off($)
{
	my ($time) = $_[0];
	my (@l) = localtime($time);
	my (@g) = gmtime($time);
	my $off;

	$off =	   $l[0] - $g[0]
		+ ($l[1] - $g[1]) * 60
		+ ($l[2] - $g[2]) * 3600;

	# subscript 7 is yday.

	if ($l[7] == $g[7]) {
		# done
	} elsif ($l[7] == $g[7] + 1) {
		$off += 86400;
	} elsif ($l[7] == $g[7] - 1) {
		$off -= 86400;
	} elsif ($l[7] < $g[7]) {
		# crossed over a year boundry!
		# localtime is beginning of year, gmt is end
		# therefore local is ahead
		$off += 86400;
	} else {
		$off -= 86400;
	}
	return $off;
}

# Given a daynumber relative to today (0, 1, 2 etc.), return the
# daynumber in TiVo terms
sub Get_Daynumber($)
{
  my ($wanteday)= $_[0];
  my ($dayseconds, $offset_from_utc, $daynum);

  # Get information about the requested date plus the day offset
  #
  $dayseconds= time(); $offset_from_utc= Calc_Off($dayseconds);
  $dayseconds += 86400 * $wanteday;

  # Guess, this day's number is the dayseconds / 86400;
  # We need to add on our offset from UTC to make sure
  # we find the correct day number!
  $daynum = int(($dayseconds + $offset_from_utc) / 86400);
  return($daynum);
}

# Given a daynumber relative to today (0, 1, 2 etc.), return the
# dayname as a string.
sub Get_Dayname($)
{
  my ($wanteday)= $_[0];
  my ($dayseconds, $offset_from_utc, $daynum, $dayname);
  my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst);

  # Get information about the requested date plus the day offset
  #
  $dayseconds= time(); $offset_from_utc= Calc_Off($dayseconds);
  $dayseconds += 86400 * $wanteday;

  # Guess, this day's number is the dayseconds / 86400;
  # We need to add on our offset from UTC to make sure
  # we find the correct day number!
  $daynum = int(($dayseconds + $offset_from_utc) / 86400);

  # Get the day of month, month of year and day of week of the wanted day
  #
  ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst)= localtime($dayseconds);

  # We want a dayname_day_month value
  #
  $dayname= "$Weekday[$wday]" . "_$mday" . "_$Month[$mon]";
  return($dayname);
}


# Given a daynumber relative to today (0, 1, 2 etc.), return the
# dayname as a Foxtel string.
sub Get_Foxtel_Dayname($)
{
  my ($wanteday)= $_[0];
  my ($dayseconds, $offset_from_utc, $daynum, $dayname);
  my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst);
      
  # Get information about the requested date plus the day offset
  #
  $dayseconds= time(); $offset_from_utc= Calc_Off($dayseconds);
  $dayseconds += 86400 * $wanteday;
 
  # Guess, this day's number is the dayseconds / 86400;
  # We need to add on our offset from UTC to make sure
  # we find the correct day number!
  $daynum = int(($dayseconds + $offset_from_utc) / 86400);
  
  # Get the day of month, month of year and day of week of the wanted day
  #
  ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst)= localtime($dayseconds);

  # We want a dd/mm/yy format.
  $mon++; $year%= 100;
  $dayname= sprintf("%02d/%02d/%02d", $mday, $mon, $year);
  return($dayname);
}


sub Open_Debug_File($) {
  my ($file)= $_[0];

  open(DEBUG, ">$file") || return;
  $DebuggingOn=1;
}

sub Debug($) {
  my ($line)= $_[0];

  if ($DebuggingOn==1) {
    print(DEBUG $line);
  }
}

# Convert all letters to lowercase except the start of each word.
# Do this for each line in the input array.
sub LowercaseString(@) {
  my($input, $w, $result);
  my(@ResultAry, @word);

  foreach $input (@_) {
    @word= split(/\s+/, $input);
    $result="";

    foreach $w (@word) {
      $w= lc($w); $w= ucfirst($w);
      if ($result eq "") { $result=$w; }
      else { $result= $result . " " . $w; }
    }
    push(@ResultAry, $result);
  }
  return(@ResultAry);
}


# Convert a time from one timezone
# to another. Args are:
#   daynum in days in zone 1, Unix epoch
#   time since midnight in seconds in zone 1, Unix epoch
#   zone 1 name, e.g Australia/Sydney
#   zone 2 name, e.g Australia/Brisbane
# Result is new (daynum,time) in zone 2, Unix epoch
#
sub ZoneConvert($$$$) {
  my($daynum1,$daysecs1,$zone1,$zone2)=@_;
  my($sec,$min,$hours,$mday,$mon,$year,$wday,$yday,$isdst);
  my($orig_time,$time1,$time2,$out_time,$daynum2,$daysecs2,$diff);

  $orig_time= $daynum1 * 86400 + $daysecs1;
  
  # We convert to gmtime here because the given time is alway
  # the localtime on the TiVo, i.e. when $daysecs1==0 it's midnight

  ($sec,$min,$hours,$mday,$mon,$year,$wday,$yday,$isdst) = gmtime($orig_time);
  #print("Zone1 is $year/$mon/$mday $hour:$min:$sec\n");

  # Now convert this back into seconds for both timezones
  $ENV{TZ}=$zone1;
  $time1 = timelocal($sec,$min,$hours,$mday,$mon,$year);
  $ENV{TZ}=$zone2;
  $time2 = timelocal($sec,$min,$hours,$mday,$mon,$year);

  # Work out the difference between the timezones
  $diff=$time1-$time2;
  $out_time= $orig_time + $diff;

  # And convert back into TiVo day/seconds format
  $daynum2= int($out_time / 86400);
  $daysecs2= $out_time % 86400;
  return($daynum2,$daysecs2);
}

# ReadIntermediateFile: Read each record from an
# intermediate file and store it into an array
# called @ProgramData. Returns 1 if ok, 0 on error.
#
sub Read_IntermediateFile($)
{
  my ($infile)= @_;
  my ($k, $l, $index);

  unless (open(IN, $infile)) {
    return(0);
  }
  undef(@ProgramData); $index=0;
  
  # Read the whole file
  while (!eof(IN)) {

    # Read a single program's data until a blank line
    while (<IN>) {
      chop;
      # Change stupid characters to sane ones
      y/\x85\x91\x92\x93\x94\x96\xe9/ ''"":e/;
      y/\x80-\xff//d;

      if (/^Title:\s*(.*)/)       { $ProgramData[$index]{title}=$1; next; }
      if (/^Time:\s*(.*)/)        { $ProgramData[$index]{time}=$1; next; }
      if (/^Daynum:\s*(.*)/)      { $ProgramData[$index]{daynum}=$1; next; }
      if (/^Duration:\s*(.*)/)    { $ProgramData[$index]{duration}=$1; next; }
      if (/^Genres:\s*(.*)/)      { $ProgramData[$index]{genre}=$1; next; }
      if (/^Episodic:\s*(.*)/)    { $ProgramData[$index]{episodic}=$1; next; }
      if (/^Episode:\s*(.*)/)     { $ProgramData[$index]{episode}=$1; next; }
      if (/^Description:\s*(.*)/) { $ProgramData[$index]{description}=$1; next; }
      if (/^TvRating:\s*(.*)/)    { $ProgramData[$index]{tvrating}=$1; next; }
      if (/^Bits:\s*(.*)/)        { $ProgramData[$index]{bits}=$1; next; }
      if (/^Actor:\s*(.*)/)       { $ProgramData[$index]{actor}=$1; next; }
      if (/^Director:\s*(.*)/)    { $ProgramData[$index]{director}=$1; next; }
      if (/^Advisory:\s*(.*)/)    { $ProgramData[$index]{advisory}=$1; next; }
      if (/^StarRating:\s*(.*)/)  { $ProgramData[$index]{starrating}=$1; next; }
      if (/^Year:\s*(.*)/)        { $ProgramData[$index]{year}=$1; next; }
      if (/^$/)                   { last; }
    }

    # Trim unwanted stuff on the end of the title
    # We do full-stops, whitespace and stuff after (
    $ProgramData[$index]{title}=~ s/\s*$//;
    $ProgramData[$index]{title}=~ s/\.*$//;
    $ProgramData[$index]{title}=~ s/\s*\(.*//;

    # Debug("Title after trim is now $ProgramData[$index]{title}\n");

    # Some things are mandatory, give errors if they are missing
    if (!defined($ProgramData[$index]{title})) {
      Debug("Skipping record in $infile: no program title\n"); next;
    }
    if (!defined($ProgramData[$index]{daynum})) {
      Debug("Skipping $ProgramData[$index]{title} in $infile: no day number\n");
      next;
    }
    if (!defined($ProgramData[$index]{time})) {
      Debug("Skipping $ProgramData[$index]{title} in $infile: no time\n");
      next;
    }
    if (!defined($ProgramData[$index]{duration})) {
      Debug("Skipping $ProgramData[$index]{title} in $infile: no duration\n");
      next;
    }
    $index++;
  }
  close(IN);
  # Return 0 if there was no data, otherwise ok.
  return (0) if ($index==0);
  return(1);
}


# Write_IntermediateFile: Write all the data in
# the @ProgramData array out to the file.
# Returns 1 if ok, 0 on error.
#
sub Write_IntermediateFile($)
{
  my ($outfile)= @_;
  my ($k,$index);

  unless (open(OUT, "> $outfile")) {
    print(STDERR "Can't write data file $outfile: $!\n");
    return(0);
  }

  for ($index=0; $index<=$#ProgramData;$index++) {
    print(OUT "Title: $ProgramData[$index]{title}\n");
    print(OUT "Daynum: $ProgramData[$index]{daynum}\n");
    print(OUT "Time: $ProgramData[$index]{time}\n");
    print(OUT "Duration: $ProgramData[$index]{duration}\n");
    foreach $k (sort keys(%Fileprefix)) {
      if (defined($ProgramData[$index]{$k})) {
        print(OUT "$Fileprefix{$k}: $ProgramData[$index]{$k}\n");
      }
    }
    print(OUT "\n");
  }
  close(OUT);
  return(1);
}

# Short subroutine used to sort a list into
# reverse string length order. 
#
sub Strlensort { length($b) <=> length($a); }



1;
