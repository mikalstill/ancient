#!/usr/bin/perl
# 
# GenreSearch.pm: Routines to search for unknown genres in intermediate data.
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
package TiVo::GenreSearch;
use strict;
use TiVo::ConfigFiles;
use TiVo::MiscUtil;
use TiVo::ProgramGenre;
use TiVo::ProgramDb;
use HTTP::Lite;
use CGI::Enurl;


BEGIN {
  use Exporter	 ();
  use vars	 qw($VERSION @ISA @EXPORT @EXPORT_OK %EXPORT_TAGS);

  $VERSION = do { my @r = (q$Revision: 1.1.1.1 $ =~ /\d+/g); sprintf "%d."."%02d" x $#r, @r }; # must be all one line, for MakeMaker

  @ISA	       = qw(Exporter);
  @EXPORT      = qw(List_Unknown_Genres Search_Unknown_Genres);
  %EXPORT_TAGS = ( );
  @EXPORT_OK   = qw();
}

use vars	 @EXPORT_OK;
END { }	 # module clean-up code here (global destructor)


#####################################
###### Package-private Globals ######
#####################################

my (%UnknownProgram, %Program);

# Hashes to convert Yahoo! genres to TiVo genres
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
  "Horror" => "Horror,Thriller",
  "Infotainment" => "Shopping",
  "Lifestyle" => "Outdoors,RealLife",
  "Music" => "Music,ArtsMusicLiving",
  "Musical" => "Musical,ArtsMusicLiving",
  "Mystery" => "Mystery,MysteryThriller",
  "N/A" => "Unknown",
  "Nature" => "Nature,DocumentaryGroup",
  "News" => "News,NewsBusinessWeather",
  "Real Life" => "RealLife",
  "Romance" => "Romance,DramaGroup",
  "Romantic Comedy" => "RomanticComedy,ComedyGroup",
  "Sci-Fi" => "ScienceFiction,SciFiFantasy",
  "Science and Technology" => "Science,ScienceNatureTechnology",
  "Shopping" => "Shopping,Daytime",
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

# Load data from intermediate files, find which programs
# don't have a genre, and build a list of them
#
sub List_Unknown_Genres($$$)
{
  my ($chan,$snum,$enum)= @_;
  my ($wanteday, $dayname, $infile);
  my ($match, $newtitle, $genrelist, $is_episodic);
  my ($i, $title, $leftovertitle);
  my (%TryList);

  # Do each day in turn
  for ($wanteday= $snum; $wanteday <= $enum; $wanteday++) {

    $dayname= Get_Dayname($wanteday);
    $infile= "$ConfigVar{Datadir}/$chan" . "_$dayname";
    unless (Read_IntermediateFile($infile)) {
      print(STDERR "Can't open data file $infile: $!\n");
      next;
    }
    Debug("Read intermediate data from $infile\n");

    for ($i=0; $i <= $#ProgramData; $i++) {

      # Skip this program if it has a genre with the work Movie in it
      if (defined($ProgramData[$i]{genre}) &&
	  ($ProgramData[$i]{genre}=~ /Movie/)) {
        Debug("Skip $ProgramData[$i]{title}, it's a movie\n");
	next;
      }
      $title= $ProgramData[$i]{title};
      ($title)= LowercaseString($title);
      $TryList{$title}{channel}= $chan;
      $TryList{$title}{time}=$ProgramData[$i]{time};
      $TryList{$title}{duration}=$ProgramData[$i]{duration};
      # Record the genre, if any. It could be a per-station genre.
      if (defined($ProgramData[$i]{genre}) && !defined($TryList{$title}{genre})){
        Debug("Saving genre $ProgramData[$i]{genre} for $title\n");
        $TryList{$title}{genre}=$ProgramData[$i]{genre};
      }
    }
  }

  # We have built a list of all titles. Now see which ones
  # don't have genres.
  foreach $title (sort keys %TryList) {
    # Start with a match on the trimtitles
    #
    ($match, $newtitle, $genrelist, $is_episodic)= Find_Trimtitle($title);
    if ($match==1) { next; }

    # No trimtitles match, we have to try the programs database now
    #
    ($match, $newtitle, $leftovertitle, $genrelist)= Find_Program($title);
    if ($match==1) {
      Debug("Program match found on $title\n");
    } else {
      print("Program $title unknown\n");
      $UnknownProgram{$title}{channel}=$TryList{$title}{channel};
      $UnknownProgram{$title}{time}=$TryList{$title}{time};
      $UnknownProgram{$title}{duration}=$TryList{$title}{duration};
      if (defined($TryList{$title}{genre}) &&
				!defined($UnknownProgram{$title}{genre})) {
        $UnknownProgram{$title}{genre}=$TryList{$title}{genre};
      }
    }
  }
  undef(%TryList);
}


# Search the list of unknown genre programs
sub Search_Unknown_Genres {
  my ($body, $enctitle, $event, $found, $genre);
  my ($http, $shorttitle, $partmatch, $req, $title);
  my ($trytitle, %AlreadyDone);
  my ($web_proxy, $webdelay);
  my($tmpfile)= "/tmp/ya.$$";
  my($tmpfile2)= "/tmp/ya2.$$";

  $web_proxy= $ConfigVar{Webproxy};

  # Set the delay in seconds between web fetches
  $webdelay=0;
  if (defined($ConfigVar{WebDelay})) {
    $webdelay= int($ConfigVar{WebDelay});
  }

  foreach $title (sort keys(%UnknownProgram)) {
    $shorttitle= $title;
    $partmatch=0;
    # Get rid of stuff after last colon
    if ($shorttitle=~ /: [^:]*$/) {
      $shorttitle=~ s/: [^:]*$//;
      # $partmatch=1;
    }
    # Ditto for " - "
    if ($shorttitle=~ / - .*$/) {
      $shorttitle=~ s/ - .*$//;
      # $partmatch=1;
    }

    if ($AlreadyDone{$title}) { next; }
    print(STDERR "Searching for $title");
    $found=0;
  
    # Convert title into URL encoded form, and lowercase it for later
    $enctitle= enurl($shorttitle);
    ($title)= LowercaseString($title);
  
    # Now send the request to search for this title
    $http = new HTTP::Lite;
    $http->add_req_header("Referer", "http://au.tv.yahoo.com/");

    if (defined($web_proxy)) { $http->proxy($web_proxy); }
    $req = $http->request("http://au.tv.yahoo.com/searchresults.html?" .
  		"kw=$enctitle&x=32&y=8") || die "Unable to search $title: $!";
    if ($req ne "200") {
      print(STDERR "Search for $title failed ($req): ".$http->status_message());
      next;
    }
    $body = $http->body();
  
    # Save and convert the incoming HTML to one tag per line
    Rehtml($body,$tmpfile);
  
    # Build up some of the final record now
    $Programs{$title}{partial}=$partmatch;
    $Programs{$title}{title}=$title;
    $Programs{$title}{channel}=$UnknownProgram{$title}{channel};
    $Programs{$title}{time}=$UnknownProgram{$title}{time};
    $Programs{$title}{duration}=$UnknownProgram{$title}{duration};

    # Now walk the file, getting event URLs and the following titles
    open(HIN, $tmpfile) || die("Cannot open temp file $tmpfile\n");
    while(<HIN>) {
      if (/event.html\?ev=(\d+)\"/) {
        $event= $1;
        $trytitle= <HIN>;
        chop($trytitle);
        ($trytitle)= LowercaseString($trytitle);
  
        # Do a match on the title and the trytitle, both lowercased
        if ($title eq $trytitle) {
  	# Now fetch that web page, given the event number
    	$http = new HTTP::Lite;
        if (defined($web_proxy)) { $http->proxy($web_proxy); }
    	$http->add_req_header("Referer", "http://au.tv.yahoo.com/");
    	$req = $http->request("http://au.tv.yahoo.com/event.html?ev=$event")
  					|| die "Unable to get document: $!";
    	if ($req ne "200") {
      	  print(STDERR "Request failed ($req): ".$http->status_message());
	  close(HIN); next;
    	}
    	$body = $http->body();
  
    	# Save and convert the incoming HTML to one tag per line
    	Rehtml($body,$tmpfile2);
  
  	# Find the yahoo genre and print it and the original title to stdout
  	open(GIN, $tmpfile2) || die("Cannot open $tmpfile2");
  	while(<GIN>) {
  	  chop;
  	  if (/Genre:\s*(.*)/) {
  	    $genre=$1; $found=1;
  	    if (defined($Genre{$genre})) {
  	      print(STDERR ", found genre $Genre{$genre}\n");
	      $Programs{$title}{genre}=$Genre{$genre};
	      # Don't add movies!
	      if (! ($Programs{$title}{genre}=~ /^Movie/) ) {
	        Create_Program($title);
	      }
  	    } else {
  	      print(STDERR ", unknown yahoo genre $genre\n");
	      if (defined($UnknownProgram{$title}{genre})) {
		$Programs{$title}{genre}= $UnknownProgram{$title}{genre};
	      } else {
                $Programs{$title}{genre}= "Unknown";
	      }
	      if (! ($Programs{$title}{genre}=~ /^Movie/) ) {
	        Create_Program($title);
	      }
  	    }
  	    last;
  	  }
  	}
  	close(GIN); last;
        }
      }
    }
    close(HIN);
    if ($found==0) {
      print(STDERR ", not found");
      if (defined($UnknownProgram{$title}{genre})) {
	$Programs{$title}{genre}= $UnknownProgram{$title}{genre};
        print(STDERR ", using $UnknownProgram{$title}{genre}");
      } else {
        $Programs{$title}{genre}= "Unknown";
      }
      print(STDERR "\n");
      if (! ($Programs{$title}{genre}=~ /^Movie/) ) {
        Create_Program($title);
      }
    }
    
    # Now sleep for $webdelay seconds so as not to annoy yahoo
    $AlreadyDone{$title}=1;
    sleep($webdelay);
  }
  
  # All done
  close(HIN); unlink($tmpfile);
  unlink($tmpfile2);
}

1;
