#!/usr/bin/perl
# 
# AugmentData.pm: Augment data in intermediate files to speed up slice making.
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
package TiVo::AugmentData;
use strict;
use TiVo::ConfigFiles;
use TiVo::MiscUtil;
use TiVo::ProgramDb;
use TiVo::Ratings;
use TiVo::Directors;
use TiVo::ProgramGenre;
use HTTP::Lite;
use CGI::Enurl;


BEGIN {
  use Exporter	 ();
  use vars	 qw($VERSION @ISA @EXPORT @EXPORT_OK %EXPORT_TAGS);

  $VERSION = do { my @r = (q$Revision: 1.1.1.1 $ =~ /\d+/g); sprintf "%d."."%02d" x $#r, @r }; # must be all one line, for MakeMaker

  @ISA	       = qw(Exporter);
  @EXPORT      = qw(Augment_Data);
  %EXPORT_TAGS = ( );
  @EXPORT_OK   = qw();
}

use vars	 @EXPORT_OK;
END { }	 # module clean-up code here (global destructor)


#####################################
###### Package-private Globals ######
#####################################


######################
###### Routines ######
######################

# Load data from intermediate files, find which programs
# don't have a genre, find their genre, rewrite the file.
#
sub Augment_Data($$$$)
{
  my ($chan,$snum,$enum,$write_files)= @_;
  my ($wanteday, $dayname, $infile, $outfile);
  my ($match, $newtitle, $genrelist, $is_episodic);
  my ($i, $found, $title, $leftovertitle);
  my ($g, $rating, $startitle, $result, $director);
  my (%AlreadyDone);

  $result=1;
  # We need the %Programs database to be loaded, but it
  # is possible that make_slice will run and call us
  # without loading the database. So we test and load it
  # if required.
  if (!defined(%Programs)) {
    Debug("Loading Programs database in Augment_Data!\n");
    Read_Programs;
  }

  # Do each day in turn
  for ($wanteday= $snum; $wanteday <= $enum; $wanteday++) {

    $dayname= Get_Dayname($wanteday);
    $infile= "$ConfigVar{Datadir}/$chan" . "_$dayname";
    $outfile= "$ConfigVar{Extradir}/$chan" . "_$dayname";

    # Don't re-augment if the outfile exists
    if (-f $outfile ) {
	Debug("Not re-augmenting $outfile, it already exists\n");
	next;
    }

    # If no data found, and we are only doing 1 day, return an error.
    # Otherwise, just loop so that we don't write out the file.
    unless (Read_IntermediateFile($infile)) {
      Debug("No data found to augment for $chan on $dayname\n");
      if ($snum==$enum) { return(0); } else { next; }
    }
    Debug("Read intermediate data from $infile\n");

    # Now work on each title in turn
    for ($i=0; $i <= $#ProgramData; $i++) {

      # Always lowercase the title, so even if we don't
      # find it, it will still look good. Also get the
      # genre out of the file, in case it is a movie.
      $title= $ProgramData[$i]{title};
      $genrelist= $ProgramData[$i]{genre};

      ($title)= LowercaseString($title);
      $ProgramData[$i]{title}= $title;

      # See if we have some results for this title already
      if (defined($AlreadyDone{$title})) {
	Debug("We have already augmented $title\n");
	$ProgramData[$i]{title}= $AlreadyDone{$title}{title};
	$ProgramData[$i]{genre}= $AlreadyDone{$title}{genre};
	next;
      }

      # Nothing in the cache, so let's try the trimtitles first
      #
      $found=0;
      Debug("About to blah blah for $newtitle\n");
      ($match, $newtitle, $g, $is_episodic)= Find_Trimtitle($title);
      if ($match==1) {
	$found=1;
	$genrelist= $g;		# Have to do this in case match==0
	$ProgramData[$i]{title}= $newtitle;
	$ProgramData[$i]{genre}= $genrelist;
	$ProgramData[$i]{episodic}= $is_episodic;
      } else {
	$newtitle= $title;
      }

      # If we have star ratings, search for a movie's star ratings
      Debug("About to find star rating for $newtitle\n");
      if ($genrelist=~ /Movie/) {
        $rating= Find_Rating($newtitle,$ProgramData[$i]{year});
        if (defined($rating)) {
          Debug("Found star rating $rating for $newtitle\n");
          $ProgramData[$i]{starrating}= $rating;
        }
        $director= Find_Director($newtitle,$ProgramData[$i]{year});
        if (defined($director)) {
          Debug("Found director $director for $newtitle\n");
          $ProgramData[$i]{director}= $director;
        }
      }

      # Even if we found something in the trimtitles, we still
      # search for it in the programs database, in case the
      # genre is better. We use $newtitle in case something was
      # trimmed from the original title.
     ($match, $newtitle, $leftovertitle, $genrelist)= Find_Program($newtitle);
      if ($match==1) {
	Debug("Found match, saving $newtitle vs. $title in Augment\n");
	$found=1;
	$ProgramData[$i]{title}= $newtitle;
	$ProgramData[$i]{genre}= $genrelist;
	$ProgramData[$i]{episode}= $leftovertitle
				if (!defined($ProgramData[$i]{episode}));
	$ProgramData[$i]{episodic}= 1;
      }

      # If we found a non-episodic program, cache it for later.
      # We don't cache episodic programs, as we want to get
      # different episode names.
      if (!defined($ProgramData[$i]{episodic}) ||
			($ProgramData[$i]{episodic}==0)) {
	$AlreadyDone{$title}{title}= $ProgramData[$i]{title};
	$AlreadyDone{$title}{genre}= $ProgramData[$i]{genre};
      }
    }

    # All done, write the new file out.
    if ($write_files==1) {
      unless (Write_IntermediateFile($outfile)) {
        print(STDERR "Unable to write to file $infile\n");
	$result=0;
      }
    }
  }
  return($result);
}

1;
