# -*- Perl -*-
#
# ORA version of getopts
#
# USE OF THIS LIBRARY IS DEPRECIATED.  USE LONGOPTS5.PL INSTEAD.
#
# $Id: oragetopts.pl,v 1.1.1.1 2002-07-15 21:33:31 root Exp $
#
#
# The second argument to Getopts is a list of options that are passed
# transparently through.  If &Getopts is called in an array context
# the value returned is (rc, other_options).  In a scalar context, only
# rc is returned.  'rc' is NON-zero if the parsing SUCCEEDS.
#
# Note: 
#
# @ARGV=-a -xbc -x -y -b
# ($rc, $others) = &Getopts('abc','xyz') 
#
# sets:
#
# $rc    = 1
# $opt_a = 1
# $opt_b = 1
# $opt_c = 0
# $others = '-xbc -x -y';
#
# Usage:
#      do Getopts('a:bc', 'x:yz);  # -a takes arg. -b & -c not. 
#                           #  Sets opt_* as a side effect.
#
######################################################################

sub Getopts {
    local($argumentative, $optionallyargumentative) = @_;
    local(@args, $_, $first, $rest, $OTHER_OPTIONS);
    local($errs) = 0;
    local($[) = 0;

    @args = split( / */, $argumentative );
    @optargs = split ( / */, $optionallyargumentative );

    while(@ARGV && ($_ = $ARGV[0]) =~ /^-(.)(.*)/) {
	($first,$rest) = ($1,$2);
	$pos = index($argumentative,$first);
	$optpos = index($optionallyargumentative,$first);
	if($pos >= $[) {
	    if($args[$pos+1] eq ':') {
		shift(@ARGV);
		if($rest eq '') {
		    ++$errs unless @ARGV;
		    $rest = shift(@ARGV);
		}
		eval "\$opt_$first = \$rest;";
	    }
	    else {
		eval "\$opt_$first = 1";
		if($rest eq '') {
		    shift(@ARGV);
		}
		else {
		    $ARGV[0] = "-$rest";
		}
	    }
	} elsif ($optpos >= $[) {
	    if($optargs[$optpos+1] eq ':') {
		shift(@ARGV);
		if($rest eq '') {
		    ++$errs unless @ARGV;
		    $rest = shift(@ARGV);
		    $OTHER_OPTIONS .= "-$first $rest ";
		} else {
		    $OTHER_OPTIONS .= "-$first$rest ";
		}
	    }
	    else {
		$OTHER_OPTIONS .= "-$first$rest ";
		shift (@ARGV);
	    }
	} else {
	    print STDERR "Unknown option: $first\n";
	    ++$errs;
	    if($rest ne '') {
		$ARGV[0] = "-$rest";
	    }
	    else {
		shift(@ARGV);
	    }
	}
    }

    chop($OTHER_OPTIONS);
    return wantarray ? ($errs == 0, $OTHER_OPTIONS) : $errs == 0;
}

1;
