# -*- Perl -*-
# Classes for memototxt
#
# $Id: memototxt.cls,v 1.1.1.1 2002-07-15 21:33:31 root Exp $
#
#
##############################################################################

if ($VERBOSE) {
    my($VERSION) = '$Id: memototxt.cls,v 1.1.1.1 2002-07-15 21:33:31 root Exp $'; # '
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
	local($_) = $text;

	return if $_ eq "";

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

{
    package GENERIC_TAG;

    # TAG = "tag"           name of this tag
    # CONTENT->tag          pointer to first subelement
    # CONTENT_LAST->tag     pointer to last subelement
    # NEXT->tag             pointer to next element at this level
    # PREV->tag             pointer to prev element at this level
    # PARENT->tag           pointer to parent element
    # ATTR->{}              attributes of this tag
    # NEXT_ELEMENT->tag     pointer to next element of this type
    # PREV_ELEMENT->tag     pointer to prev element of this type

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
	my($s, $child, $tagid, $sep);

	return if $self->{'ATTR'}->{'ID'} eq "";

	$chap = $main::PI_CHAPTER_NUMBER || "1";

	$sect = $main::TAGLEVEL{'SECT1'};
	foreach $s ('SECT2', 'SECT3', 'SECT4', 'SECT5') {
	    $sect .= "." . $main::TAGLEVEL{$s} if defined($main::TAGLEVEL{$s});
	}

	$refsect = $main::TAGLEVEL{'REFSECT1'};
	foreach $s ('REFSECT2', 'REFSECT3', 'REFSECT4', 'REFSECT5') {
	    $refsect .= "." . $main::TAGLEVEL{$s} if defined($main::TAGLEVEL{$s});
	}

	$count = $main::TAGLEVEL{$self->{'TAG'}};

	$title = "";
	$child = $self->{'CONTENT'};
	if ($child && $child->{'TAG'} eq "TITLE") {
	    $title = $child->markup();
	}

	$sep = $main::TAGIDSEP;

	$tagid = sprintf ("%s$sep%s$sep%s$sep%s$sep%s$sep%s",
			  $self->{'TAG'},
			  $chap, $sect, $refsect, $count,
			  $title);

	$main::TAGID_CHANGE = 1 
	    if $main::TAGID{$self->{'ATTR'}->{'ID'}} ne $tagid;

	$main::TAGID{$self->{'ATTR'}->{'ID'}} = $tagid;
    }

    sub next {
	my($self) = @_;
	$self->{'NEXT'};
    }

    sub add_child {
	my($self, $ref) = @_;
	my($prevelem);

	$ref->{'PARENT'} = $self;
	$ref->{'NEXT'} = 0;

	if ($self->{'CONTENT'}) {
	    $ref->{'PREV'} = $self->{'CONTENT_LAST'};
	    $self->{'CONTENT_LAST'}->{'NEXT'} = $ref;
	    $self->{'CONTENT_LAST'} = $ref;
	} else {
	    $self->{'CONTENT'} = $ref;
	    $self->{'CONTENT_LAST'} = $ref;
	    $ref->{'PREV'} = 0;
	}

	$prevelem = $LAST{$ref->{'TAG'}};
	$prevelem->{'NEXT_ELEMENT'} = $ref if ($prevelem);
	$ref->{'PREV_ELEMENT'} = $prevelem;
	$LAST{$ref->{'TAG'}} = $ref;
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
	my($self) = @_;
	my($child, $nextchild);
	my($block, $last) = (0, 0);

	$nextchild = $self->{'CONTENT'};
	while ($nextchild) {
	    $child = $nextchild;
	    $nextchild = $nextchild->{'NEXT'};

	    if (!&main::isa($child, 'GENERIC_INLINE')) {
#		print STDERR "Unlinking: $child->{TAG}\n";
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
#	    } else {
#		print STDERR "Leaving: $child->{TAG}\n";
#	    }
	}

#	$child = $self->{'CONTENT'};
#	while ($child) {
#	    print STDERR "Left: $child->{TAG}\n";
#	    $child= $child->{'NEXT'};
#	}

	$block;
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !defined($markup);

	&main::WARNING("No markup method for $self->{TAG}.\n");
	$self->markup_children($markup, $self->{'CONTENT'});
	
	$inmarkup ? $markup : $markup->output();
    }
	
#    sub print {
#	my($self) = @_;
#	&main::MARKUP($self->text());
#    }

    sub markup_children {
	my($self, $markup, $child) = @_;

	while ($child) {
	    $child->markup($markup);
	    $child = $child->{'NEXT'};
p	}

	$markup;
    }	
}

{
    package GENERIC_INLINE;
    @ISA = ('GENERIC_TAG');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !defined($markup);

	$markup->text($self->markup_start());
	$self->markup_children($markup, $self->{'CONTENT'});
	$markup->text($self->markup_end());

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

	$self->markup_children($markup, $self->{'CONTENT'});

	$inmarkup ? $markup : undef;
    }
}

{
    package GENERIC_HEADER;
    @ISA = ('GENERIC_TAG');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($child) = $self->{'CONTENT'};
	my($header, $data);

	$markup = new MARKUP_OBJECT if !defined($markup);

	$header = $self->{'TAG'};
	$data   = $child->markup();

	eval "\$MEMO::$header = \$data";

	$inmarkup ? $markup : $data;
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

	if (/^\s*(\S+)\s+(.*)\s*$/) {
	    $pi = $1;
	    $rest = $2;
	} elsif (/^\s*(\S+)\s*$/) {
	    $pi = $1;
	    $rest = "";
	} else {
	    die "Unparsable PI: $_\n";
	}

	$pi =~ s/-/_/g;

	($pi, $rest);
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($pi, $rest, $sub);
	local($_);

	$markup = new MARKUP_OBJECT if !defined($markup);

	($pi, $rest) = $self->parse_pi();

	$sub = "\&main::PI_${pi}(\$self, \$markup, \$rest)";
	$_ = eval $sub;

	if ($@) {
	    $_ = $@;
	    s/called at.*$//;

	    warn "Attempt to process <?$pi $rest> failed:\n\t$_\n";
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

	$markup = new MARKUP_OBJECT if !defined($markup);

	s/\\/\\e/g;		# "\" -> "\e" for troff

	$markup->text($_);

	$inmarkup ? $markup : $markup->output();
    }
}


{
    package EMPHASIS;
    @ISA = ('GENERIC_TAG');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($tag_start, $tag_end);
	my($data);

	$markup = new MARKUP_OBJECT if !defined($markup);

	$tag_start = $self->markup_start();
	$tag_end = $self->markup_end();

	$data = ($self->{'CONTENT'})->markup();

	$markup->text("$tag_start$data$tag_end");

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package MEMO;
    @ISA = ('GENERIC_NOP');

    sub new {
	return bless { };
    }
}

{
    package HEADERS;
    @ISA = ('GENERIC_NOP');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);

	$MEMO::TO = "";
	$MEMO::CC = "";
	$MEMO::DATE = "";
	$MEMO::FROM = "";
	$MEMO::RE = "";
	$MEMO::SALUTATION = "";
	$MEMO::CLOSING = "";
	$MEMO::SIGNED = "";

	$markup = new MARKUP_OBJECT if !$inmarkup;

	$self->markup_children($markup, $self->{'CONTENT'});

	$MEMO::FROM       = "U.N. Derling" if $MEMO::FROM eq "";
	$MEMO::SALUTATION = $MEMO::TO      if $MEMO::SALUTATION eq "";

	if ($MEMO::CLOSING eq "") {
	    $MEMO::CLOSING = "Sincerely,";
	    $MEMO::SIGNED = $MEMO::FROM;
	} else {
	    if ($MEMO::CLOSING =~ /^(.*)\/(.*)$/) {
		$MEMO::CLOSING = $1;
		$MEMO::SIGNED = $2;
	    } else {
		$MEMO::SIGNED = $MEMO::FROM;
	    }
	}
	
	if ($MEMO::DATE eq "") {
	    chop($MEMO::DATE = `date`);
	}

	$markup->text("To  : $MEMO::TO\n");
	$markup->text("CC  : $MEMO::CC\n") if $MEMO::CC ne "";
	$markup->text("From: $MEMO::FROM\n");
	$markup->text("Date: $MEMO::DATE\n");
	$markup->text("Re  : $MEMO::RE\n");

	$inmarkup ? $markup : undef;
    }
}

{
    package TO;
    @ISA = ('GENERIC_HEADER');

    sub new {
	return bless { };
    }
}

{
    package CC;
    @ISA = ('GENERIC_HEADER');

    sub new {
	return bless { };
    }
}

{
    package FROM;
    @ISA = ('GENERIC_HEADER');

    sub new {
	return bless { };
    }
}

{
    package RE;
    @ISA = ('GENERIC_HEADER');

    sub new {
	return bless { };
    }
}

{
    package DATE;
    @ISA = ('GENERIC_HEADER');

    sub new {
	return bless { };
    }
}


{
    package SALUTATION;
    @ISA = ('GENERIC_HEADER');

    sub new {
	return bless { };
    }
}

{
    package CLOSING;
    @ISA = ('GENERIC_HEADER');

    sub new {
	return bless { };
    }
}

{
    package PARA;
    @ISA = ('GENERIC_HEADER');

    sub new {
	return bless { };
    }


    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($paramarkup) = new MARKUP_OBJECT;
	my(@words, $word);
	local($_);

	$markup = new MARKUP_OBJECT if !$inmarkup;

	$markup->text("\n");

	$self->markup_children($paramarkup, $self->{'CONTENT'});
	$_ = $paramarkup->output();

	s/\n/ /g;		# No newlines
	@words = split(/\s+/, $_);
	$_ = "";
	while (@words) {
	    $word = shift(@words);
	    if (length($_) + length($word) > 72) {
		$markup->text("$_\n");
		$_ = $word;
	    } else {
		$_ .= " " if $_ ne "";
		$_ .= $word;
	    }
	}
	$markup->text("$_\n");

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package BODY;
    @ISA = ('GENERIC_HEADER');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($tag_start);

	$markup = new MARKUP_OBJECT if !defined($markup);

	$tag_start = $self->markup_start();

	$markup->text("\n$tag_start $MEMO::SALUTATION:\n");
	$self->markup_children($markup, $self->{'CONTENT'});
	$markup->text("\n");
	$markup->text(" " x 40 . $MEMO::CLOSING . "\n");
	$markup->text("\n" x 6 . " " x 40 . $MEMO::SIGNED . "\n")
	    if $MEMO::SIGNED ne "";

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package LIST;
    @ISA = ('GENERIC_HEADER');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !defined($markup);

	$markup->text("\n");
	$ITEM_COUNT = 0;

	$self->markup_children($markup, $self->{'CONTENT'});

	$inmarkup ? $markup : $markup->output();
    }
}	

{
    package ITEM;
    @ISA = ('GENERIC_TAG');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($data, $type);

	$markup = new MARKUP_OBJECT if !defined($markup);

	$type = $self->{'PARENT'}->{'ATTR'}->{'TYPE'} || 'BULLET';
	$data = ($self->{'CONTENT'})->markup();

	$LIST::ITEM_COUNT++;

	if ($type eq 'ARABIC') {
	    $markup->text(" " . sprintf("%2d", $LIST::ITEM_COUNT) .
			  ". " . "$data\n");
	} elsif ($type eq 'ROMAN') {
	    $markup->text(" " . sprintf("%s", $self->roman_numeral($LIST::ITEM_COUNT)) .
			  ". " . "$data\n");
	} elsif ($type eq 'ALPHA') {
	    $markup->text(" " . sprintf("%c", $LIST::ITEM_COUNT + 96) .
			  " " . "$data\n");
	} else {
	    $markup->text("  * $data\n");
	}

	$inmarkup ? $markup : $markup->output();
    }

    sub roman_numeral {    
	# works for 1-99, but not 100+
	my($self,$arabic) = @_;
	my($tens, $ones);

	@roman[0] = "";
	@roman[1] = "i";
	@roman[2] = "ii";
	@roman[3] = "iii";
	@roman[4] = "iv";
	@roman[5] = "v";
	@roman[6] = "vi";
	@roman[7] = "vii";
	@roman[8] = "viii";
	@roman[9] = "ix";
	@roman[10] = "x";
	@roman[20] = "xx";
	@roman[30] = "xxx";
	@roman[40] = "xl";
	@roman[50] = "l";
	@roman[60] = "lx";
	@roman[70] = "lxx";
	@roman[80] = "lxxx";
	@roman[90] = "xc";

	$tens = int($arabic / 10);
	$ones = $arabic % 10;

	return "$roman[$tens]$roman[$ones]";
    }
}

1;


