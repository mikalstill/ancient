# -*- Perl -*-
#
# The SMGL Parser Object
#
# $Id: sparser.pl,v 1.1.1.1 2002-07-15 21:33:31 root Exp $
#
#
######################################################################

package SGML_PARSER;

$FH_count = 1;
$DEFAULT_sgmls_exec = "/usr/local/bin/nsgmls";
$DEFAULT_sgmls_opts = "degru";
$DEFAULT_sgmls_decl = "/usr/local/share/gmat/sgml/sdecl";
$DEFAULT_sgmls_errs = "/dev/null";

sub new {
    my($ref) = bless { };

    $ref->{'FH'} = sprintf("FH_%03d", $FH_count++);
    $ref->{'SGMLS-EXEC'} = $DEFAULT_sgmls_exec;
    $ref->{'SGMLS-OPTS'} = $DEFAULT_sgmls_opts;
    $ref->{'SGMLS-DECL'} = $DEFAULT_sgmls_decl;
    $ref->{'SGMLS-ERRS'} = $DEFAULT_sgmls_errs;
    
    return $ref;
}

sub sgmls_exec {
    my($self, $exec) = @_;
    
    if (-x $exec && -f $exec) { 
	$self->{'SGMLS-EXEC'} = $exec;
    } else {
	print STDERR "$main::PROGNAME: $exec is not executable.\n"
	    if $main::VERBOSE;
    }
}

sub sgmls_opts {
    my($self, $opts) = @_;
    
    $opts = $1 if $opts =~ /^-(.*)$/;
    
    $self->{'SGMLS-OPTS'} = $opts;
}

sub sgmls_decl {
    my($self, $decl) = @_;

    if (-r $decl && -f $decl) {
	$self->{'SGMLS-DECL'} = $decl;
    } else {
	print STDERR "$main::PROGNAME: $decl isn\'t readable.\n"
	    if $main::VERBOSE;
    }
}

sub sgmls_errs {
    my($self, $errs) = @_;
    
    if (! -f $errs || -w $errs) {
	$self->{'SGMLS-ERRS'} = $errs;
    } else {
	print STDERR "$main::PROGNAME: $errs isn\'t writable.\n"
	    if $main::VERBOSE;
    }
}

sub parse {
    my($self, $filename) = @_;
    my($FH) = $self->{'FH'};
    my($rc, $exec, $opts, $decl, $errs);

    $exec = $self->{'SGMLS-EXEC'};
    $opts = $self->{'SGMLS-OPTS'};
    $decl = $self->{'SGMLS-DECL'};
    $errs = $self->{'SGMLS-ERRS'};

    $opts = "-$opts" if $opts !~ /^\s*$/;
    
    eval "\$rc = open ($FH, \"$exec $opts $decl $filename 2> $errs |\")";
    
    $rc;
}

sub save {
    # save sgmls output, don't actually setup to parse anything...
    my($self, $filename, $output_file) = @_;
    my($FH) = $self->{'FH'};
    my($rc, $exec, $opts, $decl, $errs);

    $exec = $self->{'SGMLS-EXEC'};
    $opts = $self->{'SGMLS-OPTS'};
    $decl = $self->{'SGMLS-DECL'};
    $errs = $self->{'SGMLS-ERRS'};

    $opts = "-$opts" if $opts !~ /^\s*$/;

    print STDERR "$main::PROGNAME: saving ESIS in $output_file\n"
	if $main::VERBOSE;

    $rc = system ("$exec $opts $decl $filename 2> $errs > $output_file");
    
    $rc;
}

sub load {
    # load sgmls output from a file, pretend to be parsing...
    my($self, $filename) = @_;
    my($FH) = $self->{'FH'};
    my($rc, $exec, $opts, $decl, $errs);

    $exec = $self->{'SGMLS-EXEC'};
    $opts = $self->{'SGMLS-OPTS'};
    $decl = $self->{'SGMLS-DECL'};
    $errs = $self->{'SGMLS-ERRS'};

    $opts = "-$opts" if $opts !~ /^\s*$/;

    print STDERR "$main::PROGNAME: loading ESIS from $filename\n"
	if $main::VERBOSE;

    unlink ($errs);
    eval "\$rc = open ($FH, \"$filename\")";
    
    $rc;
}

sub next_element {
    my($self) = @_;
    my($done) = 0;
    my($elem);
    local($_);

    $elem = undef;
    while (!$done) {
	$_ = $self->next_line();
#	print STDERR "NE: $_\n";
	&sgmlsESIS::dispatch($_) if defined($_);
	$elem = $_;
	if (!defined($_)) {
	    $done = 1;
	} elsif (/^\((.*)$/) {
	    $done = 1;
	    $elem = $1;
	}
    }
    
    $elem;
}

sub next_line {
    my($self) = @_;
    my($FH);
    local($_);
    
    $FH = $self->{'FH'};
    
    eval "\$_ = scalar(<$FH>)";
    
    $_;
}

sub shutdown {
    my($self) = @_;
    my($FH);

    $FH = $self->{'FH'};

    eval "close($FH)";
}

sub DESTROY {
    my($self) = @_;

    $self->shutdown();
}
