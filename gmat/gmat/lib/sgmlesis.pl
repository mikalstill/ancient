# -*- Perl -*-
#
# Handle each type of ESIS output from (n)sgmls.
#
# $Id: sgmlesis.pl,v 1.1.1.1 2002-07-15 21:33:31 root Exp $
#
#
######################################################################

package sgmlsESIS;

@TAGSTACK = ();
%Attributes = ();
$IGNORE_ALL_MARKUP = 0;

sub new {
    my($class, %attributes) = @_;
    my($obj);

    if (&main::ispackage($class)) {
	$obj = $class->new(%attributes);

	# if $class has a new method that overrides GENERIC_TAG's new
	# method, the resulting {'TAG'} field may be a holdover from
	# the call to GENERIC_TAG->new, so we force it back...
	$obj->{'TAG'} = ref $obj;

#	print STDERR "NEW: $class; ", $obj->{'TAG'}, "\n";
    } else {
	&main::WARNING("<$class> is unknown, using generic.\n");
	$obj = GENERIC_TAG->new(%attributes);
    }

#    print "\$obj: $obj\n";
#    print "ref \$obj: ", ref $obj, "\n";
#    print "ppi: ", $obj->{'PROPER_INIT'}, "\n";

    if (!$obj || !$obj->{'PROPER_INIT'}) {
	&main::WARNING("<$class> initialization FAILED! (debug: $obj, " . $obj->{'PROPER_INIT'} . ")\n");
    }

    return $obj;
}

sub perl_conforming {
    $main::SGMLS_OK = 1;
}

sub perl_attribute {
    my($name, $type, $value) = @_;

    $Attributes{$name} = $value;
}

sub perl_location {
    my($lineno, $filename) = @_;

    $main::SGMLS_LINE_NO = $lineno;
    $main::SGMLS_FILENAME = $filename if defined($filename);

    #nop
}

sub perl_startElement {
    local($_) = @_;
    my($ref);

    $ref = &new($_, %Attributes);
    %Attributes = ();

    if (@TAGSTACK) {
	$TAGSTACK[$#TAGSTACK]->add_child($ref);
    } else {
	$main::ROOT = $ref;
	$GENERIC_TAG::LAST_ELEM = $ref;
    }

#    print STDERR "PUSH: $ref->{'TAG'} ($ref->{'ATTR'}->{'ID'})\n";
    push (@TAGSTACK, $ref);
}

sub perl_endElement {
    local($_) = @_;
    my($ref) = pop(@TAGSTACK);
    my($count);

    if ($ref->{'TAG'} ne $_) {
	print STDERR "Corrupt tag stack; found ", $ref->{'TAG'};
	print STDERR " where $_ was expected.\n";
	die "Giving up...\n";
    }

    $ref->{'CONTENT_LAST'}->{'NEXT'} = 0
	if $ref->{'CONTENT_LAST'};
}

sub perl_pi {
    my($s) = @_;
    my($ref) = &new('PI');

    $ref->{'TAG'} = 'PI';
    $ref->{'PI'} = $s;
    $TAGSTACK[$#TAGSTACK]->add_child($ref) if (@TAGSTACK);
}

sub perl_sysid {
    my($sysid) = @_;
    
    #nop
}

sub perl_pubid {
    my($pubid) = @_;

    #nop
}

sub perl_defineNotation {
    my($name) = @_;

    #nop
}

sub perl_appinfo {
    my($info) = @_;

    #nop
}

sub perl_defineExternalEntity {
    my($arg1, $arg2, $arg3) = @_;

    #nop
}

sub perl_defineInternalEntity {
    my($arg1, $arg2, $arg3) = @_;

    #nop
}

sub perl_data {
    local($_) = @_;
    my($ref) = &new('PCDATA');

    $_ = sprintf("%c%s", oct($1), $') if /^\\(\d\d\d)/;
    while (/([^\\])\\(\d\d\d)/) {
	$_ = sprintf("%s%s%c%s", $`, $1, oct($2), $');
    }

    $ref->{'TAG'} = 'PCDATA';
    $ref->{'DATA'} = $_;
    $TAGSTACK[$#TAGSTACK]->add_child($ref) if (@TAGSTACK);
}

sub perl_sdata {
    my($s) = @_;
    my($ref) = &new('SDATA');

    $ref->{'TAG'} = 'SDATA';
    $ref->{'SDATA'} = $s;
    $TAGSTACK[$#TAGSTACK]->add_child($ref) if (@TAGSTACK);
}

######################################################################

sub dispatch {
    local($_) = @_;

    return if $IGNORE_ALL_MARKUP;

    &perl_startElement($1),              return if /^\((.*)$/;
    &perl_endElement($1),                return if /^\)(.*)$/;
    &perl_process_data($1),              return if /^\-(.*)$/;
    &perl_referenceEntity($1),           return if /^\&(.*)$/;
    &perl_process_pi($1),                return if /^\?(.*)$/;
							   
    &perl_attribute($1, "IMPLIED",  ""), return if /^A(\S+) IMPLIED/;
    &perl_attribute($1, "CDATA",    $2), return if /^A(\S+) CDATA (.*)$/;
    &perl_attribute($1, "NOTATION", $2), return if /^A(\S+) NOTATION (.*)$/;
    &perl_attribute($1, "ENTITY",   $2), return if /^A(\S+) ENTITY (.*)$/;
    &perl_attribute($1, "TOKEN",    $2), return if /^A(\S+) TOKEN (.*)$/;
    
    &perl_dataAttribute($1, $2, $3),     return if /^D(\S+) (\S+) (.*)$/;
    
    &perl_linkAttribute($1, "IMPLIED",  ""), return if /^a(\S+) IMPLIED/;
    &perl_linkAttribute($1, "CDATA",    $2), return if /^a(\S+) CDATA (.*)$/;
    &perl_linkAttribute($1, "NOTATION", $2),return if /^a(\S+) NOTATION (.*)$/;
    &perl_linkAttribute($1, "ENTITY",   $2), return if /^a(\S+) ENTITY (.*)$/;
    &perl_linkAttribute($1, "TOKEN",    $2), return if /^a(\S+) TOKEN (.*)$/;
    
    &perl_defineNotation($1), return        if /^N(.*)$/;

    &perl_defineExternalEntity($1, $2, $3), return  if /^E(\S+) (\S+) (.*)$/;
    &perl_defineInternalEntity($1, $2, $3), return  if /^I(\S+) (\S+) (.*)$/;

    &perl_defineSubdocEntity($1), return    if /^S(.*)$/;

    &perl_sysid($1), return                 if /^s(.*)$/;
    &perl_pubid($1), return                 if /^p(.*)$/;
						       
    &perl_file($1), return                  if /^f(.*)$/;
						       
    &perl_startSubdoc($1), return           if /^\{(.*)$/;
    &perl_endSubdoc($1),   return           if /^\}(.*)$/;
						       
    &perl_location($1, $2), return          if /^L(\d+) (.+)$/;
    &perl_location($1, ""), return          if /^L(\d+)$/;
						       
    &perl_appinfo($1), return               if /^\#(.*)$/;
						       
    &perl_conforming(), return              if /^C/;

    die "Unexpected line in ESIS: $_\n";
}

sub perl_process_data {
    local($_) = @_;
    my(@data) = ();

    while (/\\\|(.*?)\\\|/) {
	push(@data, $`);
	push(@data, "\\|$1\\|");
	$_ = $';
    }
    push (@data, $_) if $_ ne "";

    foreach $_ (@data) {
	if (/\\\|(.*?)\\\|/) {
	    perl_sdata($1);
	} else {
	    s/\\\\/\001/g;	# Holding pen \\ is a literal slash
	    s/\\n/\n/g;
	    s/\001/\\/g;	# Put back literal slashes as "\".
	    perl_data($_);
	}
    }
}

sub perl_process_pi {
    local($_) = @_;
    my($pi) = "";
    my($ch);

    # Make sure we translate "\" "n" into "\n"
    # And "\" "\" into "\"
    # But, avoid turning "\\n" into "\n"!
    while (/\\(.)/) {
	$pi .= $`;
	$ch = $1;
	$_ = $';

	if ($ch eq "\\") {
	    $pi .= "\\";
	} elsif ($ch eq "n") {
	    $pi .= "\n";
	} else {
	    $pi .= "\\$ch";
	}
    }
    $pi .= $_;

    perl_pi($pi);
}

######################################################################

1;

