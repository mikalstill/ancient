#!/usr/bin/perl
# 
# ProgramDb.pm: Routines to load, modify and save the programs database.
#		This is the SQL interface version of the package.
#
# PLEASE FILL IN YOUR MySQL ACCESS INFORMATION BELOW WHERE INDICATED!!!!
#
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
use DBI;
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

###################################
###### Database Schemas Used ######
###################################

# The programs database uses this schema.
#
# id: unique id for each program
# title: program's title
# partmatch: show we do a partial match, if 1 then yes, if 0 then on.
# primgenre, secgenre: two possible TiVo genres for the program
# primgengrp, secgengrp: two possible TiVo genre groups for the program
# status: I is waiting for insert, N is normal, R is requested to be deleted,
#	D is deleted.
# who and changed: who changed the record last, and when
# channel, time, duration stores when the program was on & duration (in seconds)
# and what channel it was first on.
# 
# CREATE TABLE programs (
#   id INT UNSIGNED DEFAULT 0 AUTO_INCREMENT PRIMARY KEY,
#   title VARCHAR(64) BINARY,
#   partmatch TINYINT UNSIGNED DEFAULT 0,
#   primgenre INT UNSIGNED DEFAULT 0,
#   primgengrp INT UNSIGNED,
#   secgenre INT UNSIGNED,
#   secgengrp INT UNSIGNED,
#   status ENUM ('I','N','R','D') DEFAULT 'N',
#   who VARCHAR(16),
#   changed DATETIME,
#   channel VARCHAR(16) DEFAULT "???",
#   time INT UNSIGNED DEFAULT 0,
#   duration INT UNSIGNED DEFAULT 0
# );


#####################################
###### Package-private Globals ######
#####################################

# MySQL access information.
#
# XXXXXXX You need to change:
#	- the hostname on which the MySQL server runs
#	- the MySQL user you will use to access the database
#	- the name of the database table
#	- your password, which is left blank below
#
my ($hostname,$user,$database,$driver,$password,$dsn)=
   ("localhost","www","tivo","mysql","","");
my ($dbh);

######################
###### Routines ######
######################

# Open connection to the database.
#
sub Open_Dbconnection {
  if (defined($dbh)) { return; }
  $dsn = "DBI:$driver:database=$database;host=$hostname";
  $dbh = DBI->connect($dsn, $user, $password);
}

# Open the list of known programs, and read it in
# Results are stored in the %Programs and
# %PartialPrograms hashes.
sub Read_Programs
{
  my ($prog, $lcprog, $genre);
  my ($sth, $ref);
  my ($query)= "select title,partmatch,primgenre,primgengrp,secgenre," .
	   "secgengrp,status from programs";

  undef(%Programs);

  # Open the database connection if required
  if (!defined($dbh)) { &Open_Dbconnection; }
  if (!defined($dbh)) { die("Could not connect to programs database\n"); }

  # Perform the select operation
  $sth= $dbh->prepare($query);
  $sth->execute;

  # Get each record one at a time
  while (my $ref = $sth->fetchrow_hashref()) {

    # Skip this if it isn't a current record
    if ($ref->{status} ne 'N') { next; }

    # Find the program's title
    $prog= $ref->{title};

    # Convert the title into mostly lowercase, so
    # that we do all comparisons case-insensitively.
    ($lcprog)= &LowercaseString($prog);

    # Convert the four genre numbers into a textual list
    $genre= $TextualGenre{ $ref->{primgenre} };
    if ($ref->{secgenre} != 0) {
      $genre= $genre . "," . $TextualGenre{ $ref->{secgenre} };
    }
    if ($ref->{primgengrp} != 0) {
      $genre= $genre . "," . $TextualGenre{ $ref->{primgengrp} };
    }
    if ($ref->{secgengrp} != 0) {
      $genre= $genre . "," . $TextualGenre{ $ref->{secgengrp} };
    }

    if ($ref->{partmatch} == 1) {
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

  # Finally, build a list of partial titles sorted by
  # string length descending.
  @SortedPrograms= sort Strlensort (keys(%PartialPrograms));
}

# When dealing with a file, we don't have to do much here.
#
sub Create_Program($)
{
  my ($index)= @_;
  my ($sth, $g);
  my ($primgenre, $primgengrp, $secgenre, $secgengrp);
  my (@nlist);

  # Open the database connection if required
  if (!defined($dbh)) { &Open_Dbconnection; }

  if (!defined($Programs{$index})) { return; }

  # Convert the genres to numbers
  @nlist= Genre_To_Numeric($Programs{$index}{genre},0);
  $primgenre= $secgenre= $primgengrp= $secgengrp= 0;
  foreach $g (@nlist) {
    if ($g<1000) {
      if ($primgenre==0) { $primgenre=$g; } else { $secgenre= $g; }
    } else {
      if ($primgengrp==0) { $primgengrp=$g; } else { $secgengrp= $g; }
    }
  }

  # Now add it all to the database
  #
  $sth = $dbh->prepare( "INSERT INTO programs(title,partmatch," .
                        "primgenre,primgengrp," .
                        "secgenre,secgengrp,who,changed," .
                        "channel,time,duration)" .
                        "VALUES (?,?,?,?,?,?,?,NOW(),?,?,?)" );

  $sth->execute($Programs{$index}{title},$Programs{$index}{partial},
                $primgenre, $primgengrp, $secgenre, $secgengrp,
                "GenreSearch", $Programs{$index}{channel},
                $Programs{$index}{time},$Programs{$index}{duration});
}

# Write out the Programs hash to the database, and close connection.
# We don't have to write anything here, as Create_Program does it.
#
sub Save_Programs
{
  if (defined($dbh)) { $dbh->disconnect; }
  return(1);
}


1;
