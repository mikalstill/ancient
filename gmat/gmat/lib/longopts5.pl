# -*- Perl -*-
#
# Handle long options
#
# $Id: longopts5.pl,v 1.1.1.1 2002-07-15 21:33:31 root Exp $
#
#
# Usage: 
# 
#      do Longopts('a:bc'     # -a takes arg. -b & -c do not
#                 '--list'   # --list does not take an arg.
#                 '--file:'  # --file does
#                 '--asm:as' # --asm does, default=as
#                 '*asm=a'   # --asm and -a are aliases
#
# Implemented as a package to minimize name clashes...
#
######################################################################

package Longopts;

$SUPRESS_WARNINGS = 0;
$PASSON_LONGOPTS  = 0;
$INVALID_OPTIONS  = "";

sub main::Freeopts {
# Clear any $opt_* variables in package 'main' so that we can be sure
# were starting from scratch...useful if multiple calls to Longopts
# are made in the same script.
#
    local(*stab) = eval("*_main");
    local(%opts) = ();

    while (($key,$val) = each(%stab)) {
	if ($key =~ /^opt_/) {
	    eval "\$val = \$main::$key";
	    $opts{$key} = $val;
	    eval "undef \$main::$key";
	}
    }

    %opts;
}

sub main::Currentopts {
# Returns an associative array of the current opt_ variables set in main.
    my(%stab) = %main::;
    my(%opts) = ();
    my($key, $val);

    while (($key,$val) = each(%stab)) {
	if ($key =~ /^opt_/) {
	    $opts{$key} = $$val if ($$val ne "");
	}
    }

    %opts;
}

sub main::Longopts {
# Parse the options...
# Returns -1 if options are in error
# Returns 1 if successfull
# Returns 0 if there are user option errors
#
    local(@pats) = @_;
    local($_, $pat, $val);

    $Longopts::INVALID_OPTIONS = "";

    $_ = shift @pats;
    while (/(.)(:?)(.*)/) {
	$shortpats{$1} = $2;
	$_ = $3;
    }

    while (@pats && $pats[0] =~ /^--([^:]+)(:?.*)$/) {
	$pat = $1;
	$val = $2;

	$pat =~ tr/-/_/;

	$longpats{$pat} = $val;
	shift @pats;
    }

    while (@pats && $pats[0] =~ /^\*(.+)=(.)$/) { 
	$pat = $1;
	$val = $2;

	$pat =~ tr/-/_/;

	$aliases{$pat} = $val;
	shift @pats;
    }

    if (@pats) {
	warn "Longopts: bad arguments ($pats[0])\n" if !$SUPRESS_WARNINGS;
	return -1;
    }

    # Check that the aliases all make sense
    foreach $_ (keys %aliases) {
	local($long, $short, $lf, $sf);

	($long = $_) =~ tr/-/_/;
	$short = $aliases{$_};

	if (! defined($longpats{$long})) {
	    warn "Longopts: undefined long option in alias ($long)\n"
		if !$SUPRESS_WARNINGS;
	    return -1;
	}

	if (! defined($shortpats{$short})) {
	    warn "Longopts: undefined short option in alias ($short)\n"
		if !$SUPRESS_WARNINGS;
	    return -1;
	}

	$lf = substr($longpats{$long}, 0, 1);
	$sf = substr($shortpats{$short}, 0, 1);

	if ((($lf eq ":") && ($sf ne ":"))
	    || (($lf ne ":") && ($sf eq ":"))) {
	    warn "Longopts: attempt to alias incompatible options ($long)\n"
		if !$SUPRESS_WARNINGS;
	    return -1;
	}
    }

    $errs = 0;

    # Handle each of the command line arguments
    $done = 0;
    while (@ARGV && ($_ = $ARGV[0]) =~ /^-/) {
	shift @ARGV;

	last if /^--$/;
	if (/^--/) {
	    &handle_long($_);
	} else {
	    &handle_short($_);
	}
    }

    # Equalize aliased options
    foreach $_ (keys %aliases) {
	local($long, $short) = ($_, $aliases{$_});
	local($dl, $ds);
	
	eval "\$dl = defined(\$main::opt_$long)";
	eval "\$ds = defined(\$main::opt_$short)";
	
	if ($dl && $ds) {
	    warn "Longopts: Aliased options $long ($dl) and $short ($ds) are set differently.\n"
		if !$SUPRESS_WARNINGS && ($dl ne $ds);
	    $errs++;
	} elsif ($dl && !$ds) {
	    eval "\$main::opt_$short = \$main::opt_$long";
	} elsif (!$dl && $ds) {
	    eval "\$main::opt_$long = \$main::opt_$short";
	}
    }

    # Assign defaults to unspecified long options
    foreach $_ (keys %longpats) {
	local($d);

	eval "\$d = defined(\$main::opt_$_)";

	if (!$d && $longpats{$_} =~ /^:(.+)$/) {
	    eval "\$main::opt_$long = $1";
	}
    }

    # Check the aliases one more time in case a long option now has a 
    # value (that it got from the default)
    foreach $_ (keys %aliases) {
	local($long, $short) = ($_, $aliases{$_});
	local($dl, $ds);
	
	eval "\$dl = defined(\$main::opt_$long)";
	eval "\$ds = defined(\$main::opt_$short)";
	
	if ($dl && !$ds) {
	    eval "\$main::opt_$short = \$main::opt_$long";
	}
    }

    $errs == 0;
}

sub handle_long {
    local($_) = @_;
    local($var, $val);
    local($match) = 0;

    $_ = substr($_,2);		# remove the leading --

    if (/^([^:=]+)[:=](.*)$/) {
	$var = $1;
	$val = $2;
	$match = 1;
    } else {
	$var = $_;
	$val = "";
    }

    $var =~ tr/-/_/;

    if ($longpats{$var} eq ':') {
	if ($val eq "" && !$match) {
	    $val = shift @ARGV;
	}

	if ($val eq "") {
	    warn "Longopts: option $var requires an argument.\n"
		if !$SUPRESS_WARNINGS;
	    $errs++;
	}

	eval "\$main::opt_$var = \'$val\'";
    } elsif (defined($longpats{$var})) {
	eval "\$main::opt_$var = 1";
	if ($val ne "") {
	    warn "Longopts: argument to option $var ($val) ignored.\n"
		if !$SUPRESS_WARNINGS;
	    $errs++;
	    $val = "";
	}
	$_ = $val;
    } else {
	warn "Longopts: unknown option $var ignored.\n"
	    if !$SUPRESS_WARNINGS && !$PASSON_LONGOPTS;

	if ($val) {
	    $INVALID_OPTIONS .= "--$var:$val ";
	} else {
	    $INVALID_OPTIONS .= "--$var ";
	}

	$errs++ if !$PASSON_LONGOPTS;

	$val = "" if ($val =~ /^[:=]/);
	$_ = $val;
    }
}

sub handle_short {
    local($_) = @_;
    local($var, $val);

    $_ = substr($_,1);		# remove the leading -

    while (/^(.)(.*)$/) {
	$var = $1;
	$val = $2;

	if ($shortpats{$var} eq ':') {
	    if ($val eq "") {
		$val = shift @ARGV;
	    } else {
		$val = $1 if $val =~ /^[:=](.*)$/;
	    }

	    if ($val eq "") {
		warn "Longopts: option $var requires an argument.\n" 
		    if !$SUPRESS_WARNINGS;
		$errs++;
	    }

	    eval "\$main::opt_$var = \'$val\'";
	    $_ = "";
	} elsif (defined($shortpats{$var})) {
	    eval "\$main::opt_$var = 1";
	    if ($val =~ /^[:=](.*)$/) {
		warn "Longopts: argument to option $var ($1) ignored.\n"
		    if !$SUPRESS_WARNINGS;
		$errs++;
		$val = "";
	    }
	    $_ = $val;
	} else {
	    warn "Longopts: unknown option $var ignored.\n"
		if !$SUPRESS_WARNINGS;

	    $errs++;
	    $val = "" if ($val =~ /^[:=]/);
	    $_ = $val;
	}
    }
}

1;
