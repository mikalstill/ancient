# -*- Perl -*-
#
# Code common to gmat.driver, gmat.troff, and possibly other gmat filters
#
# $Id: gmat.pl,v 1.2 2002-07-18 20:57:52 mikal Exp $
#
#
######################################################################

sub initialize {
    select(STDERR); $| = 1;
    select(STDOUT);

    $WHOAMI = $ENV{"USER"} || $ENV{"LOGNAME"} || "user";
    $PID    = $$;

    print STDERR "Checking for config in $ORATOOLSRC\n";
    $gmat = &parsecfg($ORATOOLSRC);
    print STDERR "Checking for config in $ENV{HOME}/.oratoolsrc\n";
    $gmat = &parsecfg("$ENV{HOME}/.oratoolsrc", $gmat);
    print STDERR "Checking for config in ./.oratoolsrc\n";
    $gmat = &parsecfg("./.oratoolsrc", $gmat);

    $QUIET   = &cfg($gmat, $PROGNAME, 'quiet');
    $VERBOSE = &cfg($gmat, $PROGNAME, 'verbose') && !$QUIET;

    &check_gmat();
    &update_path();

    @temp_files = ();
    $SIG{'INT'} = 'sig_handler';
    $SIG{'KILL'} = 'sig_handler';
    $SIG{'HUP'} = 'sig_handler';
    
    $default_gmat = $gmat;		# Save the defaults...

    # Note: all common options must have long names
    @COMMON_OPTIONS = ('dfF:jJ:kK:o:pP:qsSvWx',
		       '--debug',
		       '--keep-ps-file',
		       '--ps-filename:',
		       '--keep-formatter-file',
		       '--formatter-filename:',
		       '--job:',
		       '--nocrop',
		       '--nojob',
		       '--pages:',
		       '--print',
		       '--printer:',
		       '--quiet',
		       '--keep-sgml-driver',
		       '--sgml-separate',
		       '--preview',
		       '--wait-on-error',
		       '--check-errors',
		       '--verbose',

		       '--no-bookfiles-update',
		       '--bookfiles-add',
		       '--no-bookfiles-add',
		       '--no-update-bookids',
		       '--hires',
		       '--fopt:',
		       '--keep-sgmls-output:',
		       '--use-sgmls-output:',
		     
		       '*debug=d',
		       '*keep-ps-file=f',
		       '*ps-filename=F',
		       '*keep-formatter-file=k',
		       '*formatter-filename=K',
		       '*job=J',
		       '*nojob=j',
		       '*pages=o',
		       '*print=p',
		       '*printer=P',
		       '*quiet=q',
		       '*keep-sgml-driver=s',
		       '*sgml-separate=S',
		       '*preview=v',
		       '*wait-on-error=W',
		       '*check-errors=x');

    @USER_OPTIONS = @COMMON_OPTIONS;

    %OPT_DESC = ('--debug'               => 'Save debugging information',
		 '--keep-ps-file'        => 'Keep the PS file',
		 '--ps-filename:'        => 'Specify the name of the PS file',
		 '--keep-formatter-file' => 'Keep the formatter input file',
		 '--formatter-filename:' => 'Specify the name of the formatter input file',
		 '--job:'                => 'Specify the name of the BOOKFILES file',
		 '--nocrop'              => 'Don\'t put in crop marks',
		 '--nojob'               => 'Don\'t use any BOOKFILES file',
		 '--pages:'              => 'Specify the pages to process',
		 '--print'               => 'Print the output file',
		 '--printer:'            => 'Specify the printer',
		 '--quiet'               => 'Turn off informative messages',
		 '--insert-ps-fonts'     => 'Include PostScript fonts in PostScript file',
		 '--keep-sgml-driver'    => 'Keep the SGML driver file',
		 '--sgml-separate'       => 'Process multiple SGML files as seperate documents',
		 '--preview'             => 'Preview the output file',
		 '--wait-on-error'       => 'If errors occur, wait for input before proceeding',
		 '--check-errors'        => 'Just check for errors, don\'t preview or print',
		 '--verbose'             => 'Turn on additional informative messages',
		 '--no-bookfiles-update' => 'Don\'t update the BOOKFILES file',
		 '--bookfiles-add'       => 'Add new files to the BOOKFILES file',
		 '--no-bookfiles-add'    => 'Don\'t add new files to the BOOKFILES file',
		 '--no-update-bookids'   => 'Don\'t update the BOOKIDS file',
		 '--hires'               => 'Use hires figures',
		 '--fopt'                => 'Options to pass through to the formatter');
}

sub parse_cmdline {
    die "$0: Invalid options ($Longopts::INVALID_OPTIONS).\n"
	if &Longopts(@USER_OPTIONS) ne 1;
}

sub init_globals {
    my($rc);

    #########################################################################
    # Load the BOOKFILES database... 
    #
    $BOOKFILES = $opt_job;
    $BOOKFILES = &cfg($gmat, $PROGNAME, "bookfiles") if !$BOOKFILES;
    $LOCKFILE  = "$BOOKFILES.lock";
    
    if (!open (BOOKFILES, "$BOOKFILES")) {
	die "Cannot load job file: $BOOKFILES.\n" 
	    if $BOOKFILES ne 'BOOKFILES';
    } else {
	$_ = scalar(<BOOKFILES>);
	
	die "You cannot process this job with $PROGNAME.\n" 
	    if !/^\s*\#\s$bookfiles/;
	
	close (BOOKFILES);
	
	$rc = &set_lock($LOCKFILE);

	die "Cannot proceed without lock on $BOOKFILES.\n"
	    if $rc == 75;

	$bookfiles = &parsecfg($BOOKFILES);
	&unset_delete_lock($LOCKFILE);
    }

    ##########################################################################
    # Initialize globals...
    #
    $BOOKFILES_VERSION = &cfg($bookfiles, $PROGNAME, "bookfiles_version");
    $LANGUAGE = &cfg($bookfiles, $PROGNAME, "language") || "en";

    $UPDATE_BOOKFILES = !$opt_no_bookfiles_update;
    $ADDNEW_BOOKFILES = (&cfg($bookfiles, $PROGNAME, "add_new_sections")
			 && !$opt_no_bookfiles_add) || $opt_bookfiles_add;
}

sub temp_files {
    local(@files) = @_;
    local($file);

    while (@files) {
	$file = shift @files;
	push(@temp_files, $file) if $file;
    }
}

sub sig_handler {  # 1st argument is signal name
    local($sig) = @_;

    $SIG{'INT'} = 'IGNORE';
    $SIG{'KILL'} = 'IGNORE';
    $SIG{'HUP'} = 'IGNORE';

    if ($sig eq "INT") {
	print STDERR "Interrupted.  Cleaning up...\n";
	close (COMMANDS);
	&cleanup_files();
	exit 2;
    } elsif ($sig eq "HUP") {
	print STDERR "Interrupted by hangup.  Leaving temporary files...\n";
	exit 3;
    } else {
	print STDERR "Interrupted by SIG$sig.  Cleaning up...\n";
	&cleanup_files();
	exit 4;
    }
}

sub cleanup_files {
    local(*TMPDIR, *FMTDIR, $_);

    opendir (FMTDIR, "./fmt/index");
    while (defined($_ = readdir(FMTDIR))) {
	unlink "./fmt/index/$_"
	    if /\.$PID$/;
    }
    closedir (FMTDIR);

    if (&cfg($gmat, $PROGNAME, "debug")) {
	print STDERR "Cleanup skipped because debug flag was specified.\n";
	return;
    }

    # cleanup the /tmp/Pf.fig.#.* and /tmp/Pf.size.#.* files
    opendir (TMPDIR, "/var/tmp");
    while (defined($_ = readdir(TMPDIR))) {
	unlink "/var/tmp/$_" 
	    if /^Pf\.fig\.$PID\./ || /^Pf\.size\.$PID\./;
    }
    closedir (TMPDIR);

    unlink (@temp_files);
}

sub update_path {
    local($PATH, $path);

    $PATH = &add_path_element($ENV{"PATH"}, &cfg($gmat, $PROGNAME, "bindir"));
    $PATH = &add_path_element($PATH, &cfg($gmat, $PROGNAME, "workdir"));

    $ENV{"PATH"} = $PATH;
}

sub add_path_element {
    local($PATH, $path) = @_;

    $PATH = "$path:$PATH"
	if ($PATH !~ /:$path:/ 
	    && $PATH !~ /^$path:/
	    && $PATH !~ /:$path$/);

    $PATH;
}

sub show_usage {
    my($rc) = @_;
    my($maxlen) = 0;
    my(%short) = ();
    my($opt);

    print <<END_OF_USAGE ;

 $PROGNAME version $VERSION.

 Usage: $0 [ options] file1 ... filen

 Options:

END_OF_USAGE

    foreach $_ (sort keys %OPT_DESC) {
	$maxlen = length($_) if length($_) > $maxlen;
    }

    foreach $_ (@USER_OPTIONS) {
	$short{"--$1"} = "-$2" if /^\*(.*)=(.)$/;
    }

    foreach $_ (sort keys %OPT_DESC) {
	if ($short{$_}) {
	    $opt = $short{$_};
	} else {
	    $opt = "  ";
	}

	printf "    %-${maxlen}s %s %s\n", $_, $opt, $OPT_DESC{$_};
    }

    if ($rc && $wait_on_error) {
	print "Press Enter or Return to continue...\n";
	scalar(<STDIN>);
    }

    exit ($rc);
}

sub backup_bookfiles {
    local($b) = "$BOOKFILES~";
    local($rc);

    if (-f $b) {
	chmod(0664, $b);
	unlink($b);
    }

    $rc = system ("cp $BOOKFILES $b");
    chmod (0444, $b);
}

1;

