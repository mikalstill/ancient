#!/usr/bin/perl
# 
# EbroadcastFetcher.pm: parse the data in web pages from www.ebroadcast.com.au
# and construct suitable data structures which can be converted into slice
# files for the TiVo.
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
package TiVo::EbroadcastFetcher;
use strict;
use TiVo::ConfigFiles;
use TiVo::MiscUtil;
use HTTP::Lite;

BEGIN {
  use Exporter	 ();
  use vars	 qw($VERSION @ISA @EXPORT @EXPORT_OK %EXPORT_TAGS);

  $VERSION = do { my @r = (q$Revision: 1.1.1.1 $ =~ /\d+/g); sprintf "%d."."%02d" x $#r, @r }; # must be all one line, for MakeMaker

  @ISA	       = qw(Exporter);
  @EXPORT      = qw(Fetch_Ebroadcast_Channel);
  %EXPORT_TAGS = ( );
  @EXPORT_OK   = qw();
}

use vars	 @EXPORT_OK;
END { }	 # module clean-up code here (global destructor)


#####################################
###### Package-private Globals ######
#####################################

# Base URL used for Ebroadcast.
my ($EbroadcastUrl)= "http://www.ebroadcast.com.au/cgi-bin/TV/byChannel?date=";

# Words to look for in program description to find the actors
my (@Try_cast)= ("Starring", "Hosted by", "Presented by",
		 "Cast:", "Guest stars", "Stars");

# Which days have already been parsed
my (%AlreadyDoneDay);

# The array of program data that is
# built up.
my (@EbroadcastData, $progindex);

# The genre for this station
my ($Stationgenre);
      
# Per-channel genres
my (%GenreTranslate) = (
	"Movie" => "Movie,Movies",
	"Greats" => "Movie,Movies",
	"wmov" => "Movie,Movies",
	"Enc" => "Movie,Movies",
	"Show" => "Movie,Movies",
	"Classics" => "Movie,Movies",
	"TCM" => "Movie,Movies",
	"31" => "Variety",
	"Disc" => "Documentary,DocumentaryGroup",
	"Odyssey" => "Documentary,DocumentaryGroup",
	"FoxHST" => "Documentary,DocumentaryGroup",
	"NatGe" => "Documentary,DocumentaryGroup",
	"FoxFS1" => "Sports,SportsGroup",
	"FoxSP2" => "Sports,SportsGroup",
	"SkyRacing" => "Sports,SportsGroup",
	"Sport" => "Sports,SportsGroup",
	"FoxFF" => "Sports,SportsGroup",
	"ESPN" => "Sports,SportsGroup",
	"kids" => "Children,ChildrensGroup",
	"Nick" => "Children,ChildrensGroup",
	"Cartoon" => "Children,ChildrensGroup",
	"Disney" => "Children,ChildrensGroup",
	"CNN" => "News,NewsBusinessWeather",
	"SkyNews" => "News,NewsBusinessWeather",
	"FoxFNC" => "News,NewsBusinessWeather",
	"BBC" => "News,NewsBusinessWeather",
	"CNBC" => "News,NewsBusinessWeather",
	"ACC" => "Religion",
	"Adventure" => "Action,ActionAdventureGroup",
	"Animal" => "Animals,DocumentaryGroup"
);

######################
###### Routines ######
######################

# Given a channel name, the relative start and
# end days, fetch the web data for this channel.
# Rehtml it and save it. Then parse the data
# into the intermediate format. Returns 1 on
# no errors, 0 if errors.
sub Fetch_Ebroadcast_Channel($$$)
{
  my ($chan,$snum,$enum)= @_;
  my ($result1,$result2)= (1,1);
  $result1=Fetch_and_Store_Data($chan,$snum,$enum);
  $result2=Parse_Data($chan,$snum,$enum);
  return($result1 && $result2);
}

sub Fetch_and_Store_Data($$$)
{
  my ($chan,$snum,$enum)= @_;
  my ($web_proxy, $webdelay, $wanteday);
  my ($html_file, $dayname, $daynum, $url);
  my ($result, $try_again, $try, $http, $req, $body);

  # We start 1 day early so that we get the midnight to 6am data
  $result=1; $snum--;

  # See if we need a web proxy, and # of times to try again
  $web_proxy= $ConfigVar{Webproxy};
  $try_again= $ConfigVar{RepeatWebConnect};

  # Set the delay in seconds between web fetches
  $webdelay=0;
  if (defined($ConfigVar{WebDelay})) {
    $webdelay= int($ConfigVar{WebDelay});
  }

  # Do each day in turn
  for ($wanteday= $snum; $wanteday <= $enum; $wanteday++) {

    $dayname= Get_Dayname($wanteday);
    $daynum= Get_Daynumber($wanteday);

    # Determine the Ebroadcast web page for this station and dayname
    $url= $EbroadcastUrl . $dayname . "&chan=" .  $StationUrl{$chan};

    # Get the output filename
    $html_file= "$ConfigVar{Webdir}/$chan" . "_$dayname";

    # If the HTML file already exists, assume we have already run
    # this before and stored it in the cache.
    if ( -f $html_file) {
      Debug("Using cached data for channel $chan for $dayname\n");
    } else {
      Debug("Fetching programs for channel $chan for $dayname\n");

      $http = new HTTP::Lite;
      if (defined($web_proxy)) { $http->proxy($web_proxy); }

      # Try 1 or more times to get the file
      if (!defined($try_again)) { $try_again=1; }

      for ($try=$try_again;$try>0;$try--) {
        my($req) = $http->request($url) && last;
      }

      if ($try==0) {
	print(STDERR "Unable to get data from Ebroadcast: $!\n");
	$result=0; next;
      }

      # Now pass the returned document to rehtml to place 1 tag per line
      # and sleep afterwards
      $body= $http->body();
      Rehtml($body, $html_file);
      WebLink($html_file, $chan, $dayname);
      sleep($webdelay);
    }
  }
  return($result);
}

# WebLink: see if other channels use the same URL and
# make a symlink to the file instead of fetching it.
sub WebLink($$$)
{
  my ($file, $origchan, $dayname)= @_;
  my ($chan, $html_file);

  foreach $chan (keys(%StationUrl)) {
    next if ($chan eq $origchan);
    if ($StationUrl{$chan} eq $StationUrl{$origchan}) {
      $html_file= "$ConfigVar{Webdir}/$chan" . "_$dayname";
      link($file, $html_file);
      Debug("Linked $file to $html_file as same URL\n");
    }
  }
}


# Given the channel, start day and end day, read the webfiles and
# convert into intermediate format. We now put all of one day, and
# only that day, into an output file.
# New overall strategy: read in each file and parse it.
# Build up a list of records with no duration. Once the
# files are read in, go back and deduce the duration
# by looking at the differences between start times.
# Finally, write the files out.
#
sub Parse_Data($$$)
{
  my ($chan,$snum,$enum)= @_;
  my (%IgnoreDay);
  my ($wanteday, $outfile, $result);
  my ($dayname, $daynum, $i, $pdindex, $start_index);

  $result=1; $progindex=0; undef(@EbroadcastData);

  # Small kludge here. Determine if all programs are movies or other
  # depending on the url of the channel. Mainly for Pay TV channels.
  # Data obtained from http://www.foxtel.com.au/channel/lineup.jsp.
  $Stationgenre= $GenreTranslate{ $StationUrl{$chan} };
  if (defined($Stationgenre)) {
    Debug("Set genre for $chan to $Stationgenre\n");
  }

  # Find out what days we must actually do, and parse the web files.
  for ($wanteday= $snum; $wanteday <= $enum; $wanteday++) {
    $dayname= Get_Dayname($wanteday);
    $outfile=	"$ConfigVar{Datadir}/$chan" . "_$dayname";

    # Skip if we have already generated the output file
    if (-f $outfile) {
      Debug("Intermediate file $outfile exists, not overwriting\n");
      $IgnoreDay{$wanteday}=1;
      # We insert a fictitious record with a 6am start anyway
      $EbroadcastData[$progindex]{title}="Fictitious 6am record";
      $EbroadcastData[$progindex]{daynum}=$EbroadcastData[$progindex-1]{daynum};
      $EbroadcastData[$progindex]{time}=21600; $progindex++;
      next;
    }

    # Build the list of records for the previous day and this day
    if (!defined($AlreadyDoneDay{$wanteday-1})) {
      $result &= Parse_Data_For_Day($chan,$wanteday-1);
    }
    $result &= Parse_Data_For_Day($chan,$wanteday);
    $AlreadyDoneDay{$wanteday}=1;
  }

  # Insert a final record to allow the last duration calculation
  $EbroadcastData[$progindex]{title}="Fictitious 6am record";
  $EbroadcastData[$progindex]{daynum}= $EbroadcastData[$progindex-1]{daynum};
  $EbroadcastData[$progindex]{time}=21600; $progindex++;

  # Calculate the duration for each program
  for ($i=0; $i< $progindex -1; $i++) {
    $EbroadcastData[$i]{duration}=
	$EbroadcastData[$i+1]{time} - $EbroadcastData[$i]{time};
    $EbroadcastData[$i]{duration}+= 86400 if ($EbroadcastData[$i]{duration}<0);
    # Debug("$i: $EbroadcastData[$i]{title}\n");
    # Debug("$EbroadcastData[$i]{daynum}\n");
    # Debug("$EbroadcastData[$i]{time} $EbroadcastData[$i]{duration}\n\n");
  }

  # Now write the results out for each day where there is no data yet
  $start_index=0;
  for ($wanteday= $snum; $wanteday <= $enum; $wanteday++) {
    if (defined($IgnoreDay{$wanteday})) { next; }

    # Print out the relevant records for this daynum
    $dayname= Get_Dayname($wanteday);
    $daynum=  Get_Daynumber($wanteday); $pdindex=0; undef(@ProgramData);
    $outfile=	"$ConfigVar{Datadir}/$chan" . "_$dayname";

    for ($i=$start_index; $i< $progindex -1; $i++) {

      next if ($EbroadcastData[$i]{daynum}<$daynum);
      if ($EbroadcastData[$i]{daynum}>$daynum) {
	$start_index=$i; last;
      }
      Debug("Adding $EbroadcastData[$i]{title} to $daynum\n");
      $ProgramData[$pdindex]= $EbroadcastData[$i]; $pdindex++;
    }
    if ($pdindex!=0) {
      Debug("Writing file $outfile\n");
      $result &= Write_IntermediateFile($outfile);
    } else {
      Debug("No program data for $daynum $chan $dayname\n");
      print(STDERR "No program data for $chan on $dayname\n");
      $result=0;
    }
  }
  return($result);
}


# Parse a web file for only a single day.
sub Parse_Data_For_Day($$)
{
  my ($chan,$wanteday)= @_;
  my ($dayname, $daynum);
  my ($html_file, $g, $rating);
  my ($this_time, $last_start_time);
  my ($hr, $min, $apm, $start_time);
  my ($title, $year, $tvrating, $showingbits);
  my ($i, @Advisory, @Actorlist, $description);
  my ($advisory, $actor, $genre, $episode_title);

  $dayname= Get_Dayname($wanteday);
  $daynum=  Get_Daynumber($wanteday);
  $last_start_time=-1;

  # Get the input filename
  $html_file= "$ConfigVar{Webdir}/$chan" . "_$dayname";

  unless (open(IN, $html_file)) {
    print(STDERR "Can't open rehtml'd page $html_file: $!\n");
    return(0);
  }
  Debug("Parsing Ebroadcast data from $html_file\n");

  while (<IN>) {
    chop;

    # Search for the beginning of a program by finding <A NAME.*_[ap]m">
    #
    if (/<A NAME=\"(.*_[ap]m)\">/) {
      $this_time=$1;

      # Reset all the attributes
      undef($year);
      $tvrating = 0;
      $showingbits = 0;
      undef($episode_title);
      undef($genre);
      undef($rating);
      undef(@Actorlist);
      undef(@Advisory);
      $description = "";

      # Convert this time to seconds
      #
      ($hr,$min,$apm)= split(/_/, $this_time);
      # Debug("$this_time becomes [$hr:$min:$apm]\n");
      if ($hr==12) { $hr=0; }
      $start_time= 3600 * $hr + 60 * $min;
      if ($apm eq "pm") { $start_time += 12 * 3600; }
      # Debug("Found start time of $start_time which was $this_time\n");

      # If $start_time < $last_start_time, we are now into the next day,
      # so bump up the daynumber by 1.
      if ($start_time < $last_start_time) {
        Debug("Incremented daynum from $daynum to $daynum + 1\n");
        $daynum++;
      }

      # Find a <B> and capture every non-tag line up to </B>
      #
      $title= "";
      while (<IN>) {
        if (/<B>/) { last; }
      }
      while (<IN>) {
        if (/<\/B>/) { last; }
        if (/</) { next; }
        chop; $title = $title . $_;
      }

      # No title, go back & search for a proper title
      if ($title eq "") {
	Debug("Skipping empty title at time $this_time\n");
      	$last_start_time= $start_time;
	next;
      }

      # Some Pay TV data from Ebroadcast has single-letter program
      # titles. Fix this stupidity by making them called "TBA".
      if (length($title)==1) { $title= "TBA"; }
      # Debug("Found title which is $title\n");

      # Find the description between two <BR> tags
      #
      while (<IN>) {
        if (/<BR>/) { last; }
      }
      while (<IN>) {
        if (/<b>/) { last; }
        if (/<BR>/) { last; }
        if (/</) { next; }
        chop; $description = $description . $_;
      }

      # Sometimes the description has some advisories in it.
      # Strip it out now and deal with it later.
      $advisory="";
      if ($description =~ /(.*)(\(Cons.Advice.*)/) {
        $description=$1; $advisory=$2;
        $advisory=~ s/\(Cons.Advice//;
      }

      # If the description starts with uppercase ending in a full stop,
      # treat it as an episode title.
      if ($description =~ /^([A-Z\s]+)\.(.*)/) {
	$description= $2;
	($episode_title)= LowercaseString($1);
      }

      # Search for a </b> and get the rating from the line after
      #
      while (<IN>) {
        if (/<\/b>/) { last; }
      }
      $rating= <IN>; chop($rating);

      if    ($rating =~ /PG/)	 { $tvrating= 4; }	  
      elsif ($rating =~ /G/)	 { $tvrating= 3; }	  
      elsif ($rating =~ /M/)	 { $tvrating= 6; }	  
      if ($rating =~ /CC/)	  { $showingbits |= 0x1; }
      if ($rating =~ /Repeat/i) { $showingbits |= 0x200; }
      if ($rating =~ /Rpt/i)	  { $showingbits |= 0x200; }

      # The PayTV stuff from Ebroadcast also has interesting things.
      # BBC World on Ebroadcast puts (r) in the title to
      # indicate a repeated broadcast. Ditto (Replay).
      if ($title =~ /\(r\)/) {
        $title=~ s/\(r\)//g; $showingbits |= 0x200;
      }
      if ($title =~ /\(Replay\)/i) {
        $title=~ s/\(Replay\)//ig; $showingbits |= 0x200;
      }
      # Looks like Odyssey does the same with (PG) and (M)
      if ($title =~ /\(PG\)/i) {
        $title=~ s/\(PG\)//ig; $tvrating= 4;
      }
      if ($title =~ /\(M\)/i) {
        $title=~ s/\(M\)//ig; $tvrating= 6;
      }

      # Map the single letter warnings onto Advisory values.
      # I try to convert G, PG, M, MA, R to TV Ratings,
      # but I'm not sure what the right conversions are.
      #
      #
      if ($rating =~ /(\([VLSAHND,]+\))/) {	  # H is horror
        $advisory= $advisory . $1;
      }
      if ($advisory =~ /V/) { push(@Advisory, 6); $showingbits |= 0x20000; }
      if ($advisory =~ /L/) { push(@Advisory, 1); $showingbits |= 0x40000; }
      if ($advisory =~ /S/) { push(@Advisory, 8); $showingbits |= 0x10000; }
      if ($advisory =~ /A/) { push(@Advisory, 10); }
      if ($advisory =~ /N/) { push(@Advisory, 3); }

      # Sometimes the rating has a year number in it
      #
      if ($rating =~ /19([0-9][0-9])/) { $year = "19$1"; }
      if ($rating =~ /20([0-9][0-9])/) { $year = "20$1"; }

      # At this point, we jump off to a heuristic section which
      # works out who is in the cast, if any.
      #
      if ($description ne "") {
	@Actorlist= Find_The_Cast($chan, \$description);
      }

      # We now have the title of this program and its starting time.
      # Kludge: if the title has the word (Cont.) in it, don't write
      # this record out as it is a continuation of the previous
      # record.
      if ($title=~ /(Cont\.)/) {
	Debug("Doing the Cont skip\n");
        $last_start_time= $start_time;
        next;
      }

      # Save the information into the array
      Debug("Output: $title start $start_time day $daynum index $progindex\n"); 
      $EbroadcastData[$progindex]{title}= $title;
      $EbroadcastData[$progindex]{daynum}= $daynum;
      $EbroadcastData[$progindex]{time}= $start_time;
      $EbroadcastData[$progindex]{description}= $description;
      $EbroadcastData[$progindex]{tvrating}=$tvrating if ($tvrating>0);
      $EbroadcastData[$progindex]{bits}=$showingbits if ($showingbits>0);
      $EbroadcastData[$progindex]{year}=$year if (defined($year));
      $EbroadcastData[$progindex]{genre}=$genre if (defined($genre));
      $EbroadcastData[$progindex]{episode}=$episode_title
                                                if (defined($episode_title));

      if (defined(@Actorlist)) {
	$actor=$Actorlist[0];
        for ($i=1; $i<=$#Actorlist; $i++) { $actor= $actor . ",$Actorlist[$i]";}
	$EbroadcastData[$progindex]{actor}= $actor;
      }
      if (defined(@Advisory)) {
	$advisory=$Advisory[0];
          for ($i=1; $i<=$#Advisory; $i++) {
	    $advisory= $advisory . ",$Advisory[$i]";
	  }
	$EbroadcastData[$progindex]{advisory}= $advisory;
      }
      $progindex++;
      $last_start_time= $start_time;
    }
  }
  close(IN);

  if ($last_start_time == -1) {
    Debug("No program data in $html_file\n");
    # Unlink the file as it contained no data
    unlink($html_file);
    return(0);
  }
  return(1);
}

# Procedure find_the_cast:
# Use heuristics to try & work out who is in the cast.
# Look for CAST, Stars, Starring, UPPERCASE WORDS (for Channel Ten).
#
sub Find_The_Cast($$) {
    my ($channel, $dref)= @_;
    my ($description)= $$dref;
    my (@Actor, $trycast, $left, $right, $actor);

    foreach $trycast (@Try_cast) {
      #
      # If we find a match using any of the trycast words,
      # followed by a : or a space, then ....
      #
      if ($description =~ /(.*)$trycast(:\s*|\s+)/i) {
        $left=$1;
        #
        # Remove the stuff leading up to and including the trycast.
        # Then remove anything starting with a dot, a '(' and
        # words starting with lowercase letters.
        #
        $description=~ s/.*$trycast\s*:*\s*//i;
        $description=~ s/((\b[a-z].*|\..*|\(.*))//;

        if (defined($1)) { $right = $1; } else { $right = ""; }
        $$dref = $left . $right;	# Send new description back.
        # 
        # Now split on either commas or the patterns ' and '
        # and ' & '
        #
        push(@Actor,
      	LowercaseString(split(/,\s*|\s+and\s+|\s*&\s+/,$description)));
        return(@Actor);
      }
    }

    # No, luck try the uppercase method then :-)
    #
    #while ($description =~ /([A-Z][A-Z]+ [A-Z][A-Z]+)/g) {
    #  ($actor)= LowercaseString($1);
    #  Debug("Working on $actor\n");
    #  push(@Actor,$actor);
    #}
    return(@Actor);
}

1;
