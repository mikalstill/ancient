#!/usr/bin/perl
# 
# FoxtelFetcher.pm: parse the data in web pages from www.foxtel.com.au
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
package TiVo::FoxtelFetcher;
use strict;
use TiVo::ConfigFiles;
use TiVo::MiscUtil;
use HTTP::Lite;

BEGIN {
  use Exporter	 ();
  use vars	 qw($VERSION @ISA @EXPORT @EXPORT_OK %EXPORT_TAGS);

  $VERSION = do { my @r = (q$Revision: 1.1.1.1 $ =~ /\d+/g); sprintf "%d."."%02d" x $#r, @r }; # must be all one line, for MakeMaker

  @ISA	       = qw(Exporter);
  @EXPORT      = qw(Fetch_Foxtel_Channel);
  %EXPORT_TAGS = ( );
  @EXPORT_OK   = qw();
}

use vars	 @EXPORT_OK;
END { }	 # module clean-up code here (global destructor)


#####################################
###### Package-private Globals ######
#####################################

# Base URL used for Foxtel.
my ($FoxtelUrl)= "http://foxtel.com.au/foxtelguide";

# Words to look for in program description to find the actors
my (@Try_cast)= ("Starring", "Hosted by", "Presented by",
		 "Cast:", "Guest stars", "Stars");

# Which days have already been parsed
my (%AlreadyDoneDay);

# The array of program data that is
# built up.
my (@FoxtelData, $progindex);

# The genre for this station
my ($Stationgenre);
      
# Per-channel genres
my (%GenreTranslate) = (
        "MVX" => "Movie,Movies",
        "TCM" => "Movie,Movies",
        "WLD" => "Movie,Movies",
        "MV1" => "Movie,Movies",
        "MVG" => "Movie,Movies",
        "SHO" => "Movie,Movies",
        "SH2" => "Movie,Movies",
        "CLA" => "Movie,Movies",
        "EVE" => "Movie,Movies",
        "ENC" => "Movie,Movies",
        "NIC" => "Children,ChildrensGroup",
        "DSY" => "Children,ChildrensGroup",
        "KDS" => "Children,ChildrensGroup",
        "CNN" => "News,NewsBusinessWeather",
        "CNB" => "News,NewsBusinessWeather",
        "FNC" => "News,NewsBusinessWeather",
        "SKY" => "News,NewsBusinessWeather",
        "COM" => "Comedy,ComedyGroup",
        "FFC" => "Football,SportsGroup",
        "ESP" => "Sports,SportsGroup",
        "SP2" => "Sports,SportsGroup",
        "SRA" => "Sports,SportsGroup",
        "FSP" => "Sports,SportsGroup",
        "MMX" => "Music,ArtsMusicLiving",
        "MTV" => "Music,ArtsMusicLiving",
        "NGE" => "Documentary,DocumentaryGroup",
        "ADO" => "Adult,DramaGroup",
        "AP"  => "Animals,DocumentaryGroup",
        "RAI"  => "International",
        "ANT"  => "International",
        "TVS"  => "Shopping,Daytime",
        "CNE" => "Animated,Children,ChildrensGroup"
);

# Hashes to convert Foxtel genres to TiVo genres
my (%Genre) = (
  "Action" => "Action,ActionAdventureGroup",
  "Adventure" => "Action,ActionAdventureGroup",
  "Animation" => "Animated,ChildrensGroup",
  "Arts and Culture" => "Art,ArtsMusicLiving",
  "Biography" => "Biography,DocumentaryGroup",
  "Business and Finance" => "Business,NewsBusinessWeather",
  "Cartoon" => "Animated,ChildrensGroup",
  "Children" => "Animated,ChildrensGroup",
  "Classic" => "Classic",
  "Comedy" => "Comedy,ComedyGroup",
  "Crime" => "Crime,DramaGroup",
  "Current Affairs" => "PublicAffairs,NewsBusinessWeather",
  "Documentary" => "Documentary,DocumentaryGroup",
  "Drama" => "Drama,DramaGroup",
  "Education" => "Educational,EducationalGroup",
  "Entertainment" => "Unknown",
  "Family" => "Family,ChildrensGroup",
  "Game Show" => "GameShow,Daytime",
  "Historical" => "Historical,DocumentaryGroup",
  "Infotainment" => "Shopping",
  "Lifestyle" => "Outdoors,RealLife",
  "Music" => "Music,ArtsMusicLiving",
  "Mystery" => "Mystery,MysteryThriller",
  "N/A" => "Unknown",
  "Nature" => "Nature,DocumentaryGroup",
  "News" => "News,NewsBusinessWeather",
  "Real Life" => "RealLife",
  "Romance" => "Romance,DramaGroup",
  "Romantic Comedy" => "RomanticComedy,ComedyGroup",
  "Sci-Fi" => "ScienceFiction,SciFiFantasy",
  "Science and Technology" => "Science,ScienceNatureTechnology",
  "Sitcom" => "Situation,Daytime",
  "Sport" => "Sports,SportsGroup",
  "Talk Show" => "Talkshow,Daytime",
  "Thriller" => "Thriller",
  "Travel" => "Travel",
  "Variety" => "Variety",
  "War" => "War,DocumentaryGroup"
);

######################
###### Routines ######
######################

# Given a channel name, the relative start and
# end days, fetch the web data for this channel.
# Rehtml it and save it. Then parse the data
# into the intermediate format. Returns 1 on
# no errors, 0 if errors.
sub Fetch_Foxtel_Channel($$$)
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
  my ($html_file, $dayname, $daynum);
  my ($result, $try_again, $try, $http, $req, $body);
  my (%args, $daystring);

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

      # Get the day string as required by Foxtel
      $daystring= Get_Foxtel_Dayname($wanteday);

      %args = ("from" => "column", "day" => "$daystring", "time" => "99", 
                 "category" => "-1", "channel" => $StationUrl{$chan});

      # Try 1 or more times to get the file
      if (!defined($try_again)) { $try_again=1; }

      for ($try=$try_again;$try>0;$try--) {

        $http->http11_mode(1);
        $http->add_req_header("Referer", "http://foxtel.com.au/home.jsp");

	#
	# There seems to be a bug in HTTP::Lite; it does not honour the
	# Content-Type header.
	#
        $http->add_req_header("Content-Type",
					"application/x-www-form-urlencoded");
        $http->prepare_post( \%args );
        my($req) = $http->request($FoxtelUrl) && last;
      }

      if ($try==0) {
	print(STDERR "Unable to get data from Foxtel: $!\n");
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

  $result=1; $progindex=0; undef(@FoxtelData);

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
      $FoxtelData[$progindex]{title}="Fictitious 6am record";
      $FoxtelData[$progindex]{daynum}= $FoxtelData[$progindex-1]{daynum};
      $FoxtelData[$progindex]{time}=21600; $progindex++;
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
  $FoxtelData[$progindex]{title}="Fictitious 6am record";
  $FoxtelData[$progindex]{daynum}= $FoxtelData[$progindex-1]{daynum};
  $FoxtelData[$progindex]{time}=21600; $progindex++;

  # Calculate the duration for each program
  for ($i=0; $i< $progindex -1; $i++) {
    $FoxtelData[$i]{duration}=
	$FoxtelData[$i+1]{time} - $FoxtelData[$i]{time};
    $FoxtelData[$i]{duration}+= 86400 if ($FoxtelData[$i]{duration}<0);
    # Debug("$i: $FoxtelData[$i]{title}\n");
    # Debug("$FoxtelData[$i]{daynum}\n");
    # Debug("$FoxtelData[$i]{time} $FoxtelData[$i]{duration}\n\n");
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

      next if ($FoxtelData[$i]{daynum}<$daynum);
      if ($FoxtelData[$i]{daynum}>$daynum) {
	$start_index=$i; last;
      }
      Debug("Adding $FoxtelData[$i]{title} to $daynum\n");
      $ProgramData[$pdindex]= $FoxtelData[$i]; $pdindex++;
    }
    if ($pdindex!=0) {
      Debug("Writing file $outfile\n");
      $result &= Write_IntermediateFile($outfile);
    } else {
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
  my ($html_file);
  my ($this_time, $last_start_time);
  my ($hr, $min, $apm, $start_time);
  my ($title, $year, $tvrating, $showingbits);
  my ($i, @Advisory, @Actorlist, $description);
  my ($advisory, $actor, $genre);
  my ($episode_title, $g);
  my ($time_marker, $found_since_marker);

  $dayname= Get_Dayname($wanteday);
  $daynum=  Get_Daynumber($wanteday);
  $last_start_time=-1; $time_marker=-1;

  # Get the input filename
  $html_file= "$ConfigVar{Webdir}/$chan" . "_$dayname";

  unless (open(IN, $html_file)) {
    print(STDERR "Can't open rehtml'd page $html_file: $!\n");
    return(0);
  }
  Debug("Parsing Foxtel data from $html_file\n");

  while (<IN>) {
    chop;

    # Track the 6am, 12pm markers
    if (/^12am$/ && $found_since_marker) {
       	$time_marker= 0; $found_since_marker=0;
	Debug("12am time marker: $time_marker\n"); next; }
    if (/^6am$/ && $found_since_marker) {
        $time_marker= 21600; $found_since_marker=0;
	Debug("6am time marker: $time_marker\n"); next; }
    if (/^12pm$/ && $found_since_marker) {
        $time_marker= 43200; $found_since_marker=0;
	Debug("12pm time marker: $time_marker\n"); next; }
    if (/^6pm$/ && $found_since_marker) {
        $time_marker= 64800; $found_since_marker=0;
	Debug("6pm time marker: $time_marker\n"); next; }

    # Search for the start of a program by finding <span class="guideHead">
    #
    if (/<span class.*guideHead/) {

      # Reset all the attributes
      undef($year);
      $tvrating = 0;
      $showingbits = 0;
      undef($genre);
      undef(@Actorlist);
      undef(@Advisory);
      $description = "";
      $episode_title = "";

      # The next line has the start time & title separated by a space
      $_= <IN>;
      /([^ ]*)\s+(.*)/;
      $this_time=$1; $title=$2;
      Debug("Got $this_time and $title\n");

      # Convert this time to seconds
      #
      $this_time=~ /(.*)\.(.*)([ap]m)/;
      ($hr,$min,$apm)= ($1,$2,$3);
      # Debug("$this_time becomes [$hr:$min:$apm]\n");
      if ($hr==12) { $hr=0; }
      $start_time= 3600 * $hr + 60 * $min;
      if ($apm eq "pm") { $start_time += 12 * 3600; }
      Debug("Found start time of $start_time which was $this_time\n");

      # If $start_time < $last_start_time, we are now into the next day,
      # so bump up the daynumber by 1.
      if ($start_time < $last_start_time) {
        Debug("Incremented daynum from $daynum to $daynum + 1\n");
        $daynum++;
      }

      # Skip the </span> line and capture the episode title
      $_= <IN>;
      $episode_title= <IN>; chop($episode_title);
      ($episode_title)= LowercaseString($episode_title);
      Debug("Episode title is $episode_title\n");

      # No title, go back & search for a proper title
      if ($title eq "") { next; }

      # Some Pay TV data from Foxtel has single-letter program
      # titles. Fix this stupidity by making them called "TBA".
      if (length($title)==1) { $title= "TBA"; }
      Debug("Found title which is $title\n");

      # Find the description between two <br /> tags
      #
      while (<IN>) {
        if (/<br /) { last; }
      }
      while (<IN>) {
        if (/<br /) { last; }
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

      # This code comes from Ebroadcast. How do we do it for Foxtel?
      #if    ($rating =~ /PG/)	 { $tvrating= 4; }	  
      #elsif ($rating =~ /G/)	 { $tvrating= 3; }	  
      #elsif ($rating =~ /M/)	 { $tvrating= 6; }	  
      #if ($rating =~ /CC/)	  { $showingbits |= 0x1; }
      #if ($rating =~ /Repeat/i) { $showingbits |= 0x200; }
      #if ($rating =~ /Rpt/i)	  { $showingbits |= 0x200; }

      # The PayTV stuff from Foxtel also has interesting things.
      # BBC World on Foxtel puts (r) in the title to
      # indicate a repeated broadcast. Ditto (Replay).
      if ($description =~ /\(r\)/) {
        $description=~ s/\(r\)//g; $showingbits |= 0x200;
      }
      if ($description =~ /\(Replay\)/i) {
        $description=~ s/\(Replay\)//ig; $showingbits |= 0x200;
      }
      # Looks like Odyssey does the same with (PG) and (M)
      if ($description =~ /\(PG\)/i) {
        $description=~ s/\(PG\)//ig; $tvrating= 4;
      }
      if ($description =~ /\(M\)/i) {
        $description=~ s/\(M\)//ig; $tvrating= 6;
      }

      # Map the single letter warnings onto Advisory values.
      # I try to convert G, PG, M, MA, R to TV Ratings,
      # but I'm not sure what the right conversions are.
      #
      #
      if ($description =~ /(\([VLSAHND,]+\))/) {	  # H is horror
        $advisory= $advisory . $1;
      }
      if ($advisory =~ /V/) { push(@Advisory, 6); $showingbits |= 0x20000; }
      if ($advisory =~ /L/) { push(@Advisory, 1); $showingbits |= 0x40000; }
      if ($advisory =~ /S/) { push(@Advisory, 8); $showingbits |= 0x10000; }
      if ($advisory =~ /A/) { push(@Advisory, 10); }
      if ($advisory =~ /N/) { push(@Advisory, 3); }

      # Sometimes the description has a year number in it
      #
      if ($description =~ /19([0-9][0-9])/) { $year = "19$1"; }
      if ($description =~ /20([0-9][0-9])/) { $year = "20$1"; }

      # Pick up any genres from the description.
      $genre= $Stationgenre if (defined($Stationgenre));
      foreach $g (keys(%Genre)) {
        if ($description=~ /\Q$g\E/) {
          if (defined($genre)) {
            $genre= $genre . "," . $Genre{$g};
          } else {
            $genre= $Genre{$g};
          }
          last;
        }
      }

      # At this point, we jump off to a heuristic section which
      # works out who is in the cast, if any.
      #
      if ($description ne "") { Find_The_Cast($chan, \$description); }

      # We now have the title of this program and its starting time.

      # Foxtel doesn't use the (Cont.) thing, so we don't have to do
      # the Cont kludge. I'm leaving the code in here for later....

      # Kludge: if the title has the word (Cont.) in it, don't write
      # this record out as it is a continuation of the previous
      # record.
      # if ($title=~ /(Cont\.)/) {
      #   next;
      # }

      # Save the information into the array
      Debug("Output: $title start $start_time day $daynum\n"); 
      $FoxtelData[$progindex]{title}= $title;
      $FoxtelData[$progindex]{daynum}= $daynum;
      $FoxtelData[$progindex]{time}= $start_time;
      $FoxtelData[$progindex]{description}= $description;
      $FoxtelData[$progindex]{tvrating}=$tvrating if ($tvrating>0);
      $FoxtelData[$progindex]{bits}=$showingbits if ($showingbits>0);
      $FoxtelData[$progindex]{year}=$year if (defined($year));
      $FoxtelData[$progindex]{episode}=$episode_title
						if ($episode_title ne "");
      $FoxtelData[$progindex]{genre}=$genre if (defined($genre));

      if (defined(@Actorlist)) {
	$actor=$Actorlist[0];
        for ($i=1; $i<=$#Actorlist; $i++) { $actor= $actor . ",$Actorlist[$i]";}
	$FoxtelData[$progindex]{actor}= $actor;
      }
      if (defined(@Advisory)) {
	$advisory=$Advisory[0];
          for ($i=1; $i<=$#Advisory; $i++) {
	    $advisory= $advisory . ",$Advisory[$i]";
	  }
	$FoxtelData[$progindex]{advisory}= $advisory;
      }
      $found_since_marker=1; $progindex++;
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

  # If we haven't seen any data since a time marker, put in
  # a record indicating that the station closed at that time
  if (($time_marker!=-1) && ($found_since_marker==0)) {
     Debug("Adding a CLOSE record for $daynum at time $time_marker\n");
     $FoxtelData[$progindex]{title}="Close";
     $FoxtelData[$progindex]{daynum}= $FoxtelData[$progindex-1]{daynum};
     $FoxtelData[$progindex]{time}=$time_marker;
     # Adjust in case it's on the next day
     if ($time_marker < $FoxtelData[$progindex-1]{time}) {
	$FoxtelData[$progindex]{daynum}++;
        Debug("CLOSE record daynum was bumped\n");
     }
     $progindex++;
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
