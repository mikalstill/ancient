#!/usr/bin/perl
# 
# ProgramDb.pm: Routines to load, modify and save the programs database
#		and the ratings database.
#		This is the file version of the package.
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
package TiVo::ProgramDb;
use strict;
use TiVo::ConfigFiles;
use TiVo::MiscUtil;

BEGIN {
  use Exporter	 ();
  use vars	 qw($VERSION @ISA @EXPORT @EXPORT_OK %EXPORT_TAGS);

  $VERSION = do { my @r = (q$Revision: 1.1.1.1 $ =~ /\d+/g); sprintf "%d."."%02d" x $#r, @r }; # must be all one line, for MakeMaker

  @ISA	       = qw(Exporter);
  @EXPORT      = qw(Read_Programs Create_Program Save_Programs
		    %Programs %PartialPrograms @SortedPrograms);
  %EXPORT_TAGS = ();
  @EXPORT_OK   = qw(%Programs %PartialPrograms @SortedPrograms);
}

use vars	 @EXPORT_OK;
END { }	 # module clean-up code here (global destructor)


#####################################
###### Package-private Globals ######
#####################################

my ($programs)=   "programs";

######################
###### Routines ######
######################

# Open the list of known programs, and read it in
# Results are stored in the %Programs and
# %PartialPrograms hashes. No return value.
sub Read_Programs
{
  my ($prog, $genre, $partial, $i, $lcprog, @genre_set);
  my ($Configdir);


  if (defined($ConfigVar{Programs})) {
    open(IN, $ConfigVar{Programs}) ||
                                die("Cannot read $ConfigVar{Programs}: $!\n");
  } else {
    $Configdir= $ConfigVar{Configdir};
    open(IN, "$Configdir/$programs") ||
                                die("Cannot read $Configdir/$programs: $!\n");
  }
  while(<IN>) {
    chop;
    if (/^#/) { next; }         # Ignore comment lines and blank lines
    if (/^\s*$/) { next; }
    ($prog,$genre)= split(/\t+/);

    # Find out if this is a full match or a partial match
    $partial=0;
    if ($prog=~ /(.*)\|P/) {
      $prog=$1; $partial=1;
    }

    # Convert the title into mostly lowercase, so
    # that we do all comparisons case-insensitively.
    ($lcprog)= &LowercaseString($prog);

    # Check that the genres are actually defined
    #
    @genre_set= split(/[,\s]/,$genre);
    for ($i=0;$i<=$#genre_set;$i++) {
      if (!defined($NumericGenre{$genre_set[$i]})) {
        print(STDERR "Unknown genre $genre_set[$i] for $prog in $programs\n");
      } 
    }

    if ($partial==1) {
      $Programs{$lcprog}{title}=$prog;
      $Programs{$lcprog}{genre}=$genre;
      $Programs{$lcprog}{partial}=1;
      $PartialPrograms{$lcprog}{title}=$prog;
      $PartialPrograms{$lcprog}{genre}=$genre;
    } else {
      $Programs{$lcprog}{title}=$prog;
      $Programs{$lcprog}{genre}=$genre;
      $Programs{$lcprog}{partial}=0;
    }
  }
  close(IN);

  # Finally, build a list of partial titles sorted by
  # string length descending.
  @SortedPrograms= sort Strlensort (keys(%PartialPrograms));
}

# When dealing with a file, we don't have to do much here.
#
sub Create_Program($)
{
  my ($title)= @_;
  Debug("Creating program $title\n");
  Debug("  Title: $Programs{$title}{title}\n");
  Debug("  Partial: $Programs{$title}{partmatch}\n");
  Debug("  Genres: $Programs{$title}{genre}\n");
  Debug("  Channel: $Programs{$title}{channel}\n");
  Debug("  Time: $Programs{$title}{time}\n");
  Debug("  Duration: $Programs{$title}{duration}\n\n");
}

# Write out the Programs hash to the programs file.
#
sub Save_Programs
{
  my ($i, $Configdir);

  if (defined($ConfigVar{Programs})) {
    open(OUT, "> $ConfigVar{Programs}") ||
                                die("Cannot write $ConfigVar{Programs}: $!\n");
  } else {
    $Configdir= $ConfigVar{Configdir};
    open(OUT, "> $Configdir/$programs") ||
                                die("Cannot write $Configdir/$programs: $!\n");
  }

  print(OUT "#\n# List of known program titles and their categories\n#\n");
  print(OUT "# The first column holds the title of the program. The\n");
  print(OUT "# second column holds a comma-separated genre list. See\n");
  print(OUT "# the genretypes for the set of possible genre categories. If\n");
  print(OUT "# the title ends with a |P, then partial matching is done.\n#\n");

  foreach $i (sort(keys(%Programs))) {
    print(OUT "$Programs{$i}{title}");
    if (defined($Programs{$i}{partial}) && ($Programs{$i}{partial}==1)) {
      print(OUT "|P");
    }
    print(OUT "\t\t$Programs{$i}{genre}\n");
  }
  close(OUT);
}

1;
