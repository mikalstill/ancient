#!/usr/bin/perl
# 
# ConfigFiles.pm: Routines to load and save configuration files.
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
package TiVo::ConfigFiles;
use strict;
use TiVo::MiscUtil;

BEGIN {
  use Exporter	 ();
  use vars	 qw($VERSION @ISA @EXPORT @EXPORT_OK %EXPORT_TAGS);

  $VERSION = do { my @r = (q$Revision: 1.1.1.1 $ =~ /\d+/g); sprintf "%d."."%02d" x $#r, @r }; # must be all one line, for MakeMaker

  @ISA	       = qw(Exporter);
  @EXPORT      = qw(Read_Guiderc Read_Websources Read_Genrelist
		    Read_Trimtitles Read_Episodes Cleanup_Old_Files
		    Read_Stations Read_Numbers Genre_To_Numeric
		    Write_Numbers Write_Episodes Find_Outfilename
		    %ConfigVar %TrimTitle %NumericGenre %Episodes
		    %TextualGenre
		    %StationId %DestinationTimezone %Numbers
		    %StationWebsrc %SourceTimezone %StationUrl);
  %EXPORT_TAGS = ( );
  @EXPORT_OK   = qw(%ConfigVar %TrimTitle %NumericGenre %Episodes
		    %TextualGenre
		    %StationId %DestinationTimezone %Numbers
		    %StationWebsrc %SourceTimezone %StationUrl);
}

use vars	 @EXPORT_OK;
END { }	 # module clean-up code here (global destructor)


#####################################
###### Package-private Globals ######
#####################################

# Default filenames for the genre list, sequence numbers, stations, episodes etc.
my ($Programs)=	  "programs";
my ($Numbers)=	  "numbers";
my ($Stations)=	  "stations";
my ($Epsofile)=	  "episodes";
my ($Trimfile)=	  "trimtitles";
my ($Genretypes)= "genretypes";
my ($Fudgefile)=  "fudge";
my ($Websrcfile)= "websources";

my ($Configdir);

my ($BiggestEpisodeSeries)=0;


######################
###### Routines ######
######################

# Read the contents of the global configuration file named as the
# argument into the %ConfigVar hash array. No return value.
#
sub Read_Guiderc($)
{
  my($var,$val,$i);
  my($toplevel_confile)= $_[0];

  open(IN, "$toplevel_confile") ||
			die("Cannot open config file $toplevel_confile: $!\n");
  
  # Extract the variable = value pairs from the file
  while (<IN>) {
	chop;
	if (/^#/) { next; }
	if (/^\s*$/) { next; }
	($var,$val)= split (/\s*=\s*/);
	$ConfigVar{$var}=$val;
  }
  close(IN);

  foreach $i ("Configdir", "Datadir", "Outputformat", "Slicefile", "Textfile",
		"Webdir") {
    if (!defined($ConfigVar{$i})) {
      die("Variable $i not found in $toplevel_confile configuration file\n");
    }
  }
  $Configdir= $ConfigVar{Configdir};

  # Open up a debug file if it was defined
  if (defined($ConfigVar{Debugfile})) {
    Open_Debug_File($ConfigVar{Debugfile});
  }
}



# Open the list of web sources for each channel and read it in.
# No return value.
#
sub Read_Websources {
  my ($name,$websource,$timezone,$url);

  if (defined($ConfigVar{Websources})) {
    open(IN, $ConfigVar{Websources}) ||
			die("Cannot read $ConfigVar{Websources}: $!\n");
  } else {
    open(IN, "$Configdir/$Websrcfile") ||
				die("Cannot read $Configdir/$Websrcfile: $!\n");
  }

  while(<IN>) {
    if (/^#/) { next; }
    chop;
    ($name,$websource,$timezone,$url)= split(/\t+/);
    $StationWebsrc{$name} =   $websource;
    $SourceTimezone{$name} =  $timezone;
    $StationUrl{$name} =      $url;
  }
  close(IN);
}

# Open the list of genres known by the TiVo, and read it in
#
sub Read_Genrelist
{
  my ($genre,$genrenum);

  if (defined($ConfigVar{Genretypes})) {
    open(IN,$ConfigVar{Genretypes}) ||
                        die("Cannot read $ConfigVar{Genretypes}: $!\n");
  } else {
    open(IN,"$Configdir/$Genretypes") ||
                                die("Cannot read $Configdir/$Genretypes: $!\n");
  }
  while(<IN>) {
    if (/^#/) { next; }
    chop;
    ($genre,$genrenum)= split(/\t+/);
    $NumericGenre{$genre}=$genrenum;
    $TextualGenre{$genrenum}=$genre;
  }
  close(IN);
}

# Read in the trimtitles file.
# No return value.
#
sub Read_Trimtitles {
  my ($prog,$genre,$never,$i,@genre_set);

  if (defined($ConfigVar{Trimtitles})) {
    open(IN, $ConfigVar{Trimtitles}) ||
                        die("Cannot read $ConfigVar{Trimtitles}: $!\n");
  } else {
    open(IN, "$Configdir/$Trimfile") ||
                                die("Cannot read $Configdir/$Trimfile: $!\n");
  }
  while(<IN>) {
    if (/^#/) { next; }
    chop;
    ($prog,$genre,$never)= split(/\t+/);
    if (defined($TrimTitle{$prog}{genre})) {
        print(STDERR "Warning: redefinition of $prog in $Trimfile\n"); next;
    } 

    # Set the TrimTitles to define the program and that it is episodic
    $TrimTitle{$prog}{episodic}=1;
    $genre=~ s/\s//g; $TrimTitle{$prog}{genre}=$genre;

    # Check that the genres are actually defined
    #
    @genre_set= split(/[,\s]/,$genre);
    for ($i=0;$i<=$#genre_set;$i++) {
      if (!defined($NumericGenre{$genre_set[$i]})) {
        print(STDERR "Unknown genre $genre_set[$i] for $prog in $Trimfile\n");
      } 
    }

    # If we find the never set to be Never, then change $TrimTitles{$prog}
    # to reflect that this is not episodic
    if (defined($never) && ($never eq "Never")) {
      $TrimTitle{$prog}{episodic}=0;
    }
  }
  close(IN);
}

# Read the episodes and series information.
#
sub Read_Episodes
{
  my ($i, $prog, $series, $episodenum);
  my (@elist, %SeenSeries);

  if (defined($ConfigVar{Episodes})) {
    open(IN, $ConfigVar{Episodes}) ||
                                die("Cannot read $ConfigVar{Episodes}: $!\n");
  } else {
    open(IN, "$Configdir/$Epsofile") ||
                                die("Cannot read $Configdir/$Epsofile: $!\n");
  }
  while(<IN>) {
    if (/^#/) { next; }
    chop;

    (@elist)= split(/\|/);
    for ($i=0; $i<=$#elist; $i++) {
      $elist[$i]=~ s/^\s*//;
      $elist[$i]=~ s/\s*$//;
    }
    $prog= shift(@elist); $series= shift(@elist); $episodenum= shift(@elist);
    if ($series>$BiggestEpisodeSeries) { $BiggestEpisodeSeries= $series; }

    if (defined($SeenSeries{$series})) {
     print(STDERR "Panic, series-id $series for $prog already in use\n");
    }
    $SeenSeries{$series}=1;
    $Episodes{$prog}{series}= $series;
    $Episodes{$prog}{episodenum}= $episodenum;
  }
  close(IN);
  undef(%SeenSeries);
}

# Read the station list
#
sub Read_Stations
{
  my ($name,$id,$timezone);

  if (defined($ConfigVar{Stations})) {
    open(IN, $ConfigVar{Stations}) ||
                                die("Cannot read $ConfigVar{Stations}: $!\n");
  } else {
    open(IN, "$Configdir/$Stations") ||
                                die("Cannot read $Configdir/$Stations: $!\n");
  }
  while(<IN>) {
    if (/^#/) { next; }
    chop;
    ($name,$id,$timezone)=split(/\t+/);
    $StationId{$name}=$id;
    $DestinationTimezone{$name}=$timezone;
  }
  close(IN);
}

# Read the incremental numbers
#
sub Read_Numbers
{
  if (defined($ConfigVar{Numbers})) {
    open(IN, $ConfigVar{Numbers}) ||
                                die("Cannot read $ConfigVar{Numbers}: $!\n");
  } else {
    open(IN, "$Configdir/$Numbers") ||
                                die("Cannot read $Configdir/$Numbers: $!\n");
  }
  while(<IN>) {
    if (/^#/) { next; }
    chop;
    if (/^Series:\s*(.*)/) {     $Numbers{series}= $1; next; }
    if (/^Program:\s*(.*)/) {    $Numbers{program}= $1; next; }
    if (/^StationDay:\s*(.*)/) { $Numbers{stationday}= $1; next; }
    if (/^Slice:\s*(.*)/) {      $Numbers{slice}= $1; next; }
  }
  close(IN);
  if ($BiggestEpisodeSeries>= $Numbers{series}) {
    print(STDERR "Panic: series overlap, numbers $Numbers{series} " .
		 "vs. episodes $BiggestEpisodeSeries\n");
  }
}

# Write out the new incremental numbers
#
sub Write_Numbers
{
    if (defined($ConfigVar{Numbers})) {
      open(OUT, ">$ConfigVar{Numbers}") ||
                                die("Cannot save $ConfigVar{Numbers}: $!\n");
    } else {
      open(OUT, ">$Configdir/$Numbers") ||
                                die("Cannot save $Configdir/$Numbers: $!\n");
    }
    print(OUT "# List of incremental numbers\n");
    print(OUT "Series:  $Numbers{series}\n");
    print(OUT "Program: $Numbers{program}\n");
    print(OUT "StationDay: $Numbers{stationday}\n");
    print(OUT "Slice: $Numbers{slice}\n");
    close(OUT);
}

# Write out the new episodes information
#
sub Write_Episodes
{
  my ($i);
    if (defined($ConfigVar{Episodes})) {
      open(OUT, ">$ConfigVar{Episodes}") ||
                        die("Cannot save $ConfigVar{Episodes}: $!\n");
    } else {
      open(OUT, ">$Configdir/$Epsofile") ||
                                die("Cannot save $Configdir/$Epsofile: $!\n");
    }
    print(OUT "#\n# List of programs that are a series\n#\n");
    print(OUT "# First column gives title (or portion of title).\n");
    print(OUT "# This must match the name in the genrelist.\n");
    print(OUT "# Second column is the TiVo series number.\n");
    print(OUT "# Third column holds the next episode number to use.\n#\n");
    foreach $i (sort(keys(%Episodes))) {
      print(OUT "$i | $Episodes{$i}{series} | $Episodes{$i}{episodenum}\n");
    }
    close(OUT);
}



####################################################
#### These functions don't really belong here, they
#### belong in MiscUtil.pm. However, they use some
#### of the variables in this package. I didn't want
#### to have a cross-dependency between the two
#### packages, so I put the functions in here.
####################################################


# Get rid of any Webdir, Extradir or Datadir files
# that are older than 14 days.
#
sub Cleanup_Old_Files() {
  my ($Webdir, $Datadir, $Extradir);
  my ($result, $file);

  $Webdir=   $ConfigVar{Webdir};
  $Datadir=  $ConfigVar{Datadir};
  $Extradir= $ConfigVar{Extradir};

  # Clean out files in the Datadir which are older than 14 days
  $result= opendir(DIR, $Datadir);
  if ($result) {
    foreach $file (readdir(DIR)) {
      if ((-f "$Datadir/$file") && (-M "$Datadir/$file" > 14)) {
	unlink("$Datadir/$file");
      }
    }
  }

  # Clean out files in the Webdir which are older than 14 days
  $result=0;
  if (defined($Webdir)) { $result= opendir(DIR, $Webdir); }
  if ($result) {
    foreach $file (readdir(DIR)) {
      if ((-f "$Webdir/$file") && (-M "$Webdir/$file" > 14)) {
	unlink("$Webdir/$file");
      }
    }
  }

  # Clean out files in the Extradir which are older than 14 days
  $result=0;
  if (defined($Extradir)) { $result= opendir(DIR, $Extradir); }
  if ($result) {
    foreach $file (readdir(DIR)) {
      if ((-f "$Extradir/$file") && (-M "$Extradir/$file" > 14)) {
	unlink("$Extradir/$file");
      }
    }
  }
}

# Genre_To_Numeric: Convert a comma-separated list
# of textal genres to an array of integers.
# If second arg is 1, convert Unknown to Curling
#
sub Genre_To_Numeric($$)
{
  my ($input, $curling)=@_;
  my (@genre_set);
  my ($numeric_genre);
  my ($i, @result);
   
  @genre_set= split(/,/, $input);
  for ($i=0;$i<=$#genre_set;$i++) {
    if (!defined($NumericGenre{$genre_set[$i]})) {
      print(STDERR "Unknown genre $genre_set[$i] in Genre_To_Numeric\n");
    } else {
      $numeric_genre= $NumericGenre{$genre_set[$i]};
      # This is where we set 0 back to 31, i.e. Curling
      if (($numeric_genre==0) && ($curling==1)) { $numeric_genre=31; }
    }
    push(@result, $numeric_genre);
  }
  return(@result);
}

# Determine the name of the output file.
# We need 2 params, the start day (e.g 0)
# and the end day (e.g 6).
#
sub Find_Outfilename($$)
{
  my ($start,$end)= @_;
  my ($slice_filename, $slice_format);
  my ($startdaynum, $enddaynum);
  my ($Outputformat);

  $Outputformat = $ConfigVar{Outputformat};

  if ($Outputformat eq "slice") {
    $slice_format= $ConfigVar{Slicefile};
  } elsif ($Outputformat eq "text") {
    $slice_format= $ConfigVar{Textfile};
  } else {
    print("Outputformat = $Outputformat is unrecognised in config file\n");
    exit(1);
  }

  if (!defined($slice_format)) {
    print("The output file name that matches the $Outputformat format\n");
    print("is not defined in the config file\n");
    exit(1);
  }

  # Now use it to generate the real slice file name
  $startdaynum= Get_Daynumber($start);
  $enddaynum= Get_Daynumber($end);
  $slice_filename= sprintf($slice_format, $startdaynum, $enddaynum);
  return($slice_filename);
}


1;
