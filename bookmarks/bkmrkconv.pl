#!/usr/bin/perl -w
#
# bkmrkconv - Convert Netscape bookmarks.html into a tree of HTML
# ~~~~~~~~~   (c) Andrew Flegg 1999, 2000.
#             Released under the Artistic License.
#
# Syntax:
#   bkmrkconv [OPTIONS] ...
#
# (see bkmrkconv --help for full list)

use strict;

my $VERSION    = '1.09 (20-Sep-2000)';

my ($ME)         = ($0 =~ m!/([^/]+)$!);
my $inputFile    = '-';   # File to use as input
my $outputDir    = '.';   # Path to output files to and read templates from
my $doContents   = 1;     # Produce the contents page?
my $justSections = 0;     # Only put sections in the contents page?

my %outputFiles  = ();    # Keep track of files we're going to produce

my ($i, $var);
for ($i = 0; $i < scalar(@ARGV); $i++) {
    $var = $ARGV[$i];
    if (($var eq '-f') || ($var eq '--file')) {
        $inputFile = $ARGV[++$i];
    } elsif (($var eq '-d') || ($var eq '--dir')){
        $outputDir = $ARGV[++$i];
    } elsif (($var eq '-n') || ($var eq '--nocontents')) {
        $doContents = 0;
    } elsif (($var eq '-s') || ($var eq '--onlysections')) {
        $justSections = 1;
    } elsif (($var eq '-h') || ($var eq '--help')) {
        print <<EOM;

$ME - Convert Netscape bookmarks.html into a tree of HTML
(c) Andrew Flegg 1999, 2000. $VERSION.
    http://www.eh.org/~jaffa/

Syntax:
EOM
        print "    ".&getSyntax. <<EOM;

Options:
  -h, --help           Show this online help
  -n, --nocontents     Don't create the contents.html and so don't require
                       the existence of OUTPUTDIR/src/contentspage.html
  -s, --onlysections   Only put sections in the contents page, not all the
                       links in the source file
  -f, --file <FILE>    File to read in (default: standard input)
  -d, --dir <DIR>      Directory to use as OUTPUTDIR (default: current)

bkmrkconv is released under the Artistic Licence without any form of
warranty, either express or implied. Please report bugs to
<jaffa\@eh.org>.

EOM
        exit;
    }
}

if (! (defined($inputFile) && defined($outputDir))) {
    print "$ME: syntax error:\n    ".&getSyntax;
    exit;
}

$outputDir =~ s!/$!!;

chomp(my $oldcwd = `pwd`);
chdir $outputDir;
my $rootpage     = &readFile("src/rootpage.html");
my $linkspage    = &readFile("src/linkspage.html");
my $contentspage = &readFile("src/contentspage.html") if ($doContents);
chdir $oldcwd;

my %root;

# Read in the file a section at a time
#
open(INFILE, "<$inputFile") or die "Unable to open $inputFile: $!\n";
&readSection(\*INFILE, \%root);
close(INFILE) or die "Unable to close $inputFile: $!\n";

#&printTree(\%root, 0);

&outputTree(\%root, 1, "");

if ($doContents) {
    my @links = ();
    &sortContents(\%root, \@links, $justSections);
    &outputContents(\@links);
}

exit;
# ========================================================================


# -- Get the syntax ------------------------------------------------------
sub getSyntax {
    return "$ME [OPTIONS] ... (--help for full list)\n";
}


# -- Read a section recursively ------------------------------------------
#
sub readSection {
    my ($fh, $ref) = @_;
    my $line;
    my $lastRef;

    $ref->{sections} = [];
    $ref->{links}    = [];
    $ref->{sublinks} = 0;

    while ( defined($fh) && ($line = <$fh>) && ($line !~ m!</DL><p>!i) ) {
        $line =~ s/(\r|\n)//g;
        if ($line =~ m!^<H1>(.*?)</H1>$!i) {
            $ref->{name} = 'Root';
            $ref->{url}  = 'index.html';

        } elsif ($line =~ m!<H3 .*?>(.*)</H3>$!i) {
            my $skipping = 0;
            $skipping = 1 if (index($1, '(P)') >= 0); # Private link

            my %sub;
            $sub{url}  = &hashcode($ref->{name}, $1).".html";
            $sub{name} = $1;
            &readSection($fh, \%sub);
            if (! $skipping) {
                push @{ $ref->{sections} }, \%sub;
                $ref->{sublinks} += scalar( $sub{sublinks} );
            }

        } elsif ($line =~ m!<DT><A HREF="(.*?)"[^>]*>(.*?)</A>$!i) {
            next if (index($2, '(P)') >= 0); # Private link

            my %url;
            $url{name} = $2;
            $url{url}  = $1;
            $lastRef   = \%url;

            push @{ $ref->{links} }, $lastRef;
            $ref->{sublinks}++;

        } elsif ($line =~ m!<DD>(.*?)$!i) {
            if (defined($lastRef)) { $lastRef->{desc} = $1; }
        }
    }

    return;
}


# -- Prints a tree -------------------------------------------------------
#
sub printTree {
    my ($ref, $indent) = @_;
    my %tree = %{ $ref };
    my $i;

    print ' ' x $indent.$tree{name}."/\n";
    foreach $i (@{ $tree{sections} })  {
        &printTree($i, $indent + 2);
    }

    foreach $i (@{ $tree{links} }) {
        print ' '.' ' x $indent.$i->{name}."\n";
    }
    return;
}


# -- Output the files ----------------------------------------------------
#
sub outputTree {
    my ($ref, $root, $path) = @_;
    my %tree = %{ $ref };
    my $page = ($root ? $rootpage : $linkspage);
    my ($i, $wrap);

    study($page);
    my ($sBlock) = ($page   =~ m%<!--SECTION-->(.*)<!--/SECTION-->%si);
    my ($lBlock) = ($page   =~ m%<!--LINK-->(.*)<!--/LINK-->%si);
    my $dBlock;

    if ($lBlock) {
        ($dBlock) = ($lBlock =~ m%<!--DESC-->(.*)<!--/DESC-->%si);
        $lBlock   =~ s%(<!--DESC-->).*?<!--/DESC-->%$1%sig;
    }

    $page   =~ s%(<!--(SECTION|LINK)-->).*?<!--/(SECTION|LINK)-->%$1%sig;
    $path   =~ s/^ : //;

    my $sectionTable = ($page =~ m%<table[^>]*?>.{0,16}<!--SECTION-->%si);
    my $linkTable    = ($page =~ m%<table[^>]*?>.{0,16}<!--LINK-->%si);
    my $numSections  = scalar(@{ $tree{sections} });
    my $numLinks     = scalar(@{ $tree{links} });

    if (($sBlock) && ($numSections > 0)) {
        my $section;
        $wrap = 0;
        foreach $i (@{ $tree{sections} }) {
            my ($u, $n, $l, $s) = ($i->{url},
                                   $i->{name},
                                   $i->{sublinks},
                                   &sublinks($i));

            $section = $sBlock;
            $section =~ s/\$URL/$u/g;
            $section =~ s/\$NAME/$n/g;
            $section =~ s/\$NEXT/$s/g;
            $section =~ s/\$NUMLINKS/$l/g;
            if ($sectionTable) {
                $section = ($wrap ? $section.'</tr>' : '<tr class="section">'.$section);
                $wrap = 1 - $wrap;
            }

            $page    =~ s%(<!--SECTION-->)%$section$1%;
            &outputTree($i, 0, "$path : <a href=\"$tree{url}\">$tree{name}</a>");
        }
        $page =~ s%(<!--SECTION-->)%</tr>$1% if $wrap; # Finish off row
    }

    if (($lBlock) && ($numLinks > 0)) {
        my ($link, $desc);
        $wrap = 0;
        foreach $i (@{ $tree{links} }) {
            my ($u, $n, $d) = ($i->{url}, $i->{name}, $i->{desc});

            if (! $d) { $d = ""; }
            $link = $lBlock;
            $link =~ s/\$URL/$u/g;
            $link =~ s/\$NAME/$n/g;

            if ($dBlock && $d) {
                $desc = $dBlock;
                $desc =~ s/\$DESC/$d/g;
            } else {
                $desc = "";
            }
            $link =~ s%<!--DESC-->%$desc%;

            if ($linkTable) {
                $link = ($wrap ? $link.'</tr>' : '<tr class="link">'.$link);
                $wrap = 1 - $wrap;
            }

            $page =~ s%(<!--LINK-->)%$link$1%;
        }
        $page =~ s%(<!--LINK-->)%</tr>$1% if $wrap; # Finish row if necessary
    }

    # -- Post processing of the page
    #
    my $pathNoURL = $path;
    my $date      = scalar(localtime());
    $pathNoURL =~ s/<.*?>//g;
    $page =~ s/\$TITLE/$tree{name}/g;
    $page =~ s/\$PATHURL/$path/g;
    $page =~ s/\$PATH/$pathNoURL/g;
    $page =~ s/\$DATE/$date/g;
    $page =~ s%<!--\s*EXEC\s+"([^>]*?)"\s*-->%my $p = quotemeta($1); `$p 2>/dev/null`%eg;

    # If-Then-Else and If-Then segments
    #
    $page =~ s/<!--\s*IF\s+(.*?)\s+THEN\s*-->(.*?)<!--\s*ELSE\s*-->(.*?)<!--\s*ENDIF\s*-->/
               (eval($1) ? $2 : $3)
              /seg;
    $page =~ s/<!--\s*IF\s+(.*?)\s+THEN\s*-->(.*?)<!--\s*ENDIF\s*-->/
               (eval($1) ? $2 : "")
              /seg;

    $page =~ s/^\s+//gm;                  # Trim leading whitespace
    $page =~ s/\s+$//gm;                  # Trim trailing whitespace
    $page =~ s/<!--(SECTION|LINK)-->//g;  # Tidy up the source

    open(OUTFILE,">$outputDir/$tree{url}") or die "Unable to write file: $!\n";
    print OUTFILE $page;
    close(OUTFILE);

    return;
}


# -- Prints a tree -------------------------------------------------------
#
sub sortContents {
    my ($ref, $arr, $sections) = @_;
    my %tree  = %{ $ref };
    my $i;

    foreach $i (@{ $tree{sections} })  {
        if ($sections) {
            $i->{sectionurl} = "<a href=\"$tree{url}\">$tree{name}</a>";
            push @{ $arr }, $i;
        }
        &sortContents($i, $arr, $sections);
    }

    if (! $sections) {
        foreach $i (@{ $tree{links} }) {
            # Add a URL for the section this link comes from and add to list...
            $i->{sectionurl} = "<a href=\"$tree{url}\">$tree{name}</a>";
            push @{ $arr }, $i;
        }      
    }

    return;
}


# -- Output the sorted list ----------------------------------------------
#
sub outputContents {
    my ($arr)  = @_;
    my @sorted = sort { lc($a->{name}) cmp lc($b ->{name}) } @{ $arr };
    my $letter = ord('!');

    my $page   = $contentspage;

    study($page);
    my ($sBlock) = ($page =~ m%<!--SECTION-->(.*)<!--/SECTION-->%si);
    my ($lBlock) = ($page =~ m%<!--LINK-->(.*)<!--/LINK-->%si);

    $sBlock =~ s%(<!--LINK-->).*?<!--/LINK-->%$1%sig;
    $page   =~ s%(<!--SECTION-->).*?<!--/SECTION-->%$1%sig;

    my ($s, $section, $link) = (undef, $sBlock, undef);
    foreach $i (@sorted) {
        if ((($s = ord( lc($i->{name}) )) > $letter) &&
             ($s >= ord('a'))) {
            $section =~ s/\$PREFIX/uc(chr($letter))/eg;
            $page    =~ s%(<!--SECTION-->)%$section$1%;

            $section = $sBlock;
            $letter  = $s;
        }

        my ($u, $n, $p) = ($i->{url}, $i->{name}, $i->{sectionurl});

        $link = $lBlock;
        $link =~ s/\$URL/$u/g;
        $link =~ s/\$NAME/$n/g;
        $link =~ s/\$SECTION/$p/g;

        $section =~ s%(<!--LINK-->)%$link$1%;
    }
    $section =~ s/\$PREFIX/uc(chr($letter))/eg;  # Update whatever was left
    $page    =~ s%<!--SECTION-->%$section%;      # in $section...

    my $date = scalar(localtime());
    $page    =~ s/\$DATE/$date/g;
    $page    =~ s%<!--\s*EXEC\s+"([^>]*?)"\s*-->%my $p = quotemeta($1);`$p 2>/dev/null`%eg;
    $page    =~ s/^\s+//gm;                  # Trim leading whitespace
    $page    =~ s/\s+$//gm;                  # Trim trailing whitespace
    $page    =~ s/<!--LINK-->//g;            # Tidy up the source

    open(OUTFILE,">$outputDir/contents.html") or die "Unable to write file: $!\n";
    print OUTFILE $page;
    close(OUTFILE);
    return;
}

    
# -- Produces a URL for a new section ------------------------------------
#
sub hashcode {
    my ($from, $to) = @_;

    my $prefix = lc($from."_".$to);
    $prefix    =~ s/\W//g;

    my $tail   = "";

    if ($outputFiles{$prefix}) { $tail = "001"; }
    while ($outputFiles{$prefix.'_'.$tail}) { $tail++; }

    if ($tail) { $prefix .= '_'.$tail; }
    $outputFiles{$prefix} = 1;             # Mark this as used
    return $prefix;
}


# -- Gets a string for the first three URLs of a section -----------------
#
sub sublinks {
    my ($ref)    = @_;
    my %tree     = %{ $ref };
    my @sections = @{ $tree{sections} };
    my @links    = @{ $tree{links} };

    my $data     = "";
    my $offset   = scalar(@sections);
    my @sublink;
    my $i;
    for ($i = 0; $i < 2; $i++) {
        if ($i < $offset) {
            $sublink[$i] = $sections[$i];
        } elsif ($i < (scalar(@links) + $offset)) {
            $sublink[$i] = $links[$i - scalar(@sections)];
        }
    }

    foreach $i (@sublink) {
        $data .= '<a href="'.$i->{url}.'">'.$i->{name}.'</a>, ';
    }
    if ((scalar(@links) + $offset) < 3) {
        chop($data); chop($data);
    } else {
        $data .= '...';
    }

    return $data;
}


# -- Reads in a file and returns its content -----------------------------
#
sub readFile {
    my ($file) = @_;
    my $data   = '';

    open(INFILE, "<$file") or die "Unable to open $file: $!\n";
    while(<INFILE>) { $data .= $_; }
    close(INFILE) or die "Unable to close $file: $!\n";

    while ($data =~ m/<!--\s*INCLUDE\s*"(.*)"\s*-->/i) {
        my $file      = $1;
	my $otherFile = readFile($file);
	$data =~ s/<!--\s*INCLUDE\s*"$file"\s*-->/$otherFile/ig;
    }

    return $data;
}
