# -*- Perl -*-
# Classes for dbtohtml
#
# $Id: dbtohtml.cls,v 1.1.1.1 2002-07-15 21:33:31 root Exp $
#
#
##############################################################################

use vars qw($PROGNAME $VERSION);

if ($VERBOSE) {
    my($VERSION) = '$Id: dbtohtml.cls,v 1.1.1.1 2002-07-15 21:33:31 root Exp $'; # '
    my($REQNAME) = (split(/\s+/, $VERSION))[1];
    my($vers) = (split(/\s+/, $VERSION))[2];

    print STDERR "$REQNAME version $vers.\n";
}

##############################################################################

{
    package GENERIC_TAG;

    $TAG_NULL = "::TAG::NULL::";
    %TAGLEVEL = ();

    # TAG = "tag"           name of this tag
    # CONTENT->tag          pointer to first subelement
    # CONTENT_LAST->tag     pointer to last subelement
    # NEXT->tag             pointer to next element at this level
    # PREV->tag             pointer to prev element at this level
    # PARENT->tag           pointer to parent element
    # ATTR->{}              attributes of this tag
    # NEXT_ELEMENT->tag     pointer to next element of this type
    # PREV_ELEMENT->tag     pointer to prev element of this type

    sub new {
	my($type, %attributes) = @_;
	my($class) = ref($type) || $type;
	my($self) = {};
	my($count, $element);

	bless $self, $class;

	$self->{'PROPER_INIT'} = 1; # make sure GENERIC_TAG->new is called

	$self->{'TAG'}     = $type;
	$self->{'CONTENT'} = $GENERIC_TAG::TAG_NULL;
	$self->{'NEXT'}    = $GENERIC_TAG::TAG_NULL;
	$self->{'PREV'}    = $GENERIC_TAG::TAG_NULL;
	%{$self->{'ATTR'}} = %attributes;

	# TAGLEVEL counts the number of each kind of tag that we've seen.
	# For example, TAGLEVEL{SECT1} is incremented each time a SECT1 is
	# seen.
	#
	if (! defined($TAGLEVEL{$self->tag()})) {
	    $TAGLEVEL{$self->tag()} = 1;
	} else {
	    $TAGLEVEL{$self->tag()}++;
	}

	# If we see a SECTn, all SECTm (n>m) have to be reset (in other words
	# the sequence is 1, 1.1, 1.2, 2, 2.1, 2.2 and
	# not 1, 1.1, 1.2, 2.2, ...
	#
	if ($self->tag() =~ /^SECT(\d)/) {
	    $count = $1;
	    for ($count = $count+1; $count <= 5; $count++) {
		delete($TAGLEVEL{"SECT$count"});
	    }
	}

	# The same is true of REFSECTn
	#
	if ($self->tag() =~ /^REFSECT(\d)/) {
	    $count = $1;
	    for ($count = $count+1; $count <= 5; $count++) {
		delete($TAGLEVEL{"REFSECT$count"});
	    }
	}
	
	# Everything inside a table is cleared each time a table starts
	#
	if ($self->tag('TABLE') || $self->tag('INFORMALTABLE')) {
	    delete($TAGLEVEL{'COLSPEC'});
	    delete($TAGLEVEL{'SPANSPEC'});
	    delete($TAGLEVEL{'TGROUP'});
	    delete($TAGLEVEL{'THEAD'});
	    delete($TAGLEVEL{'TBODY'});
	    delete($TAGLEVEL{'ROW'});
	    delete($TAGLEVEL{'ENTRY'});
	}

	# Everything is cleared when a new chapter-level element starts...
	#
	%TAGLEVEL = () if ($self->tag('PREFACE')
			   || $self->tag('CHAPTER')
			   || $self->tag('GLOSSARY')
			   || $self->tag('BIBLIOGRAPHY')
			   || $self->tag('APPENDIX'));
	
	# Copy the "interesting" fields into the TAGLEVEL structure of the
	# current element:

	$self->{'TAGLEVEL'} = { };
	foreach $element ($self->tag(),
			  'SECT1', 'SECT2', 'SECT3', 'SECT4', 'SECT5',
			  'REFSECT1', 'REFSECT2', 'REFSECT3', 
			  'FIGURE', 'TABLE', 'EXAMPLE') {
	    next if !defined($TAGLEVEL{$element});
	    $self->{'TAGLEVEL'}->{$element} = $TAGLEVEL{$element};
	}

	return $self;
    }

    sub markup_start {
	my($self) = @_;
	my($tag) = $self;
	my($ref) = $main::WHENTREE{$tag->{'TAG'}};

	while (ref $ref) {
	    $tag = $tag->{'PARENT'};
	    if (defined($ref->{$tag->{'TAG'}})) {
		$ref = $ref->{$tag->{'TAG'}}
	    } else {
		$ref = $ref->{"*tag"};
	    }
	}

	if (!defined($ref)) {
	    &main::WARNING("Can\'t form markup_start for $self->{TAG}\n");
	}

	return $ref;
    }

    sub markup_end {
	my($self) = @_;
	my($tag) = $self;
	my($ref) = $main::WHENTREE{"/" . $tag->{'TAG'}};

	while (ref $ref) {
	    $tag = $tag->{'PARENT'};
	    if (defined($ref->{"/" . $tag->{'TAG'}})) {
		$ref = $ref->{"/" . $tag->{'TAG'}}
	    } else {
		$ref = $ref->{"*tag"};
	    }
	}

	if (!defined($ref)) {
	    $ref = $self->markup_start();
	    if ($ref =~ /^\<([^<>]+)\>\s*$/) {
		$ref = "</$1>";
	    } elsif ($ref eq "") {
		# ok...
	    } else {
		&main::WARNING("Can\'t form markup_end for $self->{TAG}\n");
	    }
	}

	return $ref;
    }
	
    sub save_id {
	my($self) = @_;
	my($chap, $sect, $refsect, $count, $title, $basename, $sourcefile);
	my($s, $child, $id, $tag);

	$id = $self->{'ATTR'}->{'ID'};
	$tag = $self->{'TAG'};

	return if $id eq "" || $id =~ /^TMPTMP\.\d+$/;

	$chap = $main::PI_CHAPTER_NUMBER || "0";

	$sect = $self->{'TAGLEVEL'}->{'SECT1'};
	foreach $s ('SECT2', 'SECT3', 'SECT4', 'SECT5') {
	    $sect .= "." . $self->{'TAGLEVEL'}->{$s} 
	        if defined($self->{'TAGLEVEL'}->{$s});
	}

	$refsect = $self->{'TAGLEVEL'}->{'REFSECT1'};
	foreach $s ('REFSECT2', 'REFSECT3', 'REFSECT4', 'REFSECT5') {
	    $refsect .= "." . $self->{'TAGLEVEL'}->{$s} 
	        if defined($self->{'TAGLEVEL'}->{$s});
	}

	$count = $self->{'TAGLEVEL'}->{$tag};

	$title = "";
	$child = $self->content();
	if ($child && $child->{'TAG'} eq "TITLE") {
	    $title = $child->markup();
	}

	if ($self->tag('REFENTRY') && $child) {
	    # title is either in REFMETA or REFNAMEDIV...
	    my($subchild) = $child->content();
	    if ($subchild 
		&& (($child->tag('REFMETA') 
		     && $subchild->tag('REFENTRYTITLE'))
		    || (($child->tag('REFNAMEDIV') 
			 && $subchild->tag('REFNAME'))))) {
		$title = $subchild->markup();
	    }
	}

	if ($self->tag('GLOSSENTRY') && $child) {
	    $title = $child->markup();
	}

	$title =~ s/<.lb>/ /g;
	$title =~ s/\n/ /g;

	# In order to be able to determine if an ID has expired, we
	# need to know what file it came from; so remember that...
	$sourcefile = $main::PI_SOURCE_FILE || $main::SOURCE_FILE;
	$sourcefile = $1 if $sourcefile =~ /.*\/([^\/]+)$/;

	$main::TAGID->{$id} = { } if !defined($main::TAGID->{$id});
	$main::TAGID->{$id}->{'*TOUCHED'} = 1;

	$basename = $main::PI_BASENAME;
	if (!defined($basename)) {
	    $basename = $main::PI_SOURCE_FILE || $main::SOURCE_FILE;
	    $basename = $1 if $basename =~ /.*\/([^\/]+)$/;
	    $basename = $1 if $basename =~ /^(.*)\.[^\.]+$/;
	}

	$self->{'CHP-NUM'} = $chap;
	$self->{'SECT-NUM'} = $sect;
	$self->{'RSECT-NUM'} = $refsect;
	$self->{'ITEM-NUM'} = $count;
	$self->{'TITLE'} = $title;
	$self->{'BASENAME'} = $basename;

	$main::TAGID->{$id} = { } if !defined($main::TAGID->{$id});

	if ($tag ne 'INDEXTERM') {
	    $main::TAGID_CHANGE = 
		($main::TAGID_CHANGE 
		 || $main::TAGID->{$id}->{'TAG'} ne $tag
		 || $main::TAGID->{$id}->{'SOURCEFILE'} ne $sourcefile
		 || $main::TAGID->{$id}->{'CHP-NUM'} ne $chap
		 || $main::TAGID->{$id}->{'SECT-NUM'} ne $sect
		 || $main::TAGID->{$id}->{'RSECT-NUM'} ne $refsect
		 || $main::TAGID->{$id}->{'ITEM-NUM'} ne $count
		 || $main::TAGID->{$id}->{'TITLE'} ne $title
		 || $main::TAGID->{$id}->{'BASENAME'} ne $basename);
	}

	$main::TAGID->{$id}->{'TAG'} = $tag;
	$main::TAGID->{$id}->{'SOURCEFILE'} = $sourcefile;
	$main::TAGID->{$id}->{'CHP-NUM'} = $chap;
	$main::TAGID->{$id}->{'SECT-NUM'} = $sect;
	$main::TAGID->{$id}->{'RSECT-NUM'} = $refsect;
	$main::TAGID->{$id}->{'ITEM-NUM'} = $count;
	$main::TAGID->{$id}->{'TITLE'} = $title;
	$main::TAGID->{$id}->{'BASENAME'} = $basename;
	$main::TAGID->{$id}->{'CHUNK'} = $SECT1::SECT_COUNT
	    if $SECT1::SECT_COUNT;
    }

    sub attr {
	my($self, $attr, $value) = @_;
	my($self_value);

	$self_value = uc($self->{'ATTR'}->{$attr});
	if (defined($value)) {
	    return $value eq $self_value;
	} else {
	    return $self->{'ATTR'}->{$attr};
	}
    }

    sub null {
	my($self, $ptr) = @_;

	return (!$ptr || $ptr eq $GENERIC_TAG::TAG_NULL);
    }

    sub tag {
	my($self, $tag) = @_;

	if (defined($tag)) {
	    $tag =~ tr/a-z/A-Z/;
	    return ($self->{'TAG'} eq $tag);
	} else {
	    return $self->{'TAG'};
	}
    }

    sub role {
	my($self, $role) = @_;

	return $self->attr('ROLE', $role);
    }

    sub id {
	my($self, $id) = @_;

	return $self->attr('ID', $id);
    }

    sub parent {
	my($self) = @_;

	return $self->{'PARENT'};
    }

    sub next {
	my($self) = @_;

	while (($self->{'NEXT'} eq $TAG_NULL)
	       && $main::parser->next_element()) {
	    #nop;
	}

	$self->{'NEXT'} eq $TAG_NULL ? undef : $self->{'NEXT'};
    }

    sub prev {
	my($self) = @_;
	$self->{'PREV'};
    }

    sub content {
	my($self) = @_;

	while (($self->{'CONTENT'} eq $TAG_NULL)
	       && $main::parser->next_element()) {
	    #nop;
	}

	$self->{'CONTENT'} eq $TAG_NULL ? undef : $self->{'CONTENT'};
    }

    sub next_element {
	my($self) = @_;

	while (($self->{'NEXT_ELEMENT'} eq $TAG_NULL)
	       && $main::parser->next_element()) {
	    #nop;
	}

	$self->{'NEXT_ELEMENT'} eq $TAG_NULL ? undef : $self->{'NEXT_ELEMENT'};
    }

    sub prev_element {
	my($self) = @_;
	$self->{'PREV_ELEMENT'};
    }

    sub add_child {
	my($self, $ref) = @_;
	my($prevelem);

	$ref->{'PARENT'} = $self;
	$ref->{'NEXT'} = 0;

	if ($self->{'CONTENT'} ne $TAG_NULL) {
	    $ref->{'PREV'} = $self->{'CONTENT_LAST'};
	    $self->{'CONTENT_LAST'}->{'NEXT'} = $ref;
	    $self->{'CONTENT_LAST'} = $ref;
	} else {
	    $self->{'CONTENT'} = $ref;
	    $self->{'CONTENT_LAST'} = $ref;
	    $ref->{'PREV'} = 0;
	}

	$LAST_ELEM->{'NEXT_ELEMENT'} = $ref if $LAST_ELEM;
	$ref->{'PREV_ELEMENT'} = $LAST_ELEM;
	$LAST_ELEM = $ref;
    }

    sub unlink_child {
	my($self, $child) = @_;

	if ($child->{'PREV'}) {
	    $child->{'PREV'}->{'NEXT'} = $child->{'NEXT'};
	} else {
	    $child->{'PARENT'}->{'CONTENT'} = $child->{'NEXT'};
	}

	if ($child->{'NEXT'}) {
	    $child->{'NEXT'}->{'PREV'} = $child->{'PREV'};
	} else {
	    $child->{'PARENT'}->{'CONTENT_LAST'} = $child->{'PREV'};
	}

	$child->{'NEXT'} = "";
	$child->{'PREV'} = "";

	$child;
    }

    sub block_markup {
	# Extract "block markup" from the markup of a tag.  In other
	# words if presented with the contents of 
	#
	# <title>Foo<indexterm><primary>foo</></> and <emphasis>Bar</></title>
	#
	# It changes the markup for <title> to be
	#
	# <title>Foo and <emphasis>Bar</></title>
	#
	# and returns
	#
	# <indexterm><primary>foo</></>
	#
	my($self) = @_;
	my($child, $nextchild);
	my($block, $last) = (0, 0);

	$nextchild = $self->content();
	while ($nextchild) {
	    $child = $nextchild;
	    $nextchild = $nextchild->{'NEXT'};

	    if (!&main::isa($child, 'GENERIC_INLINE')) {
		$child = $self->unlink_child($child);
		if ($last) {
		    $last->{'NEXT'} = $child;
		    $child->{'PREV'} = $last;
		    $last = $child;
		} else {
		    $block = $child;
		    $last = $child;
		}
	    }
	}

	$block;
    }

    sub default_markup {  
	my($self, $markup) = @_;

	$self->save_id();

	if ($self->id()) {
	    my($tag) = $self->tag();
	    my($id) = $self->id();

	    if ($tag !~ /CHAPTER/i
		&& $tag !~ /APPENDIX/i
		&& $tag !~ /PREFACE/i
		&& $tag !~ /PTCHAPTER/i
		&& $tag !~ /GLOSSARY/i
		&& $tag !~ /BIBLIOGRAPHY/i
		&& $tag !~ /SECT\d/i
		&& $tag !~ /PTSECT\d/i
		&& $tag !~ /INDEXTERM/i
		&& $tag !~ /REFENTRY/i
		&& $tag !~ /REFSYNOPSISDIV/i
		&& $tag !~ /REFPURPOSE/i
		&& $tag !~ /TITLE/i
		&& $tag !~ /TABLE/i
		&& $tag !~ /GLOSSENTRY/i
		&& $tag !~ /EXAMPLE/i
		&& $tag !~ /FIGURE/i) {
		&main::WARNING("ID unexpected on $tag, <A NAME...> may not be generated\n");
	    }
	}
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	&main::WARNING("No markup method for $self->{TAG}.\n");
	$self->markup_children($markup, $self->content());
	
	$inmarkup ? $markup : $markup->formatted_markup();
    }
	
    sub markup_children {
	my($self, $markup, $child) = @_;
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !$inmarkup;

	while ($child && $child ne $TAG_NULL) {
	    $child->markup($markup);
	    $child = $child->next();
	}

	$inmarkup ? $markup : $markup->formatted_markup();
    }	

    sub prev_node {
	my($self, @tags) = @_;
	my($prev) = $self->prev_element();
	my($title) = "Top";

	while ($prev && !&main::in($prev->{'TAG'}, @tags)) {
	    $prev = $prev->prev_element();
	}

	if ($prev) {
	    $prev = $prev->content();
	    if ($prev && $prev->{'TAG'} eq 'TITLE') {
	        $title = $prev->markup();
		$title =~ s/<\001lb>/ /g;
	    }
	}

	wantarray ? ($prev, $title) : $title;
    }

    sub next_node {
	my($self, @tags) = @_;
	my($next) = $self->next_element();
	my($title) = "";

	while ($next && !&main::in($next->{'TAG'}, @tags)) {
	    $next = $next->next_element();
	}

	if ($next) {
	    $next = $next->content();
	    if ($next && $next->{'TAG'} eq 'TITLE') {
	        $title = $next->markup();
		$title =~ s/<\001lb>/ /g;
	    }
	}

	wantarray ? ($next, $title) : $title;
    }

    sub html_basename {
	my($self, $section) = @_;
	my($basename);

	$basename = &main::cfg($main::bookfiles, $section, "pi_basename");
	if (!defined($basename)) {
	    $basename = $section;
	    $basename = $1 if $basename =~ /.*\/([^\/]+)$/;
	    $basename = $1 if $basename =~ /^(.*)\.[^\.]+$/;
	}

	$basename;
    }

    sub basename {
	my($self) = @_;

	return $self->{'BASENAME'} if defined($self->{'BASENAME'});

	$basename = $main::PI_BASENAME;
	if (!defined($basename)) {
	    $basename = $main::PI_SOURCE_FILE || $main::SOURCE_FILE;
	    $basename = $1 if $basename =~ /.*\/([^\/]+)$/;
	    $basename = $1 if $basename =~ /^(.*)\.[^\.]+$/;
	}

	$self->{'BASENAME'} = $basename;
	return $basename;
    }

    sub htmlpath {
	my($self) = @_;

	$htmlpath = &main::cfg($main::bookfiles, 
			       $main::PI_SOURCE_FILE || $main::SOURCE_FILE,
			       'html_path') 
	    || ".";

	$htmlpath .= "/" if $htmlpath && $htmlpath !~ /\/$/;
    }

    sub newname {
	my($self, $other) = @_;
	my($newname, $htmlpath);
	my($html_names);

	$htmlpath = $self->htmlpath();

	$newname = $main::PI_BASENAME;
 	if (!defined($newname)) {
	    if (!defined ($main::PI_SOURCE_FILE)) {
		my($chap) = $main::PI_CHAPTER_NUMBER || "1";
		# there's no PIs for this file...
		$newname = $main::SOURCE_FILE;
		$newname = $1 if $newname =~ /^(.*)\.[^\.]+$/;
		
		if ($chap =~ /^\d+/) {
		    $newname .= sprintf ("-%02d", $chap);
		} else {
		    $newname .= "-$chap";
		}
	    } else {
		$newname = $main::PI_SOURCE_FILE;
		$newname = $1 if $newname =~ /^(.*)\.[^\.]+$/;
	    }
	}

	$newname = $1 if $newname =~ /^.*\/([^\/]+)$/;
	$newname .= $other if defined($other);
	$newname .= $main::HTML_EXT;

	# Note: we want to be able to use the current value of the
	# html_names variable while we're processing this file, so we
	# must make sure not to blow it away.  END in dbtohtml.sup 
	# fixes this up.
	$html_names = &main::cfg($main::bookfiles, 
				 $main::PI_SOURCE_FILE,
				 'html_names-new');
	$html_names .= "," if $html_names ne "";
	$html_names .= $newname;

	&main::cfg_set($main::bookfiles, 
		       $main::PI_SOURCE_FILE,
		       'html_names-new',
		       $html_names);

	$main::HTML_FILENAME = $newname;

	return "$htmlpath$newname";
    }

    sub prev_html_file {
	my($self) = @_;
	my($basename, $num, $chunk, $prev, $ord);
	my($prevtitle);
	local($_);

	if (! ($self->tag('PREFACE') 
	       || $self->tag('CHAPTER') 
	       || $self->tag('APPENDIX')
	       || $self->tag('GLOSSARY')
	       || ($self->tag() =~ /^SECT\d/)
	       || ($self->tag() =~ /^REFSECT\d/))) {
	    print STDERR "Previous HTML file for a ", $self->tag(), "?\n";
	    return wantarray ? ("", "") : "";
	}

	$num = $main::TAGID->{$self->id()}->{'CHP-NUM'};
	$chunk = $main::TAGID->{$self->id()}->{'CHUNK'};

	if ($chunk > 1) {
	    $prevtitle = "";
	    $prev = sprintf("%s-%02d%s",
			    $self->basename(),
			    $chunk-1,
			    $main::HTML_EXT);

	    foreach $_ (keys %{$main::TAGID}) {
		if ($main::TAGID->{$_}->{'CHUNK'} == $chunk-1
		    && $main::TAGID->{$_}->{'CHP-NUM'} eq $num) {
		    $prevtitle = $main::TAGID->{$_}->{'TITLE'};
		    last;
		}
	    }

	    return wantarray ? ($prev,$prevtitle) : $prev;
	}

	if ($num =~ /[A-Z]/i) {
	    $num =~ tr/a-z/A-Z/;
	    
	    if ($num eq "A") {
		$num = 0;
		foreach $_ (keys %{$main::TAGID}) {
		    $num = $main::TAGID->{$_}->{'CHP-NUM'}
		        if $main::TAGID->{$_}->{'CHP-NUM'} > $num;
		}
	    } else {
		my($ord) = ord($num);
		$num = sprintf("%c", $ord-1);
	    }
	} else {
	    $num--;
	}

	if ($num < 0) {
	    return wantarray ? ("", "") : "";
	}

	# Now $num is equal to the chapter preceding this one.  Find last
	# section of that chapter...

	$prevtitle = "";
	undef $basename;
	$chunk = 1;
	foreach $_ (keys %{$main::TAGID}) {
	    if ($main::TAGID->{$_}->{'CHUNK'} > $chunk
		&& $main::TAGID->{$_}->{'CHP-NUM'} eq $num) {
		$chunk = $main::TAGID->{$_}->{'CHUNK'};
		$basename = $main::TAGID->{$_}->{'BASENAME'};
		$prevtitle = $main::TAGID->{$_}->{'TITLE'};
	    }
	}

	return wantarray ? ("", "") : "" if (!$basename);

	$prev = sprintf("%s-%02d%s",
			$basename,
			$chunk,
			$main::HTML_EXT);

	return wantarray ? ($prev, $prevtitle) : $prev;
    }

    sub next_html_file {
	my($self) = @_;
	my($basename, $num, $chunk, $prev, $ord, $found, $nextchunk);
	my($nexttitle);
	local($_);

	if (! ($self->tag('PREFACE') 
	       || $self->tag('CHAPTER') 
	       || $self->tag('APPENDIX')
	       || $self->tag('GLOSSARY')
	       || ($self->tag() =~ /^SECT\d/)
	       || ($self->tag() =~ /^REFSECT\d/))) {
	    print STDERR "Next HTML file for a ", $self->tag(), "?\n";
	    return wantarray ? ("", "") : "";
	}

	$num = $main::TAGID->{$self->id()}->{'CHP-NUM'};
	$chunk = $main::TAGID->{$self->id()}->{'CHUNK'} || 1;

	# See if there is a next section in this file...
	$nexttitle = "";
	undef $nextchunk;
	foreach $_ (keys %{$main::TAGID}) {
	    if ($main::TAGID->{$_}->{'CHUNK'} == $chunk+1
		&& $main::TAGID->{$_}->{'CHP-NUM'} eq $num
		&& ($main::TAGID->{$_}->{'TAG'} eq 'SECT1')) {
		$nextchunk = $chunk + 1;
		$basename = $main::TAGID->{$_}->{'BASENAME'};
		$nexttitle = $main::TAGID->{$_}->{'TITLE'};
		last;
	    }
	}

#	print STDERR "Test($num,$chunk): next=$nextchunk\n";

	if (defined($nextchunk)) {
	    $next = sprintf("%s-%02d%s",
			    $basename,
			    $nextchunk,
			    $main::HTML_EXT);
	    return wantarray ? ($next, $nexttitle) : $next;
	}

	# Ok, there was no next chunk, so maybe there's a next file
	
	if ($num =~ /[A-Z]/i) {
	    $num =~ tr/a-z/A-Z/;
	    $ord = ord($num);
	    $num = sprintf("%c", $ord+1);
	} else {
	    $num++;

	    # Switch to App A if we're at the last chapter...
	    $found = 0;
	    foreach $_ (keys %{$main::TAGID}) {
		last if $found;
		$found = ($main::TAGID->{$_}->{'CHP-NUM'} eq $num);
	    }

	    $num = 'A' if !$found;
	}

	$nexttitle = "";
	undef $basename;
	foreach $_ (keys %{$main::TAGID}) {
	    if ($main::TAGID->{$_}->{'CHP-NUM'} eq $num
		&& ($main::TAGID->{$_}->{'TAG'} eq 'CHAPTER'
		   || $main::TAGID->{$_}->{'TAG'} eq 'APPENDIX'
		   || $main::TAGID->{$_}->{'TAG'} eq 'GLOSSARY'
		   || $main::TAGID->{$_}->{'TAG'} eq 'BIBLIOGRAPHY'
		   || $main::TAGID->{$_}->{'TAG'} eq 'PREFACE')) {
		$chunk = 1;
		$basename = $main::TAGID->{$_}->{'BASENAME'};
		$nexttitle = $main::TAGID->{$_}->{'TITLE'};
	    }

	    last if $basename;
	}

	return wantarray ? ("", "") : "" if !$basename;

	$next = sprintf("%s-%02d%s",
			$basename,
			$chunk,
			$main::HTML_EXT);

	return wantarray ? ($next, $nexttitle) : $next;
    }
}

{
    package GENERIC_CHAPTER;
    @ISA = ('GENERIC_TITLED');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($child) = $self->content();
	my($chapnum) = $main::PI_CHAPTER_NUMBER || "1";
	my($html, $htmlpath, $newname, $title);
	my($thishref, $prevhref, $nexthref, $prevtitle, $nexttitle);
	my($chaptype);

	$markup = new MARKUP_OBJECT if !$inmarkup;

	$FOOTNOTE::footnote_count = 0;
	$SECT1::SECT_COUNT = 0;

	$newname = $self->newname("-01");
	$markup->change_output_files($newname);

	$self->default_markup($markup);

	&main::WARNING("Chapter with missing ID will not appear in TOC.")
	    if !$self->id();

	$html = "<h1>";
	$html .= "<A NAME=\"" . $self->id() . "\">" if $self->id();

	if ($child && ($child->tag('TITLE'))) {
	    $title = $child->markup();
	    
	    # Handle <?lb> in title
	    $title =~ s/<\001lb>/ /g;

	    ($chaptype = $self->tag()) =~ tr/A-Z/a-z/;
	    $chaptype = uc(substr($chaptype, 0, 1)) . substr($chaptype, 1);
	    $html .= "$chaptype ";

	    $html .= $self->{'CHP-NUM'} . ". ";

	    $html .= $title;

	    $html .= "</A>" if $self->id();

	    $html .= "</h1>";

	    $child = $child->next();
	} else {
	    $title = "";
	    $html = "<!--Chapter $chapnum: no title-->";
	    &main::WARNING("No title on Chapter $chapnum");
	}

	$main::HTML_TITLE = $title;

	$markup->output("<html>");
	$markup->output("<head>");
	$markup->output("<title>$title</title>");

	$markup->output("<META NAME=\"metadata\"   CONTENT=\"dublincore.0.1\">\n");
	$markup->output("<META NAME=\"subject\"    CONTENT=\"JavaScript\">\n");
	$markup->output("<META NAME=\"title\"      CONTENT=\"JavaScript: The Definitive Guide, Beta\">\n");
	$markup->output("<META NAME=\"author\"     CONTENT=\"David Flanagan\">\n");
	$markup->output("<META NAME=\"otheragent\" CONTENT=\"gmat dbtohtml\">\n");
	$markup->output("<META NAME=\"source\"     CONTENT=\"SGML\">\n");
	$markup->output("<META NAME=\"publisher\"  CONTENT=\"O'Reilly &amp; Associates, Inc.\">\n");
	$markup->output("<META NAME=\"date\"       CONTENT=\"" . &main::date() . "\">\n");
	$markup->output("<META NAME=\"objecttype\" CONTENT=\"book section chapterstart\">\n");
	$markup->output("<META NAME=\"form\"       CONTENT=\"html\">\n");

	$markup->output("</head>");
	$markup->output("<body bgcolor=\"\#FFFFFF\">");

	$self->prevnext($markup, 1);

	$markup->output($html);

	# Always build the TOC (to figure out IDs on sections).  If you
	# don't want it in the HTML file, discard its markup.
	$self->toc($markup, "\n\n<p>\n<b>Contents:</b><br>\n");

	$self->markup_children($markup, $child);

	$markup->output($self->markup_end());

	# The basename only applies to this chapter...
	undef $main::PI_BASENAME;

	# What if there is no PI_CHAPTER_NUMBER?  Then make sure we
	# "fake it" for subsequent chapters that we encounter.
	$chapnum++;
	$main::PI_CHAPTER_NUMBER = $chapnum;

	my($chapnum) = $main::PI_CHAPTER_NUMBER || "1";


	$inmarkup ? $markup : $markup->formatted_markup();
    }

    sub prevnext {
	my($self, $markup, $attop) = @_;
	my($inmarkup) = defined($markup);
	my($html) = "";
	my($sfile) = $main::FILTERCFG;
	my($telem, $title, $href, $chpnum, $id, $prev, $next, $up);
	my($uptext, $prevtext, $nexttext);
	local($_);

	$uptext   = &main::cfg($main::bookfiles, $sfile, 'html_uptext');
	$prevtext = &main::cfg($main::bookfiles, $sfile, 'html_prevtext');
	$nexttext = &main::cfg($main::bookfiles, $sfile, 'html_nexttext');

	$markup = new MARKUP_OBJECT if !$inmarkup;

	$markup->output("<p>");
	if (!$attop) {
	    $markup->output("\n<hr>\n<p>\n");
	}

	if ($main::OUTPUT) {
	    $up = &main::cfg($main::bookfiles, 
			     $main::PI_SOURCE_FILE,
			     'html_root');
	    $markup->output("<A HREF=\"$up\">$uptext</A>\n")
		if $up ne "";

	    $prev = $self->prev_html_file();
	    $markup->output("<A HREF=\"$prev\">$prevtext</A>\n")
		if $prev ne "";

	    $next = $self->next_html_file();
	    $markup->output("<A HREF=\"$next\">$nexttext</A>\n")
		if $next ne "";
	}

	$markup->output("<p>");
	if ($attop) {
	    $markup->output("\n<hr>\n<p>\n");
	}

	$inmarkup ? $markup : $markup->formatted_markup();
    }

    sub toc {
	my($self, $markup, $toc_header) = @_;
	my($inmarkup) = defined($markup);
	my($elem) = $self->content();
	my($telem, $title, @tidcount, $href, $chpnum, $id, $chunk);
	my($first);
	local($_);

	@tidcount = (0, 0, 0, 0, 0, 0);

	$markup = new MARKUP_OBJECT if !$inmarkup;

	if (!$self->{'BUILT_MENU'}) {
	    $first = 1;
	    while ($elem && $elem ne $GENERAL_TAG::TAG_NULL) {
		if ($elem->tag() =~ 'SECT(\d)') {
		    my($level) = $1;
		    my($clevel);

		    for ($clevel = $level+1; $clevel <= 5; $clevel++) {
			$tidcount[$clevel] = 0;
		    }

		    $id = $elem->id();
		    if (!$id) {
			$tidcount[$level]++;
			$id = sprintf("%s-DMYID.%d.%d.%d.%d.%d", 
				      $self->basename(),
				      $tidcount[1],
				      $tidcount[2],
				      $tidcount[3],
				      $tidcount[4],
				      $tidcount[5]);
			$id = $1 while $id =~ /^(.*)\.0$/;
			$id = uc($id);
			$elem->{'ATTR'}->{'ID'} = $id;
		    }
		    $telem = $elem->content();
		    $title = $telem->markup();
		    $title =~ s/<\001lb>//g;

		    $chunk = $main::TAGID->{$id}->{'CHUNK'};

		    if ($first) {
			$markup->output($toc_header);
			$markup->output($title);
			$markup->output("<br>\n");
			$first = 0;
			$toc_header = "";
		    } else {
			if ($elem->tag('SECT1')) {
			    if ($chunk) {
				$href = sprintf("%s-%02d%s",
						$self->basename(),
						$chunk,
						$main::HTML_EXT);
				$markup->output("<A HREF=\"$href\">$title</A>");
				$markup->output("<br>\n");
			    } else {
				$href = sprintf("#%s", $id);
				$markup->output("<A HREF=\"$href\">$title</A>");
				$markup->output("<br>\n");
			    }
			}
		    }
		}

		$elem = $elem->next_element();
		last if (!$self->null($elem) 
			 && ($elem->tag('PREFACE')
			     || $elem->tag('CHAPTER')
			     || $elem->tag('APPENDIX')));
	    }

	    $markup->output("\n");
	    $self->{'BUILT_MENU'} = 1;
	}

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}

{
    package GENERIC_INLINE;
    @ISA = ('GENERIC_TAG');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$markup->output($self->markup_start());
	$self->markup_children($markup, $self->content());
	$markup->output($self->markup_end());

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}

{
    package GENERIC_NOP;
    @ISA = ('GENERIC_INLINE');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$inmarkup ? $markup : undef;
    }
}

{
    package GENERIC_WRAPPER;
    @ISA = ('GENERIC_TAG');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$markup->output($self->markup_start());
	$self->markup_children($markup, $self->content());
	$markup->output($self->markup_end());

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}

{
    package GENERIC_OBEYSPACES;
    @ISA = ('GENERIC_WRAPPER');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($parent, $next);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$markup->{'OBEY_SPACES'}++;
	$markup->output($self->markup_start());
	$self->markup_children($markup, $self->content());
	$markup->output($self->markup_end());
	$markup->{'OBEY_SPACES'}--;

	# In HTML, a PRE cannot occur inside a paragraph, so we need to
	# "restart" the paragraph after the pre
	$parent = $self->parent();

	# This is kind of funky; self->next_element is always the same
	# as self->content if the element has content.  Since we want
	# the next element not in our content, we have to walk past 
	# it.
	$next = $self->content();
	while ($next && $next->next()) {
	    $next = $next->next();
	}
	$next = $next->next_element();

	if ($parent->tag('PARA') && $next && $next->tag('PCDATA')) {
	    $markup->output("<P>");
	}

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}

{
    package GENERIC_ASARG;
    @ISA = ('GENERIC_WRAPPER');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($text, $textmarkup);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$textmarkup = new MARKUP_OBJECT;

	$self->markup_children($textmarkup, $self->content());
	$text = $textmarkup->formatted_markup();

	$text =~ s/\n/ /g;

	$markup->output($self->markup_start() . "$text");

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}

{ 
    package GENERIC_TITLED;
    @ISA = ('GENERIC_TAG');

    sub markup_title {
	my($self, $markup) = @_;
	my($child) = $self->content();

	if ($child && ($child->tag('TITLE'))) {
	    my($block) = $child->block_markup($child);
	    my($title) = $child->markup();
	    
	    # Handle <?lb> in title
	    $title =~ s/<\001lb>/ /g;

	    if ($main::PI_NUMBERED_SECTIONS) {
		if ($self->tag() =~ /^SECT\d/) {
		    $title = $self->{'CHP-NUM'} . "." .
			$self->{'SECT-NUM'} . ". $title";
		} elsif ($self->tag() =~ /^REFSECT\d/) {
		    $title = $self->{'CHP-NUM'} . "." .
			$self->{'RSECT-NUM'} . ". $title";
		} else {
		    #nop;
		}
	    }

	    if ($self->tag('TABLE') || $self->tag('FIGURE')) {
		my($objname);

		$objname = "Table ";
		$objname = "Figure " if $self->tag('FIGURE');

		$objname .= $self->{'CHP-NUM'} . "-" . $self->{'ITEM-NUM'};
		$title = "$objname: $title";
	    }

	    if ($self->attr('ID')) {
		$title = "<A NAME=\"" . $self->id() . "\">" 
		    . $title . "</A>";
	    }

	    if ($self->markup_end() =~ /^<\/h\d>$/) {
		# <H\d> markup only goes around the title...
		$markup->output($self->markup_start() 
				. $title
				. $self->markup_end());
	    } else {
		# Other sorts of markup (e.g. <blockquote>) goes around the
		# whole element
		$markup->output($self->markup_start() . $title);
	    }

	    $block->markup($markup) if $block;
	} else {
	    $markup->output($self->markup_start());
	}

	$markup;
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($child) = $self->content();

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$self->markup_title($markup);
	$child = $child->next() if $child->tag('TITLE');
	$self->markup_children($markup, $child);

	$markup->output($self->markup_end())
	    if $self->markup_end() !~ /^<\/h\d>$/;

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}

{
    package GENERIC_INDEX_TERM;
    @ISA = ('GENERIC_INLINE');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$markup->output($self->markup_start());
	$self->markup_children($markup, $self->content());
	$markup->output($self->markup_end());

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}

{
    package GENERIC_LIST;
    @ISA = ('GENERIC_TAG');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($prev) = $self->prev() || $self->parent();

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	if ($prev && $prev->tag() !~ /^TITLE/) {
	    $markup->output("<P>");
	}

	$markup->output($self->markup_start());
	$self->markup_children($markup, $self->content());
	$markup->output($self->markup_end());

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}

{
    package PARA;
    @ISA = ('GENERIC_WRAPPER');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($prev) = $self->prev();
	my($parent) = $self->parent();
	my($gparent) = "";

	$gparent = $parent->parent() if defined ($parent);

	$prev = $prev->tag() if $prev;
	$parent = $parent->tag() if $parent;
	$gparent = $gparent->tag() if $gparent; 

	$prev = "" if !$prev;
	$parent = "" if !$parent;

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

#	print STDERR "PARA: $gparent, $parent, $prev\n";

	# Don't output <P> after <LI>
	$markup->output($self->markup_start())
	    if ($gparent eq 'VARLISTENTRY' 
		|| $gparent eq 'GLOSSENTRY' 
		|| ($prev ne "" || $parent ne 'LISTITEM'));

	$self->markup_children($markup, $self->content());
	$markup->output($self->markup_end());

	if ($parent ne 'FOOTNOTE' 
	    && $gparent ne 'FOOTNOTE' 
	    && @FOOTNOTE::footnotes) {
	    $markup->output("<blockquote>");
	    while (@FOOTNOTE::footnotes) {
		$_ = shift @FOOTNOTE::footnotes;
		$markup->output($_);
	    }
	    $markup->output("</blockquote>");
	}

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}

{ 
    package SECT1;
    @ISA = ('GENERIC_TITLED');

    $SECT_COUNT = 0;

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($child) = $self->content();
	my($node) = $self->parent();
	my($newfile) = 1;
	my($sfile) = $main::FILTERCFG;
	my($uptext, $prevtext, $nexttext);
	my($up, $next, $prev, $title, $title_obj);
	my($prevtitle, $nexttitle, $uptitle);
	
	$markup = new MARKUP_OBJECT if !$inmarkup;

	$uptext   = &main::cfg($main::bookfiles, $sfile, 'html_uptext');
	$prevtext = &main::cfg($main::bookfiles, $sfile, 'html_prevtext');
	$nexttext = &main::cfg($main::bookfiles, $sfile, 'html_nexttext');

	$home = &main::cfg($main::bookfiles, $sfile, 'html_home');
	$orahome = &main::cfg($main::bookfiles, $sfile, 'html_orahome');
	$hometxt = &main::cfg($main::bookfiles, $sfile, 'html_hometxt');
	$orahometxt = &main::cfg($main::bookfiles, $sfile, 'html_orahometxt');

	$SECT_COUNT++;
	$self->{'CHUNK'} = $SECT_COUNT;
	$newfile = 0 if $SECT_COUNT == 1;

	$self->default_markup($markup);
	($prev, $prevtitle) = $self->prev_html_file();
	($next, $nexttitle) = $self->next_html_file();

	if ($newfile) {
	    my($newname) = $self->newname(sprintf("-%02d", $SECT_COUNT));
	    $markup->change_output_files($newname);
	}

	($prev, $prevtitle) = $self->prev_html_file();
	($next, $nexttitle) = $self->next_html_file();

#	print STDERR "Sect1: prev=$prev, next=$next\n";

	$up = &main::cfg($main::bookfiles, 
			 $main::PI_SOURCE_FILE, 
			 'html_root');

	$uptitle = &main::cfg($main::bookfiles, 
			      $main::PI_SOURCE_FILE, 
			      'book_title');

	$title_obj = $self->content();
	if ($title_obj->tag('TITLE')) {
	    $title = "[Chapter " . $self->{'CHP-NUM'} . "] ";
	    $title .= $title_obj->markup();

	    # Handle <?lb> in title
	    $title =~ s/<\001lb>/ /g;

	} else {
	    $title = "SECT1: UNKNOWN TITLE\n";
	}

	if ($newfile) {
	    $markup->output("<html>\n<head>\n");
	    $markup->output("<title>$title</title>\n");
	    $markup->output("<META NAME=\"metadata\"   CONTENT=\"dublincore.0.1\">\n");
	    $markup->output("<META NAME=\"subject\"    CONTENT=\"JavaScript\">\n");
	    $markup->output("<META NAME=\"title\"      CONTENT=\"JavaScript: The Definitive Guide, Beta\">\n");
	    $markup->output("<META NAME=\"author\"     CONTENT=\"David Flanagan\">\n");
	    $markup->output("<META NAME=\"otheragent\" CONTENT=\"gmat dbtohtml\">\n");
	    $markup->output("<META NAME=\"source\"     CONTENT=\"SGML\">\n");
	    $markup->output("<META NAME=\"publisher\"  CONTENT=\"O'Reilly &amp; Associates, Inc.\">\n");
	    $markup->output("<META NAME=\"date\"       CONTENT=\"" . &main::date() . "\">\n");
	    $markup->output("<META NAME=\"objecttype\" CONTENT=\"book section\">\n");
	    $markup->output("<META NAME=\"form\"       CONTENT=\"html\">\n");

	    $markup->output("</head>\n<body bgcolor=\"\#FFFFFF\">\n");

	    $markup->output("<a href=\"$up\">$uptext</a>\n") if $up;
	    $markup->output("<a href=\"$prev\">$prevtext</a>\n") if $prev;
	    $markup->output("<a href=\"$next\">$nexttext</a>\n") if $next;

	    $markup->output("<br>\n<hr>\n<p>\n");
	}

	$self->markup_title($markup);
	$child = $child->next() if $child->tag('TITLE');
	$self->markup_children($markup, $child);

	# Output the page footer...
	$markup->output("\n<p>\n");
	$markup->output("\n<HR>\n");
	$markup->output("\n<p>\n");

	if ($next) {
	    $markup->output("<a href=\"$next\">$nexttext</a>&nbsp; ");
	    $markup->output("$nexttitle<br>\n");
	}
	if ($prev) {
	    $markup->output("<a href=\"$prev\">$prevtext</a>&nbsp; ");
	    $markup->output("$prevtitle<br>\n");
	}
	if ($up ne "") {
	    $markup->output("<a href=\"$up\">$uptext</a>&nbsp; ");
	    $markup->output("$uptitle<br>\n");
	}

	$markup->output("\n<p>\n");
	$markup->output("\n<HR>\n");
	$markup->output("\n<p>\n");

	$markup->output("<a href=\"$home\">$hometxt</a> \n");
	$markup->output("<a href=\"$orahome\">$orahometxt</a>");

	$markup->output("</body>\n</html>\n");

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}

{
    package BOOK;
    @ISA = ('GENERIC_WRAPPER');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($bktitle) = &main::cfg($main::bookfiles, 
				  $main::FILTERCFG,
				  "book_title")
	    || "No title supplied in $main::BOOKFILES";

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$markup->output("<html>\n<head>\n<title>$bktitle</title>");
	$markup->output("</head>\n<body>\n");
	
	$self->markup_children($markup, $self->content());

	$markup->output("</body>\n</html>\n");

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}

{
    package GLOSSARY;
    @ISA = ('GENERIC_CHAPTER');
    
    sub new {
	my($type) = shift @_;
	my($self) = GENERIC_TAG->new(@_);

	$self->{'IN_DL'} = 0;

	bless $self, $type;

	return $self;
    }
}

{
    package X_GLOSSARY;
    @ISA = ('GENERIC_CHAPTER');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($child) = $self->content();
	my($chapnum) = $main::PI_CHAPTER_NUMBER || "1";
	my($html, $htmlpath, $newname, $title);
	my($thishref, $prevhref, $nexthref, $prevtitle, $nexttitle);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$FOOTNOTE::footnote_count = 0;
	$SECT1::SECT_COUNT = 0;

	$newname = $main::PI_BASENAME;
	if (!defined($newname)) {
	    $newname = $main::PI_SOURCE_FILE;
	    $newname = $1 if $newname =~ /^(.*)\.[^\.]+$/;
	}
	$newname = $1 if $newname =~ /^.*\/([^\/]+)$/;
	$newname .= $main::HTML_EXT;

	$htmlpath = &main::cfg($main::bookfiles, 
			       $main::PI_SOURCE_FILE,
			       'html_path');
	$htmlpath .= "/" if $htmlpath && $htmlpath !~ /\/$/;

	$markup->change_output_files("$htmlpath$newname");

	$self->default_markup($markup);

	&main::WARNING("Chapter with missing ID will not appear in TOC.")
	    if !$self->id();

	$html = "<h1>";
	$html .= "<A NAME=\"" . $self->id() . "\">"
	    if $self->id();

	if ($child && ($child->tag('TITLE'))) {
	    $title = $child->markup();
	    
	    # Handle <?lb> in title
	    $title =~ s/<\001lb>/ /g;

	    $html .= $self->{'CHP-NUM'} . ". "
		if $main::PI_NUMBERED_SECTIONS;

	    $html .= $title;

	    $html .= "</A>" if $self->id();

	    $html .= "</h1>";

	    $child = $child->next();
	} else {
	    $title = "";
	    $html = "<!--Chapter $chapnum: no title-->";
	    &main::WARNING("No title on Chapter $chapnum");
	}

	$main::HTML_TITLE = $title;

	$markup->output("<html>");
	$markup->output("<head>");
	$markup->output("<title>$title</title>");
	$markup->output("</head>");
	$markup->output("<body>");

	$self->prevnext($markup, 1);

	$markup->output($html);

	$markup->output("<dl>\n");

	# Build the TOC (to figure out IDs on sections), but 
	# discard the markup...
	$self->toc();

	$self->markup_children($markup, $child);

	$self->prevnext($markup, 0);

	$markup->output("</dl>\n");

	$markup->output($self->markup_end());

	$markup->output("\n</body>\n</html>\n");

	# The basename only applies to this chapter...
	undef $main::PI_BASENAME;

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}

{ 
    package PCDATA;
    @ISA = ('GENERIC_INLINE');

    sub markup_start {
	my($self) = @_;

	return "";
    }

    sub markup_end {
	my($self) = @_;
	
	return "";
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	local($_) = $self->{'DATA'};

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$markup->output_text($_);

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}

{ 
    package SDATA;
    @ISA = ('GENERIC_INLINE');

    sub markup_start {
	my($self) = @_;

	return "";
    }

    sub markup_end {
	my($self) = @_;
	
	return "";
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($t);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$t = $self->{'SDATA'};
	if ($t =~ /^\[\s*(\S+)\s*\]$/) {
	    $t = $1;
	    if (defined($main::ENTITIES{"&$t"})) {

		if ($markup->{'OBEY_SPACES'} 
		    && ($t eq "ldquo" || $t eq "rdquo")) {
		    $t = "\"";
		} else {
		    $t = $main::ENTITIES{"&$t"};
		}

	    } else {
		&main::WARNING("$0: Unrecognized entity: $t\n");
		$t = "??ENTITY-$t";
	    }
	} else {
	    # Just an ordinary textual replacement
	}

	$markup->output($t);

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}

{ 
    package PI;
    @ISA = ('GENERIC_INLINE');

    sub markup_start {
	my($self) = @_;

	return "";
    }

    sub markup_end {
	my($self) = @_;
	
	return "";
    }

    sub parse_pi {
	my($self) = @_;
	my($pi, $rest);
        local($_) = $self->{'PI'};

	# Avoid regular expressions in case there are RE characters
	# in the PI

	while ($_ ne "" 
	       && (substr($_, 0, 1) eq " "
		   || substr($_, 0, 1) eq "\t")) {
	    $_ = substr($_, 1);
	}

	$pi = "";
	while ($_ ne "" 
	       && (substr($_, 0, 1) ne " "
		   && substr($_, 0, 1) ne "\t")) {
	    $pi .= substr($_, 0, 1);
	    $_ = substr($_, 1);
	}

	while ($_ ne "" 
	       && (substr($_, 0, 1) eq " "
		   || substr($_, 0, 1) eq "\t")) {
	    $_ = substr($_, 1);
	}

	$rest = $_;

	$pi =~ s/-/_/g;

	($pi, $rest);
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($pi, $rest, $sub);
	local($_);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	($pi, $rest) = $self->parse_pi();

	if (!$BAD_PI{$pi}) {
	    eval "\*stab = \*{\"main::\"}";
	    if ($stab{"PI_$pi"} ne "*main::PI_$pi") {
		&main::WARNING("Unsupported processing instruction \"$pi\" ignored.\n");
		$BAD_PI{$pi} = 1;
	    } else {
		$sub = "main::PI_${pi}";
		$subptr = \&$sub;
		$_ = &$subptr($self, $markup, $rest);
	    }
	}

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}

{ 
    package SIMPLELIST;
    @ISA = ('GENERIC_TAG');
    
    sub new {
	my($type) = shift @_;
	my($self) = GENERIC_TAG->new(@_);

	$self->{'COLSEP'} = "\t";

	bless $self, $type;

	return $self;
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($child) = $self->content();
	my($child_markup) = "";
	my(@STACK) = ();
	my($type, $elements, $cols, $rows, $thiscol, $count, $table);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	while ($child) {
	    if ($child->tag('MEMBER')) {
		$child_markup = $child->markup();
		push(@STACK, $child_markup);
	    }
	    $child = $child->next();
	}

	$table = "";

	while (@STACK) {
	    $_ = shift(@STACK);
	    if (@STACK) {
		$table .= ", " if $table;
		$table .= "$_";
	    } else {
		$table .= ", and $_";
	    }
	}

	$markup->output($table);

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}

{
    package XREF;
    @ISA = ('GENERIC_INLINE');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($linkend) = $self->attr('LINKEND');
	my($endterm) = $self->attr('ATTR');
	my($pattern, $linkto);
	my($xreftag, $xrefchap, $xrefsect, $xrefrsct, $xrefitem, $xreftxt);
	my($xrefnum, $xrefwarning);
	my($sep, $sep_config, $href);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$linkto = $linkend;
	$linkto = $endterm if $endterm ne "";	

	if (defined($main::TAGID->{$linkto})) {
	    $main::TAGCOUNT{$linkto}++;

	    $xreftag = $main::TAGID->{$linkto}->{'TAG'};
	    $xrefchap = $main::TAGID->{$linkto}->{'CHP-NUM'};
	    $xrefsect = $main::TAGID->{$linkto}->{'SECT-NUM'};
	    $xrefrsct = $main::TAGID->{$linkto}->{'RSECT-NUM'};
	    $xrefitem = $main::TAGID->{$linkto}->{'ITEM-NUM'};
	    $xreftxt = $main::TAGID->{$linkto}->{'TITLE'};

	    $pattern = &main::cfg($main::bookfiles, 
				  $main::FILTERCFG,
				  "xref_$xreftag");

	    if ($main::TAGCOUNT{"$linkto"} > 1) {
		my($spattern) = &main::cfg($main::bookfiles, 
					   $main::FILTERCFG,
					   "xrefs_$xreftag");
		$pattern = $spattern if $spattern ne "";
	    }

	    &main::WARNING("Warning: no pattern for $xreftag ($linkto)\n")
		if !$pattern;

	    ($sep_config = "${xreftag}_xref_num_sep") =~ tr/A-Z/a-z/;
	    $sep = &main::cfg($main::bookfiles, 
			      $main::FILTERCFG,
			      $sep_config) 
		|| &main::cfg($main::bookfiles, 
			      $main::FILTERCFG,
			      'xref_num_sep');

	    $xrefnum = $xrefchap;
	    $xrefwarning = 0;

	    if (($xreftag eq "TABLE") 
		|| ($xreftag eq "FIGURE") 
		|| ($xreftag eq "EXAMPLE")) {
		$xrefnum .= ".$xrefitem";
	    } elsif ($xreftag =~ /^SECT[1-5]$/) {
		$xrefnum .= ".$xrefsect";
	    } elsif ($xreftag =~ /^REFSECT[1-5]$/) {
		$xrefnum .= ".$xrefrsct";
	    } elsif (($xreftag ne "CHAPTER")
		     && ($xreftag ne "APPENDIX")
		     && ($xreftag ne "PREFACE")) {
		$xrefwarning = 1;
	    }

	    $xrefnum =~ s/\./$sep/g if ($sep && ($sep ne '.'));

	    &main::WARNING("Numeric cross reference to $xreftag contains only the chapter number.\n")
		if $xrefwarning && ($pattern =~ /\%n/);

	    $pattern =~ s/\%n/$xrefnum/g;
	    $pattern =~ s/\%t/$xreftxt/g;
	} else {
	    &main::WARNING("Warning: xref ID \"$linkto\" is unknown.\n");
	    $pattern = "";
	}

	$pattern = "**UNKNOWN XREF**" if $pattern eq "";

	if ($main::TAGID->{$linkto}) {
	    $basename = $main::TAGID->{$linkto}->{'BASENAME'};
	    $chunk = $main::TAGID->{$linkto}->{'CHUNK'};
	    $inaspec = $main::TAGID->{$linkto}->{'IN_A_SPEC'};
	    $tag = $main::TAGID->{$linkto}->{'TAG'};

	    $chunk = 1 if (!$chunk && 
			   ($main::TAGID->{$linkto}->{'TAG'} eq 'CHAPTER'
			    || $main::TAGID->{$linkto}->{'TAG'} eq 'APPENDIX'
			    || $main::TAGID->{$linkto}->{'TAG'} eq 'PREFACE'));
	    
	    if ($chunk) {
		$href = sprintf("%s-%02d%s", $basename, $chunk, 
				$main::HTML_EXT);
	    } else {
		$href = sprintf("%s%s", $basename, $main::HTML_EXT);
	    }
	    
	    $href .= "#$linkto" if ($tag ne 'CHAPTER'
				    && $tag ne 'APPENDIX'
				    && $tag ne 'PREFACE'
				    && $tag ne 'GLOSSARY'
				    && $tag ne 'REFENTRY');
	} else {
	    &main::WARNING("Warning: link ID \"$linkto\" is unknown.\n");
	}

	$markup->output("<A HREF=\"$href\">$pattern</A>");

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}

{
    package VARLISTENTRY;
    @ISA = ('GENERIC_WRAPPER');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($child) = $self->content();
	my($terms) = "";
	my($blocklist, $last, $block) = (0, 0, 0);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	if ($self->tag('GLOSSENTRY')) {
	    my($parent) = $self->parent();
	    if ($parent && !$parent->{'IN_DL'}) {
		$markup->output("<DL>");
		$parent->{'IN_DL'} = 1;
	    } else {
		$markup->output ("<P>");
	    }
	}

	while ($child->tag('TERM') || $child->tag('GLOSSTERM')) {
	    $block = $child->block_markup();
	    if ($last) {
		$last->{'NEXT'} = $block;
		$block->{'PREV'} = $last;
	    } else {
		$blocklist = $block;
		$last = $blocklist;
	    }
	    $last = $last->next() while $last && $last->next();

	    if ($child->tag('GLOSSTERM')) {
		my($tdata) = $child->markup();
		my($title) = $tdata;
		my($key);

		$title =~ s/<.*?>//g;
		$key = $title;

		$key =~ s/\s+/-/g;
		$key = "G-$key";
		$key =~ tr/a-z/A-Z/;

		$child->{'ATTR'}->{'ID'} = $key; # for later...

		$main::TAGID->{$key} = { } if !defined($main::TAGID->{$id});
		$main::TAGID->{$key}->{'TAG'} = 'GLOSSTERM';
		$main::TAGID->{$key}->{'TITLE'} = $title;

		$terms .= "<A NAME=\"$key\">$tdata</A> ";
	    } else {
		$terms .= $child->markup() . " ";
	    }

	    $child = $child->next();
	}
	chop($terms);		# get rid of the trailing blank

	# Dumb browser bug, need another <P> to make it look good.
	if ($self && $self->prev() && ($self->prev())->tag('VARLISTENTRY')) {
	    $markup->output("<p>");
	}

	$markup->output($self->markup_start());
	$markup->output($terms) if $terms;
	$markup->output($self->markup_end());
	for ($block = $blocklist; $block; $block = $blocklist->next()) {
	    $block->markup($markup);
	}
	$self->markup_children($markup,$child);

	if ($self->tag('GLOSSENTRY')) {
	    my($next) = $self->next();
	    if (!$next || !$next->tag('GLOSSENTRY')) {
		$markup->output("</DL>");
		$parent->{'IN_DL'} = 0;
	    }
	}

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}

{
    package LISTITEM;
    @ISA = ('GENERIC_INLINE');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	if (!$self->parent()->tag('VARLISTENTRY')) {
	    $markup->output($self->markup_start());
	    $self->markup_children($markup, $self->content());
	    $markup->output($self->markup_end());
	} else {
	    $self->markup_children($markup, $self->content());
	}

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}

{
    package SYSTEMITEM;
    @ISA = ('GENERIC_INLINE');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($content);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$content = $self->markup_children(undef, $self->content());

	if ($content =~ /[a-z]:\/\/[^\/]/) {
	    $content =~ s/\s+//g;
	    $content = "<A HREF=\"$content\">$content</A>";
	}

	$markup->output($content);

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}

{ 
    package GRAPHIC;
    @ISA = ('GENERIC_WRAPPER');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($format, $fileref) = ($self->attr('FORMAT'),
				 $self->attr('FILEREF'));
	my($imageok, $gif);
	my($parent, $figure_number, $alt_text);

	my($eref) = $self->attr('ENTITYREF');

	if ($eref && defined($sgmlsESIS::ExternalEntity{$eref})) {
	    $fileref = $sgmlsESIS::ExternalEntity{$eref};
	    print STDERR "Entity $eref=>$fileref\n" if $VERBOSE;
	}

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$figure_number = "from the text";
	$parent = $self->parent();
	if ($parent && $parent->tag('FIGURE') && $parent->id()) {
	    my($id, $chap, $item);
	    $id = $parent->id();

	    $chap = $main::TAGID->{$id}->{'CHP-NUM'};
	    $item = $main::TAGID->{$id}->{'ITEM-NUM'};

	    $figure_number = "$chap-$item";
	}

	# If the format is unspecified, try to figure it out...
	if ($format =~ /^\s*$/) {
	    $format = "gif" if $fileref =~ /\.gif$/;
	    $format = "jpg" if $fileref =~ /\.jpg$/;
	    $format = "eps" if $fileref =~ /\.eps\.?g?z?/i;

	    # PS is an EPS?
	    $format = "eps" if $fileref =~ /\.ps\.?g?z?/i;

	    # GIF if nothing else?
	    $format = "gif" if $format =~ /^\s*$/;
	}

	# For print books, the most common format (at ORA) is EPS.
	# If this looks like an EPS figure, see if there's a gif
	# instead/also.

	$gif = $fileref;

	if ($format =~ /eps/i || $fileref =~ /\.eps/) {
	    $gif = $1 if $gif =~ /.*\/([^\/]+)$/;
	    $gif =~ s/\.gz$//;
	    $gif =~ s/\.Z$//;
	    $gif =~ s/\.eps$//;
	    $gif =~ s/\.ps$//;
	}

# Massage the gif file to make it 8.3 if possible
	{
	    my($path) = "";
	    if ($gif =~ /^(.*\/)([^\/]+)$/) {
		$path = $1;
		$gif = $2;
	    }
	    
	    $gif = "g$1" if $gif =~ /^gff\.(.*)$/;
	    $gif =~ s/\.//g;
	    
	    $gif = "$path$gif";
	}

	$gif .= ".gif";
	    
	$gif = "figs.gif/$gif" if -f "figs.gif/$gif";
	$gif = "gifs/$gif" if -f "gifs/$gif";
	$gif = "figs/$gif" if -f "figs/$gif";

	if (-f $gif) {
	    $format = 'gif';
	    $fileref = $gif;
	} else {
	    &main::WARNING("Cannot find $gif; leaving EPS\n");
	}

	if ($format =~ /^tbl$/i) {
	    &main::WARNING("$0: Can\'t handle TBL graphics!\n");
	    $markup->output("[TBL GRAPHIC: $fileref]");
	} elsif ($format =~ /^eqn$/i) {
	    &main::WARNING("$0: Can\'t handle EQN graphics!\n");
	    $markup->output("[EQN GRAPHIC: $fileref]");
	} elsif ($format =~ /^eps$/i || $format =~ /^postscript$/i) {
	    &main::WARNING("$0: Can\'t handle PostScript graphics!\n");
	    $markup->output("[EPS GRAPHIC: $fileref]");
	} elsif ($format !~ /^gif$/i) {
	    &main::WARNING("$0: unrecognized format $format in <GRAPHIC> tag.\n");
	} else {		# ASSUME ITS GIF!
	    my (@figdirs, @figfiles, $figdir, $figfile, $found);

	    @figdirs = (".", "./figs", "./figs.gif", "./gifs");

	    @figfiles = ("$fileref", "$fileref.gif");

	    $found = "";
	    foreach $figdir (@figdirs) {
		foreach $figfile (@figfiles) {
		    $found = "$figdir/$figfile"
			if -f "$figdir/$figfile" && -r "$figdir/$figfile";
		    last if $found;
		}
		last if $found;
	    }

	    $fileref = $found if $found;

	    if (! -f $fileref) {
		&main::WARNING("$0: Cannot find image $fileref from <GRAPHIC> tag!\n");
		return;
	    } else {
		$fileref =~ s/^.*\/([^\/]+)$/$1/;
		$fileref = "figs/$fileref";
	    }

	    $alt_text = "[Graphic: Figure $figure_number]";

	    $markup->output("\n<p>\n<img align=center src=\"$fileref\" alt=\"$alt_text\">");
	}

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}

{
    package ULINK;
    @ISA = ('GENERIC_TAG');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($type) = $self->attr('TYPE');
	my($url) = $self->attr('URL');

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	&main::WARNING("Warning: unknown ULINK type: $type\n")
	    if ($type && $type !~ /^cprog$/i);

	open (F, $url) 
	    || &main::WARNING("Warning: cannot open URL in ULINK: $url\n");

	while (<F>) {
	    s/\&/\&amp;/g;
	    s/>/\&gt;/g;
	    s/</\&lt;/g;
	    $markup->output($_);
	}
	close (F);

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}

{
    package LINK;
    @ISA = ('GENERIC_INLINE');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($id) = $self->attr('LINKEND');
	my($href) = "";
	my($basename, $chunk);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	if ($main::TAGID->{$id}) {
	    $basename = $main::TAGID->{$id}->{'BASENAME'};
	    $chunk = $main::TAGID->{$id}->{'CHUNK'};

	    if ($chunk) {
		$href = sprintf("%s-%02d%s", $basename, $chunk, 
				$main::HTML_EXT);
	    } else {
		$href = sprintf("%s%s", $basename, $main::HTML_EXT);
	    }
	} else {
	    &main::WARNING("Warning: link ID \"$id\" is unknown.\n");
	}

	$markup->output("<A HREF=\"$href\">") if $href;
	$self->markup_children($markup, $self->content());
	$markup->output("</A>") if $href;

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}


######################################################################
# TABLES
######################################################################

{   # Just a placeholder, this is where table column info goes
    package ColSpec;

    sub init {
	$curcolspec = "";
	$DEFAULT_ALIGN = "";
	$COLUMN = 0;
	$DEFAULT_COLSEP = "";
	$DEFAULT_ROWSEP = "";
	%ROWSEP = ();
	%COLSEP = ();
	%ALIGN = ();
	%NAME = ();
	%COL = ();
	%CHAR = ();
	%CHAROFF = ();
	%COLWIDTH = ();
	$DEFAULT_ROLE = "";
	%ROLE = ();
    }
}

{   # Just a placeholder, this is where table span info goes
    package SpanSpec;

    sub init {
	$DEFAULT_ALIGN = "";
	$DEFAULT_COLSEP = "";
	$DEFAULT_ROWSEP = "";
	%ALIGN = ();
	%CHAR = ();
	%CHAROFF = ();
	%COLSEP = ();
	%ROWSEP = ();
	%NAMEST = ();
	%NAMEEND = ();
	$DEFAULT_ROLE = "";
	%ROLE = ();
    }
}

{   # Just a placeholder, this is where table footnote info goes
    package FootnoteSpec;

    sub init {
	@FN = ();
    }

    sub output_footnotes {
	my($fn);

	if (@FN) {
	    foreach $fn (@FN) {
		$fn =~ s/\n/\001/g;	# \n -> \001
		$fn =~ s/^\.P\001//g; # remove .P
		$fn =~ s/\001\.P/\001/g; # remove .P
		$fn =~ s/\001\.\/P/\001/g; # remove ./P
		$fn =~ s/\001\001/\001/g; # remove dup \001
		$fn = $1 if $fn =~ /^(.*)\001+$/; # trim trailing \001
		$fn =~ s/\001/\n/g;	# \001 -> \n
		&main::MARKUP(".TFS");
		&main::MARKUP($fn);
		&main::MARKUP(".TFE");
	    }
	}
    }
}

{
    package INFORMALTABLE;
    @ISA = ('GENERIC_WRAPPER');

    sub markup {
	my($self, $markup) = @_;
	my($child) = $self->content();
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$FOOTNOTE::footnote_count_old = $FOOTNOTE::footnote_count;
	$FOOTNOTE::footnote_count = 0;

	$self->{"FIRST_GROUP"} = 1;

	$self->{'SAVE_ABOVE_SPACE'} = $main::TABLE_ABOVE_RULE_SPACE;
	$self->{'SAVE_BELOW_SPACE'} = $main::TABLE_BELOW_RULE_SPACE;
	$self->{'SAVE_FRAME_RULE'}  = $main::TABLE_FRAME_RULE;

	&ColSpec::init();
	&SpanSpec::init();

	$main::ERR_TABLE_COUNT++;
	$main::ERR_TABLE_TYPE = "TABLE";

	if ($self->attr('FRAME', 'ALL')) {
	    $markup->output("<P>\n<TABLE BORDER>\n");
	} else {
	    $markup->output("<p>\n<TABLE>\n");
	}

	# ---

	$self->markup_children($markup, $child);
	
	# ---
	
	$main::TABLE_ABOVE_RULE_SPACE = $self->{'SAVE_ABOVE_SPACE'};
	$main::TABLE_BELOW_RULE_SPACE = $self->{'SAVE_BELOW_SPACE'};
	$main::TABLE_FRAME_RULE       = $self->{'SAVE_FRAME_RULE'};
	
	if (@FOOTNOTE::table_footnotes) {
            die ("TGROUP expected and not found")
		unless ($self->content->tag('TGROUP'));

	    $markup->output("<tr>\n<td colspan=");
	    $markup->output($self->content->attr('COLS'));
	    $markup->output(">\n<p>\n<b>Footnotes:</b>\n<p>\n<blockquote>");
	    while (@FOOTNOTE::table_footnotes) {
		$_ = shift @FOOTNOTE::table_footnotes;
		$markup->output($_);
	    }
	    $markup->output("</blockquote></td></tr>");
	}

	$markup->output("</TABLE>\n<P>\n");
        $FOOTNOTE::footnote_count = $FOOTNOTE::footnote_count_old;

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}

{
    package TABLE;
    @ISA = ('GENERIC_TITLED');

    sub markup {
	my($self, $markup) = @_;
	my($child) = $self->content();
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);
	
	$FOOTNOTE::footnote_count_old = $FOOTNOTE::footnote_count;
	$FOOTNOTE::footnote_count = 0;

	$self->{"FIRST_GROUP"} = 1;
	$self->{"TABLE_STARTED"} = 0;

	$self->{'SAVE_ABOVE_SPACE'} = $main::TABLE_ABOVE_RULE_SPACE;
	$self->{'SAVE_BELOW_SPACE'} = $main::TABLE_BELOW_RULE_SPACE;
	$self->{'SAVE_FRAME_RULE'}  = $main::TABLE_FRAME_RULE;

	&ColSpec::init();
	&SpanSpec::init();

	$main::ERR_TABLE_COUNT++;
	$main::ERR_TABLE_TYPE = "TABLE";

	$markup->output("<P>\n<TABLE>");

	$self->markup_title($markup);
	$child = $child->next() if $child->tag('TITLE');

	# ---

	$self->markup_children($markup, $child);

	# ---

	$main::TABLE_ABOVE_RULE_SPACE = $self->{'SAVE_ABOVE_SPACE'};
	$main::TABLE_BELOW_RULE_SPACE = $self->{'SAVE_BELOW_SPACE'};
	$main::TABLE_FRAME_RULE       = $self->{'SAVE_FRAME_RULE'};

	if (@FOOTNOTE::table_footnotes) {

            die ("TGROUP expected and not found")
		unless ($self->content()->tag('TGROUP'));

	    $markup->output("<tr>\n<td colspan=");
	    $markup->output($self->content()->attr('COLS'));
	    $markup->output(">\n<p>\n<b>Footnotes:</b>\n<p>\n<blockquote>");
	    while (@FOOTNOTE::table_footnotes) {
		 $_ = shift @FOOTNOTE::table_footnotes;
		 $markup->output($_);
	     }
	     $markup->output("</blockquote></td></tr>");
	 }

	$markup->output("</TABLE>\n<P>\n");
        $FOOTNOTE::footnote_count = $FOOTNOTE::footnote_count_old;

        $inmarkup ? $markup : $markup->formatted_markup();
    }
}

{
    package TGROUP;
    @ISA = ('GENERIC_WRAPPER');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($parent) = $self->parent();

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$pgwide = $parent->attr('PGWIDE');
	$frame  = $parent->attr('FRAME');

	$colsep = $self->attr('COLSEP');
	$rowsep = $self->attr('ROWSEP');
	$colsep = $parent->attr('COLSEP') if $colsep eq "";
	$rowsep = $parent->attr('ROWSEP') if $rowsep eq "";

	$ColSpec::DEFAULT_ALIGN = $self->attr('ALIGN');
	$ColSpec::COLUMN = 1;
	$ColSpec::DEFAULT_COLSEP = $colsep;
	$ColSpec::DEFAULT_ROWSEP = $rowsep;
	$ColSpec::DEFAULT_ROLE = $self->attr('ROLE');

	$SpanSpec::DEFAULT_ALIGN = $self->attr('ALIGN');
	$SpanSpec::DEFAULT_COLSEP = $colsep;
	$SpanSpec::DEFAULT_ROWSEP = $rowsep;
	$SpanSpec::DEFAULT_ROLE = $self->attr('ROLE');

	$tgroup_count++;
	$row_count = 0;

	if ($parent->{"FIRST_GROUP"} == 1) {
	    if ($frame eq "ALL") {
		if ($main::TABLE_FRAME_RULE eq $main::TABLE_DOUBLE_HORIZ_RULE){
		    $options = &add_option($options, "doublebox");
		} else {
		    $options = &add_option($options, "box");
		}
	    }

	    $options = &add_option($options, "expand") if $pgwide == 1;

	    $self->{"TABLE_OPTIONS"} = $options;
	    $parent->{"FIRST_GROUP"} = 0;
	}

	$needs_a_topframe = 0;
	$needs_a_topframe = 1 
	    if ($frame eq "TOP") || ($frame eq "TOPBOT");

	$self->markup_children($markup, $self->content());

	$inmarkup ? $markup : $markup->formatted_markup();
    }

    sub add_option {
	my($options, $opt) = @_;

	$options .= ", " if $options ne "";
	$options .= $opt;

	return $options;
    }
}

{
    package COLSPEC;
    @ISA = ('GENERIC_WRAPPER');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($column);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$column = $self->attr('COLNUM') || $ColSpec::COLUMN;
	$ColSpec::COLUMN = $column+1;

	$ColSpec::ALIGN{$column} = ($self->attr('ALIGN') 
				    || $ColSpec::DEFAULT_ALIGN);

	if ($self->attr('COLNAME')) {
	    $ColSpec::NAME{$column} = $self->attr('COLNAME');
	    $ColSpec::COL{$self->attr('COLNAME')} = $column;
	}

	$ColSpec::COLSEP{$column} = $self->attr('COLSEP');
	$ColSpec::ROWSEP{$column} = $self->attr('ROWSEP');

	$ColSpec::CHAR{$column} = ($self->attr('CHAR')) 
	    if $self->attr('CHAR');

	$ColSpec::CHAROFF{$column} = ($self->attr('CHAROFF'))
	    if $self->attr('CHAROFF');

	$ColSpec::COLWIDTH{$column} = ($self->attr('COLWIDTH'))
	    if $self->attr('COLWIDTH');

	$ColSpec::ROLE{$column} = ($self->attr('ROLE'))
	    if $self->attr('ROLE');

	$inmarkup ? $markup : undef;
    }
}

{
    package SPANSPEC;
    @ISA = ('GENERIC_WRAPPER');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($name);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$name = $self->attr('SPANNAME');

	$SpanSpec::ALIGN{$name}   = ($self->attr('ALIGN') 
				     || $SpanSpec::DEFAULT_ALIGN);

	$SpanSpec::CHAR{$name}    = ($self->attr('CHAR'))
	    if $self->attr('CHAR');

	$SpanSpec::CHAROFF{$name} = ($self->attr('CHAROFF'))
	    if $self->attr('CHAROFF');

	$SpanSpec::COLSEP{$name}  = $self->attr('COLSEP'); 
	$SpanSpec::ROWSEP{$name}  = $self->attr('ROWSEP'); 
	$SpanSpec::NAMEST{$name}  = $self->attr('NAMEST');
	$SpanSpec::NAMEEND{$name} = $self->attr('NAMEEND');

	$SpanSpec::ROLE{$name} = $self->attr('ROLE');

	$inmarkup ? $markup : undef;
    }
}

{
    package THEAD;
    @ISA = ('GENERIC_WRAPPER');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	@ROW::RowSpans = (0,0,0,0,0,0,0,0,0,0);

	$markup->output($self->markup_start());
	$self->markup_children($markup, $self->content());
	$markup->output($self->markup_end());
    }
}

{
    package TBODY;
    @ISA = ('GENERIC_WRAPPER');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	@ROW::RowSpans = (0,0,0,0,0,0,0,0,0,0);

	$markup->output($self->markup_start());
	$self->markup_children($markup, $self->content());
	$markup->output($self->markup_end());
    }
}

{
    package ROW;
    @ISA = ('GENERIC_TAG');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($tgroup) = $self;
	my($width) = 0;

	while ($tgroup && !$tgroup->tag('TGROUP')) {
	    $tgroup = $tgroup->parent();
	}
	$width = $tgroup->attr('COLS') if $tgroup;

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	for ($ROW::CurCol = 0; $ROW::CurCol < 10; $ROW::CurCol++) {
	    if ($ROW::RowSpans[$ROW::CurCol] > 0) {
		$ROW::RowSpans[$ROW::CurCol]--;
	    }
	}
	$ROW::CurCol = 1;

	$markup->output($self->markup_start());
	$self->markup_children($markup, $self->content());

	while ($ROW::CurCol <= $width) {
	    $markup->output("<td>&nbsp;</td>\n");
	    $ROW::CurCol++;
	}

	$markup->output($self->markup_end());
    }
}

{
    package ENTRY;
    @ISA = ('GENERIC_WRAPPER');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($parent) = $self->parent();
	my($gparent) = $parent->parent();
	my($name, $attr) = ("TD", "");
	my($rspan, $cspan, $align, $col);
	my($emarkup);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	while ($ROW::RowSpans[$ROW::CurCol]) {
	    $ROW::CurCol++;
	}

	if ($gparent->tag('THEAD')) {
	    $name = "TH";
	} else {
	    $name = "TD";
	}

	$rspan = $self->attr('MOREROWS');
	if ($rspan) {
	    $rspan++;
	    $attr .= " ROWSPAN=$rspan";
	    $ROW::RowSpans[$ROW::CurCol] = $rspan;
	}

	$cspan = $self->attr('SPANNAME');
	if ($cspan) {
	    my($namest, $nameend);
	    my($firstcol, $lastcol);
	    my($count);
	    
	    $namest  = $SpanSpec::NAMEST{$cspan};
	    $nameend = $SpanSpec::NAMEEND{$cspan};

	    $firstcol = "";
	    $lastcol  = "";

	    for ($count = 0; $count < 10; $count++) {
		$firstcol = $count
		    if $ColSpec::NAME{$count} eq $namest;
		$lastcol = $count
		    if $ColSpec::NAME{$count} eq $nameend;
	    }

	    $cspan = $lastcol - $firstcol + 1;
	    
	    $attr .= " COLSPAN=$cspan";
	}

	$align = $self->attr('ALIGN');                   # entry alignment

	if ($align eq "") {
	    $align = $self->parent()->attr('ALIGN');     # row alignment
	}

	# Default alignment for tgroup is left, bet we want to make it
	# center for c-spanning entries unless there's a more local
	# override
	if ($align eq "" && $cspan) {
	    $align="center";
	}

	if ($align eq "") {
	    $align = $ColSpec::ALIGN{$ROW::CurCol};      # column alignment
	}

	if ($align eq "") {
	    $align = "left";
	}

	if ($align) {
	    $attr .= " ALIGN=\"$align\"";
	}	    

	$markup->output("<$name$attr>"); 
	$emarkup = $self->markup_children(undef, $self->content());

	if ($emarkup =~ /^\s*$/
	    || $emarkup =~ /^[\n\s]*<p>[\n\s]*$/i) {
	    $markup->output("&nbsp;");
	} else {
	    $markup->output($emarkup);
	}

	$markup->output("</$name>\n");

	if ($cspan) {
	    $ROW::CurCol += $cspan;
	} else {
	    $ROW::CurCol++;
	}
    }
}

######################################################################
# Footnotes
######################################################################

{
    package FOOTNOTEREF;
    @ISA = ("GENERIC_INLINE");

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);

	&main::WARNING("Can\'t handle footnoteref.\n");

	$inmarkup ? $markup : undef;
    }
}

{
    package FOOTNOTE;
    @ISA = ("GENERIC_TAG");

    $footnote_count = 0;
    @footnotes = ();

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($f) = new MARKUP_OBJECT;
	local($_);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$footnote_count++;
	$markup->output("[$footnote_count]");
	$self->markup_children($f, $self->content());

	$_ = $f->formatted_markup();

	if (/^\s*<p>/i) {
	    $_ = "<p>\[$footnote_count\] $'";
	} else {
	    $_ = "[$footnote_count] $_";
	}

	push (@footnotes, $_);

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}

{
    package LITERALLAYOUT;
    @ISA = ('GENERIC_WRAPPER');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($llcontentm, $llcontent);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$markup->{'OBEY_SPACES'}++;
	$markup->output($self->markup_start());

	$llcontentm = new MARKUP_OBJECT;
	$self->markup_children($llcontentm, $self->content());

	$llcontent = $llcontentm->formatted_markup();
	$llcontent =~ s/\n/<br>\n/g;
	
	$markup->output($llcontent);

	undef $llcontentm;
	undef $llcontent;

	$markup->output($self->markup_end());
	$markup->{'OBEY_SPACES'}--;

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}

{
    package xREFMETA;
    @ISA = ('GENERIC_WRAPPER');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($child) = $self->content();
	my($title) = "";

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$markup->output($self->markup_start());

	while (!$self->null($child)) {
	    $title = $child->markup() if $child->tag('REFENTRYTITLE');
	    $child = $child->next();
	}

	$markup->output($title) if $title;

	$markup->output($self->markup_end());

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}

{
    package REFNAMEDIV;
    @ISA = ('GENERIC_WRAPPER');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($child) = $self->content();
	my($name, $purpose) = ("", "");

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$markup->output($self->markup_start());

	while (!$self->null($child)) {
	    if ($child->tag('REFNAME')) {
		$name = $child->markup();
	    } elsif ($child->tag('REFPURPOSE')) {
		$purpose = $child->markup();
	    } elsif ($child->tag('INDEXTERM')) {
		$markup->output ($child->markup());
	    } else {
		print STDERR "Unexpected tag in refnamediv: ", $child-tag(), "\n";
	    }
	    $child = $child->next();
	}

	$markup->output("<H1>$name</H1>");
	$markup->output("<H2>Name</H2>");

	if ($purpose) {
	    $markup->output("$name---$purpose");
	} else {
	    $markup->output($name);
	}
	$markup->output($self->markup_end());

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}

{
    package PHRASE;
    @ISA = ('GENERIC_TAG');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !$inmarkup;

	if ($self->role('GFFCD') || ($self->role() eq "")) {
	    $self->default_markup($markup);

#	    $markup->output($self->markup_start());
	    $self->markup_children($markup, $self->content());
#	    $markup->output($self->markup_end());
	}

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}

# ======================================================================

{ 
    package REFENTRY;
    @ISA = ('GENERIC_TITLED');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($child) = $self->content();
	my($node) = $self->{'PARENT'};
	my($newname) = "-";
	my($sfile) = $main::FILTERCFG;
	my($uptext, $prevtext, $nexttext);
	my($up, $next, $prev, $title, $title_obj);
	my($prevtitle, $nexttitle, $uptitle);
	
	my($footer, @footers, @footerhrefs, @footertxts, $count);
	
	$markup = new MARKUP_OBJECT if !$inmarkup;

	$uptext   = &main::cfg($main::bookfiles, $sfile, 'html_uptext');
	$prevtext = &main::cfg($main::bookfiles, $sfile, 'html_prevtext');
	$nexttext = &main::cfg($main::bookfiles, $sfile, 'html_nexttext');

	$footer = &main::cfg($main::bookfiles, 
			     $main::PI_SOURCE_FILE, 
			     'page_footer')
	          || &main::cfg($main::bookfiles, 
				$main::FILTERCFG,
				'page_footer');

	@footers = split(/\s+/, $footer);

	foreach $footer (@footers) {
	    my($footh) = &main::cfg($main::bookfiles, 
				    $main::PI_SOURCE_FILE, 
				    $footer)
		         || &main::cfg($main::bookfiles, 
				       $main::FILTERCFG,
				       $footer);

	    my($foott) = &main::cfg($main::bookfiles, 
				    $main::PI_SOURCE_FILE, 
				    $footer . "txt")
		         || &main::cfg($main::bookfiles, 
				       $main::FILTERCFG,
				       $footer . "txt");

	    push (@footerhrefs, $footh);
	    push (@footertxts, $foott);
	}

	$self->default_markup($markup);
	($prev, $prevtitle) = $self->prev_html_file();
	($next, $nexttitle) = $self->next_html_file();

	$newname = $self->newname();
	$markup->change_output_files($newname);

	$prev = $self->prev_html_file();
	$next = $self->next_html_file();

	$up = &main::cfg($main::bookfiles, 
			 $main::PI_SOURCE_FILE, 
			 'html_root');

	$uptitle = &main::cfg($main::bookfiles, 
			 $main::PI_SOURCE_FILE, 
			 'html_roottitle');

	$title_obj = $self->content();
	$title_obj = $title_obj->content() 
	    if $title_obj && $title_obj->tag('REFMETA');
	$title_obj = $title_obj->content() 
	    if $title_obj && $title_obj->tag('REFNAMEDIV');
	if ($title_obj 
	    && ($title_obj->tag('REFNAME') 
		|| $title_obj->tag('REFENTRYTITLE'))) {
	    $title = "[Reference] ";
	    $title .= $title_obj->markup();

	    # Handle <?lb> in title
	    $title =~ s/<\001lb>/ /g;

	} else {
	    $title = "REFENTRY: UNKNOWN REFNAME\n";
	}

	$markup->output("<html>\n<head>\n");
	$markup->output("<title>$title</title>\n");
	$markup->output("<META NAME=\"metadata\"   CONTENT=\"dublincore.0.1\">\n");
	$markup->output("<META NAME=\"subject\"    CONTENT=\"JavaScript\">\n");
	$markup->output("<META NAME=\"title\"      CONTENT=\"JavaScript: The Definitive Guide, Beta\">\n");
	$markup->output("<META NAME=\"author\"     CONTENT=\"David Flanagan\">\n");
	$markup->output("<META NAME=\"otheragent\" CONTENT=\"gmat dbtohtml\">\n");
	$markup->output("<META NAME=\"source\"     CONTENT=\"SGML\">\n");
	$markup->output("<META NAME=\"publisher\"  CONTENT=\"O'Reilly &amp; Associates, Inc.\">\n");
	$markup->output("<META NAME=\"date\"       CONTENT=\"" . &main::date() . "\">\n");
	$markup->output("<META NAME=\"objecttype\" CONTENT=\"book section\">\n");
	$markup->output("<META NAME=\"form\"       CONTENT=\"html\">\n");
	
	$markup->output("</head>\n<body bgcolor=\"\#FFFFFF\">\n");
	
	$markup->output("<a href=\"$up\">$uptext</a>\n") if $up;
	$markup->output("<a href=\"$prev\">$prevtext</a>\n") if $prev;
	$markup->output("<a href=\"$next\">$nexttext</a>\n") if $next;

	$markup->output("<br>\n<hr>\n<p>\n");

	$self->markup_title($markup);
	$child = $child->next() if $child->{'TAG'} eq 'TITLE';
	$self->markup_children($markup, $child);

	# Output the page footer...
	$markup->output("\n<p>\n");
	$markup->output("\n<HR>\n");
	$markup->output("\n<p>\n");

	if ($next) {
	    $markup->output("<a href=\"$next\">$nexttext</a>&nbsp; ");
	    $markup->output("$nexttitle<br>\n");
	}
	if ($prev) {
	    $markup->output("<a href=\"$prev\">$prevtext</a>&nbsp; ");
	    $markup->output("$prevtitle<br>\n");
	}
	if ($up ne "") {
	    $markup->output("<a href=\"$up\">$uptext</a>&nbsp; ");
	    $markup->output("$uptitle<br>\n");
	}

	$markup->output("\n<p>\n");
	$markup->output("\n<HR>\n");
	$markup->output("\n<p>\n");

	for ($count = 0; $count <= $#footerhrefs; $count++) {
	    my($hr, $txt) = ($footerhrefs[$count], $footertxts[$count]);

	    $markup->output("<a href=\"$hr\">$txt</a>&nbsp; \n");
	}

	$markup->output("</body>\n</html>\n");

	$inmarkup ? $markup : $markup->formatted_markup();
    }

    sub prev_html_file {
	return &GENERIC_TAG::prev_html_file(@_);
    }

    sub next_html_file {
	return &GENERIC_TAG::next_html_file(@_);
    }
}

{
    package REFSYNOPSISDIV;
    @ISA = ('GENERIC_WRAPPER');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$markup->output($self->markup_start());
	$markup->output("Synopsis");
	$markup->output($self->markup_end());

	$self->markup_children($markup, $self->content());

	$inmarkup ? $markup : $markup->formatted_markup();
    }
}

1;
