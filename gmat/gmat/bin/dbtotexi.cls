# -*- Perl -*-
# Classes for dbtotexi
#
# $Id: dbtotexi.cls,v 1.1.1.1 2002-07-15 21:33:31 root Exp $
#
#
##############################################################################

if ($VERBOSE) {
    my($VERSION) = '$Id: dbtotexi.cls,v 1.1.1.1 2002-07-15 21:33:31 root Exp $'; # '
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

	    $self->{'OUTPUT'} .= $_;
	    $self->{'MARKUP_COL'} += length($_);
	    $self->{'MARKUP_COL'} = 0 if substr($_, length($_)-1, 1) eq "\n";
	}

	$self->flush();
    }

    sub markup {
	my($self, $text) = @_;
	my($place, @lines, $save_, $nl);
	local($_) = $text;

	return if $_ eq "";

	$nl = !(/^\@emph\{/
		|| /^\@TeX/
		|| /^\@t\{/
		|| /^\@samp\{/
		|| /^\@cite\{/
		|| /^\@file\{/
		|| /^\@copyright\{/
		|| /^\@xref\{/
		|| /^\@var\{/
		|| !/^\@/);

	# Can't just match a regexp in here because \n's 
	# cause all sorts of prob.

	# break incoming text into lines
	while (($place = index($_, "\n")) >= 0) {
	    push (@lines, substr($_, 0, $place+1));
	    $_ = substr($_, $place+1);
	}
	push (@lines, $_) if $_ ne "";

	# Output several newlines if this is one of those tags that 
	# wants it
	if ($nl) {
	    $self->{'OUTPUT'} .= "\n";
	    $self->{'OUTPUT'} .= "\n" if $self->{'MARKUP_COL'} > 0;
	    $self->{'MARKUP_COL'} = 0;
	}

	while (@lines) {
	    $_ = shift @lines;

	    $self->{'OUTPUT'} .= "$_";
	    $self->{'MARKUP_COL'} += length($_);
	    $self->{'MARKUP_COL'} = 0 if substr($_, length($_)-1, 1) eq "\n";
	}

	if ($nl && $self->{'MARKUP_COL'} > 0) {
	    $self->{'OUTPUT'} .= "\n";
	    $self->{'MARKUP_COL'} = 0;
	}

	$self->flush();
    }

    sub raw_markup {
	my($self, $text) = @_;
	local($_) = $text;

	$self->{'OUTPUT'} .= $_;
	$self->{'MARKUP_COL'} += length($_);
	$self->{'MARKUP_COL'} = 0 if substr($_, length($_)-1, 1) eq "\n";

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

{
    package GENERIC_TAG;

    $TAG_NULL = "::TAG::NULL::";
    $LAST_ELEM = "";

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
	    if ($ref =~ /\{\s*$/) {
		$ref = "}";
	    } else {
		$ref = "";
	    }
	}

	return $ref;
    }
	
    sub save_id {
	my($self) = @_;
	my($chap, $sect, $refsect, $count, $title);
	my($s, $child, $id, $tag);
	my(%TAGLEVEL);

	$id = $self->{'ATTR'}->{'ID'};
	$tag = $self->{'TAG'};

	%TAGLEVEL = %{$self->{'TAGLEVEL'}};

	$chap = $main::PI_CHAPTER_NUMBER || "1";

	$sect = $TAGLEVEL{'SECT1'};
	foreach $s ('SECT2', 'SECT3', 'SECT4', 'SECT5') {
	    $sect .= "." . $TAGLEVEL{$s} if defined($TAGLEVEL{$s});
	}

	$refsect = $TAGLEVEL{'REFSECT1'};
	foreach $s ('REFSECT2', 'REFSECT3', 'REFSECT4', 'REFSECT5') {
	    $refsect .= "." . $TAGLEVEL{$s} if defined($TAGLEVEL{$s});
	}

	$count = $TAGLEVEL{$tag};

	$title = "";
	$child = $self->content();
	if ($child && $child->{'TAG'} eq "TITLE") {
	    $title = $child->markup();
	}

	$self->{'CHP-NUM'} = $chap;
	$self->{'SECT-NUM'} = $sect;
	$self->{'RSECT-NUM'} = $refsect;
	$self->{'ITEM-NUM'} = $count;
	$self->{'TITLE'} = $title;

#	if ($self->{'TAG'} =~ /^CHAP/ || $self->{'TAG'} =~ /^SECT/) {
#	    print STDERR "TAG: $self->{TAG}, ($chap,$sect,$refsect,$count)";
#	    print STDERR " $title\n";
#	}

	return if $id eq "";

	$main::TAGID->{$id} = { } if !defined($main::TAGID->{$id});

	if ($tag ne 'INDEXTERM') {
	    $main::TAGID_CHANGE = 
		($main::TAGID_CHANGE 
		 || $main::TAGID->{$id}->{'TAG'} ne $tag
		 || $main::TAGID->{$id}->{'CHP-NUM'} ne $chap
		 || $main::TAGID->{$id}->{'SECT-NUM'} ne $sect
		 || $main::TAGID->{$id}->{'RSECT-NUM'} ne $refsect
		 || $main::TAGID->{$id}->{'ITEM-NUM'} ne $count
		 || $main::TAGID->{$id}->{'TITLE'} ne $title
		 || $main::TAGID->{$id}->{'SOURCE-FILE'} ne $main::PI_SOURCE_FILE);
	}

	$main::TAGID->{$id}->{'TAG'} = $tag;
	$main::TAGID->{$id}->{'CHP-NUM'} = $chap;
	$main::TAGID->{$id}->{'SECT-NUM'} = $sect;
	$main::TAGID->{$id}->{'RSECT-NUM'} = $refsect;
	$main::TAGID->{$id}->{'ITEM-NUM'} = $count;
	$main::TAGID->{$id}->{'TITLE'} = $title;
	$main::TAGID->{$id}->{'SOURCE-FILE'} = $main::PI_SOURCE_FILE;
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
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup(markup);

	&main::WARNING("No markup method for $self->{TAG}.\n");
	$self->markup_children($markup, $self->content());
	
	$inmarkup ? $markup : $markup->output();
    }
	
    sub markup_children {
	my($self, $markup, $child) = @_;

	while ($child && $child ne $TAG_NULL) {
	    $child->markup($markup);
	    $child = $child->next();
	}

	$markup;
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
	my($prev) = $self->prev();
	my($parent) = $self->{'PARENT'};
	my($gparent) = "";

	$gparent = $parent->{'PARENT'} if defined ($parent);

	$prev = $prev->{'TAG'} if $prev;
	$parent = $parent->{'TAG'} if $parent;
	$gparent = $gparent->{'TAG'} if $gparent; 

	$prev = "" if !$prev;
	$parent = "" if !$parent;

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	# Don't output \n after <LI>
	$markup->markup("\n")
	    if ($gparent eq 'VARLISTENTRY' ||
		($prev ne "" || $parent ne 'LISTITEM'));

	$markup->markup($self->markup_start());
	$self->markup_children($markup, $self->content());
	$markup->markup($self->markup_end());
	$markup->markup("\n");

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
	my($indent) = "\"\"";	# there's no hook for this yet...
	my($ptsize) = "";
	my($child);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	if ($self->{'ATTR'}->{'ROLE'} =~ /^([0-9]+)/) {
	    $ptsize = $1;
	}

	$markup->{'OBEY_SPACES'}++;
	$markup->markup($self->markup_start());
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
	    $title =~ s/<\001lb>/ /g;

	    if ($main::PI_NUMBERED_SECTIONS) {
		if ($self->{'TAG'} =~ /^SECT\d/) {
		    $title = $self->{'CHP-NUM'} . "." .
			$self->{'SECT-NUM'} . ". $title";
		} elsif ($self->{'TAG'} =~ /^REFSECT\d/) {
		    $title = $self->{'CHP-NUM'} . "." .
			$self->{'RSECT-NUM'} . ". $title";
		} else {
		    #nop;
		}
	    }

	    $markup->markup($self->markup_start() 
			    . " $title"
			    . $self->markup_end());

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

	$inmarkup ? $markup : $markup->output();
    }
}

{ 
    package SECT1;
    @ISA = ('GENERIC_TITLED');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($child) = $self->content();
	my($nexttitle, $prevtitle, $uptitle, $title);
	my($nexte, $preve, $upe);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$upe = $self->{'PARENT'};
	$upe->menu($markup);

	$uptitle = "Top";
	$upe     = $upe->content();
	if ($upe && $upe->{'TAG'} eq 'TITLE') {
	    $uptitle = $upe->markup();
	    $uptitle =~ s/<\001lb>//g;
	}

	$prevtitle = $self->prev_node(('PREFACE', 'CHAPTER', 'APPENDIX', 'SECT1'));
	$nexttitle = $self->next_node(('PREFACE', 'CHAPTER', 'APPENDIX', 'SECT1'));

	$title = "?none?";
	if ($child->{'TAG'} eq 'TITLE') {
	    $title = $child->markup();
	    $title =~ s/<\001lb>//g;
	}

	$markup->markup("\@node $title, $nexttitle, $prevtitle, $uptitle");

	$self->markup_title($markup);
	$child = $child->next() if $child->{'TAG'} eq 'TITLE';
	$self->markup_children($markup, $child);

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
	my($bktitle) = &main::cfg($main::bookfiles, 
				  $main::FILTERCFG, "book_title")
	    || "No title supplied in $main::BOOKFILES";

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$markup->markup("<html>\n<head>\n<title>$bktitle</title>");
	$markup->markup("</head>\n<body>\n");
	
	$self->markup_children($markup, $self->content());

	$markup->markup("</body>\n</html>\n");

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package GENERIC_CHAPTER;
    @ISA = ('GENERIC_TITLED');

    sub new {
	return bless {};
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($child) = $self->content();
	my($chapnum) = $main::PI_CHAPTER_NUMBER || "1";
	my($nexttitle, $prevtitle, $uptitle);
	my($nexte, $preve, $upe);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$uptitle   = "Top";

	$prevtitle = $self->prev_node(('PREFACE', 'CHAPTER', 'APPENDIX', 'SECT1'));
	$nexttitle = $self->next_node(('PREFACE', 'CHAPTER', 'APPENDIX', 'SECT1'));

	if ($child && ($child->{'TAG'} eq 'TITLE')) {
	    my($title) = $child->markup();
	    
	    # Handle <?lb> in title
	    $title =~ s/<\001lb>//g;

	    $markup->markup("\@node $title, $nexttitle, $prevtitle, $uptitle");

	    if ($main::PI_NUMBERED_SECTIONS) {
		$markup->markup("\@chapter " . $self->{'CHP-NUM'} 
				. ". $title");
	    } else {
		$markup->markup("\@chapter $title");
	    }

	    $child = $child->next();
	} else {
	    $markup->markup("\@node ?none?, $nexttitle, $prevtitle, $uptitle");
	    $markup->markup("% Chapter $chapnum: no title");
	}

	$self->markup_children($markup, $child);
	$markup->markup($self->markup_end());

	$inmarkup ? $markup : $markup->output();
    }

    sub menu {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($elem) = $self->content();
	my($telem, $title);

	$markup = new MARKUP_OBJECT if !$inmarkup;

	if (!$self->{'BUILT_MENU'}) {
	    $markup->markup("\@menu");
	    while ($elem && $elem ne $GENERAL_TAG::TAG_NULL) {
		if ($elem->{'TAG'} eq 'SECT1') {
		    $telem = $elem->content();
		    $title = $telem->markup();
		    $title =~ s/<\001lb>//g;
		    $markup->markup("* ${title}::\n");
		}
		$elem = $elem->next();
	    }
	    $markup->markup("\@end menu");
	    $self->{'BUILT_MENU'} = 1;
	}

	$inmarkup ? $markup : $markup->output();
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
	local($_) = $self->{'DATA'};

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	s/@/@@/g;
	s/\{/@\{/g;
	s/\}/@\}/g;

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
    package SYSTEMITEM;
    @ISA = ('GENERIC_INLINE');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($tag_start, $tag_end);
	my($type);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$tag_start = $self->markup_start();
	$tag_end = $self->markup_end();

	$type = $self->{'ATTR'}->{'ROLE'} || $self->{'ATTR'}->{'CLASS'};

	if (($type ne "") && ($tag_start =~ /\\\*\[.*\]/)) {
	    $type = uc($type);
	    chop($tag_start);
	    chop($tag_end);
	    $tag_start .= "-$type]";
	    $tag_end .= "-$type]";
	}

	$markup->markup($tag_start);
	$self->markup_children($markup, $self->content());
	$markup->markup($tag_end);

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

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$markup->markup("\\c\n") if $markup->{'OBEY_SPACES'};
	$markup->markup($self->markup_start() 
		      . " \"" . $child->markup() . "\" "
		      . "\\&\\c");

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
	my($type, $elements, $cols, $rows, $thiscol, $count, $table);

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

	if ($type eq "HORIZ" || $type eq "VERT") {
	    $elements = $#STACK + 1;
	    $cols     = $self->{'ATTR'}->{'COLUMNS'} || 1;
	    $rows     = int(($elements+($cols - 1)) / $cols);

	    $markup->markup(".TS");
	    $table .= "l " x $cols;
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
	my($xrefnum, $xrefwarning);
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

	    if ($xreftag =~ /PREFACE/i
		|| $xreftag =~ /CHAPTER/i
		|| $xreftag =~ /APPENDIX/i
		|| $xreftag =~ /SECT1/i) {
		$markup->markup("\@xref\{$xreftxt\}");
	    } else {
		$markup->text($xreftxt);
	    }
	} else {
	    &main::WARNING("Warning: xref ID \"$linkto\" is unknown.\n");

	    $markup->text("**UNKNOWN XREF**");
	}

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
	    $last = $last->{'NEXT'} while $last->{'NEXT'};

	    $terms .= $child->markup() . " ";
	    $child = $child->next();
	}
	chop($terms);		# get rid of the trailing blank

	$markup->markup($self->markup_start() . " $terms");
	$markup->markup($self->markup_end());
	for ($block = $blocklist; $block; $block = $blocklist->next()) {
	    $block->markup($markup);
	}
	$self->markup_children($markup,$child);

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package LISTITEM;
    @ISA = ('GENERIC_INLINE');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	if ($self->{'PARENT'}->{'TAG'} ne 'VARLISTENTRY') {
	    $markup->markup($self->markup_start());
	    $self->markup_children($markup, $self->content());
	    $markup->markup($self->markup_end());
	} else {
	    $self->markup_children($markup, $self->content());
	}

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

	&main::WARNING("$0: Cannot handle <GRAPHIC> tag!\n");

	$markup->markup("[ A GRAPHIC GOES HERE ]");

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

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	$self->markup_children($markup, $self->content());
	
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
	my(@markup) = ('XE1', 'XE2', 'XE3', 'XSE', 'XSA');
	my($count);
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

	if (($key = $self->{'ATTR'}->{'SPANEND'}) ne "") {
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

	    @terms = ($main::TAGID->{$key}->{'PRIMARY'},
		      $main::TAGID->{$key}->{'SECONDARY'},
		      $main::TAGID->{$key}->{'TERTIARY'},
		      $main::TAGID->{$key}->{'SEE'},
		      $main::TAGID->{$key}->{'SEEALSO'});

	    @sorts = ($main::TAGID->{$key}->{'PRIMARY-SORTAS'},
		      $main::TAGID->{$key}->{'SECONDARY-SORTAS'},
		      $main::TAGID->{$key}->{'TERTIARY-SORTAS'},
		      $main::TAGID->{$key}->{'SEE-SORTAS'},
		      $main::TAGID->{$key}->{'SEEALSO-SORTAS'});
	}

	if ($self->{'ATTR'}->{'SPANBEG'} =~ /^y/i) {
	    $markup->markup($self->markup_start() . " $significance S");
	} elsif ($self->{'ATTR'}->{'SPANEND'} ne "") {
	    $markup->markup($self->markup_start() . " $significance E");
	} else {
	    $markup->markup($self->markup_start() . " $significance");
	}

	for ($count = 0; $count < 5; $count++) {
	    if ($terms[$count] ne "") {
		$_ = ".$markup[$count] \"$terms[$count]\"";
		$_ .= " \"$sorts[$count]\"" if $sorts[$count];
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
	    if ($type !~ /^cprog$/i);

	open (F, $url) 
	    || &main::WARNING("Warning: cannot open URL in ULINK: $url\n");

	while (<F>) {
	    $markup->markup($_);
	}
	close (F);

	$inmarkup ? $markup : $markup->output();
    }
}

######################################################################
# TABLES
######################################################################

$main::TABLE_ABOVE_RULE_SPACE = "2p";
$main::TABLE_BELOW_RULE_SPACE = "2p";
$main::TABLE_BETWEEN_PARA_SPACE = ".25v";

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

	$markup->markup('.TE') if $self->{'TABLE_STARTED'};
	&FootnoteSpec::output_footnotes($self);
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

	$markup->markup('.TE') if $self->{'TABLE_STARTED'};
	&FootnoteSpec::output_footnotes($self);
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

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	if (!$table->{'TABLE_STARTED'}) {
	    $markup->markup($self->markup_start());
	    $markup->markup("$tgroup->{'TABLE_OPTIONS'} ;\n")
		if $tgroup->{'TABLE_OPTIONS'} ne "";
	}
	$table->{'TABLE_STARTED'} = 1;

	$self->markup_children($markup, $self->content());

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

	$rowsepspec = "";

	# Loop through all of the entries: calculate the attributes for
	# each particular entry.  The calculated attributes may include
	# several levels of defaulting.  These calculated attributes are
	# stored back into each entry so that they are available immediately
	# when the actual data for the row is being output.

	$child = $self->content();
	$column = 1;
	while ($child) {
	    foreach $_ ('align', 'char', 'charoff', 'colname', 'colsep', 
			'morerows', 'namest', 'nameend', 'rotate', 'rowsep',
			'spanname', 'valign', 'role') {
		my($attr) = uc($_);
		eval "\$$_ = \$child->{'ATTR'}->{$attr}";
	    }

	    $self->output_index_terms($child, $markup);

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

	    if ($namest || $nameend) {
		$colst = $ColSpec::COL{$namest};
		$colend = $ColSpec::COL{$nameend};
	    } else {
		$colst = $column;
		$colend = $column;
	    }

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

	    if ($rowsep >= 2) {
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
		    && !$self->{'NEXT'} 
) {
#		    && $self->{'PARENT'}->{'TAG'} eq 'THEAD'
#		    && $self->{'PARENT'}->{'NEXT'}->{'TAG'} eq 'TBODY') {
		    $rowsepspec .= $main::TABLE_DEFAULT_ROW_SEP;
		} else {
		    $rowsepspec .= " ";
		}

#		if (($lookahead ne "ROW") 
#		    && ($rowsep eq "")
#		    && $main::TABLE_DEFAULT_ROW_SEP) {
#		    $rowsepspec .= $main::TABLE_DEFAULT_ROW_SEP;
#		} else {
#		    $rowsepspec .= " ";
#		}
	    }

	    &main::WARNING("Warning: span out of place in table.\n")
		if ($colst != $column);

	    $colspec .= &align($align, $ColSpec::COLWIDTH{$column});
	    for ($count = $colst+1; $count <= $colend; $count++) {
		$colspec .= "s ";
	    }

	    if ($colend < $tgroup->{'ATTR'}->{'COLS'}) {
		$colspec .= "$main::TABLE_SINGLE_VERT_RULE " if $colsep == 1;
		$colspec .= "$main::TABLE_DOUBLE_VERT_RULE " if $colsep == 2;

		# Implement ORA standards, table columns are seperated by
		# rules.  (You you can override this with a rowsep attr.)
		$colspec .= "$main::TABLE_DEFAULT_COL_SEP "
		    if ($colsep eq "") && $main::TABLE_DEFAULT_COL_SEP;
	    }

	    foreach $_ ('align', 'char', 'charoff', 'colname', 'colsep', 
			'morerows', 'namest', 'nameend', 'rotate', 'rowsep',
			'spanname', 'valign', 'role') {
		my($attr) = uc($_);
		eval "\$child->{'ATTR'}->{$attr} = \$$_";
	    }

	    $column = $colend+1;
	    $child = $child->next();
	}

	if ($column-1 != $tgroup->{'ATTR'}->{'COLS'}) {
	    my($msg) = sprintf("Expected %d but got %d.",
			       $tgroup->{'ATTR'}->{'COLS'},
			       $column-1);
	    
	    $self->table_warning("wrong number of columns", $msg);
	}

	if ($colspec ne $ColSpec::curcolspec) {
	    $markup->markup(".T&") if $ColSpec::curcolspec ne "";
	    $markup->markup("$colspec.\n");
	    $ColSpec::curcolspec = $colspec;
	}

	$child = $self->content();
	$column = 1;
	$row = "";
	while ($child) {
	    my($entry) = undef;
	    my($estart, $eend) = ("", "");

	    foreach $_ ('align', 'char', 'charoff', 'colname', 'colsep', 
			'morerows', 'namest', 'nameend', 'rotate', 'rowsep',
			'spanname', 'valign', 'role') {
		my($attr) = uc($_);
		eval "\$$_ = \$child->{'ATTR'}->{$attr}";
	    }

	    if ($align eq "CHAR") {
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

	    # If the entry is a paragraph and the alignment isnt fully
	    # justified, insert a .na to make the paragraph not be filled
	    # flush right...
	    if ($self->block_elements($child)) {
		$estart = "T{\n";
		$eend = "\nT}";
 
		if ($align ne 'JUSTIFY') {
		    $estart = "T{\n.na\n";
		}
	    }

	    # Handle special markup for table headers
	    if ($self->{'PARENT'}->{'TAG'} eq "THEAD") {
		$role = "";	# roles dont change headers
		$estart .= "\\*[EM-TBLHD]";
		$eend = "\\*[/EM-TBLHD]$eend";
	    }

	    if ($role ne "") {
		$role =~ tr/A-Z/a-z/;
		if (defined($main::COLROLE_START{$role})) {
		    $estart .= $main::COLROLE_START{$role};
		    $eend .= $main::COLROLE_END{$role};
		} else {
		    &main::WARNING("Undefined column role ignored: $role\n");
		}
	    }

	    $markup->markup("\t") if $column != 1;
	    $markup->markup("$estart");

	    $markup->markup($child->markup() || "\\&");

	    $markup->markup("$eend");

#	    $row .= "\t" if $column != 1;
#
#	    $row = "\\&" if ($row eq "") 
#		&& (($_ eq "-") || ($_ eq "=") || /^\./);
#
#	    if ($self->{'PARENT'}->{'TAG'} eq "THEAD") {
#		$role = "";	# roles dont change headers
#		$_ = "\\*[EM-TBLHD]$_\\*[/EM-TBLHD]";
#	    }
#
#	    if ($role ne "") {
#		$role =~ tr/A-Z/a-z/;
#		if (defined($main::COLROLE_START{$role})) {
#		    $_ = $main::COLROLE_START{$role} . $_ . $main::COLROLE_END{$role};
#		} else {
#		    &main::WARNING("Undefined column role ignored: $role\n");
#		}
#	    }
#
#	    $row .= $_;

	    $column++;
	    $child = $child->next();
	}

	$markup->markup("\n");

	&hrule($markup, 'TF', $self)
	    if ($TGROUP::row_count == 1 && $TGROUP::needs_a_topframe);

	{
	    my($index) = '';

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
	} elsif ($rowsepspec !~ /^\ +$/) { # rule across at least one column
	    while ($rowsepspec =~ /^(.)(.*)$/) {
		if ($1 eq "=") {
		    $markup->markup($main::TABLE_DOUBLE_HORIZ_RULE);
		} elsif ($1 eq "_") {
		    $markup->markup($main::TABLE_SINGLE_HORIZ_RULE);
		}
		$markup->markup("\t") if $2 ne "";
		$rowsepspec = $2;
	    }
	    $markup->markup("\n");
	}


	$inmarkup ? $markup : $markup->output();
    }

    sub align {
	local($atype, $width) = @_;
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

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	if ($parent->{'TAG'} ne "ENTRY") {
	    $markup->markup($self->markup_start());
	    $self->markup_children($markup, $self->content());
	    $markup->markup($self->markup_end());
	} else {
	    $markup->markup("\\*=");
	    $markup->markup($self->markup_end());
	}

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package FOOTNOTE;
    @ISA = ("GENERIC_TAG");

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($parent) = $self->{'PARENT'};
	my($data);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup($markup);

	if ($parent->{'TAG'} ne "ENTRY") {
	    $markup->markup("\\**");
	    $markup->markup($self->markup_start());
	}

	$self->markup_children($markup, $self->content());

	if ($parent->{'TAG'} ne "ENTRY") {
	    $markup->markup($self->markup_end());
	} else {
	    $data = "NORM:THIS FOOTNOTE DOESNT WORK";
	    push(@FootnoteSpec::FN, $data);
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
			if $innerchild->{'TAG'} eq "REFMISCINFO";
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

1;

