# -*- Perl -*-
#
# Handle each type of ESIS output from (n)sgmls.
#
# $Id: sgmls3l.pl,v 1.1.1.1 2002-07-15 21:33:31 root Exp $
#
#
######################################################################

package sgmlsESIS;

# These variables form the most rudimentary beginnings of a proper
# entity manager.  File references are associated with external
# entity names.
#
%ExternalEntity = ();
%InternalEntity = ();
$fileReference = "";

sub init {
    %main::ID = ();
    %main::LAST = ();
    @main::TAGSTACK = ();
    $main::PCDATA_SIZE = 0;
    $main::MAX_PCDATA_SIZE = 1024000;
    %main::Attributes = ();
    $sgmlsParse::DISPATCH_COUNT = 0;
    $sgmlsParse::IGNORE_ALL_MARKUP = 0;
}

sub new {
    my($class) = @_;

    if (&main::ispackage($class)) {
	return eval "new $class";
    } else {
	&main::WARNING("Document contains <$class>, but that tag is not recognized by gmat.\n");
	return new GENERIC_TAG;
    }
}

sub perl_conforming {
    $main::SGMLS_OK = 1;
}

sub perl_attribute {
    my($name, $type, $value) = @_;

    $main::Attributes{$name} = $value;
}

sub perl_location {
    my($lineno, $filename) = @_;

    $main::SGMLS_LINE_NO = $lineno;
    $main::SGMLS_FILENAME = $filename if defined($filename);

    #nop
}

sub perl_startElement {
    local($_) = @_;
    my($ref, $hash);

    $ref = &new($_);

    $ref->{'TAG'} = $_;
    $ref->{'ATTR'} = {};
    $hash = $ref->{'ATTR'};
    %$hash = %main::Attributes;

    %main::Attributes = ();

    $ref->init();

    if (@main::TAGSTACK) {
	$main::TAGSTACK[$#TAGSTACK]->add_child($ref);
    } else {
	$main::ROOT = $ref;
	$GENERIC_TAG::LAST_ELEM = $ref;
    }

    push (@main::TAGSTACK, $ref);

    # TAGLEVEL counts the number of each kind of tag that we've seen.
    # For example, TAGLEVEL{SECT1} is incremented each time a SECT1 is
    # seen.
    #
    if (! defined($main::TAGLEVEL{$ref->{'TAG'}})) {
	$main::TAGLEVEL{$ref->{'TAG'}} = 1;
    } else {
	$main::TAGLEVEL{$ref->{'TAG'}}++;
    }

    # If we see a SECTn, all SECTm (n>m) have to be reset (in other words
    # the sequence is 1, 1.1, 1.2, 2, 2.1, 2.2 and not 1, 1.1, 1.2, 2.2, ...
    #
    if ($ref->{'TAG'} =~ /^SECT(\d)/) {
	$count = $1;
	for ($count = $count+1; $count <= 5; $count++) {
	    delete($main::TAGLEVEL{"SECT$count"});
	}
    }

    # The same is true of REFSECTn
    #
    if ($ref->{'TAG'} =~ /^REFSECT(\d)/) {
	$count = $1;
	for ($count = $count+1; $count <= 5; $count++) {
	    delete($main::TAGLEVEL{"REFSECT$count"});
	}
    }

    # Everything inside a table is cleared each time a table starts
    #
    if ($ref->{'TAG'} eq 'TABLE' || $ref->{'TAG'} eq 'INFORMALTABLE') {
	delete($main::TAGLEVEL{'COLSPEC'});
	delete($main::TAGLEVEL{'SPANSPEC'});
	delete($main::TAGLEVEL{'TGROUP'});
	delete($main::TAGLEVEL{'THEAD'});
	delete($main::TAGLEVEL{'TBODY'});
	delete($main::TAGLEVEL{'ROW'});
	delete($main::TAGLEVEL{'ENTRY'});
    }

    # Everything is cleared when a new preface, chapter, or appendix starts
    #
    %main::TAGLEVEL = () if ($ref->{'TAG'} eq "CHAPTER"
			     || $ref->{'TAG'} eq "APPENDIX"
			     || $ref->{'TAG'} eq "PREFACE");

    # Copy the "interesting" fields into the TAGLEVEL structure of the
    # current element:


    $ref->{'TAGLEVEL'} = { };
    foreach $_ ($ref->{'TAG'},
		'SECT1', 'SECT2', 'SECT3', 'SECT4', 'SECT5',
		'REFSECT1', 'REFSECT2', 'REFSECT3', 'REFSECT4', 
		'FIGURE', 'TABLE', 'EXAMPLE') {
	next if !defined($main::TAGLEVEL{$_});
	$ref->{'TAGLEVEL'}->{$_} = $main::TAGLEVEL{$_};
    }
}

sub perl_endElement {
    local($_) = @_;
    my($ref) = pop(@main::TAGSTACK);
    my($count);

    die "Corrupt tag stack.\n"
	if $ref->{'TAG'} ne $_;

    $ref->{'CONTENT_LAST'}->{'NEXT'} = 0
	if $ref->{'CONTENT_LAST'};
}

sub perl_pi {
    my($s) = @_;
    my($ref) = &new('PI');

    $ref->{'TAG'} = 'PI';
    $ref->{'PI'} = $s;
    $main::TAGSTACK[$#TAGSTACK]->add_child($ref) if (@main::TAGSTACK);
}

sub perl_sysid {
    my($sysid) = @_;

#    print STDERR "Perl syid: $sysid\n";
    
    #nop
}

sub perl_pubid {
    my($pubid) = @_;

    #nop
}

sub perl_file {
    my($file) = @_;

    $fileReference = $file;
    $fileReference = $1 if $file =~ /^<[^<>]*>(.*)$/;
    
    #nop
}

sub perl_defineNotation {
    my($name) = @_;

    #nop
}

sub perl_defineExternalEntity {
    my($ename, $y, $z) = @_;

    $ExternalEntity{$ename} = $fileReference;

    #nop
}

sub perl_defineInternalEntity {
    my($ename, $type, $value) = @_;

    $InternalEntity{$ename} = $value;

    #nop
}

sub perl_data {
    local($_) = @_;
    my($ref) = &new('PCDATA');

    $_ = sprintf("%c%s", oct($1), $') if /^\\(\d\d\d)/;
    while (/([^\\])\\(\d\d\d)/) {
	$_ = sprintf("%s%s%c%s", $`, $1, oct($2), $');
    }

    $main::PCDATA_SIZE += length($_);

    $ref->{'TAG'} = 'PCDATA';
    $ref->{'DATA'} = $_;
    $main::TAGSTACK[$#TAGSTACK]->add_child($ref) if (@main::TAGSTACK);
}

sub perl_sdata {
    my($s) = @_;
    my($ref) = &new('SDATA');

    $ref->{'TAG'} = 'SDATA';
    $ref->{'SDATA'} = $s;
    $main::TAGSTACK[$#TAGSTACK]->add_child($ref) if (@main::TAGSTACK);
}

1;

