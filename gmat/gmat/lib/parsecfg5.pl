# -*- Perl -*-
#
# Parse CFG files.  This code requires Perl5
#
# $Id: parsecfg5.pl,v 1.1.1.1 2002-07-15 21:33:31 root Exp $
#
#
######################################################################

package ParseCfg;

$GLOBAL = "*";
$PREFIX = "*";
$SECTION_LIST    = "$PREFIX sections";
$NEWSECTION_LIST = "$PREFIX newsections";
$REGEXP_INDEX    = "$PREFIX resection";

# $cfg->{'section'}->{'variable'} = value
# $cfg->{'section'}->{'* variable'} = variable source file
# $cfg->{'section'}->{'* variable modified'} = variable was modified?
# $cfg->{'*'}->{'variable'} = global value
# $cfg->{'* sections'} = ('*', 'first-section', 'second-section', ... )
# $cfg->{'* newsections'} = ('first-new-section', 'second-new-section', ...)
# $cfg->{'* resection'} = last re section

sub canon_name {
    my($name, $monocase) = @_;

    $name =~ s/^\s+//;
    $name =~ s/\s+$//;
    $name =~ s/\s+/ /;

    $name =~ tr/A-Z/a-z/ if $monocase;

    $name;
}

sub re_match {
    my($root, $name, $variable) = @_;
    my($recount, $val);

    undef $val;

    $recount = $root->{$REGEXP_INDEX};

    while (!defined($val) && ($recount >= 0)) {
	$section = $root->{$SECTION_LIST}->[$recount];

	# For RE matching purposes,
	# . => \.
	# * => .*
	# ? => .
	# in the section names:

	($resection = $section) =~ s/\./\\\./g;
	$resection =~ s/\*/\.\*/g;
	$resection =~ s/\?/\./g;

	if ($name eq $GLOBAL || $name =~ /^$resection$/) {
	    $val = $root->{$section}->{$variable};
	}
	$recount--;
    }

    $val;
}

sub main::parsecfg {
    my ($cfgfile, $root) = @_;
    my ($variable, $value, $key);
    my ($linecount, $sectioncount);
    my (%curopts);
    my (%lines);
    my ($section) = $GLOBAL;
    my ($lookup, $lastsection, $ref, $tref, $eol, $change);
    my ($resection_ok);

    if (defined($root)) {
	if (ref $root ne 'HASH') {
	    warn "$0: Non-hash passed to parsecfg, $cfgfile ignored.\n";
	    return undef;
	}
	if (!defined($root->{$REGEXP_INDEX})) {
	    warn "$0: Non-cfg hash passed to parsecfg, $cfgfile ignored.\n";
	    return undef;
	}
	$resection_ok = 0;
    } else {
	$root = { };
	$root->{$SECTION_LIST} = ();
	$root->{$NEWSECTION_LIST} = ();
	$root->{$REGEXP_INDEX} = 0;

	push (@{$root->{$SECTION_LIST}}, $GLOBAL);

	$resection_ok = 1;
    }

    open (CFG, "$cfgfile") || return $root;

    $lastsection = "";
    $linecount = 0;
    while (<CFG>) {
	chop;
	$linecount++;
	
	next if /^\s*$/;
	next if /^\s*\#/;
	
	if (/^\s*\[([^\[\]]+)]\s*$/) {
	    $section = &canon_name($1);
	    next if defined($root->{$section});

	    $root->{$section} = { };
	    push (@{$root->{$SECTION_LIST}}, $section);

	    if ($resection_ok) {
		if ($section =~ /[\*\?]/) {
		    $root->{$REGEXP_INDEX}++;
		} else {
		    $resection_ok = 0;
		}
	    } else {
		if ($section =~ /[\*\?]/) {
		    warn "$0: $cfgfile: sections with regular expressions must be first.\n";
		}
	    }

	    next;
	}
	    
	if (/^([^=]+)=\s*(.*)\s*$/) {
	    $value = $2;
	    $variable = &canon_name($1, 1);
	    while (($value =~ /\\$/) && ($_ = <CFG>)) {
		chop;
		s/\s+$//;
		$value .= "\n$_";
	    }
	} else {
	    warn "$0: Unparsable line in configuration file.\n";
	    warn "$cfgfile: $linecount: $_\n";
	    next;
	}

	if (defined($root->{$section})
	    && defined($root->{$section}->{"$PREFIX $variable"})
	    && $root->{$section}->{"$PREFIX $variable"} eq $cfgfile) {
	    warn "$0: Duplicate configuration item, overrides previous.\n";
	    warn "$cfgfile: $linecount: $_\n";
	}

	$root->{$section}->{$variable} = $value;
	$root->{$section}->{"$PREFIX $variable"} = $cfgfile;
    }

    close(CFG);

    return $root;
}

sub main::mergecfg {
    # Note: mergecfg cannot detect when things have been deleted...
    my ($cfgfile, $curroot) = @_;
    my ($section, $variable, $value, $key);
    my ($root, @sections, %variables);

    $root = &main::parsecfg ($cfgfile);
    if (!defined($curroot)) {
	return $root;
    }

    if (!defined($curroot->{$REGEXP_INDEX})) {
	warn "$0: Non-cfg hash passed to mergecfg, merge ignored.\n";
	return $root;
    }

    @sections = &main::cfg_sections ($curroot, 1);
#    print STDERR "Sections=@sections\n";
    foreach $section (@sections) {
	%variables = &main::cfg_variables ($curroot, $section);
#	print STDERR "Variables=", keys %variables, "\n";
	foreach $variable (keys %variables) {
	    $value = &main::cfg_modified ($curroot, $section, $variable);
	    if (defined($value)) {
		&main::cfg_set ($root, $section, $variable, $value);
#		print STDERR "Merging $section $variable=$value\n";
	    }
	}
    }

    return $root;
}

sub main::debugcfg {
    my($root) = @_;
    my(%prsect) = ();
    my($sect, $var, $val);
    my(@arr);

    if (defined($root)) {
	if (ref $root ne 'HASH') {
	    warn "$0: Non-hash passed to debugcfg.\n";
	    return undef;
	}
	if (!defined($root->{$REGEXP_INDEX})) {
	    warn "$0: Non-cfg hash passed to debugcfg.\n";
	    return undef;
	}
    }

    foreach $sect (@{$root->{$SECTION_LIST}}, @{$root->{$NEWSECTION_LIST}}) {
	$prsect{$sect} = 1;
	if (ref $root->{$sect} eq 'HASH') {
	    print "\%$sect\n";
	    foreach $var (sort keys %{$root->{$sect}}) {
		$val = $root->{$sect}->{$var};
		print "\t$var=$val\n";
	    }
	} elsif (ref $root->{$sect} eq 'ARRAY') {
	    print "\@$sect\n";
	    @arr = @{$root->{$sect}};
	    $var = 0;
	    while (@arr) {
		$val = shift @arr;
		print "\t\[$var\]=$val\n";
		$var++;
	    }
	} else {
	    $val = $root->{$sect};
	    print "\$$sect=$val\n";
	}
    }

    print "debugcfg: the following sections are not in a section list.\n";

    foreach $sect (keys %$root) {
	next if $prsect{$sect};

	if (ref $root->{$sect} eq 'HASH') {
	    print "\%$sect\n";
	    foreach $var (sort keys %{$root->{$sect}}) {
		$val = $root->{$sect}->{$var};
		print "\t$var=$val\n";
	    }
	} elsif (ref $root->{$sect} eq 'ARRAY') {
	    print "\@$sect\n";
	    @arr = @{$root->{$sect}};
	    $var = 0;
	    while (@arr) {
		$val = shift @arr;
		print "\t\[$var\]=$val\n";
		$var++;
	    }
	} else {
	    $val = $root->{$sect};
	    print "\$$sect=$val\n";
	}
    }
}


sub main::cfg {
    my($root, $name, $variable) = @_;
    my($val, $recount, $section, $resection);

    return undef if (!$root 
		     || ref $root ne 'HASH' 
		     || !defined($root->{$REGEXP_INDEX}));

    $name = &canon_name($name);
    $variable = &canon_name($variable, 1);

    undef $val;

    $val = $root->{$name}->{$variable}
        if defined($root->{$name});

    $val = &re_match($root, $name, $variable) if !defined($val);

    return undef if !defined($val);

    $change = 1;
    while ($change) {
	$change = 0;

	# Yes, No, True, and False are ok, but abbreviations are not 
	# (since for example, appendix 'n' would become 0 if n was an 
	# acceptable abbreviation for no.

	# Replace true or yes with 1
	if ($val =~ /^\s*true\s*$/ || $val =~ /^\s*yes\s*$/) {
	    $val = 1;
	    last;
	}

	# Replace false or no with 0
	if ($val =~ /^\s*false\s*$/ || $val =~ /^\s*no\s*$/) {
	    $val = 0;
	    last;
	}

# We decided that this was a bad idea, because it breaks
# key="value" "with" "multiple" "words"
#	# Replace "value" with value
#	if ($val =~ /^\s*\"(.*)\"\s*$/) {
#	    $val = $1;
#	    last;
#	}

	while ($val =~ /\[(.*?)\]/) {
	    my($pre, $env, $post) = ($`, $1, $');

	    $env = $1 if $env =~ /^\s*(.*)\s*$/;
	    if ($env =~ /^(.*?)\|(.*)$/) {
		$env = $2;
		$env = $ENV{$1} if defined($ENV{$1});
	    } else {
		$env = $ENV{$env};
	    }

	    $val = "$pre$env$post";
	    $change = 1;
	}
    }

    $val =~ s/\\\n/ /g;		# Replace backslash-newline with space
    
    return $val
}

sub main::cfg_set {
    my($root, $name, $variable, $value) = @_;

    return undef if (!$root 
		     || ref $root ne 'HASH' 
		     || !defined($root->{$REGEXP_INDEX}));

    $name = &canon_name($name);
    $variable = &canon_name($variable, 1);

    if (defined($root->{$name})) {
	if (defined($root->{$name}->{$variable})) {
	    $root->{$name}->{$variable} = $value;
	} else {
	    $root->{$name}->{$variable} = $value;
	    $root->{$name}->{"$PREFIX $variable"} = "*";
	}
    } else {
	$root->{$name} = { };
	$root->{$name}->{$variable} = $value;
	$root->{$name}->{"$PREFIX $variable"} = "*";
	push(@{$root->{$NEWSECTION_LIST}}, $name);
    }

#    print STDERR "SET: $name: $PREFIX $variable modified\n";

    $root->{$name}->{"$PREFIX $variable modified"} = 1;

    $value =~ s/\\\n/ /g;	# Replace backslash-newline with space

    $value;
}

sub main::cfg_modified {
    my($root, $name, $variable) = @_;

    return undef if (!$root 
		     || ref $root ne 'HASH' 
		     || !defined($root->{$REGEXP_INDEX}));

    $name = &canon_name($name);
    $variable = &canon_name($variable, 1);

    return undef 
	if !defined($root->{$name}) || !defined($root->{$name}->{$variable});

#    print STDERR "TST: $name: $PREFIX $variable modified\n";

    $value = $root->{$name}->{$variable};
    if (defined($root->{$name}->{"$PREFIX $variable modified"})) {
	return $value;
    } else {
	return undef;
    }
}

sub main::cfg_delete {
    my($root, $name, $variable) = @_;
    my($value);

    return undef if (!$root 
		     || ref $root ne 'HASH' 
		     || !defined($root->{$REGEXP_INDEX}));

    $name = &canon_name($name);
    $variable = &canon_name($variable, 1);

    return undef 
	if !defined($root->{$name}) || !defined($root->{$name}->{$variable});

    $value = $root->{$name}->{$variable};
    delete($root->{$name}->{$variable});

    $value;
}

sub main::cfg_delete_section {
    my($root, $name) = @_;
    my(%vars);

    return undef if (!$root 
		     || ref $root ne 'HASH' 
		     || !defined($root->{$REGEXP_INDEX}));

    $name = &canon_name($name);

    return undef if !defined($root->{$name});

    %vars = %{$root->{$name}};
    delete $root->{$name};

    %vars;
}

sub main::cfg_defined {
    my($root, $name, $variable) = @_;
    my($val);

    return undef if (!$root 
		     || ref $root ne 'HASH' 
		     || !defined($root->{$REGEXP_INDEX}));

    $name = &canon_name($name);
    $variable = &canon_name($variable, 1);

    undef $val;

    $val = $root->{$name}->{$variable}
        if defined($root->{$name});

    $val = &re_match($root, $name, $variable) if !defined($val);

    $val = 1 if defined($val);

    $val;
}

sub main::cfg_section_defined {
    my($root, $name) = @_;

    return undef if (!$root 
		     || ref $root ne 'HASH' 
		     || !defined($root->{$REGEXP_INDEX}));

    return defined($root->{$name});
}

sub main::printcfg {
    my($root, $filename, $add_new_sections) = @_;
    my($section, @sections, %vars, $var, $value, $long);
    my($reprefix);
    local(*F, $_);

    return undef if (!$root 
		     || ref $root ne 'HASH' 
		     || !defined($root->{$REGEXP_INDEX}));

    open (F, ">$filename") || return 1;

    seek (F, 0, 0);
    truncate(F, 0);

    $reprefix = $PREFIX;
    $reprefix =~ s/\*/\\\*/;
    $reprefix =~ s/\$/\\\$/;

    chop($date = `date`);
    print F "# bookfiles\n";
    print F "# This configuration file was created by $0\n";
    print F "# on $date by $ENV{USER}.\n";
    print F "#\n";

    @sections = @{$root->{$SECTION_LIST}};
    push(@sections, @{$root->{$NEWSECTION_LIST}}) if $add_new_sections;

    foreach $section (@sections) {
	next if !defined($root->{$section}); # It might have been deleted...

	if ($section ne $GLOBAL) {
	    print F "\n";
	    print F "[ $section ]\n";
	}

	%vars = %{$root->{$section}};
	$long = 0;
	foreach $var (sort keys %vars) {
	    next if $var =~ /^$reprefix /;
	    $long = length($var) if length($var) > $long;
	}

	foreach $var (sort keys %vars) {
	    next if $var =~ /^$reprefix /;
	    $value = $root->{$section}->{$var};
	    printf F "%-${long}s = %s\n", $var, $value;
	}
    }

    close (F);
    return 0;
}

sub main::cfg_sections {
    my($root, $spaces_ok) = @_;
    my(@arr, @retarr);
    local($_);

    return undef if (!$root 
		     || ref $root ne 'HASH' 
		     || !defined($root->{$REGEXP_INDEX}));

    @arr = ();
    @arr = @{$root->{$SECTION_LIST}};
    shift @arr if $arr[0] eq $GLOBAL;

    push (@arr, @{$root->{$NEWSECTION_LIST}}) 
	if defined($root->{$NEWSECTION_LIST});

    @retarr = ();
    while (@arr) {
	$_ = shift @arr;
	next if !defined($root->{$_});
	next if !$spaces_ok && /[\s\*\?]/;
	push (@retarr, $_);
    }

    @retarr;
}

sub main::cfg_variables {
    my($root, $name) = @_;
    my(%vars) = ();
    local($_);

    return undef if (!$root 
		     || ref $root ne 'HASH' 
		     || !defined($root->{$REGEXP_INDEX}));

    $name = &canon_name($name);

    return undef if !defined($root->{$name});

    $reprefix = $PREFIX;
    $reprefix =~ s/\*/\\\*/;
    $reprefix =~ s/\$/\\\$/;

    foreach $_ (keys %{$root->{$name}}) {
	$vars{$_} = $root->{$name}->{$_} if !/^$reprefix /;
    }

    %vars;
}

sub main::cfg_prev_section {
    my($root, $section, $spaces_ok) = @_;
    my(@sections) = &main::cfg_sections($root, $spaces_ok);
    my($prev) = undef;
    local($_);

    while (@sections) {
	$_ = shift @sections;
	return $prev if $_ eq $section;
	$prev = $_;
    }

    undef;
}

sub main::cfg_next_section {
    my($root, $section, $spaces_ok) = @_;
    my(@sections) = &main::cfg_sections($root, $spaces_ok);
    local($_);

    while (@sections) {
	$_ = shift @sections;
	return shift @sections if $_ eq $section;
    }

    undef;
}

1;
