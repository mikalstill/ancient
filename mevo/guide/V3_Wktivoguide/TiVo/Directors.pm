#!/usr/bin/perl
# 
# Directors.pm: Routines to load and use the directors database.
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
package TiVo::Directors;
use strict;
use TiVo::ConfigFiles;
use TiVo::MiscUtil;

BEGIN {
  use Exporter	 ();
  use vars	 qw($VERSION @ISA @EXPORT @EXPORT_OK %EXPORT_TAGS);

  $VERSION = do { my @r = (q$Revision: 1.1.1.1 $ =~ /\d+/g); sprintf "%d."."%02d" x $#r, @r }; # must be all one line, for MakeMaker

  @ISA	       = qw(Exporter);
  @EXPORT      = qw(Read_Directors Find_Director);
  %EXPORT_TAGS = ();
  @EXPORT_OK   = qw();
}

use vars	 @EXPORT_OK;
END { }	 # module clean-up code here (global destructor)


#####################################
###### Package-private Globals ######
#####################################

my ($directors_file)= "directors.tivo.gz";
my (%Directors, %YearDirectors);

######################
###### Routines ######
######################

# Read_Directors: read the directors from the directors.tivo.gz file.
sub Read_Directors
{
  my ($Configdir);
  my ($title,$year,$director);

  # Don't do anything if there is no directors file
  $Configdir= $ConfigVar{"Configdir"};
  if (! -f "$Configdir/$directors_file") {
    Debug("No directors file $Configdir/$directors_file!\n");
    return(1);
  }

  print("Loading directors file...\n");
  unless (open(RIN, "zcat $Configdir/$directors_file |")) {
    print(STDERR "Cannot open $Configdir/$directors_file: $!\n");
    return(0);
  }

  # Walk the file obtaining per-movie directors
  while (<RIN>) {
   chop;
   ($title,$director,$year)= split(/\t/);
   if (defined($year)) {
     $YearDirectors{$title.$year}= $director;
   }
   $Directors{$title}= $director;
  }
  close(RIN); return(1);
}

sub Find_Director($;$)
{
  my ($title,$year)= @_;
  # Debug("Looking up director for $title\n");

  if (defined($year) && defined($YearDirectors{$title.$year})) {
    Debug("Found yeardirector $YearDirectors{$title.$year} for $title\n");
    return($YearDirectors{$title.$year});
  } else {
    if (defined($Directors{$title})) {
      Debug("Found director $Directors{$title} for $title\n");
      return($Directors{$title});
    }
  }
  return(undef);
}

1;
