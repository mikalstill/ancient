#!/usr/bin/perl
# 
# SliceBuilder.pm: Convert intermediate guide data into TiVo slice format.
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
package TiVo::SliceBuilder;
use strict;
use TiVo::ConfigFiles;
use TiVo::MiscUtil;
use TiVo::AugmentData;
use TiVo::TiVoSlice;


BEGIN {
  use Exporter	 ();
  use vars	 qw($VERSION @ISA @EXPORT @EXPORT_OK %EXPORT_TAGS);

  $VERSION = do { my @r = (q$Revision: 1.1.1.1 $ =~ /\d+/g); sprintf "%d."."%02d" x $#r, @r }; # must be all one line, for MakeMaker

  @ISA	       = qw(Exporter);
  @EXPORT      = qw(Build_Slice_For Initialise_Slice_Output
		    Finalise_Slice_Output);
  %EXPORT_TAGS = ( );
  @EXPORT_OK   = qw();
}

use vars	 @EXPORT_OK;
END { }	 # module clean-up code here (global destructor)


#####################################
###### Package-private Globals ######
#####################################

my ($Outputformat, @Showing, $NoNagflag);
my (%AlreadyDoneSeries);

######################
###### Routines ######
######################

# Load data from intermediate files, find which programs
# don't have a genre, find their genre, build up a
# data structure which will allow us to write a slice.
#
sub Build_Slice_For($$$)
{
  my ($chan,$snum,$enum)= @_;
  my ($orig_chan)= $chan;
  my ($i, $wanteday, $wanted_daynum, $dayname, $infile, $title);
  my ($do_timeshift, $time, $daynum, $sdaynum, $edaynum, $result);
  my ($Extradir)=  $ConfigVar{Extradir};
  my ($NoNagchan)= $ConfigVar{NoNag};

  $sdaynum= Get_Daynumber($snum);
  $edaynum= Get_Daynumber($enum);
  undef(@Showing); $result=1; $NoNagflag=0;

  # If we are doing the NoNag channel, only do one day
  if ($chan eq "NoNag") {
    $enum= $snum; $edaynum= $sdaynum;
    $NoNagflag=1; $chan= $NoNagchan;
  }

  # Determine if we need to timeshift the original data
  $do_timeshift=0;
  if (($DestinationTimezone{$chan} ne "DoNotShift") &&
      ($DestinationTimezone{$chan} ne $SourceTimezone{$chan})) {
        Debug("Looks like a timeshifter for $chan: " .
          $SourceTimezone{$chan} . " => " .
          $DestinationTimezone{$chan} . "\n");
        $do_timeshift=1;
  }

  # Do each day in turn, do the Series and Program records
  for ($wanteday= $snum; $wanteday <= $enum; $wanteday++) {

    $dayname= Get_Dayname($wanteday);
    $wanted_daynum= Get_Daynumber($wanteday);
    Debug("Building slice for $orig_chan for $dayname ($wanted_daynum)\n");
    $infile= "$Extradir/$chan" . "_$dayname";

    # If the Extradir doesn't exists, or we can't read the data from
    # the augmented data file, then read the normal data file and
    # augment it in memory.
    if (!defined($Extradir) || (Read_IntermediateFile($infile)==0)) {
       Debug("No augmented data for $chan on $dayname, using Data/\n");
       unless (Augment_Data($chan,$wanteday,$wanteday,0)) {
         Debug("Unable to read data for $chan on $dayname\n");
	 print(STDERR "Unable to read data for $chan on $dayname\n");
	 $result=0; next;
       }
    }

    # Now work on each title in turn. All we have to do is to
    # do any timeshifting and add the episode-id and the episode number.
    #
    for ($i=0; $i <= $#ProgramData; $i++) {
      # Find a matching episode-id, or make one up
      if ($ProgramData[$i]{episodic}==1) {
	$title= $ProgramData[$i]{title};
	if (!defined($Episodes{$title})) {
	  Debug("No episode data for $title\n");
	  $Episodes{$title}{series}= $Numbers{series};
	  $Numbers{series}++;
	  Debug("Allocated $Episodes{$title}{series} to $i $title, saved\n");
	  $Episodes{$title}{episodenum}= 0;
        }
	$ProgramData[$i]{series}= $Episodes{$title}{series};
	$ProgramData[$i]{episodenum}= $Episodes{$title}{episodenum}++;
      }

      $daynum= $ProgramData[$i]{daynum};
      # Timeshift if required
      if ($do_timeshift==1) {
	$daynum= $ProgramData[$i]{daynum};
	$time= $ProgramData[$i]{time};
	$title= $ProgramData[$i]{title};
        Debug("Before shift we have $daynum $time $title\n");
        ($daynum,$time)= ZoneConvert($daynum,$time,
        	$SourceTimezone{$chan},$DestinationTimezone{$chan});
        Debug("After shift we have $daynum $time\n");
	$ProgramData[$i]{daynum}=$daynum;
	$ProgramData[$i]{time}=$time;
      }

      if (($daynum>=$sdaynum) && ($daynum<=$edaynum)) {

	# Move it into the future if NoNag
	#
	if ($NoNagflag==1) { $ProgramData[$i]{daynum}+= 14; }

	if ($Outputformat eq "slice") {
	  Print_Series_Record_AsSlice($i);
	  Print_Program_Record_AsSlice($i);
	} else {
          Print_Series_Record_AsText($i);
	  Print_Program_Record_AsText($i);
	}

        # We now push this record so that we can print out
        # a stationday later.
        push(@Showing, $ProgramData[$i]);
      }
    }
  }
  if ($Outputformat eq "slice") {
    Print_Showings_AsSlice($orig_chan,$snum,$enum);
  } else {
    Print_Showings_AsText($orig_chan,$snum,$enum);
  }
  return($result);
}


###########################
###### Text Routines ######
###########################

sub Print_Series_Record_AsText($)
{
  my ($index)= @_;
  my (@nlist, $snum, $i);

  # Convert the genres to numbers
  @nlist= Genre_To_Numeric($ProgramData[$index]{genre},1);

  # If this program is listed as an episode of a series,
  # then snum is the series number for this show, otherwise
  # generate a unique one just for this program
  #
  if ($ProgramData[$index]{episodic}==1) {
    $snum= $ProgramData[$index]{series};
    Debug("Allocated $snum to $index $ProgramData[$index]{title}, existing2\n");
  } else {
    $ProgramData[$index]{series}= $snum= $Numbers{series};
    $Numbers{series}++;
    Debug("Allocated $snum to $index $ProgramData[$index]{title}, non-episodic2\n");
  }

  # Don't print out this record if we have already done it
  #
  if (defined($AlreadyDoneSeries{$snum})) { return; }
  $AlreadyDoneSeries{$snum}=1;

  print(OUT "Series/1/$snum/$Numbers{slice}; {\n");
  foreach $i (@nlist) {
    print(OUT "\tGenre: $i\n");
  }
  print(OUT "\tTitle: {$ProgramData[$index]{title}}\n");
  print(OUT "\tEpisodic: $ProgramData[$index]{episodic}\n");
  print(OUT "}\n\n");
}

sub Print_Program_Record_AsText($)
{
  my ($index)= @_;
  my ($d, $i, $act);
  my (@nlist, @advisory_set, @actor_set);

  print(OUT "Program/1/$Numbers{program}/$Numbers{slice} {\n");
  $ProgramData[$index]{program}= $Numbers{program};
  $Numbers{program}++;

  # Trim the description. Too big for TiVo display, otherwise.
  $d= substr($ProgramData[$index]{description},0,198);
  print(OUT "\tSeries: Series/1/$ProgramData[$index]{series}\n"); 
  if (defined($ProgramData[$index]{advisory})) {
    @advisory_set= split(/,/, $ProgramData[$index]{advisory});
    foreach $i (@advisory_set) {
      print(OUT "\tAdvisory: $i\n"); 
    }
  }
  if (defined($ProgramData[$index]{actor})) {
    @actor_set= split(/,/, $ProgramData[$index]{actor});
    foreach $act (@actor_set) {
      if (!defined($act))  { next; }
       if ($act eq "")      { next; }
       if ($act eq " and ") { next; }
       print(OUT "\tActor: {$act}\n"); 
     }
   }

   @nlist= Genre_To_Numeric($ProgramData[$index]{genre},1);
   foreach $i (@nlist) {
     print(OUT "\tGenre: $i\n");
   }
   if ($ProgramData[$index]{episodic} == 1) {
     print(OUT "\tEpisodeNum: $ProgramData[$index]{episodenum}\n");
   }
   print(OUT "\tTitle: {$ProgramData[$index]{title}}\n");
   if ($ProgramData[$index]{episodic} == 1) {
     print(OUT "\tEpisodeTitle: {$ProgramData[$index]{episode}}\n");
   }
   print(OUT "\tDescription: {$d}\n");

   if (defined($ProgramData[$index]{year})) {
     print(OUT "\tMovieYear: $ProgramData[$index]{year}\n");
   }
   if (defined($ProgramData[$index]{starrating})) {
    print(OUT "\tStarRating: $ProgramData[$index]{starrating}\n");
   }
   if (defined($ProgramData[$index]{director})) {
    print(OUT "\tDirector: $ProgramData[$index]{director}\n");
   }
   if ($ProgramData[$index]{episodic} == 1) {
     print(OUT "\tIsEpisode: 1\n");
   }
   print(OUT "}\n\n");
}

sub Print_Showings_AsText($$$)
{
  my ($chan,$snum,$enum)= @_;
  my ($wanteday, $wanted_daynum, $i, $j, $index);
  my (@ShowingList);

  for ($wanteday= $snum; $wanteday <= $enum; $wanteday++) {
    undef(@ShowingList);
    $wanted_daynum= Get_Daynumber($wanteday);

    # Fix wanted_daynum if NoNag
    if ($NoNagflag==1) { $wanted_daynum+= 14; }

    # Build a list of showings for this day
    for ($i=0; $i<= $#Showing; $i++) {
      if ($Showing[$i]{daynum} == $wanted_daynum) {
        # Debug("Prog $Showing[$i]{title} on $wanted_daynum\n");
	push(@ShowingList, $i);
      }
    }

    # We now have a list of Showings to print out. Print out
    # the details of this StationDay.
    print(OUT "StationDay/1/$Numbers{stationday}/$Numbers{slice} \173\n");
    print(OUT "\tStation: $StationId{$chan}\n");
    print(OUT "\tDay: $wanted_daynum\n");
    $Numbers{stationday}++;

    # Print out the number of showings for today.
    for ($i=0; $i<= $#ShowingList; $i++) {
      $j=$i+11;
      print(OUT "\tShowing: Showing/$j\n");
    }

    # Now print out each showing.
    for ($i=0; $i<= $#ShowingList; $i++) {
      $j=$i+11;
      $index=$ShowingList[$i];
      print(OUT "\tSubrecord Showing/$j \173\n");
      print(OUT "\t\tProgram: Program/1/$Showing[$index]{program}\n");
      print(OUT "\t\tStation: $StationId{$chan}\n");
      print(OUT "\t\tBits: $Showing[$index]{bits}\n")
					if defined($Showing[$index]{bits});
      print(OUT "\t\tDuration: $Showing[$index]{duration}\n");
      print(OUT "\t\tDate: $wanted_daynum\n");
      print(OUT "\t\tTvRating: $Showing[$index]{tvrating}\n")
					if defined($Showing[$index]{tvrating});
      print(OUT "\t\tTime: $Showing[$index]{time}\n");
      print(OUT "\t\175\n");
    }
    print(OUT "\175\n\n");
  }
}


############################
###### Slice Routines ######
############################

sub Print_Series_Record_AsSlice($)
{
  my ($index)= @_;
  my (@nlist, $snum, $i, $numericgenre);
  my (%SeriesTiVo);

  # Convert the genres to numbers
  @nlist= Genre_To_Numeric($ProgramData[$index]{genre},1);

  # If this program is listed as an episode of a series,
  # then snum is the series number for this show, otherwise
  # generate a unique one just for this program
  #
  if ($ProgramData[$index]{episodic}==1) {
    $snum= $ProgramData[$index]{series};
    Debug("Allocated $snum to $index $ProgramData[$index]{title}, existing\n");
  } else {
    $ProgramData[$index]{series}= $snum= $Numbers{series};
    $Numbers{series}++;
    Debug("Allocated $snum to $index $ProgramData[$index]{title}, new\n");
  }

  # Don't print out this record if we have already done it
  #
  if (defined($AlreadyDoneSeries{$snum})) { return; }
  $AlreadyDoneSeries{$snum}=1;

  $SeriesTiVo{IsEpisodic}= $ProgramData[$index]{episodic};
  $SeriesTiVo{Title} = $ProgramData[$index]{title};
  $numericgenre= join('|', @nlist);
  $SeriesTiVo{Genre}= $numericgenre;
  TiVo::SliceWriteRecord("series", $snum, $Numbers{slice}, \%SeriesTiVo);
}

sub Print_Program_Record_AsSlice($)
{
  my ($index)= @_;
  my (%ProgramTiVo);
  my ($d, $i, $numericgenre, $act);
  my (@nlist, @actor_set, @advisory_set);

  $ProgramTiVo{"Title"}= $ProgramData[$index]{title};
  # Too big for TiVo display, otherwise
  $d= substr($ProgramData[$index]{description},0,198);
  $ProgramTiVo{"Description"}= $d;
  $ProgramTiVo{"Index"}= "4,1,$ProgramData[$index]{series}";

  if ($ProgramData[$index]{episodic} == 1) {
    $ProgramTiVo{"IsEpisode"}= 1;
    $ProgramTiVo{"EpisodeNum"}= $ProgramData[$index]{episodenum};
    $ProgramTiVo{"EpisodeTitle"}= $ProgramData[$index]{episode};
  }

  if (defined($ProgramData[$index]{year})) {
    $ProgramTiVo{"date"}= $ProgramData[$index]{year};
  }
  if (defined($ProgramData[$index]{director})) {
    $ProgramTiVo{"director"}= $ProgramData[$index]{director}
  }

  # Convert the genres to numbers
  @nlist= Genre_To_Numeric($ProgramData[$index]{genre},1);
  $numericgenre= join('|', @nlist);
  $ProgramTiVo{Genre}= $numericgenre;

  if (defined($ProgramData[$index]{actor})) {
    @actor_set= split(/,/, $ProgramData[$index]{actor});
    foreach $act (@actor_set) {
      if (!defined($act))  { next; }
      if ($act eq "")      { next; }
      if ($act eq " and ") { next; }
      $a = substr($act, 0, 20);
      $ProgramTiVo{"Actor"} .= "|$a";
      $ProgramTiVo{"Actor"} =~ s/^\|//;
    }
  }

  if (defined($ProgramData[$index]{advisory})) {
    @advisory_set= split(/,/, $ProgramData[$index]{advisory});
    foreach $i (@advisory_set) {
      $ProgramTiVo{"Advisory"} .= "|$i";
      $ProgramTiVo{"Advisory"} =~ s/^\|//;
    }
  }
  TiVo::SliceWriteRecord("program", $Numbers{program}, $Numbers{slice},
							\%ProgramTiVo);
  $ProgramData[$index]{program}= $Numbers{program};
  $Numbers{program}++;
}

sub Print_Showings_AsSlice($$$)
{
  my ($chan,$snum,$enum)= @_;
  my ($wanteday, $wanted_daynum, $i, $j, $index);
  my (@ShowingList, %SlotDetail, %TiVoStationDay);

  for ($wanteday= $snum; $wanteday <= $enum; $wanteday++) {
    undef(@ShowingList);
    $wanted_daynum= Get_Daynumber($wanteday);

    # Fix wanted_daynum if NoNag
    if ($NoNagflag==1) { $wanted_daynum+= 14; }

    # Build a list of showings for this day
    for ($i=0; $i<= $#Showing; $i++) {
      if ($Showing[$i]{daynum} == $wanted_daynum) {
        # Debug("Prog $Showing[$i]{title} on $wanted_daynum\n");
	push(@ShowingList, $i);
      }
    }

    # We now have a list of Showings to print out. Print out
    # the details of this StationDay.
    $TiVoStationDay{Day} = $wanted_daynum * 86400;
    $TiVoStationDay{ShowList} = $#ShowingList + 1;

    # We want the serverID part of $StationId{$chan}
    (undef,undef,my $ch) = split(/\//,$StationId{$chan});
    $TiVoStationDay{Index1} = "5,1,$ch";
    TiVo::SliceWriteRecord("stationday", $Numbers{stationday},
					$Numbers{slice}, \%TiVoStationDay);
    $Numbers{stationday}++;

    # Now print out each showing.
    for ($i=0; $i<= $#ShowingList; $i++) {
      $index=$ShowingList[$i];
      undef %SlotDetail;
      $SlotDetail{Date}     = $wanted_daynum * 86400;
      $SlotDetail{Time}     = $Showing[$index]{time};
      $SlotDetail{Duration} = $Showing[$index]{duration};
      $SlotDetail{Bits}     = $Showing[$index]{bits} if
                                        defined($Showing[$index]{bits});
      $SlotDetail{TVRating} = $Showing[$index]{tvrating} if
                                        defined($Showing[$index]{tvrating});
      $SlotDetail{Index1}   = "3,1,$Showing[$index]{program}";
      $SlotDetail{Index2}   = "5,1,$ch";
      TiVo::SliceWriteRecord("SlotDetail", $i+1, $Numbers{slice}, \%SlotDetail);
    }
  }
}

###############################################
###### Slice initialisation & finalising ######
###############################################

sub Initialise_Slice_Output {
  my($slice_file)= @_;
  $Outputformat= $ConfigVar{Outputformat};

  if ($Outputformat eq "slice") {
    TiVo::SliceOpen($slice_file,1.3,my $SliceDebug=0);
  } else { 
    open(OUT, "> $slice_file") ||
        die("Cannot create output file $slice_file: $!\n");
    #
    # Print the initial header
    print(OUT "Guide type=3\n\n");
  }
}

sub Finalise_Slice_Output {
  if ($Outputformat eq "slice") { TiVo::SliceClose(); }
  else { close(OUT); }
  $Numbers{slice}++;
}


1;
