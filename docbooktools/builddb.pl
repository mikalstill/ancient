#!/usr/local/bin/perl

use strict;
my($cmd, $cmdstr, $input, $output, $arg, $TMP);

# Make sure local scripts can run
$ENV{PATH} = $ENV{PATH} . ":.";

# This script runs through the specified docbook source, and applies the 
# commands in % blocks
#
# <execute><cmd></cmd><args></args><input></input><output></output></execute>
# <postexecute>... as above ...</postexecute>
#
# execute blocks happen as the file is read (including recursively)
# postexecute blocks happen once the entire file has been processed, and have 
# default input of the processed file

# This should be fixed...

open TMP, "> builddb-$$.tmp";
process("execute", "", \*STDIN, \*TMP);
close TMP;
open TMP, "< builddb-$$.tmp";
process("postexecute", "builddb-$$.tmp", \*TMP, \*STDOUT);
close TMP;
unlink "builddb-$$.tmp";

exit;

sub process($){
    my($exblock, $exfilename, $INPTR, $OUTPTR) = @_;
    print STDERR "builddb.pl is processing $exblock blocks...\n";

    while(<$INPTR>){
	if(/(.*)<$exblock>(.*)<\/$exblock>(.*)/){
	    print $OUTPTR $1;
	    $cmd = $2;
	    
	    $_ = $cmd;
	    if(/<input>/){
		$input = $cmd;
		$input =~ s/.*<input>//;
		$input =~ s/<\/input>.*//;
	    }
	    else{
		$input = "";
	    }
	    
	    if(/<output>/){
		$output = $cmd;
		$output =~ s/.*<output>//;
		$output =~ s/<\/output>.*//;
	    }
	    else{
		$output = "";
	    }
	    
	    if(/<args>/){
		$arg = $cmd;
		$arg =~ s/.*<args>//;
		$arg =~ s/<\/args>.*//;
	    }
	    else{
		$arg = "";
	    }
	    
	    $cmd =~ s/.*<cmd>//;
	    $cmd =~ s/<\/cmd>.*//;
	    
	    if($cmd ne "todo"){
		$cmdstr = "$cmd $arg";
		
		if($input ne ""){
		    if($exblock ne "postexecute"){
			$cmdstr = "$cmdstr $input < $input";
		    }
		    else{
			$cmdstr = "$cmdstr < $input";
		    }
		}
		elsif($exblock eq "postexecute"){
		    $cmdstr = "$cmdstr < $exfilename";
		}
		
		
		if($output ne ""){
		    $cmdstr = "$cmdstr > $output";
		}
		
		print STDERR "Executing command: $cmd\n";
		print $OUTPTR `$cmdstr`;
	    }
	    else{
		print $OUTPTR "<figure>\n";
		print $OUTPTR "<title>$input</title>\n";
		print $OUTPTR "<graphic format=\"EPS\" fileref=\"todo.eps\">\n";
		print $OUTPTR "</figure>\n";
	    }
	    print $OUTPTR "$3";
	}
	else{
	    print $OUTPTR $_;
	}
    }
}
