# -*- Perl -*-
# Classes for dbtoltx
#
# $Id: dbtoltx.cls,v 1.1.1.1 2002-07-15 21:33:31 root Exp $
#
#
##############################################################################

if ($VERBOSE) {
    my($VERSION) = '$Id: dbtoltx.cls,v 1.1.1.1 2002-07-15 21:33:31 root Exp $'; # '
    my($REQNAME) = (split(/\s+/, $VERSION))[1];
    my($vers) = (split(/\s+/, $VERSION))[2];

    print STDERR "$REQNAME version $vers.\n";
}

##########################################################################

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
	my($place, @lines);

	return if $_ eq "";

	# Can't just match a regexp in here because \n's 
	# cause all sorts of prob.

	# break incoming text into lines
	while (($place = index($_, "\n")) >= 0) {
	    push (@lines, substr($_, 0, $place+1));
	    $_ = substr($_, $place+1);
	}
	push (@lines, $_) if $_ ne"";

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
	local($_) = $text;
	my($place, @lines);

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

	    if ((/^\s*\%/ 
		 || /^\s*\\begin\s*\{/
		 || /^\s*\\end\s*\{/) 
		&& $self->{'MARKUP_COL'} != 0) {
		$self->{'OUTPUT'} .= "\n";
		$self->{'MARKUP_COL'} = 0;
	    }
	    
	    $self->{'OUTPUT'} .= $_;
	    $self->{'MARKUP_COL'} += length($_);
	    $self->{'MARKUP_COL'} = 0 if substr($_, length($_)-1, 1) eq "\n";
	}

	if ((/^\s*\%/ || /^\s*\\begin\s*\{/) 
	    && $self->{'MARKUP_COL'} != 0) {
	    $self->{'OUTPUT'} .= "\n";
	    $self->{'MARKUP_COL'} = 0;
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

sub gen_section {
    my($tag) = @_;
    &gen_empty_subclass('GENERIC_SECTION', $tag);
}

sub gen_inline {
    my($tag) = @_;
    &gen_empty_subclass('GENERIC_INLINE', $tag);
}

sub gen_wrapper {
    my($tag) = @_;
    &gen_empty_subclass('GENERIC_WRAPPER', $tag);
}

sub gen_obeyspaces {
    my($tag) = @_;
    &gen_empty_subclass('GENERIC_OBEYSPACES', $tag);
}

##############################################################################

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
	    if ($ref =~ /\{\s*$/) {
		$ref = "\}";
	    } elsif ($ref =~ /\[\s*$/) {
		$ref = "\]";
	    } elsif ($ref =~ /\|\s*$/) {
		$ref = "\|";
	    } elsif ($ref =~ /^\\begin\{(.*)\}\s*$/) {
		$ref = "\\end{$1}";
	    } elsif ($ref =~ /^\s*\%\s+begin(.*)$/) {
		$ref = "\% end$1";
	    } elsif ($ref =~ /^\\\S+\s*$/ || $ref eq "") {
		$ref = "";
	    } else {
		&main::WARNING("Can\'t form markup_end for $self->{TAG} ($ref)\n");
	    }
	}

	return $ref;
    }
	
    sub save_id {
	my($self) = @_;
	my($chap, $sect, $refsect, $count, $title);
	my($s, $child, $id, $tag);

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
	if ($child && $child->{'TAG'} eq "TITLE") {
	    $title = $child->markup();
	    $title = $1 if $title =~ /^\s*\{(.*)\}\s*$/;
	}

	$main::TAGID->{$id} = { } if !defined($main::TAGID->{$id});

	if ($tag ne 'INDEXTERM') {
	    $main::TAGID_CHANGE = 
		($main::TAGID_CHANGE 
		 || $main::TAGID->{$id}->{'TAG'} ne $tag
		 || $main::TAGID->{$id}->{'CHP-NUM'} ne $chap
		 || $main::TAGID->{$id}->{'SECT-NUM'} ne $sect
		 || $main::TAGID->{$id}->{'RSECT-NUM'} ne $refsect
		 || $main::TAGID->{$id}->{'ITEM-NUM'} ne $count
		 || $main::TAGID->{$id}->{'TITLE'} ne $title);
	}
	    
	$main::TAGID->{$id}->{'TAG'} = $tag;
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

	if ($child->{'PREV'} ne $TAG_NULL) {
	    $child->{'PREV'}->{'NEXT'} = $child->{'NEXT'};
	} else {
	    $child->{'PARENT'}->{'CONTENT'} = $child->{'NEXT'};
	}

	if ($child->{'NEXT'} ne $TAG_NULL) {
	    $child->{'NEXT'}->{'PREV'} = $child->{'PREV'};
	} else {
	    $child->{'PARENT'}->{'CONTENT_LAST'} = $child->{'PREV'};
	}

	$child->{'NEXT'} = $TAG_NULL;
	$child->{'PREV'} = $TAG_NULL;

	$child;
    }

    sub block_markup {
	my($self) = @_;
	my($child, $nextchild);
	my($block, $last) = (0, 0);

	$nextchild = $self->content();
	while ($nextchild && $nextchild ne $TAG_NULL) {
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
	    $self->save_id();
	}
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup();

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
}

{
    package GENERIC_OBEYSPACES;
    @ISA = ('GENERIC_WRAPPER');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup();

	$markup->{'OBEY_SPACES'}++;
	$markup->markup($self->markup_start());
	$self->markup_children($markup, $self->content());
	$markup->markup($self->markup_end());
	$markup->{'OBEY_SPACES'}--;

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package GENERIC_WRAPPER;
    @ISA = ('GENERIC_TAG');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup();

	$markup->markup($self->markup_start());
	$self->markup_children($markup, $self->content());
	$markup->markup($self->markup_end());

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
	$self->default_markup();

	$markup->markup($self->markup_start());
	$self->markup_children($markup, $self->content());
	$markup->markup($self->markup_end());

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

	    # Handle \n in title
	    $title =~ s/\n/ /g;

	    $markup->markup($self->markup_start() . $title);
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
	$self->default_markup();

	$self->markup_title($markup);
	$child = $child->next() if $child->{'TAG'} eq 'TITLE';
	$self->markup_children($markup, $child);
	$markup->markup($self->markup_end());

	$inmarkup ? $markup : $markup->output();
    }
}

{
    package GENERIC_WRAPPER;
    @ISA = ('GENERIC_TAG');

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup();

	$markup->markup($self->markup_start());
	$self->markup_children($markup, $self->content());
	$markup->markup($self->markup_end());

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
	$self->default_markup();

	s/\{/\\\{/g;		# { -> \{
	s/\}/\\\}/g;		# } -> \}
	s/\$/\\\$/g;		# $ -> \$
	s/\#/\\\#/g;		# # -> \#
	s/\&/\\\&/g;		# & -> \&

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
	$self->default_markup();

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

    %BAD_PI = ();

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
	    die "Unparsable PI: \"$_\"\n";
	}

	$pi =~ s/-/_/g;

	($pi, $rest);
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);
	my($pi, $rest, $sub);
	local($_, *stab);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup();

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

##############################################################################

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
	$self->default_markup();

	$tag_start = $self->markup_start();
	$tag_end = $self->markup_end();

	$type = $self->{'ATTR'}->{'ROLE'} || $self->{'ATTR'}->{'CLASS'};

	if (($type ne "") && ($tag_start =~ /\\.*\{$/)) {
	    $type = uc($type);
	    $tag_start .= "$type\}\{";
	}

	$markup->markup($tag_start);
	$self->markup_children($markup, $self->content());
	$markup->markup($tag_end);

	$inmarkup ? $markup : $markup->output();
    }
}

##############################################################################

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
	$self->default_markup();

	$markup->markup("\n");
	$self->markup_children($markup, $self->content());
	$markup->markup("\n");

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
	my($tagid, $pattern, $linkto);
	my($xreftag, $xrefchap, $xrefsect, $xrefrsct, $xrefitem, $xreftxt);
	my($xrefnum, $xrefwarning);
	my($sep, $sep_config, $field_re, $split_re);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup();

	$linkto = $linkend;
	$linkto = $endterm if $endterm ne "";	

	if (defined($main::TAGID->{$linkto})) {
 	    $tagid = $main::TAGID->{$linkto};
	    $main::TAGCOUNT{$linkto}++;

	    $xreftag = $main::TAGID->{$linkto}->{'TAG'};
	    $xrefchap = $main::TAGID->{$linkto}->{'CHP-NUM'};
	    $xrefsect = $main::TAGID->{$linkto}->{'SECT-NUM'};
	    $xrefrsct = $main::TAGID->{$linkto}->{'RSECT-NUM'};
	    $xrefitem = $main::TAGID->{$linkto}->{'ITEM-NUM'};
	    $xreftxt = $main::TAGID->{$linkto}->{'TITLE'};

	    $pattern = &main::cfg($main::bookfiles, 
				  $main::FILTERCFG, "xref_$xreftag");

	    if ($main::TAGCOUNT{"$linkto"} > 1) {
		my($spattern) = &main::cfg($main::bookfiles, 
				  $main::FILTERCFG, "xrefs_$xreftag");
		$pattern = $spattern if $spattern ne "";
	    }

	    &main::WARNING("Warning: no pattern for $xreftag\n")
		if !$pattern;

	    ($sep_config = "${xreftag}_xref_num_sep") =~ tr/A-Z/a-z/;
	    $sep = &main::cfg($main::bookfiles, 
			      $main::FILTERCFG, $sep_config) 
		|| &main::cfg($main::bookfiles, 
			      $main::FILTERCFG, 'xref_num_sep');

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
	    &main::WARNING("Warning: xref ID \"$linkto\" is unknown.\n")
		if !defined($main::TAGID->{$linkto});
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
    package LISTITEM;
    @ISA = ('GENERIC_TAG');

    sub new {
	return bless { };
    }

    sub markup {
	my($self, $markup) = @_;
	my($inmarkup) = defined($markup);

	$markup = new MARKUP_OBJECT if !$inmarkup;
	$self->default_markup();

	if ($self->{'PARENT'}->{'TAG'} eq 'VARLISTENTRY') {
	    # no tag markup for listitem here!
	    $self->markup_children($markup, $self->content());
	} else {
	    $markup->markup($self->markup_start());
	    $self->markup_children($markup, $self->content());
	    $markup->markup($self->markup_end());
	}

	$inmarkup ? $markup : $markup->output();
    }
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
	    
	    &main::WARNING("$0: Cannot handle table <GRAPHIC>!\n");
	} elsif ($format =~ /^eqn$/i) {
	    if (! -f $fileref) {
		$fileref = "./eqn/$fileref" if -f "./eqn/$fileref";
		$fileref = "./math/$fileref" if -f "./math/$fileref";
	    }
	    
	    if (! -f $fileref) {
		&main::WARNING("$0: Cannot find equation: $fileref!\n");
	    }
	    
	    &main::WARNING("$0: Cannot handle equation <GRAPHIC>!\n");
	} elsif ($format !~ /^eps$/i && $format !~ /^postscript$/i) {
	    &main::WARNING("$0: unrecognized format $format in <GRAPHIC> tag.\n");
	    $markup->markup($self->markup_start() . " $fileref");
	    $markup->markup($self->markup_end());
	} else {		# ASSUME ITS EPS!
	    my (@figdirs, @figfiles, $figdir, $figfile, $found);

	    $fileref = $1 if $fileref =~ /^(.*)\.gz$/;
	    $fileref = $1 if $fileref =~ /^(.*)\.Z$/;

	    @figdirs = ("./", "./figs");
	    push (@figdirs, ("./highfigs", "./hifigs")) if $main::opt_hires;
	    push (@figdirs, ("./lowfigs", "./lofigs"));

	    # If FILEREF="/path/figure", put "/path" in the figdirs list
	    # but just look for "figure".
	    #
	    if ($fileref =~ /^(.*)\/([^\/]+)$/) {
		unshift (@figdirs, $1);
		$fileref = $2;
	    }

	    @figfiles = ("$fileref", "$fileref.gz", "$fileref.Z",
			 "$fileref.eps", "$fileref.eps.gz", "$fileref.eps.Z");

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

	    $markup->markup("\\begin{center}");
	    $markup->markup("\\epsfbox{$fileref}");
	    $markup->markup("\\end{center}");
	}

	$inmarkup ? $markup : $markup->output();
    }
}

1;
