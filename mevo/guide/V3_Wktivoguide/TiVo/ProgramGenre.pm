#!/usr/bin/perl
# 
# ProgramGenre.pm: Routines to use the programs database, and to determine
# the genre of a given program.
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
package TiVo::ProgramGenre;
use strict;
use TiVo::ConfigFiles;
use TiVo::MiscUtil;
use TiVo::ProgramDb;

BEGIN {
  use Exporter	 ();
  use vars	 qw($VERSION @ISA @EXPORT @EXPORT_OK %EXPORT_TAGS);

  $VERSION = do { my @r = (q$Revision: 1.1.1.1 $ =~ /\d+/g); sprintf "%d."."%02d" x $#r, @r }; # must be all one line, for MakeMaker

  @ISA	       = qw(Exporter);
  @EXPORT      = qw(Find_Trimtitle Find_Program);
  %EXPORT_TAGS = ( );
  @EXPORT_OK   = qw();
}

use vars	 @EXPORT_OK;
END { }	 # module clean-up code here (global destructor)


######################
###### Routines ######
######################

# Apply the TrimTitle list to the given title
# and if there is a match, return the new title,
# the genre and if the program is episodic.
sub Find_Trimtitle($)
{
  my ($title)= @_;
  my ($trimtitle, $proglen);
  my ($match, $newtitle, $genrelist, $is_episodic);
  $match=0;

  # Convert the title to mostly lowercase
  ($title)= &LowercaseString($title);

  # Trim any part of the title from the beginning, and
  # perhaps get an idea of the genre from this bit.
  # Also remove leading whitespace, : and -
  foreach $trimtitle (keys(%TrimTitle)) {
    if ( $title =~ /^\Q$trimtitle\E/i ) {
      $proglen= length($trimtitle);
      $newtitle= substr($title, $proglen);
      $newtitle=~ s/^[-\s:]+//;
      $genrelist=$TrimTitle{$trimtitle}{genre};
      $is_episodic= $TrimTitle{$trimtitle}{episodic};
      $match=1; last;
    }
  }
  return($match, $newtitle, $genrelist, $is_episodic);
}

# Apply the %Programs and %Partialprograms lists
# to the title to find out its genre. If there is
# a match, return the correct title, anything leftover
# from the title and the genre.
sub Find_Program($)
{
  my ($title)= @_;
  my ($match, $newtitle, $leftovertitle, $genrelist, $i);

  # Convert the title to mostly lowercase
  ($title)= &LowercaseString($title);

  # Try to find an exact match
  if (defined($Programs{$title})) {
    Debug("Found exact match for $title\n");
    $newtitle = $Programs{$title}{title};
    $leftovertitle= "";
    $genrelist= $Programs{$title}{genre};
    return(1,$newtitle,$leftovertitle,$genrelist);
  }

  # An exact match failed, so we try a partial match now
  #
  $match=0;
  foreach $i (@SortedPrograms) {
    if ( $title =~ /^\Q$i\E/ ) {
      Debug("Found partial match $i vs $title\n");
      $newtitle = $PartialPrograms{$i}{title};
      # Trim off the title to get the leftover title. At
      # the same time remove leading whitespace, : and -
      $leftovertitle= substr($title, length($i));
      $leftovertitle=~ s/^[-\s:]+//;
      Debug("Set leftover $leftovertitle for $title\n");
      $genrelist= $PartialPrograms{$i}{genre};
      $match=1; last;
    }
  }
  if ($match==0) { Debug("No programs match for $title\n"); }
  return($match,$newtitle,$leftovertitle,$genrelist);
}

1;
