# -*- Perl -*-
# Classes for dbto3l
# 
# $Id: xdbto3l.cls,v 1.1.1.1 2002-07-15 21:33:31 root Exp $
#
#
##############################################################################

use vars qw($VERSION);

if ($VERBOSE) {
    my($VERSION) = '$Id: xdbto3l.cls,v 1.1.1.1 2002-07-15 21:33:31 root Exp $'; # '
    my($REQNAME) = (split(/\s+/, $VERSION))[1];
    my($vers) = (split(/\s+/, $VERSION))[2];

    print STDERR "$REQNAME version $vers.\n";
}

##############################################################################

{
    package MARKUP_OBJECT;

    sub new {
	my($ref) = bless { };

	$ref->{'MARKUP_COL'} = 0;
	$ref->{'LEADING_DOT'} = 1;
	$ref->{'OBEY_SPACES'} = 0;
	$ref->{'OUTPUT'} = "";
	$ref->{'OUTPUT_FILEHANDLE'} = "";

	return $ref;
    }

    sub text {
	my($self, $text) = @_;
	my($place, @lines, $save_);
	local($_) = $text;

	return if $_ eq "";

	# Can't just match a regexp in here because \n's 
	# cause all sorts of prob.

	# break incoming text into lines
	while (($place = index($_, "\n")) >= 0) {
	    push (@lines, substr($_, 0, $place+1));
	    $_ = substr($_, $place+1);
	}
	push (@lines, $_) if $_ ne "";

	while (@lines) {
	    # skip blank lines...
	    shift @lines
		while ($self->{'MARKUP_COL'} == 0 
		       && $self->{'OBEY_SPACES'} == 0
		       && @lines 
		       && $lines[0] =~ /^\s*$/);
	    last if !@lines;
	    
	    $_ = shift @lines;
	    
	    # discard leading whitespace
	    if (/^(\s+)/ 
		&& ($self->{'MARKUP_COL'} == 0) 
		&& ($self->{'OBEY_SPACES'} == 0)) {
		$_ = substr($_, length($1)); 
	    }

	    # protect leading periods with \&.  Neither leading whitespace
	    # nor leading font changes (\*[...] or \fX) suffice to protect
	    # a . from being leading.
	    $save_ = $_;
	    while ($_ && 
		   (/^\s+(.*)$/
		    || /^\\\*\[[^\[\]]+\](.*)$/
		    || /^\\f[A-Za-z](.*)$/)) {
		$_ = $1;
	    }
	    $self->{'OUTPUT'} .= "\\&" if /^[\.\']/ && $self->{'LEADING_DOT'};
	    $self->{'LEADING_DOT'} = 0;

	    $_ = $save_;

	    $self->{'OUTPUT'} .= $_;
	    $self->{'MARKUP_COL'} += length($_);
	    $self->{'MARKUP_COL'} = 0 if substr($_, length($_)-1, 1) eq "\n";
	    $self->{'LEADING_DOT'} = 1 if $self->{'MARKUP_COL'} == 0;
	}

	$self->flush();
    }

    sub markup {
	my($self, $text) = @_;
	my($place, @lines, $save_);
	local($_) = $text;

	return if $_ eq "";

	# Can't just match a regexp in here because \n's 
	# cause all sorts of prob.

	# break incoming text into lines
	while (($place = index($_, "\n")) >= 0) {
	    push (@lines, substr($_, 0, $place+1));
	    $_ = substr($_, $place+1);
	}
	push (@lines, $_) if $_ ne "";

	while (@lines) {
	    $_ = shift @lines;

	    if (/^\s*\./ && $self->{'MARKUP_COL'} != 0) {
		$self->{'OUTPUT'} .= "\n";
		$self->{'MARKUP_COL'} = 0;
		$self->{'LEADING_DOT'} = 1;
	    }

	    # Look to see if we are output something that protects leading
	    # dots.  Neither leading whitespace 
	    # nor leading font changes (\*[...] or \fX) suffice to protect
	    # a . from being leading.
	    $save_ = $_;
	    while (/^\s+(.*)$/
		    || /^\\\*\[[^\[\]]+\](.*)$/
		    || /^\\f[A-Za-z](.*)$/) {
		$_ = $1;
	    }
	    $self->{'LEADING_DOT'} = 0 if !/^\s*$/;
	    $_ = $save_;
	    
	    $self->{'OUTPUT'} .= $_;
	    $self->{'MARKUP_COL'} += length($_);
	    $self->{'MARKUP_COL'} = 0 if substr($_, length($_)-1, 1) eq "\n";
	    $self->{'LEADING_DOT'} = 1 if $self->{'MARKUP_COL'} == 0;
	}

	if (/^\s*\./ && $self->{'MARKUP_COL'} > 0) {
	    $self->{'OUTPUT'} .= "\n";
	    $self->{'MARKUP_COL'} = 0;
	    $self->{'LEADING_DOT'} = 1;
	}

	$self->flush();
    }

    sub flush {
	my($self) = @_;
	my($pr);

	if ($self->{'OUTPUT_FILEHANDLE'}) {
	    $pr = $self->{'OUTPUT_FILEHANDLE'};
	    eval "print $pr \$self->{'OUTPUT'}";
	    $self->{'OUTPUT'} = "";
	}
    }

    sub output_filehandle {
	my($self, $file) = @_;

	$self->{'OUTPUT_FILEHANDLE'} = $file;
    }

    sub output {
	my($self) = @_;
	
	return $self->{'OUTPUT'};
    }
}

sub gen_empty_subclass {
    my($superclass, $tag) = @_;
    local($_) = '{
       package __PACKAGE__;
       @ISA = (\'__SUPERCLASS__\');

       sub new {
	   return bless { };
       }
   }';

   s/__PACKAGE__/$tag/g;
   s/__SUPERCLASS__/$superclass/g;

   eval $_;
}

sub gen_titled {
    my($tag) = @_;
    &gen_empty_subclass('GENERIC_TITLED', $tag);
}

sub gen_chapter {
    my($tag) = @_;
    &gen_empty_subclass('GENERIC_CHAPTER', $tag);
}

sub gen_inline {
    my($tag) = @_;
    &gen_empty_subclass('GENERIC_INLINE', $tag);
}

sub gen_index_term {
    my($tag) =  @_;
    &gen_empty_subclass('GENERIC_INDEX_TERM', $tag);
}    

sub gen_block {
    my($tag) = @_;
    &gen_empty_subclass('GENERIC_WRAPPER', $tag);
}

sub gen_dottag {
    &gen_block(@_);
}

sub gen_obeyspaces {
    my($tag) = @_;
    &gen_empty_subclass('GENERIC_OBEYSPACES', $tag);
}

sub gen_asarg {
    my($tag) = @_;
    &gen_empty_subclass('GENERIC_ASARG', $tag);
}

sub gen_nop {
    my($tag) = @_;
    &gen_empty_subclass('GENERIC_NOP', $tag);
}

{
    package GENERIC_TAG;

    $TAG_NULL = "::TAG::NULL::";

    # TAG = "tag"           name of this tag
    # CONTENT->tag          pointer to first subelement
    # CONTENT_LAST->tag     pointer to last subelement
    # NEXT->tag             pointer to next element at this level
    # PREV->tag             pointer to prev element at this level
    # PARENT->tag           pointer to parent element
    # ATTR->{}              attributes of this tag
    # NEXT_ELEMENT->tag     pointer to next element of this type
    # PREV_ELEMENT->tag     pointer to prev element of this type

    sub init {
	my($self) = @_;

	$self->{'NEXT'} = $TAG_NULL;
	$self->{'PREV'} = $TAG_NULL;
	$self->{'CONTENT'} = $TAG_NULL;
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
	    if ($ref =~ /^\.(\S+)/) {
		$ref = "./$1";
	    } elsif ($ref =~ /^\\\*\[(.*)$/) {
		$ref = "\\\*\[/$1";
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
	my($chap, $sect, $refsect, $count, $title);
	my($s, $child, $id, $tag, $sourcefile);

	return if $self->{'ATTR'}->{'ID'} eq "";

	$id = $self->{'ATTR'}->{'ID'};
	$tag = $self->{'TAG'};

	$chap = $main::PI_CHAPTER_NUMBER || "1";

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
	$child = $child->next() if $child && $child->tag('DOCINFO');
	if ($child && $child->tag('TITLE')) {
	    $title = $child->markup();
	    $title =~ s/<.lb>/ /g;
	    $title =~ s/\n/ /g;
	}

	# In order to be able to determine if an ID has expired, we
	# need to know what file it came from; so remember that...
	$sourcefile = $main::PI_SOURCE_FILE;
	$sourcefile = $1 if $sourcefile =~ /.*\/([^\/]+)$/;

	$main::TAGID->{$id} = { } if !defined($main::TAGID->{$id});
	$main::TAGID->{$id}->{'*TOUCHED'} = 1;

#	print STDERR "CHANGE: $main::TAGID_CHANGE:";

	if ($tag ne 'INDEXTERM') {
	    $main::TAGID_CHANGE = 
		($main::TAGID_CHANGE 
		 || $main::TAGID->{$id}->{'TAG'} ne $tag
		 || $main::TAGID->{$id}->{'SOURCEFILE'} ne $sourcefile
		 || $main::TAGID->{$id}->{'CHP-NUM'} ne $chap
		 || $main::TAGID->{$id}->{'SECT-NUM'} ne $sect
		 || $main::TAGID->{$id}->{'RSECT-NUM'} ne $refsect
		 || $main::TAGID->{$id}->{'ITEM-NUM'} ne $count
		 || $main::TAGID->{$id}->{'TITLE'} ne $title);
	}
	    
#	print STDERR " t(" . $main::TAGID->{$id}->{'TAG'} . "|" . $tag;
#	print STDERR " c(" . $main::TAGID->{$id}->{'CHP-NUM'} . "|" . $chap;
#	print STDERR " s(" . $main::TAGID->{$id}->{'SECT-NUM'} . "|" . $sect;
#	print STDERR " r(" . $main::TAGID->{$id}->{'RSECT-NUM'} . "|" . $refsect;
#	print STDERR " c(" . $main::TAGID->{$id}->{'ITEM-NUM'} . "|" . $count;
#	print STDERR " t(" . $main::TAGID->{$id}->{'TITLE'} . "|" . $title;
#	print STDERR " $main::TAGID_CHANGE\n";

	$main::TAGID->{$id}->{'TAG'} = $tag;
	$main::TAGID->{$id}->{'SOURCEFILE'} = $sourcefile;
	$main::TAGID->{$id}->{'CHP-NUM'} = $chap;
	$main::TAGID->{$id}->{'SECT-NUM'} = $sect;
	$main::TAGID->{$id}->{'RSECT-NUM'} = $refsect;
	$main::TAGID->{$id}->{'ITEM-NUM'} = $count;
	$main::TAGID->{$id}->{'TITLE'} = $title;
    }

    sub attr {
	my($self, $attr, $value) = @_;
	my($self_value);

	$self_value = uc($self->{'ATTR'}->{$attr});
	if (defined($value)) {
	    $value = uc($value);
	    return $value eq $self_value;
	} else {
	    return $self_value;
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

    sub parent {
	my($self) = @_;
	$self->{'PARENT'};
    }

    sub content {
	my($self) = @_;

	while (($self->{'CONTENT'} eq $TAG_NULL)
	       && $main::parser->next_element()) {
	    #nop;
	}

	$self->{'CONTENT'} eq $TAG_NULL ? undef : $self->{'CONTENT'};
    }

    sub add_child {
	my($self, $ref) = @_;
	my($prevelem);

	$ref->{'PARENT'} = $self;
	$ref->{'NEXT'} = $TAG_NULL;

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

	if ($child->{'PREV'} && $child->{'PREV'} ne $TAG_NULL) {
	    $child->{'PREV'}->{'NEXT'} = $child->{'NEXT'};
	} else {
	    $child->{'PARENT'}->{'CONTENT'} = $child->{'NEXT'};
	}

	if ($child->{'NEXT'} && $child->{'NEXT'} ne $TAG_NULL) {
	    $child->{'NEXT'}->{'PREV'} = $child->{'PREV'};
	} else {
	    $child->{'PARENT'}->{'CONTENT_LAST'} = $child->{'PREV'};
	}

	$child->{'NEXT'} = 0;
	$child->{'PREV'} = 0;

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
	    $nextchild = $nextchild->next();

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

	if ($self->{'ATTR'}->{'ID'}) {
	    my($tag) = $self->{'TAG'};
	    my($id) = $self->{'ATTR'}->{'ID'};

	    $self->save_id();
#	    $markup->markup(".idpage $tag $id");
	}
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	&main::WARNING("No markup method for $self->{TAG}.\n");
	$self->markup_children($markup, $self->content());
	
	$inmarkup ? $markup : $markup->output();
    }
	
    sub markup_children {
	my($self, $markup, $child) = @_;
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !$inmarkup;

	while ($child && $child ne $TAG_NULL) {
	    $child->markup($markup);
	    $child = $child->next();
	}

	$inmarkup ? $markup : $markup->output();
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

	$markup->markup($self->markup_start());
	$self->markup_children($markup, $self->content());
	$markup->markup($self->markup_end());

	$inmarkup ? $markup : $markup->output();
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

	$markup->markup($self->markup_start());
	$self->markup_children($markup, $self->content());
	$markup->markup($self->markup_end());

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package PARA;
    @ISA = ('GENERIC_WRAPPER');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	if ($self->{'PARENT'}->{'TAG'} eq 'ENTRY') {
	    $markup->markup("\n") if $markup->{'MARKUP_COL'} != 0;
	    $self->markup_children($markup, $self->content());
	    $markup->markup("\n") if $markup->{'MARKUP_COL'} != 0;
	    $markup->markup(".sp $main::TABLE_BETWEEN_PARA_SPACE") if $self->next();
	} else {
	    $markup->markup($self->markup_start());
	    $self->markup_children($markup, $self->content());
	    $markup->markup($self->markup_end());
	}

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package GENERIC_OBEYSPACES;
    @ISA = ('GENERIC_WRAPPER');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$markup->{'OBEY_SPACES'}++;
	$markup->markup($self->markup_start());
	$self->markup_children($markup, $self->content());
	$markup->markup($self->markup_end());
	$markup->{'OBEY_SPACES'}--;

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package SCREEN;
    @ISA = ('GENERIC_OBEYSPACES');

    sub new {
	return bless {};
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($indent) = "\"$main::PI_SCREEN_INDENT\"";
	my($ptsize) = "";
	my($child);

	$main::PI_SCREEN_INDENT = ""; # it applies to at most one screen...

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	if ($self->{'ATTR'}->{'ROLE'} =~ /^([0-9]+)/) {
	    $ptsize = $1;
	}

	$markup->{'OBEY_SPACES'}++;
	$markup->markup($self->markup_start() . " $indent $ptsize");
	$self->markup_children($markup, $self->content());
	$markup->markup($self->markup_end());
	$markup->{'OBEY_SPACES'}--;

	$inmarkup ? $markup : $markup->output();
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
	$text = $textmarkup->output();

	$text =~ s/\n/ /g;

	$markup->markup($self->markup_start() . " \"$text\"");

	$inmarkup ? $markup : $markup->output();
    }
}

{ 
    package GENERIC_TITLED;
    @ISA = ('GENERIC_TAG');

    sub markup_title {
	my($self, $markup) = @_;
	my($child) = $self->content();

	if ($child && ($child->{'TAG'} eq 'TITLE')) {
	    my($block) = $child->block_markup($child);
	    my($title) = $child->markup();
	    
	    # Handle <?lb> in title
	    $title =~ s/<\001lb>/\" \"/g;

	    # Remove new lines
	    $title =~ s/\n/ /g;

	    $markup->markup($self->markup_start() . " \"$title\"");

	    $block->markup($markup) if $block;
	} else {
	    $markup->markup($self->markup_start());
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
	$child = $child->next() if $child->{'TAG'} eq 'TITLE';
	$self->markup_children($markup, $child);
	$markup->markup($self->markup_end());

	$inmarkup ? $markup : $markup->output();
    }
}

{ 
    package SECT1;
    @ISA = ('GENERIC_TITLED');

    # Set by a side effect in a PI (<?ahead-for-chapter-toc xxx>)
    $AHEAD_FOR_CHAPTER_TOC = "";

    sub new {
	return bless {};
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($child) = $self->content();

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);
	$self->markup_title($markup);

	if ($child->tag('TITLE') && !$self->role('NOCHAPTOC')) {
	    my($title) = $child->markup();
	    $title =~ s/<\001lb>/ /g;
	    $title =~ s/\n/ /g;

	    if ($AHEAD_FOR_CHAPTER_TOC ne "") {
		push (@main::SeInfo, $AHEAD_FOR_CHAPTER_TOC);
		$AHEAD_FOR_CHAPTER_TOC = "";
	    } else {
		push (@main::SeInfo, $title);
	    }

	}
	$child = $child->next() if $child->tag('TITLE');

	$self->markup_children($markup, $child);
	$markup->markup($self->markup_end());

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package BOOK;
    @ISA = ('GENERIC_WRAPPER');

    sub new {
	return bless {};
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($bktitle) = 
	    &main::cfg($main::bookfiles, $main::FILTERCFG, "book_title") ||
	    &main::cfg($main::bookfiles, $main::FILTERCFG, "book-title") ||
	    "No title supplied in $main::BOOKFILES";

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$markup->markup($self->markup_start() . " \"$bktitle\"");
	$self->markup_children($markup, $self->content());
	$markup->markup($self->markup_end());

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package GENERIC_CHAPTER;
    @ISA = ('GENERIC_TITLED');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($child) = $self->content();
	my($docinfo) = undef;
	my($chapnum) = $main::PI_CHAPTER_NUMBER || "1";

	@main::SeInfo = ();

	$markup = new MARKUP_OBJECT if !$inmarkup;

	if ($main::opt_nocrop) {
	    print "no crop marks\n";
	    $markup->markup(".nr no_cutmarks 1");
	}

	$self->default_markup($markup);

	if ($child && $child->tag('DOCINFO')) {
	    $docinfo = $child;
	    $child = $child->next();
	}

	if ($child && $child->tag('TITLE')) {
	    my($title) = $child->markup();
	    
	    # Handle <?lb> in title
	    $title =~ s/<\001lb>/\" \"/g;
	    $title =~ s/\n/ /g;

	    $markup->markup($self->markup_start() . " $chapnum \"$title\"");
	    $child = $child->next();
	} else {
	    $markup->markup($self->markup_start());
	}

	$docinfo->markup($markup) if $docinfo;
	
	$markup->markup(".Nh 2") if ($main::PI_NUMBERED_SECTIONS);

	$self->markup_children($markup, $child);
	$markup->markup($self->markup_end());

	$self->chapter_toc();

	$inmarkup ? $markup : $markup->output();
    }

    sub chapter_toc {
	my($self) = @_;
	my(@headers) = @main::SeInfo;
	my($doctype) = $self->tag();
#
# Build the Se files for the Nutshell format.  Must be able to 
# build the little Chapter TOC...
#
	$file = $main::PI_FILE;
	$file = $1 if $file =~ /^\"(.*)\"$/;

	$doctype = substr($doctype,0,1) . lc(substr($doctype,1));
	
	unlink ("./fmt/toc/Se.$file.flag", "./fmt/toc/Se.$file");
	
	open (F, ">./fmt/toc/Se.$file.flag");

	if (@headers && 
	    !&main::cfg($main::bookfiles, $main::PI_SOURCE_FILE, "no_toc")) {
	    print F ".nr do_box 1\n";
	} else {
	    print F ".nr do_box 0\n";
	}
	close (F);
	
	if (@headers) {
	    open (F, ">./fmt/toc/Se.$file");
	    print F ".in 0\n";
	    print F ".ie !\'\\*[tocbox_hdr_ft]\'\' .ft \\*[tocbox_hdr_ft]\n";
	    print F ".el .ft \\fI\n";
	    $doctype2 = $doctype;
	    $doctype2 =~ s/(.*)/\L$1/;
	    print F "In this $doctype2:\n";
	    print F ".if !\'\\*[tocbox_hdr_ft]\'\' .sp 2p\n";
	    print F ".ft \\*[tocbox_ft]\n";
	    print F ".in 1m\n";
	    print F ".ta 1m\n";
	    
	    while (@headers) {
		$h = shift @headers;

		$h = $1 if $h =~ /^\"(.*)\"$/;
		$h =~ s/\" \"/ /g; # Repl foo" "bar with foo bar
		
		print F ".ti -1m\n";
		print F "\\(bu\t$h\n";

		print F ".chaptoc_sp\n" if @headers;
	    }
	    
	    print F ".in 0\n";
	    close (F);
	}
    }
}

{ 
    package PCDATA;
    @ISA = ('GENERIC_INLINE');

    sub new {
	return bless { };
    }

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
	my($parent_tag) = $self->parent->{'TAG'};
	local($_) = $self->{'DATA'};

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	s/\\/\\e/g;		# "\" -> "\e" for troff

	if ($parent_tag ne "PROGRAMLISTING" && 
	    $parent_tag ne "SCREEN" &&
	    $parent_tag ne "EXAMPLE" &&
	    $parent_tag ne "LITERALLAYOUT") {

	    s/([\.\?\!\:])  /$1 /g; # ".  " becomes ". "
	}

	$markup->text($_);

	$inmarkup ? $markup : $markup->output();
    }
}

{ 
    package SDATA;
    @ISA = ('GENERIC_INLINE');

    sub new {
	return bless { };
    }

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
		$t = $main::ENTITIES{"&$t"};
	    } else {
		&main::WARNING("$0: Unrecognized entity: $t\n");
		$t = "??ENTITY-$t";
	    }
	} else {
	    # Just an ordinary textual replacement
	}

	$markup->markup($t);

	$inmarkup ? $markup : $markup->output();
    }
}

{ 
    package PI;
    @ISA = ('GENERIC_INLINE');

    sub new {
	return bless { };
    }

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
		$sub = "\&main::PI_${pi}(\$self, \$markup, \$rest)";
		$_ = eval $sub;

		if ($@) {
		    $_ = $@;
		    s/called at.*$//;

		    warn "Attempt to process <?$pi $rest> failed:\n\t$_\n";
		}
	    }
	}

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package PROGRAMREFERENCE;
    @ISA = ('GENERIC_WRAPPER');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($tag_start, $tag_end);
	my($name) = $self->{'ATTR'}->{'NAME'};
	my($filename) = $self->{'ATTR'}->{'FILENAME'};

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	if ($name == "") {
	    $name=$filename;
	}

	$markup->markup($self->markup_start() . " $name");

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package QUOTE;
    @ISA = ('GENERIC_INLINE');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($tag_start, $tag_end);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$tag_start = $self->markup_start();
	$tag_end = $self->markup_end();

	$markup->markup($tag_start);
	$self->markup_children($markup, $self->content());
	$markup->markup($tag_end);

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package SYSTEMITEM;
    @ISA = ('GENERIC_INLINE');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($tag_start, $tag_end);
	my($type, $size);

	undef $type;
	undef $size;

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$tag_start = $self->markup_start();
	$tag_end = $self->markup_end();

	$type = $self->role() || $self->attr('CLASS');

	if ($type =~ /^(.*),(\d+)pt/i) {
	    $type = $1;
	    $size = $2;
	} elsif ($type =~ /^(\d+)pt/i) {
	    $size = $1;
	    $type = "";
	}

	if (($type ne "") && ($tag_start =~ /\\\*\[.*\]/)) {
	    $type = uc($type);
	    chop($tag_start);
	    chop($tag_end);
	    $tag_start .= "-$type]";
	    $tag_end .= "-$type]";
	}

	if (defined($size)) {
	    $markup->markup("\\s$size");
	}

	$markup->markup($tag_start);
	$self->markup_children($markup, $self->content());
	$markup->markup($tag_end);

	if (defined($size)) {
	    $markup->markup("\\s0");
	}

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package SGMLTAG;
    @ISA = ('GENERIC_INLINE');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($class) = $self->{'ATTR'}->{'CLASS'};
	my($pre, $post) = ('','');

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$class = 'ELEMENT' if !$class;
	if ($class eq 'ELEMENT') {
	    $pre = '<';
	    $post = '>';
	} elsif ($class eq 'GENENTITY') {
	} elsif ($class eq 'PARAMENTITY') {
	} elsif ($class eq 'PI') {
	    $pre = '<?';
	    $post = '>';
	} elsif ($class eq 'ATTRIBUTE') {
	} else {
	    &main::WARNING("Unexpected CLASS=$class in SGMLTAG.\n");
	}

	$markup->markup($self->markup_start() . $pre);
	$self->markup_children($markup, $self->content());
	$markup->markup($post . $self->markup_end());

	$inmarkup ? $markup : $markup->output();
    }
}


{
    package EMPHASIS;
    @ISA = ('SYSTEMITEM');

    sub new {
	return bless { };
    }
  
    # inline markup is identical to SYSTEMITEM...
}

{
    package FUNCTION;
    @ISA = ('SYSTEMITEM');

    sub new {
	return bless { };
    }
  
    # inline markup is identical to SYSTEMITEM...
}

{
    package KEYCAP;
    @ISA = ('GENERIC_INLINE');
    
    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($parent) = $self->{'PARENT'};
	my($child) = $self->content();
	my($role) = $self->role();
	chomp $role;

	$markup = new MARKUP_OBJECT if !$inmarkup;

	$self->default_markup($markup);

	$markup->markup("\\c\n") if $markup->{'OBEY_SPACES'};
	if ($role eq "HEADER") {
	    $markup->markup($self->markup_start() . " H"
			    . " \"" . $child->markup() . "\" "
			    . "\\&\\c");
	}
	else {
	    $markup->markup($self->markup_start()
			    . " \"" . $child->markup() . "\" "
			    . "\\&\\c");
	}

	$inmarkup ? $markup : $markup->output();
    }
}
	
{ 
    package SIMPLELIST;
    @ISA = ('GENERIC_TAG');
    
    sub new {
	my($ref) = bless { };
	$ref->{'COLSEP'} = "\t";
	return $ref;
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($child) = $self->content();
	my($child_markup) = "";
	my(@STACK) = ();
	my($type, $elements, $cols, $rows, $thiscol, $count, $table, $role, $temp);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	while ($child) {
	    if ($child->{'TAG'} eq 'MEMBER') {
		$child_markup = $child->markup();
		push(@STACK, $child_markup);
	    }
	    $child = $child->next();
	}

	$type = $self->{'ATTR'}->{'TYPE'};
	$role = $self->{'ATTR'}->{'ROLE'};

	$temp = $self->markup_start();
	if ($role) {
	    $temp .= " \"$role\"";
	}

	$markup->markup($temp);

	if ($type eq "HORIZ" || $type eq "VERT") {
	    $elements = $#STACK + 1;
	    $cols     = $self->{'ATTR'}->{'COLUMNS'} || 1;
	    $rows     = int(($elements+($cols - 1)) / $cols);

	    $markup->markup(".TS");
	    if ($role eq "wide-wrap") {
		$table .= "lw(27P) ";
	    }
	    else {
		$table .= "l " x $cols;
	    }
	    $table .= ".\n";
	    $markup->markup($table);
	    $table = "";
	}

	if ($type eq "HORIZ") {
	    $thiscol = 0;
	    for ($count = 0; $count < $rows*$cols; $count++) {
		$table .= $self->{'COLSEP'} if $thiscol > 0;
		$table .= $STACK[$count];
		$thiscol++;
		if ($thiscol >= $cols) {
		    $table .= "\n";
		    $thiscol = 0;
		}
	    }
	} elsif ($type eq "VERT") {
	    for ($count = 0; $count < $rows; $count++) {
		$_ = $count;
		for ($thiscol = 0; $thiscol < $cols; $thiscol++) {
		    $table .= $self->{'COLSEP'} if $thiscol > 0;
		    $table .= $STACK[$_];
		    $_ += $rows;
		}
		$table .= "\n";
	    }
	} else {
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
	}

	$markup->markup($table);

	if ($type eq "HORIZ" || $type eq "VERT") {
	    $markup->markup(".TE");
	}

	$markup->markup($self->markup_end());

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package MEMBERS;
    @ISA = ('GENERIC_TITLED');

    sub new {
	return bless { };
    }

    sub markup_title {
	my($self, $markup) = @_;
	my($child) = $self->content();
	my($package, $class, $clrole, $clrstring);

	if ($child && ($child->{'TAG'} eq 'TITLE')) {
	    my($block) = $child->block_markup($child);
	    my($title) = $child->markup();
	    
	    # Handle <?lb> in title
	    $title =~ s/<\001lb>/\" \"/g;

	    # Remove new lines
	    $title =~ s/\n/ /g;

	    if ($title =~ /\.CLR "([^"]*)" "([^"]*)" "([^"]*)"/) {
		$package = $1;
		$class = $2;
		$clrole = $3;

		if ($clrole eq "includePkg") {
		    $clrstring = $package . "." . $class;
		}
		else {
		    $clrstring = $class;
		}

		$title =~ s/( )? *\.CLR "[^"]*" "[^"]*" "[^"]*" /$1$clrstring/;
	    }

	    $markup->markup($self->markup_start() . " \"$title\"");

	    $block->markup($markup) if $block;
	} else {
	    $markup->markup($self->markup_start());
	}

	$markup;
    }
}

{
    package MEMBER;
    @ISA = ('GENERIC_WRAPPER');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($listrole);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$listrole = $self->parent->{'ATTR'}->{'ROLE'};

	if ($listrole eq "wide-wrap") {
	    $markup->markup("T{\n");
	}

	$markup->markup($self->markup_start());
	$self->markup_children($markup, $self->content());
	$markup->markup($self->markup_end());

	if ($listrole eq "wide-wrap") {
	    $markup->markup("\nT}");
	}

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package ITEMIZEDLIST;
    @ISA = ('GENERIC_WRAPPER');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($mark);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$mark = $self->attr('MARK');

	if ($mark =~ /^check$/i) {
	    $markup->markup($self->markup_start() .
			    " \"\" \"\" \"\\[check]\"");
	}
	else {
	    $markup->markup($self->markup_start());
	}

	$self->markup_children($markup, $self->content());
	$markup->markup($self->markup_end());

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package ORDEREDLIST;
    @ISA = ('GENERIC_WRAPPER');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my(%Numeration) = ('ARABIC' => 'N',
			   'UPPERALPHA' => 'A',
			   'LOWERALPHA' => 'a',
			   'UPPERROMAN' => 'R',
			   'LOWERROMAN' => 'r');
	my($num);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$num = $Numeration{$self->attr('NUMERATION')} || 'N';

	if ($self->attr('CONTINUATION') eq "CONTINUES") {
	    $markup->markup($self->markup_start() . " $num \"\" C");
	}
	else {
	    $markup->markup($self->markup_start() . " $num");
	}

	$self->markup_children($markup, $self->content());
	$markup->markup($self->markup_end());

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package VARIABLELIST;
    @ISA = ('GENERIC_WRAPPER');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($role) = $self->role();

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$markup->markup($self->markup_start() . " " . $role);

	$self->markup_children($markup, $self->content());
	$markup->markup($self->markup_end());

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package CLASSREF;
    @ISA = ('GENERIC_INLINE');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($pkg) = $self->{'ATTR'}->{'PACKAGE'};
	my($class) = $self->{'ATTR'}->{'CLASS'};
	my($role) = $self->{'ATTR'}->{'ROLE'};

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$markup->markup($self->markup_start() .
			" \"$pkg\" \"$class\" \"$role\"");

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package CLASSNAME;
    @ISA = ('GENERIC_INLINE');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($content) = $self->content;
	my($text);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$text = $content->markup();

	$markup->markup($self->markup_start() . " \"$text\"");

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package CLASSSYNOPSIS;
    @ISA = ('GENERIC_WRAPPER');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($keyword, $revision);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$keyword = $self->{'ATTR'}->{'KEYWORD'};
	$revision = $self->{'ATTR'}->{'REVISION'};

	$markup->markup($self->markup_start() . " \"$keyword\" \"$revision\"");
	$self->markup_children($markup, $self->content());
	$markup->markup($self->markup_end());

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package MODIFIERS;
    @ISA = ('GENERIC_ASARG');

    sub new {
	return bless { };
    }
}

{
    package PARAMDEF;
    @ISA = ('GENERIC_ASARG');

    sub new {
	return bless { };
    }
}

{
    package FUNCPROTOTYPE;
    @ISA = ('GENERIC_WRAPPER');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($role, $revision, $flags);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$role = $self->{'ATTR'}->{'ROLE'};
	$revision = $self->{'ATTR'}->{'REVISION'};
	$flags = $self->{'ATTR'}->{'FLAGS'};

	$markup->markup($self->markup_start() . " \"$role\" \"$revision\" \"$flags\"");
	$self->markup_children($markup, $self->content());
	$markup->markup($self->markup_end());

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package XREF;
    @ISA = ('GENERIC_INLINE');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($linkend) = $self->{'ATTR'}->{'LINKEND'};
	my($endterm) = $self->{'ATTR'}->{'ENDTERM'};
	my($pattern, $linkto);
	my($xreftag, $xrefchap, $xrefsect, $xrefrsct, $xrefitem, $xreftxt);
	my($xrefnum, $xrefwarning, $xreflabel);
	my($sep, $sep_config);

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
	    $xreflabel = $main::TAGID->{$linkto}->{'XREFLABEL'};

	    $xreftxt =~ s/^\s*(.*\S+)\s*$/$1/;

#	    print STDERR "$xreftag\n  $xrefchap\n  $xrefsect\n  $xrefrsct\n  $xrefitem\n  $xreftxt\n  $xreflabel\n";

	    $pattern = 
		&main::cfg($main::bookfiles, $main::FILTERCFG, "xref_$xreftag") ||
		&main::cfg($main::bookfiles, $main::FILTERCFG, "xref-$xreftag");

	    if ($main::TAGCOUNT{"$linkto"} > 1) {
		my($spattern) = 
		    &main::cfg($main::bookfiles, $main::FILTERCFG, "xrefs_$xreftag") ||
		    &main::cfg($main::bookfiles, $main::FILTERCFG, "xrefs-$xreftag");
		$pattern = $spattern if $spattern ne "";
	    }

	    &main::WARNING("Warning: no pattern for $xreftag ($linkto)\n")
		if !$pattern;

	    ($sep_config = "${xreftag}_xref_num_sep") =~ tr/A-Z/a-z/;
	    $sep = 
		&main::cfg($main::bookfiles, $main::FILTERCFG, $sep_config) ||
		&main::cfg($main::bookfiles, $main::FILTERCFG, 'xref_num_sep') ||
		&main::cfg($main::bookfiles, $main::FILTERCFG, 'xref-num-sep');

	    $xrefnum = $xrefchap;
	    $xrefwarning = 0;

	    if (($xreftag eq "TABLE") 
		|| ($xreftag eq "FIGURE") 
		|| ($xreftag eq "EXAMPLE")) {
		$xrefnum .= ".$xrefitem";
	    } elsif ($xreftag =~ /^SECT[1-5]$/) {
		$xrefnum .= ".$xrefsect";
	    } elsif ($xreftag eq "REFENTRY") {
		$xrefnum .= ".$xrefsect.$xrefitem";
	    } elsif ($xreftag =~ /^REFSECT[1-5]$/) {
		$xrefnum .= ".$xrefrsct";
	    } elsif (($xreftag ne "CHAPTER")
		     && ($xreftag ne "APPENDIX")
		     && ($xreftag ne "PREFACE")) {
		$xrefwarning = 1;
	    }

	    $xrefnum =~ s/\./$sep/g if ($sep && ($sep ne '.'));

#	    &main::WARNING("Numeric cross reference to $xreftag contains only the chapter number.\n")
#		if $xrefwarning && ($pattern =~ /\%n/);

	    $pattern =~ s/\%n/$xrefnum/g;         # The old format elements
	    $pattern =~ s/\%t/$xreftxt/g;

	         # Replace every occurance of %(attribute) in xref format
	         # pattern with attribute for that tag# from TAGID table:

	    $pattern =~ s/\%\(([^\)]*)\)/$main::TAGID->{$linkto}->{$1}/g;

	    $pattern = $xreflabel if $xreflabel ne "";
	} else {
	    &main::WARNING("Warning: xref ID \"$linkto\" is unknown.\n");
	    $pattern = "";
	}

	$pattern = "**UNKNOWN XREF**" if $pattern eq "";

	$markup->markup($self->markup_start() 
			. $pattern 
			. $self->markup_end());

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package VARLISTENTRY;
    @ISA = ('GENERIC_WRAPPER');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($child) = $self->content();
	my($terms) = "";
	my($term);
	my($blocklist, $last, $block) = (0, 0, 0);
	my($package, $class, $role);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	while ($child->{'TAG'} eq 'TERM') {
	    $block = $child->block_markup();

	    if ($last) {
		$last->{'NEXT'} = $block;
		$block->{'PREV'} = $last;
	    } else {
		$blocklist = $block;
		$last = $blocklist;
	    }
	    $last = $last->next() while $last && $last->next();

	    $term = $child->markup();

	    if ($term =~ /\.CLR "([^"]*)" "([^"]*)" "([^"]*)"/m) {
		$package = $1;
		$class = $2;
		$clrole = $3;

		if ($clrole eq "includePkg") {
		    $clrstring = $package . "." . $class;
		}
		else {
		    $clrstring = $class;
		}

		$term =~ s/( )? *\.CLR "[^"]*" "[^"]*" "[^"]*"\n/$1$clrstring/m;
	    }

	    $term =~ s/\"/\"\"/g;

	    # Get rid of \n and leading and trailing blanks
	    $term =~ s/\n/ /g;
	    $term =~ s/^\s+//;
	    $term =~ s/\s+$//;

	    $terms .= "\"$term\" ";
	    $child = $child->next();
	}
	chop($terms);		# get rid of the trailing blank

	$markup->markup($self->markup_start());
	$markup->markup(".LIT $terms") if $terms;
	for ($block = $blocklist; 
	     $block && $block ne $TAG_NULL; 
	     $block = $block->next()) {
	    $block->markup($markup);
	}
	$self->markup_children($markup,$child);
	$markup->markup($self->markup_end());

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package NUTLIST;
    @ISA = ('GENERIC_WRAPPER');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($longest);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$longest = $self->{'ATTR'}->{'LONGESTTERM'};

	if(!$longest) {
	    warn ("No LongestTerm attribute on NutList.\n");
	}

	$markup->markup($self->markup_start() . " $longest");

	$self->markup_children($markup, $self->content());
	$markup->markup($self->markup_end());

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package NUTSYNOPSIS;
    @ISA = ('GENERIC_ASARG');
#    @ISA = ('GENERIC_WRAPPER');

    sub new {
	return bless { };
    }
}

{
    package NUTENTRY;
    @ISA = ('GENERIC_WRAPPER');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($child) = $self->content();
	my($terms) = "";
	my($term);
	my($blocklist, $last, $block) = (0, 0, 0);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	while ($child->{'TAG'} eq 'TERM') {
	    $block = $child->block_markup();

	    if ($last) {
		$last->{'NEXT'} = $block;
		$block->{'PREV'} = $last;
	    } else {
		$blocklist = $block;
		$last = $blocklist;
	    }
	    $last = $last->next() while $last && $last->next();

	    $term = $child->markup();
	    $term =~ s/\"/\"\"/g;

	    # Get rid of \n and leading and trailing blanks
	    $term =~ s/\n/ /g;
	    $term =~ s/^\s+//;
	    $term =~ s/\s+$//;

	    $terms .= "\"$term\" ";
	    $child = $child->next();
	}
	chop($terms);		# get rid of the trailing blank

	$markup_string = $self->markup_start;
	$markup_string .= " $terms" if $terms;
	$markup->markup($markup_string);
	for ($block = $blocklist; 
	     $block && $block ne $TAG_NULL; 
	     $block = $block->next()) {
	    $block->markup($markup);
	}

	$self->markup_children($markup,$child);
	$markup->markup($self->markup_end());

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package GLOSSENTRY;
    @ISA = ('VARLISTENTRY');

    sub new {
	return bless { };
    }

    # markup is just like VARLISTENTRY
}

{ 
    package GRAPHIC;
    @ISA = ('GENERIC_WRAPPER');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($format, $fileref) = ($self->{'ATTR'}->{'FORMAT'},
				 $self->{'ATTR'}->{'FILEREF'});
	my($imageok);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	# If the format is unspecified, try to figure it out...
	if ($format =~ /^\s*$/) {
	    $format = "gif" if $fileref =~ /^\.gif$/;
	    $format = "jpg" if $fileref =~ /^\.jpg$/;
	    $format = "eps" if $fileref =~ /^\.eps\.?g?z?/i;

	    # PS is an EPS?
	    $format = "eps" if $fileref =~ /^\.ps\.?g?z?/i;

	    # EPS if nothing else?
	    $format = "eps" if $format =~ /^\s*$/;
	}

	if ($format =~ /^tbl$/i) {
	    if (! -f $fileref) {
		$fileref = "./tbl/$fileref" if -f "./tbl/$fileref";
		$fileref = "./tabs/$fileref" if -f "./tbl/$fileref";
	    }
	    
	    if (! -f $fileref) {
		&main::WARNING("$0: Cannot find table: $fileref!\n");
	    }
	    
	    $markup->markup(".so $fileref");
	} elsif ($format =~ /^eqn$/i) {
	    if (! -f $fileref) {
		$fileref = "./eqn/$fileref" if -f "./eqn/$fileref";
		$fileref = "./math/$fileref" if -f "./math/$fileref";
	    }
	    
	    if (! -f $fileref) {
		&main::WARNING("$0: Cannot find equation: $fileref!\n");
	    }
	    
	    $markup->markup(".so $fileref");
	} elsif ($format !~ /^eps$/i && $format !~ /^postscript$/i) {
	    &main::WARNING("$0: unrecognized format $format in <GRAPHIC> tag.\n");
	    $markup->markup($self->markup_start() . " $fileref");
	    $markup->markup($self->markup_end());
	} else {		# ASSUME ITS EPS!
	    my (@figdirs, @figfiles, $figdir, $figfile, $found);

	    $fileref = $1 if $fileref =~ /^(.*)\.gz$/;
	    $fileref = $1 if $fileref =~ /^(.*)\.Z$/;

	    @figdirs = ("./", "./figs",
			"./lowfigs", "./lofigs", "./figs.lo", "./figs.low",
			"./highfigs", "./hifigs", "./figs.hi", "./figs.high");

	    # If FILEREF="/path/figure", put "/path" in the figdirs list
	    # but just look for "figure".
	    #
	    if ($fileref =~ /^(.*)\/([^\/]+)$/) {
		unshift (@figdirs, $1);
		$fileref = $2;
	    }

	    @figfiles = ("$fileref", "$fileref.gz", "$fileref.Z",
			 "$fileref.eps", "$fileref.eps.gz", "$fileref.eps.Z");

#	    print STDERR "Figdirs=@figdirs\n";
#	    print STDERR "Figfils=@figfiles\n";

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
	    } 

	    if ($fileref =~ /\.Z$/ || $fileref =~ /\.gz$/) {
		$imageok = open (IMAGEFILE, "$main::ZCAT $fileref |");
	    } else {
		$imageok = open (IMAGEFILE, "$main::CRLF $fileref |");
	    }

	    if (!$imageok) {
		&main::WARNING("$0: Cannot read image file: $fileref from <GRAPHIC> tag!\n");
	    }

	    $self->{"FIGURE_HEIGHT"} = $main::PI_FIGURE_HEIGHT;
	    $main::PI_FIGURE_HEIGHT = "";

	    if ($self->{'FIGURE_HEIGHT'} eq "") {
		# This code gets the bounding box, if it occurs within 
		# 4096 bytes of the start of the file.  It works on both 
		# Mac and Unix style files [which is why its a bit
		# complicated]

		read(IMAGEFILE, $_, 4096);
		s/[\r\n]/\n/g;
		$_ = (grep(/^%%?BoundingBox/, split(/\n/, $_)))[$[];
		close (IMAGEFILE);

		if (/%%BoundingBox:\s+(\d+\.?\d*)\s+(\d+\.?\d*)\s+(\d+\.?\d*)\s+(\d+\.?\d*)/i) {
		    $self->{"FIGURE_HEIGHT"} = sprintf("%0.3fp", $4-$2);
		} else {
		    &main::WARNING("$0: No bounding box in $fileref!\n");
		}
	    }

	    # Remove .gz or .Z extension...
	    $fileref = $1 if $fileref =~ /^(.*)\.gz$/;
	    $fileref = $1 if $fileref =~ /^(.*)\.Z$/;

	    $markup->markup($self->markup_start() 
			    . " $fileref "
			    . $self->{'FIGURE_HEIGHT'} 
			    . " $main::PI_GRAPHIC_ARGS");
	    $markup->markup($self->markup_end());
	    $main::PI_GRAPHIC_ARGS = "";
	}

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package GENERIC_INDEX_TERM;
    @ISA = ('GENERIC_INLINE');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($term);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$term = $self->markup_children(undef, $self->content());

	$term =~ s/\n/ /g;	# Remove newlines from index terms!

	$markup->text($term);
	
	$inmarkup ? $markup : $markup->output();
    }
}

{
    package INDEXTERM;
    @ISA = ('GENERIC_WRAPPER');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($key) = $self->{'ATTR'}->{'ID'};
	my($significance) = $self->{'ATTR'}->{'SIGNIFICANCE'};
	my($child) = $self->content();
	my(%terms) = ();
	my(%sorts) = ();
	my(%markup) = ('PRIMARY' => 'XE1',
		       'SECONDARY' => 'XE2',
		       'TERTIARY' => 'XE3',
		       'SEE' => 'XSE',
		       'SEEALSO' => 'XSA');
	my($count, $t);
	local($_);

	return undef if $self->{'already_marked_up'};

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	# Get the markup and sortas terms for each of the children
	while ($child) {
	    $_ = $child->{'TAG'};
	    $terms{$_} = $child->markup();
	    $sorts{$_} = $child->{'ATTR'}->{'SORTAS'};
	    $child = $child->next();
	}

	if ($key ne "") {
	    $main::TAGID->{$key} = { };

	    $main::TAGID->{$key}->{'TAG'} = $self->{'TAG'};
	    foreach $_ (keys %terms) {
		$main::TAGID->{$key}->{$_} = $terms{$_};
		$main::TAGID->{$key}->{"$_-SORTAS"} = $sorts{$_};
	    }
	}

	if ($significance =~ /PREFERRED/i) {
	    $significance = 'B';
	} else {
	    $significance = 'N';
	}

	if ((($key = $self->{'ATTR'}->{'SPANEND'}) ne "") ||
	    (($key = $self->{'ATTR'}->{'STARTREF'}) ne "")) {
	    my(@fields);

	    if (!defined($main::TAGID->{$key})) {
		&main::WARNING("No spanbeg for index $key; skipping...\n");
		return;
	    }

	    if ($main::TAGID->{$key}->{'TAG'} ne 'INDEXTERM') {
		my($warn) = "Spanend $key doesn't point to an indexterm";
		my($tag) = $main::TAGID->{$key}->{'TAG'};
		&main::WARNING("$warn ($tag); skipping...\n");
		return;
	    }

	    foreach $t ('PRIMARY', 'SECONDARY', 'TERTIARY', 'SEE', 'SEEALSO') {
		$terms{$t} = $main::TAGID->{$key}->{$t};
		$sorts{$t} = $main::TAGID->{$key}->{"$t-SORTAS"};
	    }
	}

	if (($self->{'ATTR'}->{'SPANBEG'} =~ /^y/i) ||
	    ($self->{'ATTR'}->{'CLASS'} =~ /startofrange/i)) {
	    $markup->markup($self->markup_start() . " $significance S");
	} elsif (($self->{'ATTR'}->{'SPANEND'} ne "") ||
		 ($self->{'ATTR'}->{'STARTREF'} ne "")) {
	    $markup->markup($self->markup_start() . " $significance E");
	} else {
	    $markup->markup($self->markup_start() . " $significance");
	}

	foreach $t ('PRIMARY', 'SECONDARY', 'TERTIARY', 'SEE', 'SEEALSO') {
	    if ($terms{$t} ne "") {
		$_ = ".$markup{$t} \"$terms{$t}\"";
		$_ .= " \"$sorts{$t}\"" if $sorts{$t};
		$markup->markup($_);
	    }
	}

	$markup->markup($self->markup_end());

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package ULINK;
    @ISA = ('GENERIC_TAG');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($type) = $self->{'ATTR'}->{'TYPE'};
	my($url) = $self->{'ATTR'}->{'URL'};

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	&main::WARNING("Warning: unknown ULINK type: $type\n")
	    if ($type && $type !~ /^cprog$/i);

	open (F, $url) 
	    || &main::WARNING("Warning: cannot open URL in ULINK: $url\n");

	while (<F>) {
	    s/\\/\\e/g;
	    $markup->markup($_);
	}
	close (F);

	$inmarkup ? $markup : $markup->output();
    }
}

######################################################################
# TABLES
######################################################################

#      Vertical space settings:
# above-rule-space, below-rule-space: padding around h-rules
# between-para-space: padding between paras
# between-row-space: padding between rows in place of h-rule
# above-head-space: padding at top of table
# head-font-insert: font of table heads
# table-linesize-option: width of table lines if frame=all
#
# Get values from config file or set default

$main::TABLE_ABOVE_RULE_SPACE = 
    &main::cfg( $main::bookfiles, $main::FILTERCFG, "table_above_rule_sp" ) ||
    &main::cfg( $main::bookfiles, $main::FILTERCFG, "table-above-rule-sp" ) ||
    "1p";

$main::TABLE_BELOW_RULE_SPACE =
    &main::cfg( $main::bookfiles, $main::FILTERCFG, "table_below_rule_sp" ) ||
    &main::cfg( $main::bookfiles, $main::FILTERCFG, "table-below-rule-sp" ) ||
    "2p";

$main::TABLE_BETWEEN_PARA_SPACE = 
    &main::cfg( $main::bookfiles, $main::FILTERCFG, "table_para_sp" ) ||
    &main::cfg( $main::bookfiles, $main::FILTERCFG, "table-para-sp" ) ||
    ".25v";

$main::TABLE_BETWEEN_ROW_SPACE = 
    &main::cfg( $main::bookfiles, $main::FILTERCFG, "table_row_sp") ||
    &main::cfg( $main::bookfiles, $main::FILTERCFG, "table-row-sp") ||
    "2p";

$main::TABLE_ABOVE_HEAD_SPACE = 
    &main::cfg($main::bookfiles, $main::FILTERCFG, "table_above_head_sp") ||
    &main::cfg($main::bookfiles, $main::FILTERCFG, "table-above-head-sp") ||
    "";

$main::TABLE_HEAD_FONT_INSERT = 
    &main::cfg($main::bookfiles, $main::FILTERCFG, "table_head_font") ||
    &main::cfg($main::bookfiles, $main::FILTERCFG, "table-head-font") ||
    "b";

$main::TABLE_LINESIZE_OPTION = 
    &main::cfg($main::bookfiles, $main::FILTERCFG, "table_line_size") ||
    &main::cfg($main::bookfiles, $main::FILTERCFG, "table-line-size") ||
    "linesize(7.5)";

$main::TABLE_SINGLE_HORIZ_RULE = "_";
$main::TABLE_DOUBLE_HORIZ_RULE = "=";
$main::TABLE_SINGLE_VERT_RULE  = "|";
$main::TABLE_DOUBLE_VERT_RULE  = "||";

$main::TABLE_FRAME_RULE = $main::TABLE_SINGLE_HORIZ_RULE;

$main::TABLE_DEFAULT_COL_SEP = $main::TABLE_SINGLE_VERT_RULE;
$main::TABLE_DEFAULT_ROW_SEP = $main::TABLE_SINGLE_HORIZ_RULE;

%main::COLROLE_START = ();
%main::COLROLE_END   = ();

$main::COLROLE_START{"none"} = "";
$main::COLROLE_END{"none"}   = "";

$main::COLROLE_START{"small"} = "\\s-1";
$main::COLROLE_END{"small"}   = "\\s+1";

{   # Just a placeholder, this is where table column info goes
    package ColSpec;

    sub init {
	$curcolspec = "";
	$DEFAULT_ALIGN = "";
	$COLUMN = 0;
	$DEFAULT_COLSEP = "";
	$DEFAULT_ROWSEP = "";
	%ROWSEP = ();             # String of TBL formatting for horizontal lines
	%COLSEP = ();             # String of TBL formatting for vertical lines
	%MOREROWS = ();           # Height in rows of remaining vertical span in this column
	%ALIGN = ();              # Specifies alignment scheme
	%NAME = ();               # Names of columns indexed by number
	%COL = ();                # Numbers of columns indexed by name
	%CHAR = ();               # Character to align (e.g. "." in decimal numbers")
	%CHAROFF = ();
	%COLWIDTH = ();           # Width in inches
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
	my($self, $markup) = @_;
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
		$markup->markup(".TFS");
		$markup->markup($fn);
		$markup->markup(".TFE");
	    }
	}
    }
}

{
    package INFORMALTABLE;
    @ISA = ('GENERIC_WRAPPER');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($child) = $self->content();
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);
	
	$self->{"FIRST_GROUP"} = 1;
	$self->{"TABLE_STARTED"} = 0;

	$self->{'SAVE_ABOVE_SPACE'} = $main::TABLE_ABOVE_RULE_SPACE;
	$self->{'SAVE_BELOW_SPACE'} = $main::TABLE_BELOW_RULE_SPACE;
	$self->{'SAVE_FRAME_RULE'}  = $main::TABLE_FRAME_RULE;
	$self->{'SAVE_BETWEEN_PARA_SPACE'} = $main::TABLE_BETWEEN_PARA_SPACE;
	$self->{'SAVE_BETWEEN_ROW_SPACE'} = $main::TABLE_BETWEEN_ROW_SPACE;
	$self->{'HEAD_FONT_INSERT'} = $main::TABLE_HEAD_FONT_INSERT;

	&ColSpec::init();
	&SpanSpec::init();
	&FootnoteSpec::init();

	$main::ERR_TABLE_COUNT++;
	$main::ERR_TABLE_TYPE = "TABLE";

	$markup->markup($self->markup_start());

	# ---

	$self->markup_children($markup, $child);

	# ---

	$main::TABLE_ABOVE_RULE_SPACE = $self->{'SAVE_ABOVE_SPACE'};
	$main::TABLE_BELOW_RULE_SPACE = $self->{'SAVE_BELOW_SPACE'};
	$main::TABLE_FRAME_RULE       = $self->{'SAVE_FRAME_RULE'};
	$main::TABLE_BETWEEN_PARA_SPACE = $self->{'SAVE_BETWEEN_PARA_SPACE'};
	$main::TABLE_BETWEEN_ROW_SPACE  = $self->{'SAVE_BETWEEN_ROW_SPACE'};
	$main::TABLE_HEAD_FONT_INSERT = $self->{'HEAD_FONT_INSERT'};

	$markup->markup('.TE') if $self->{'TABLE_STARTED'};
	&FootnoteSpec::output_footnotes($self, $markup);
	$markup->markup($self->markup_end());

	$inmarkup ? $markup : $markup->output();
    }

}

{
    package TABLE;
    @ISA = ('GENERIC_TITLED');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($child) = $self->content();
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);
	
	$self->{"FIRST_GROUP"} = 1;
	$self->{"TABLE_STARTED"} = 0;

	$self->{'SAVE_ABOVE_SPACE'} = $main::TABLE_ABOVE_RULE_SPACE;
	$self->{'SAVE_BELOW_SPACE'} = $main::TABLE_BELOW_RULE_SPACE;
	$self->{'SAVE_FRAME_RULE'}  = $main::TABLE_FRAME_RULE;
	$self->{'SAVE_BETWEEN_PARA_SPACE'} = $main::TABLE_BETWEEN_PARA_SPACE;
	$self->{'SAVE_BETWEEN_ROW_SPACE'} = $main::TABLE_BETWEEN_ROW_SPACE;
	$self->{'HEAD_FONT_INSERT'} = $main::TABLE_HEAD_FONT_INSERT;

	&ColSpec::init();
	&SpanSpec::init();
	&FootnoteSpec::init();

	$main::ERR_TABLE_COUNT++;
	$main::ERR_TABLE_TYPE = "TABLE";

	$self->markup_title($markup);
	$child = $child->next() if $child->{'TAG'} eq 'TITLE';

	# ---

	$self->markup_children($markup, $child);

	# ---

	$main::TABLE_ABOVE_RULE_SPACE = $self->{'SAVE_ABOVE_SPACE'};
	$main::TABLE_BELOW_RULE_SPACE = $self->{'SAVE_BELOW_SPACE'};
	$main::TABLE_FRAME_RULE       = $self->{'SAVE_FRAME_RULE'};
	$main::TABLE_BETWEEN_PARA_SPACE = $self->{'SAVE_BETWEEN_PARA_SPACE'};
	$main::TABLE_BETWEEN_ROW_SPACE  = $self->{'SAVE_BETWEEN_ROW_SPACE'};
	$main::TABLE_HEAD_FONT_INSERT = $self->{'HEAD_FONT_INSERT'};

	$markup->markup('.TE') if $self->{'TABLE_STARTED'};
	&FootnoteSpec::output_footnotes($self, $markup);
	$markup->markup($self->markup_end());

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package TGROUP;
    @ISA = ('GENERIC_WRAPPER');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($parent) = $self->{'PARENT'};

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	%GUTTER = ();

	$pgwide = $parent->{'ATTR'}->{'PGWIDE'};
	$frame  = $parent->{'ATTR'}->{'FRAME'};

	$colsep = $self->{'ATTR'}->{'COLSEP'};
	$rowsep = $self->{'ATTR'}->{'ROWSEP'};
	$colsep = $parent->{'ATTR'}->{'COLSEP'} if $colsep eq "";
	$rowsep = $parent->{'ATTR'}->{'ROWSEP'} if $rowsep eq "";;

	$ColSpec::DEFAULT_ALIGN = $self->{'ATTR'}->{'ALIGN'};
	$ColSpec::COLUMN = 1;
	$ColSpec::DEFAULT_COLSEP = $colsep;
	$ColSpec::DEFAULT_ROWSEP = $rowsep;
	$ColSpec::DEFAULT_ROLE = $self->{'ATTR'}->{'ROLE'};

	$SpanSpec::DEFAULT_ALIGN = $self->{'ATTR'}->{'ALIGN'};
	$SpanSpec::DEFAULT_COLSEP = $colsep;
	$SpanSpec::DEFAULT_ROWSEP = $rowsep;
	$SpanSpec::DEFAULT_ROLE = $self->{'ATTR'}->{'ROLE'};

	$tgroup_count++;
	$row_count = 0;

	$FOOTNOTE_COUNT = 0;

	if ($parent->{"FIRST_GROUP"} == 1) {
	    if ($frame eq "ALL") {
		if ($main::TABLE_FRAME_RULE eq $main::TABLE_DOUBLE_HORIZ_RULE){
		    $option_type = "doublebox";

		} elsif ($main::TABLE_LINESIZE_OPTION) {
		    $option_type = "box, " . $main::TABLE_LINESIZE_OPTION;

		} else {
		    $option_type = "box";
		}
		$options = &add_option( $options, $option_type );
	    }

	    $options = &add_option($options, "expand") if $pgwide == 1;

	    $self->{"TABLE_OPTIONS"} = $options;
	    $parent->{"FIRST_GROUP"} = 0;
	    $options = "";
	}

	$needs_a_topframe = 0;
	$needs_a_topframe = 1 
	    if ($frame eq "TOP") || ($frame eq "TOPBOT");

	$self->markup_children($markup, $self->content());

	&ROW::hrule($markup, 'BF', $self) 
	    if ($frame eq "BOTTOM") || ($frame eq "TOPBOT");

	$inmarkup ? $markup : $markup->output();
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

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($column);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$column = $self->{'ATTR'}->{'COLNUM'} || $ColSpec::COLUMN;
	$ColSpec::COLUMN = $column+1;

	$ColSpec::ALIGN{$column} = ($self->{'ATTR'}->{'ALIGN'} 
				    || $ColSpec::DEFAULT_ALIGN);

	if ($self->{'ATTR'}->{'COLNAME'}) {
	    $ColSpec::NAME{$column} = $self->{'ATTR'}->{'COLNAME'};
	    $ColSpec::COL{$self->{'ATTR'}->{'COLNAME'}} = $column;
	}

	$ColSpec::COLSEP{$column} = $self->{'ATTR'}->{'COLSEP'};
	$ColSpec::ROWSEP{$column} = $self->{'ATTR'}->{'ROWSEP'};

	$ColSpec::CHAR{$column} = ($self->{'ATTR'}->{'CHAR'}) 
	    if $self->{'ATTR'}->{'CHAR'};

	$ColSpec::CHAROFF{$column} = ($self->{'ATTR'}->{'CHAROFF'})
	    if $self->{'ATTR'}->{'CHAROFF'};

	$ColSpec::COLWIDTH{$column} = ($self->{'ATTR'}->{'COLWIDTH'})
	    if $self->{'ATTR'}->{'COLWIDTH'};

	$ColSpec::ROLE{$column} = ($self->{'ATTR'}->{'ROLE'})
	    if $self->{'ATTR'}->{'ROLE'};

	$inmarkup ? $markup : undef;
    }
}

{
    package SPANSPEC;
    @ISA = ('GENERIC_WRAPPER');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($name);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$name = $self->{'ATTR'}->{'SPANNAME'};

	$SpanSpec::ALIGN{$name}   = ($self->{'ATTR'}->{'ALIGN'} 
				     || $SpanSpec::DEFAULT_ALIGN);

	$SpanSpec::CHAR{$name}    = ($self->{'ATTR'}->{'CHAR'})
	    if $self->{'ATTR'}->{'CHAR'};

	$SpanSpec::CHAROFF{$name} = ($self->{'ATTR'}->{'CHAROFF'})
	    if $self->{'ATTR'}->{'CHAROFF'};

	$SpanSpec::COLSEP{$name}  = $self->{'ATTR'}->{'COLSEP'}; 
	$SpanSpec::ROWSEP{$name}  = $self->{'ATTR'}->{'ROWSEP'}; 
	$SpanSpec::NAMEST{$name}  = $self->{'ATTR'}->{'NAMEST'};
	$SpanSpec::NAMEEND{$name} = $self->{'ATTR'}->{'NAMEEND'};

	$SpanSpec::ROLE{$name} = $self->{'ATTR'}->{'ROLE'};

	$inmarkup ? $markup : undef;
    }
}

{
    package THEAD;
    @ISA = ('GENERIC_WRAPPER');

    sub new {
	return bless { };
    }


    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($tgroup) = $self->{'PARENT'};
	my($table) = $tgroup->{'PARENT'};

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	if (!$table->{'TABLE_STARTED'}) {
	    $markup->markup($self->markup_start());
	    $markup->markup("$tgroup->{'TABLE_OPTIONS'} ;\n")
		if $tgroup->{'TABLE_OPTIONS'} ne "";
	}
	$table->{'TABLE_STARTED'} = 1;

	$self->markup_children($markup, $self->content());

	$markup->markup($self->markup_end());

	$ColSpec::curcolspec = "invalid spec, force a restart";

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package TBODY;
    @ISA = ('GENERIC_TAG');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($tgroup) = $self->{'PARENT'};
	my($table) = $tgroup->{'PARENT'};
	my($content);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);
	
	# This is another hack.  We always need to output .TS H for
	# page breaking.
	if (!$table->{'TABLE_STARTED'}) {
	    $NEED_TH = 1;
	    $markup->markup($self->markup_start());
	    $markup->markup("$tgroup->{'TABLE_OPTIONS'} ;\n")
		if $tgroup->{'TABLE_OPTIONS'} ne "";
	    $table->{'TABLE_STARTED'} = 1;
	} else {
	    $NEED_TH = 0;
	}

	# This is really bad.
	#
	# <tbody>
	# <? ...>
	# <row>
	#
	# The PIs come out in the wrong place (between the .TS and the 
	# colspecs).  So we remember the PIs and process them in the row.
	# UGH.
	undef($PENDING_PIS);
	$content = $self->content;
	while (!$self->null($content) && $content->tag('PI')) {
	    $PENDING_PIS = $content if !$PENDING_PIS;
	    $content = $content->next();
	}

	$self->markup_children($markup, $content);

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package ROW;
    @ISA = ('GENERIC_TAG');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($parent) = $self->{'PARENT'};
	my($tgroup) = $parent->{'PARENT'};
	my($child) = $self->content();
	my($colspec) = "";

	my($colst, $colend);
	my($count, $row, $rowsepspec);
	my($entry, $entrycount, $column);
	my($align, $char, $charoff, $colname, $colsep, $morerows,
	   $namest, $nameend, $rotate, $rowsep, $spanname, $valign, $role);
	local($_);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$TGROUP::row_count++;

	if (!defined($parent)) {
	    &main::WARNING("Unexpected ROW tag, not a child!??\n");
	    return;
	}

	# String that determines whether & what kind of lines (rules)
	# to put after row

	$rowsepspec = "";   



#                          *** LOOP THRU ENTRIES ***


	# Loop through all of the entries: calculate the attributes for
	# each particular entry.  The calculated attributes may include
	# several levels of defaulting.  These calculated attributes are
	# stored back into each entry so that they are available immediately
	# when the actual data for the row is being output.

#	print STDERR "\n";

	$child = $self->content();   # entry
	$column = 1;               
	$done = 0;
	while (!$done) {
#	    print STDERR "TOL: $column, $ColSpec::MOREROWS{$column}\n";



#                            *** VERTICAL SPAN ***	    


	    # Handle morerows case: if last row in this column had
	    # morerows, vchild is the last entry in this column

	    if ($ColSpec::MOREROWS{$column}) {
		my($vchild) = $ColSpec::MOREROWS_CHILD{$column};
		foreach $_ ('align', 'char', 'charoff', 'colname', 'colsep', 
			    'namest', 'nameend', 'rotate', 'rowsep', 
			    'spanname', 'valign', 'role') {
		    my($attr) = uc($_);
		    eval "\$$_ = \$vchild->{'ATTR'}->{$attr}";
		}

		# Flag: is morerows still in effect?  This doesn't
		# seem to get set anywhere, so I am guessing!

		$morerows = 0;  

		# How tall is the rest of the vertical span?

		$VerticalSpan = $ColSpec::MOREROWS{$column};


	    # Attributes for entry: $<attr> = <attribute value>

	    } else {
		foreach $_ ('align', 'char', 'charoff', 'colname', 'colsep', 
			    'morerows', 'namest', 'nameend', 'rotate', 'rowsep',
			    'spanname', 'valign', 'role') {
		    my($attr) = uc($_);
		    eval "\$$_ = \$child->{'ATTR'}->{$attr}";
		}

		$self->output_index_terms($child, $markup);
		$VerticalSpan = 0;
	    }


#                            *** HORIZONTAL SPAN ***	    


	    # Information about horiz span stored in SpanSpec package.

	    if ($spanname) {
		$namest = $SpanSpec::NAMEST{$spanname};
		$nameend = $SpanSpec::NAMEEND{$spanname};
		$colsep = $SpanSpec::COLSEP{$spanname} if $colsep eq "";
		$colsep = $SpanSpec::DEFAULT_COLSEP if $colsep eq "";
		$rowsep = $SpanSpec::ROWSEP{$spanname} if $rowsep eq "";
		$rowsep = $self->{'ATTR'}->{'ROWSEP'} if $rowsep eq "";
		$rowsep = $SpanSpec::DEFAULT_ROWSEP if $rowsep eq "";

		$align  = $SpanSpec::ALIGN{$spanname} if $align eq "";
		$char   = $SpanSpec::CHAR{$spanname} if $char eq "";

		$role   = $SpanSpec::ROLE{$spanname} if $role eq "";
		$role   = $SpanSpec::DEFAULT_ROLE if $role eq "";
	    }

	    # Get numbers of columns from names

	    if ($namest || $nameend) {
		$colst = $ColSpec::COL{$namest};
		$colend = $ColSpec::COL{$nameend};
	    } else {
		$colst = $column;
		$colend = $column;
	    }


#                            *** NO SPAN ***	    


	    if (!$spanname) {
		$colsep = $ColSpec::COLSEP{$colst} if $colsep eq "";
		$colsep = $ColSpec::DEFAULT_COLSEP if $colsep eq "";
		$rowsep = $ColSpec::ROWSEP{$colst} if $rowsep eq "";
		$rowsep = $self->{'ATTR'}->{'ROWSEP'} if $rowsep eq "";
		$rowsep = $ColSpec::DEFAULT_ROWSEP if $rowsep eq "";

		$align = $ColSpec::ALIGN{$colst} if $align eq "";
		$char  = $ColSpec::CHAR{$colst} if $char eq "";
		
		$role  = $ColSpec::ROLE{$colst} if $role eq "";
		$role  = $ColSpec::DEFAULT_ROLE if $role eq "";
	    }

#	    print STDERR "$VerticalSpan, $morerows: $rowsep\n";

	    # is there a rule below this row?
	    # later, we will use this info to determine whether to plant 
	    # some extra space

	    # horizontal rule determination: append to rowsepspec

	    if ($VerticalSpan > 1 || $morerows) {
		$rowsepspec .= "\001"; # needs to be a single character...
	    } elsif ($rowsep >= 2) {
		$rowsepspec .= "=";
	    } elsif ($rowsep == 1) {
		$rowsepspec .= "_";
	    } else {
		# Implement ORA standards, table headers are seperated from
		# the table by a rule and the table has a rule below it.
		# This implementation will break if the table has multiple
		# tgroups.  (But you can override it with a rowsep attr.)
		#
		# if there is no explicit rowsep, and there are no more rows 
		# and the parent is a thead and the next thing after the 
		# parent is a tbody, we know that were in the last row of 
		# the header.
		if ($rowsep eq "" 
		    && !$self->{'NEXT'}) {
		    $rowsepspec .= $main::TABLE_DEFAULT_ROW_SEP;
		} else {
		    $rowsepspec .= " ";
		}
	    }

	    &main::WARNING("Warning: span out of place in table ($colst != $column).\n")
		if ($colst != $column);

	    # Append column info to TBL formatting string
	    # - Alignment:

	    $fontinsert = $main::TABLE_HEAD_FONT_INSERT;
	    $newcolspec = &align($column, $align, $ColSpec::COLWIDTH{$column});
	    $newcolspec =~ s/^(.)/$1$fontinsert/ if $self->{'PARENT'}->{'TAG'} eq "THEAD";
	    $colspec .= $newcolspec;

	    for ($count = $colst+1; $count <= $colend; $count++) {
		$colspec .= "s ";
	    }

	    # - Vertical rules:

	    if ($colend < $tgroup->{'ATTR'}->{'COLS'}) {
		$colspec .= "$main::TABLE_SINGLE_VERT_RULE " if $colsep == 1;
		$colspec .= "$main::TABLE_DOUBLE_VERT_RULE " if $colsep == 2;

		# Implement ORA standards, table columns are seperated by
		# rules.  (You you can override this with a rowsep attr.)
		$colspec .= "$main::TABLE_DEFAULT_COL_SEP "
		    if ($colsep eq "") && $main::TABLE_DEFAULT_COL_SEP;
	    }

	    # Increment $column & examine next entry

	    $column = $colend+1;
	    $child = $child->next() if $child && !$VerticalSpan;


#                    *** EXIT CONDITION ***


	    # Exit if no more entries in row and vertical span is completed

	    $done = 1 if !$child && !$ColSpec::MOREROWS{$column};

#	    print STDERR "ROW $TGROUP::row_count ($colst\-$colend; $column) ";
#	    print STDERR "done=$done, vs=$VerticalSpan, child=", $child, ", more=$ColSpec::MOREROWS{$column}\n";
	}

	if ($column-1 != $tgroup->{'ATTR'}->{'COLS'}) {
	    my($msg) = sprintf("Expected %d but got %d.",
			       $tgroup->{'ATTR'}->{'COLS'},
			       $column-1);
	    
	    $self->table_warning("wrong number of columns", $msg);
	}

	# Output TBL formatting string for column info

	if ($colspec ne $ColSpec::curcolspec) {
	    $markup->markup(".T&") if $ColSpec::curcolspec ne "";
	    $markup->markup("$colspec.\n");
	    $ColSpec::curcolspec = $colspec;
	}


#                    *** ROW SPACE ***


	# Add space to row right below horiz rule (only in header)

	if ($main::TABLE_ABOVE_HEAD_SPACE && 
	    $self->{'PARENT'}->{'TAG'} eq "THEAD" &&
	    $rowsepspec =~ /_/) {

	    $markup->markup( ".sp $main::TABLE_ABOVE_HEAD_SPACE" );
	}

#	print STDERR "$TGROUP::row_count: $rowsepspec\n";

	$TGROUP::lastrowsepspec = $rowsepspec;

	# End the header & start body: put down a ".TH" tag

	if ($TBODY::NEED_TH) {
	    $markup->markup(".TH");
	    undef($TBODY::NEED_TH);
	}

	if ($TBODY::PENDING_PIS) {
	    my($pi) = $TBODY::PENDING_PIS;
	    while (!$self->null($pi) && $pi->tag('PI')) {
		$pi->markup($markup);
		$pi = $pi->next();
	    }
	    undef($TBODY::PENDING_PIS);
	}


# put horiz rule at top of table

	&hrule($markup, 'TF', $self)
	    if ($TGROUP::row_count == 1 && $TGROUP::needs_a_topframe);


#                    *** PROCESS & OUTPUT ENTRY ***
	

	$child = $self->content();
	$column = 1;
	$row = "";
	$done = 0;
	while (!$done) {
	    my($entry) = undef;
	    my($estart, $eend) = ("", "");
	    my($child_markup);

	    # Get the attributes again

	    if ($ColSpec::MOREROWS{$column}) {
		my($vchild) = $ColSpec::MOREROWS_CHILD{$column};
		foreach $_ ('align', 'char', 'charoff', 'colname', 'colsep', 
			    'namest', 'nameend', 'rotate', 'rowsep', 
			    'spanname', 'valign', 'role') {
		    my($attr) = uc($_);
		    eval "\$$_ = \$vchild->{'ATTR'}->{$attr}";
		}
		$morerows = 0;
		$ColSpec::MOREROWS{$column}--;
		$VerticalSpan = 1;
	    } else {
		$VerticalSpan = 0;

		foreach $_ ('align', 'char', 'charoff', 'colname', 'colsep', 
			    'morerows', 'namest', 'nameend', 'rotate', 
			    'rowsep', 'spanname', 'valign', 'role') {
		    my($attr) = uc($_);
		    eval "\$$_ = \$child->{'ATTR'}->{$attr}";
		}
	    }

	    if ($spanname) {
		$namest = $SpanSpec::NAMEST{$spanname};
		$nameend = $SpanSpec::NAMEEND{$spanname};
	    }

	    if ($namest || $nameend) {
		$colst = $ColSpec::COL{$namest};
		$colend = $ColSpec::COL{$nameend};
	    } else {
		$colst = $column;
		$colend = $column;
	    }

	    # Save morerows info to colspec placeholder

	    if ($morerows) {
		$ColSpec::MOREROWS{$column} = $morerows;
#		print STDERR "SET ROW: $column=$morerows\n";
		$ColSpec::MOREROWS_CHILD{$column} = $child;
	    } 


#                    *** ALIGNMENT SCHEME ***

	    # Align = char

	    if ($child && $align eq "CHAR") {
		my($pre, $post);

		if ($self->block_elements($child)) {
		    $self->table_warning("cannot align=char block elements");
		} else {
		    $entry = $child->markup();
		    $pre   = $entry;
		    $post  = "";

		    if ($char ne "") {
			my($thechar) = "\\$char";
			if ($entry =~ /^([^$thechar]+)($thechar)(.*)$/) {
			    $pre = "$1";
			    $post = "$2$3";
			}
		    }

		    $entry = "$pre\\&$post";
		}
	    }

	    # Text Blocks

	    # If the entry is a paragraph and the alignment isn't fully
	    # justified, insert a .na to make the paragraph not be filled
	    # flush right...

	    if ($child && $self->block_elements($child) &&
	       ($ColSpec::ALIGN{ $column } ne "CHAR")
	       ) {
		$estart = "T{\n";
		$eend = "T}";

		if ($align ne 'JUSTIFY') {
		    $estart = "T{\n.na\n";
		}
	    }

	    # Handle special markup for table headers
	    # (Had to take this out, as it was causing TBL to do
	    # strange things.  Instead, we are passing header font
	    # declaration in the colspec string via global variable
	    # $main::TABLE_HEAD_FONT_INSERT)

#	    if ($self->{'PARENT'}->{'TAG'} eq "THEAD") {
#		$role = "";	# roles dont change headers
#		$estart .= "\\*[EM-TBLHD]";
#		$eend = "\n$eend" if $eend eq "T}";
#		$eend = "\\*[/EM-TBLHD]$eend";
#	    }

	    if ($role ne "") {
		$role =~ tr/A-Z/a-z/;
		if (defined($main::COLROLE_START{$role})) {
		    $estart .= $main::COLROLE_START{$role};
		    $eend = "\n$eend" if $eend eq "T}";
		    $eend .= $main::COLROLE_END{$role};
		} else {
		    &main::WARNING("Undefined column role ignored: $role\n");
		}
	    }



#                    *** MARKUP ENTRY ***


	    $markup->markup("\t") if $column != 1;

	    if ($VerticalSpan) {
		$markup->markup("\\\^");
	    } else {
		$markup->markup("$estart");

		$child_markup = $child->markup();

		# This fixes bug where a footnote in column of numbers
		# appears on new line

		$child_markup =~ s/\n//g if $ColSpec::ALIGN{ $column } eq "CHAR";

		# empty cell

		if ($child_markup eq "") {
		    $markup->markup("\\&");
		} else {
		    $markup->markup($child_markup);
		}
		
		$markup->markup("\n")
		    if ($eend =~ /^T\}/ && $markup->{'MARKUP_COL'} > 0);

		$markup->markup("$eend");
	    }

	    $column = $colend + 1;
	    $child = $child->next() if $child && !$VerticalSpan;

	    $done = 1 if !$child && !$ColSpec::MOREROWS{$column};
	}

	$markup->markup("\n");


#                    *** HORIZONTAL RULES ***


	{
	    while ($row =~ /(\.XET.*?\.\/XET)/) {
		$index .= $1;
		$row = "$`$'";
	    }

	    $index =~ s/\.X/\n\.X/g;
	    $index =~ s/\.\/X/\n\.\/X/g;
	    $index = substr($index,1); # Avoid the initial newline
	    $markup->markup($index) if $index ne "";
	}

	if ($rowsepspec =~ /^\=+$/) { # double rule across table
	    &hrule($markup, 2, $self);
	} elsif ($rowsepspec =~ /^\_+$/) { # single rule across table
	    &hrule($markup, 1, $self);
	} elsif ($rowsepspec !~ /^[ \001]+$/) { # rule across at least one column
	    while ($rowsepspec =~ /^(.)(.*)$/) {
#		$main::ruleabove = 1;
#		print STDERR "SepSpec: ($1)\n";
		if ($1 eq "=") {
		    $markup->markup($main::TABLE_DOUBLE_HORIZ_RULE);
		} elsif ($1 eq "_") {
		    $markup->markup($main::TABLE_SINGLE_HORIZ_RULE);
		} elsif ($1 eq "\001") {
#		    print STDERR "Got a 01\n";
		    $markup->markup("\\\^");
		}
		$markup->markup("\t") if $2 ne "";
		$rowsepspec = $2;
	    }
	    $markup->markup("\n");
	} else {
	    $markup->markup( ".sp $main::TABLE_BETWEEN_ROW_SPACE" )
		if  $main::TABLE_BETWEEN_ROW_SPACE &&
		    ($self->{'PARENT'}->{'TAG'} ne "THEAD") &&
		    !(&main::cfg($main::bookfiles, $main::FILTERCFG, 
				 "suppress_between_row_space") ||
		      &main::cfg($main::bookfiles, $main::FILTERCFG, 
				 "suppress-between-row-space"));
	}

	$inmarkup ? $markup : $markup->output();
    }

    sub align {
	local($col, $atype, $width) = @_;
	my($char);

	if ($atype eq "LEFT") {
	    $char = "l";
	} elsif ($atype eq "RIGHT") {
	    $char = "r";
	} elsif ($atype eq "CENTER") {
	    $char = "c";
	} elsif ($atype eq "CHAR") {
	    $char = "n";
	} else {
	    $char = "l";
	}

	$char .= $TGROUP::GUTTER{$col} if defined($TGROUP::GUTTER{$col});

	$char .= "w($width)" if $width ne "";
	
	return "$char ";
    }

    sub hrule {
	my($markup, $rule, $self) = @_;
	my($lookahead);

	$lookahead = $self->next() || $self->{'PARENT'}->{'NEXT'};

	$markup->markup(".sp $main::TABLE_ABOVE_RULE_SPACE") 
	    if (($rule ne 'TF')
		&& ($main::TABLE_ABOVE_RULE_SPACE ne "")
		&& ($main::TABLE_ABOVE_RULE_SPACE ne "0p"));

	if (($rule eq 'TF') || ($rule eq 'BF')) {
	    $markup->markup("$main::TABLE_FRAME_RULE\n");
	} elsif ($rule == 1) {
	    $markup->markup("$main::TABLE_SINGLE_HORIZ_RULE\n");
	} elsif ($rule >= 2) {
	    $markup->markup("$main::TABLE_DOUBLE_HORIZ_RULE\n");
	}

	if ($lookahead->{'TAG'} eq "TBODY" || $lookahead->{'TAG'} eq "ROW") {
	    $markup->markup(".sp $main::TABLE_BELOW_RULE_SPACE") 
		if (($rule ne 'BF') 
		    &&($main::TABLE_BELOW_RULE_SPACE ne "")
		    && ($main::TABLE_BELOW_RULE_SPACE ne "0p"));
	}
    }

    sub output_index_terms {
	my($self, $child, $markup) = @_;

	while ($child) {
	    if ($child->{'TAG'} eq 'INDEXTERM') {
		$child->markup($markup);
		$child->{'already_marked_up'} = 1;
	    }

	    $self->output_index_terms($child->content(), $markup);
	    $child = $child->next();
	}
    }

    sub table_warning {
	my($self, $warning, $addnl) = @_;
	my($msg, $tblnum);

	$tblnum = $main::ERR_INFORMALTABLE_COUNT;
	$tblnum = $main::ERR_TABLE_COUNT 
	    if $main::ERR_TABLE_TYPE eq "TABLE";

	$msg = sprintf("Warning: %s in %s number %d in %s (group %d, row %d).  $addnl\n",
		       $warning,
		       $main::ERR_TABLE_TYPE,
		       $tblnum, 
		       $main::PI_SOURCE_FILE,
		       $TGROUP::tgroup_count,
		       $TGROUP::row_count);

	    &main::WARNING($msg);
    }

    sub block_elements {
	my($self, $child) = @_;
	my($block) = 0;

	$child = $child->content();
	
	while (!$block && $child) {
	    $block = 1 if ($child->{'TAG'} ne 'INDEXTERM') 
#		&& ($child->{'TAG'} ne 'FOOTNOTE')
#		&& ($child->{'TAG'} ne 'FOOTNOTEREF')
		&& ($child->markup_start() =~ /^\./);
	    $child = $child->next();
	}

	$block;
    }
}

{
    package ENTRY;
    @ISA = ('GENERIC_WRAPPER');

    sub new {
	return bless { };
    }
}

######################################################################
# Footnotes
######################################################################

{
    package FOOTNOTEREF;
    @ISA = ("GENERIC_INLINE");

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($parent) = $self->{'PARENT'};
	my($gparent) = $parent->{'PARENT'};
	my($footnote, $fnmark, $linkend);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	if ($parent->tag('ENTRY') || 
	    ($parent->tag('PARA') && $gparent->tag('ENTRY'))) {

	    $linkend = $self->attr("LINKEND");

	    $footnote = $FOOTNOTE::FOOTNOTES{$linkend};
	    if (!defined($footnote)) {
		&main::WARNING("Can\'t find footnote with ID: $linkend\n");
	    } else {
		$fnmark = "\\*[" . $footnote->{'FOOTNOTE_NUMBER'} . "]";
		$markup->markup($fnmark) if !$self->role('NOMARK');
	    }
	} else {
	    $markup->markup("\\**") if !$self->role('NOMARK');
	    $markup->markup($self->markup_start());
	    $self->markup_children($markup, $self->content());
	    $markup->markup($self->markup_end());
	}

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package FOOTNOTE;
    @ISA = ("GENERIC_TAG");

    %FOOTNOTES = ();

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($parent) = $self->{'PARENT'};
	my($gparent) = $parent->{'PARENT'};
	my($data, $fnmark);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	if ($parent->tag('ENTRY') || 
	    ($parent->tag('PARA') && $gparent->tag('ENTRY'))) {
	    $TGROUP::FOOTNOTE_COUNT++;
	    $self->{'FOOTNOTE_NUMBER'} = $TGROUP::FOOTNOTE_COUNT;
	    $FOOTNOTES{$self->id()} = $self if $self->id();
	    $fnmark = "\\*[" . $TGROUP::FOOTNOTE_COUNT . "]";
	    $markup->markup($fnmark) if !$self->role('NOMARK');
	    $data = $self->markup_children(undef, $self->content());
	    push(@FootnoteSpec::FN, $data);
	} else {
	    $markup->markup("\\**") if !$self->role('NOMARK');
	    $markup->markup($self->markup_start());
	    $self->markup_children($markup, $self->content());
	    $markup->markup($self->markup_end());
	}

	$inmarkup ? $markup : $markup->output();
    }
}

######################################################################
# Table of Contents
######################################################################

{ 
    package TOCENTRY;
    @ISA = ('GENERIC_TAG');
    
    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($parent) = $self->{'PARENT'};
	my($role) = "";

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);
	
	if ($parent->{'TAG'} eq 'TOCCHAP') {
	    $role = " C ";
	    $role = " A " if $parent->{'ATTR'}->{'ROLE'} =~ /^Appendix/i;
	    $role = " P " if $parent->{'ATTR'}->{'ROLE'} =~ /^Preface/i;
	    $role = " G " if $parent->{'ATTR'}->{'ROLE'} =~ /^Glossary/i;
	    $role = " C " if $parent->{'ATTR'}->{'ROLE'} =~ /^Chapter/i;
	}

	# Wait, now shouldn't somebody use the $role?
	
	$markup->markup($self->markup_start());
	$self->markup_children($markup, $self->content());
	$markup->markup($self->markup_end());

	$inmarkup ? $markup : $markup->output();
    }
}

{ 
    package LOTENTRY;
    @ISA = ('GENERIC_TAG');
    
    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($parent) = $self->{'PARENT'};
	my($role) = "";

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$markup->markup($self->markup_start());
	$self->markup_children($markup, $self->content());
	$markup->markuip($self->markup_end());

	$inmarkup ? $markup : $markup->output();
    }
}

######################################################################
# REFENTRY PAGES
######################################################################

{
    package REFENTRY;
    @ISA = ('GENERIC_TAG');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($child, $innerchild);
	
	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$TITLE = "";
	$MISC = "";
	$VOL = "";

	$child = $self->content();
	while ($child->{'TAG'} ne "REFNAMEDIV") {
	    if ($child->{'TAG'} eq "REFMETA") {
		$innerchild = $child->content();
		while ($innerchild) {
		    $TITLE = $innerchild->markup()
			if $innerchild->{'TAG'} eq "REFENTRYTITLE";
		    $VOL = $innerchild->markup()
			if $innerchild->{'TAG'} eq "MANVOLNUM";
		    $MISC = $innerchild->markup()
			if ($innerchild->{'TAG'} eq "REFMISCINFO" &&
			    !($innerchild->attr('CLASS')));
		    $innerchild = $innerchild->next();
		}
	    }
	    $child = $child->next();
	}

	$markup->markup($self->markup_start() . " \"$TITLE\" \"$MISC\" \"$VOL\"");
	$self->markup_children($markup, $self->content());
	$markup->markup($self->markup_end());

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package REFMETA;
    @ISA = ('GENERIC_NOP');

    sub new {
	return bless { };
    }
}

{
    package REFMISCINFO;
    @ISA = ('GENERIC_INLINE');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($child) = $self->content();

	$newmarkup = new MARKUP_OBJECT;
	$child->markup($newmarkup);

	$inmarkup ? $markup : $newmarkup->output();
    }
}

{
    package REFENTRYTITLE;
    @ISA = ('GENERIC_INLINE');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($child) = $self->content();

	$newmarkup = new MARKUP_OBJECT;
	$child->markup($newmarkup);

	$inmarkup ? $markup : $newmarkup->output();
    }

}

{
    package REFNAMEDIV;
    @ISA = ('GENERIC_TAG');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$markup->markup($self->markup_start());
	$self->markup_children($markup, $self->content());
	$markup->markup($self->markup_end());

	$inmarkup ? $markup : $markup->output();
    }
}

########################################################################
# Callouts and program listings and graphics...
########################################################################

{
    package COMPLEXPROGRAMLISTING;
    @ISA = ('GENERIC_WRAPPER');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$self->markup_children($markup, $self->content());
    }
}

{
    package COMPLEXGRAPHIC;
    @ISA = ('GENERIC_WRAPPER');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$self->markup_children($markup, $self->content());
    }
}

{
    package AREASPEC;
    @ISA = ('GENERIC_INLINE');

    @Areas = ();
    @Lines = ();
    @Labels = ();
    $AreaCount = 0;

    sub new {
	my($ref) = bless { };
	$ref->{'AreaCount'} = 0;
	$ref->{'Areas'} = ();
	return $ref;
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($parent) = $self->parent();

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	# Markup the children, but discard any markup they might produce...
	$self->markup_children(undef, $self->content());

	$inmarkup ? $markup : $markup->output();
    }

    sub add_area {
	my($self, $id, $line, $label, $areanumber) = @_;

	$self->{'AreaCount'}++;
	$self->{'AreaCount'} = $areanumber if $areanumber =~ /^\d+$/;
	$label = sprintf("\\s10\\f[ZD]\\N'%d'\\fP\\s0", $self->{'AreaCount'}+201)
	    if $label eq "";

	push(@{$self->{'Areas'}}, $AreaCount);

	push (@Areas, $id);
	push (@Lines, $line);
	push (@Labels, $label);

	$AreaCount++;

	$label;
    }
}

{
    package TEXTAREA;
    @ISA = ('GENERIC_INLINE');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($parent) = $self->parent();
	my($label);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	# Markup the children, but throw away the formatted markup...
	$self->{'Label'} = "";
	$self->markup_children(undef, $self->content());

	$label = $parent->add_area($self->id(),
				   $self->attr('LINENUMBER') 
				     || $self->attr('COORDS'),
				   $self->{'Label'},
				  $self->attr('AREANUMBER'));

	$main::TAGID->{$self->id()}->{'XREFLABEL'} = $label;

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package GRAPHICAREA;
    @ISA = ('TEXTAREA');

    sub new {
	return bless { };
    }
}

{
    package LABEL;
    @ISA = ('GENERIC_INLINE');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($parent) = $self->parent();

	&main::WARNING("Label unexpected in " . $parent->tag() . ".\n")
	    if !$parent->tag('TEXTAREA') && !$parent->tag('GRAPHICAREA');

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$parent->{'Label'} = $self->markup_children(undef, $self->content());

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package PROGRAMLISTING;
    @ISA = ('GENERIC_WRAPPER');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($parent) = $self->parent();
	my(@Areas) = @AREASPEC::Areas;
	my(@Lines) = @AREASPEC::Lines;
	my(@Labels) = @AREASPEC::Labels;
	my($AreaSpec, @LocalAreas);
	my(@lines, $line, $area, $count, $linecount);
	my($longest, $flushright) = (0, 0);
	my($linemarkup);
	local($_);

	$AreaSpec = ($self->parent())->content();
	while ($AreaSpec && $AreaSpec->tag('PI')) {
	    $AreaSpec = $AreaSpec->next();
	}
	if ($AreaSpec && $AreaSpec->tag('AREASPEC')) {
	    @LocalAreas = @{$AreaSpec->{'Areas'}};
	} else {
	    @LocalAreas = ();
	}

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$markup->{'OBEY_SPACES'}++;
	$markup->markup($self->markup_start());

	$linemarkup = new MARKUP_OBJECT;
	$linemarkup->{'OBEY_SPACES'}++;
	$self->markup_children($linemarkup, $self->content());
	$_ = $linemarkup->output();
	undef $linemarkup;

	@lines = split(/\n/, $_);
	foreach $_ (@lines) {
	    $longest = length($_) if length($_) > $longest;
	}

	$linecount = 0;
	foreach $line (@lines) {
	    $linecount++;
	    $markup->markup(sprintf("%03d: ", $linecount))
		if $main::GMAT_DRAFT_MODE;
	    $markup->markup($line);
	    $flushright = 0;

	    if ($parent->tag('COMPLEXPROGRAMLISTING')) {
		foreach $count (@LocalAreas) {
		    next if $Lines[$count] != $linecount;

		    if (!$flushright) {
			$_ = " " x ($longest - length($line) + 3);
			$markup->markup($_);
			$flushright = 1;
		    }

		    $markup->markup($Labels[$count]);
		}
	    }

	    $markup->markup("\n");
	}

	$markup->markup($self->markup_end());
	$markup->{'OBEY_SPACES'}--;

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package CO;
    @ISA = ('GENERIC_WRAPPER');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($tag_start, $tag_end);
	my($label) = $self->{'ATTR'}->{'LABEL'};
	my($linkends) = $self->{'ATTR'}->{'LINKENDS'};
	my(@linkends) = split /\s+/, $linkends;
	my($linkend, $endchap, $endsect);
	my($pattern) = "";

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	while (scalar @linkends > 0) {

	    $linkend = shift(@linkends);

	    if (defined($main::TAGID->{$linkend})) {

		$endchap = $main::TAGID->{$linkend}->{'CHP-NUM'};
		$endsect = $main::TAGID->{$linkend}->{'SECT-NUM'};

		$pattern .= " \"" . $endchap . "." . sprintf("%02d",$endsect) . "\"";

	    } else {
		&main::WARNING("Warning: xref ID \"$linkend\" is unknown.\n");
		$pattern = "";
	    }

	    $pattern = "**UNKNOWN XREF**" if $pattern eq "";

	}

	$markup->markup($self->markup_start() . " \"$label\"$pattern");

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package CALLOUT;
    @ISA = ('GENERIC_WRAPPER');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($parent) = $self->parent();
	my($AreaId) = $parent->attr('OBJECTREFS');
	my(@Areas) = @AREASPEC::Areas;
	my(@Lines) = @AREASPEC::Lines;
	my(@Labels) = @AREASPEC::Labels;
	my(%AreaNum, $count, $area, $label);

	for ($count = 0; $count <= $#Areas; $count++) {
	    $AreaNum{$Areas[$count]} = $count;
	}

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$label = $Labels[$AreaNum{$self->attr('AREAREF')}];
	
	$markup->markup($self->markup_start());
	$markup->markup(".LIV");
	$markup->markup(".LIT $label");
	$markup->markup(".LI");
	$self->markup_children($markup,$self->content());
	$markup->markup("./LI");
	$markup->markup("./LIV");
	$markup->markup($self->markup_end());

	$inmarkup ? $markup : $markup->output();
    }
}
	
########################################################################
# Handle AuthorGroup and Author for DocInfo on Chapter for author
# "by lines".  These are hacks and are *clearly* incomplete
########################################################################

{
    package DOCINFO;
    @ISA = ('GENERIC_TAG');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($child) = $self->content();

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

#	$self->markup_children($markup, $self->content());

	while (!$self->null($child)) {
	    $child->markup($markup) if $child->tag('AUTHORGROUP');
	    $child = $child->next();
	}

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package SECT1INFO;
    @ISA = ('GENERIC_TAG');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($child) = $self->content();

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	while (!$self->null($child)) {
	    $child->markup($markup) if $child->tag('AUTHORINITIALS');
	    $child = $child->next();
	}

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package AUTHORGROUP;
    @ISA = ('GENERIC_WRAPPER');

    sub new {
	return bless { };
    }
}

{
    package AUTHOR;
    @ISA = ('GENERIC_ASARG');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($child) = $self->content();
	local($_);

	print STDERR "author!\n";

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$_ = "";
	while (!$self->null($child)) {
	    if ($child->tag('FIRSTNAME')
	       || $child->tag('SURNAME')
	       || $child->tag('LINEAGE')) {
		$_ .= " \"" . $child->markup() . "\"";
	    }

	    $child = $child->next();
	}

	$markup->markup($self->markup_start() . $_);

	$inmarkup ? $markup : $markup->output();
    }
}

1;
