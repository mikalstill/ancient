#!/usr/local/bin/perl

use strict;
my($incomment, $origline, $code, $possfunction, $arg, %pointers, $key, $class,
   %creates, %parents, %suppress, $temp);

$incomment = 0;
$code = "";

while(<STDIN>){
    if(/[ \t]*SUPPRESS\((.*)\)/){
	$suppress{$1} = "yes";
    }
    
    # Remove comments from the line
    s/\/\/.*$//;
    s/\/\*.*\*\///;
    s/\r//g;
    s/^[ \t]+//;
    $origline = $_;
    s/\/\*.*$//;
    s/^.*\*\///;

    # We want the newlines to stay for cpp directives
    if(/\#/){
    }
    else{
	chomp;
    }

    if($incomment == 0){
	$code = "$code$_";
    }

    $_ = $origline;
    # Block comment closed?
    if(/.*\*\//){
	$incomment = 0;
    }

    # Comments which dangle over to the next line
    if(/\/\*.*/){
	$incomment = 1;
    }
}

$code =~ s/[ \t]+/ /g;

# We now go through each line in the code, looking for functions...
foreach $possfunction (split(/;/, $code)){
    $_ = $possfunction;
    my($processed, $rval, $fval, $aval);
 
    $processed = 0;
    if(!(/INTERNAL/) && 
       (/([a-zA-Z0-9_\*]+) ([a-zA-Z0-9_\*]+) \(([^\(\)]*)\)/)){
	($rval, $fval, $aval) = ($1, $2, $3);
	$_ = $aval;
	s/ //g;
	foreach $arg (split(/,/)){
	    # If the last character is a * then this is a pointer
	    # ignore some boring pointers which are too common
	    $_ = $arg;
	    if(($arg ne "char*") && ($arg ne "int*") && ($arg ne "void*") && 
	       (/\*$/)){
		$pointers{$arg} = $pointers{$arg}."$rval!$fval!$aval;";
		$processed = 1;
	    }
	}

	# If there is a * in the combination of the return value and the 
	# function name, then this is a creation function
	$_ = "$rval$fval";
	if(($rval ne "char") && ($rval ne "void") && ($rval ne "int") &&
	   (/.*\*.*/)){
	    $creates{$rval} = $creates{$rval}."$fval;";
	}
    }
    else{
	$processed = 1;
    }

    if($processed == 0){
	my($libname);
	$libname = $fval;
	$libname =~ s/_.*$//;
	$libname =~ s/\*//;
	$pointers{$libname} = $pointers{$libname}."$rval!$fval!$aval;";
    }
}

# Determine the parent relationships between clases
foreach $key (keys %pointers){
    $class = $key;
    $class =~ s/\*$//;

    if($suppress{$class} eq ""){
	# We need to output all the functions which use one of these pointers
	my($rval, $fval, $aval, @argsarray);
	@argsarray = split(/;/, $pointers{$key});

	while($argsarray[0] ne ""){
	    ($rval, $fval, $aval) = split(/!/, shift @argsarray);
	    $aval =~ s/$class[ \t\*]+[, ]*//;
	    
	    # We only support one parent at the moment
	    if(($creates{$fval} ne "") && ($parents{$rval} eq "") &&
	       ($suppress{$rval} eq "")){
		$parents{$rval} = $parents{$rval}."$class";
	    }
	}
    }
}

# We now know the commonly passed pointers
foreach $key (keys %pointers){
    $class = $key;
    $class =~ s/\*$//;

    if($suppress{$class} eq ""){
	print "class C$class\n{\n";
	print "public:\n";

	if($creates{$class} ne ""){
	    # The constructor needs to take the reference to the parent
	    print "  C$class($class *passed_ptr";
	    if($parents{$class} ne ""){
		print ", $parents{$class}* passed_$parents{$class}) :\n";
		print "    m_$parents{$class}(passed_$parents{$class})\n";
		print "    { m_ptr = passed_ptr; }\n";
	    }
	    else{
		print ")\n";
		print "    { m_ptr = passed_ptr; }\n";
	    }

	    print "  ~C$class() { panda_xfree(m_ptr); }\n\n";
	}

	# We need to output all the functions which use one of these pointers
	my($rval, $fval, $aval, $origargs, @argsarray);
	@argsarray = split(/;/, $pointers{$key});

	while($argsarray[0] ne ""){
	    ($rval, $fval, $aval) = split(/!/, shift @argsarray);
	    $origargs = $aval;

	    # Do we have a child relationship because of this function?
	    $aval =~ s/$class[ \t\*]+[, ]*//;
	    if($parents{$class} ne ""){
		$temp = $parents{$class};
		$aval =~ s/$temp[ \t\*]+[, ]*//;
	    }

	    # Do we have a parent relationship because of this function?
	    my($skip);
	    $skip = 0;
	    foreach $temp (split(/[ ,]/, $aval)){
		if($parents{$temp} eq "$class"){
		    $skip = 1;
		}
	    }

	    if($skip == 0){
		$_ = $fval;
		s/[^_*]*_//;
		print "  $rval $_ (";

		my($argcount);
		$argcount = 0;
		foreach $temp (split/,/, $aval){
		    if($argcount > 0){
			print ", ";
			}
		    $temp =~ s/^ //;
		    print "$temp a$argcount";

		    # Feed in the passed args
		    $origargs =~ s/ *$temp *\**/ a$argcount/;
		    $argcount++;
		}
		
		print ") {";

		# Subst our class's pointer
		$origargs =~ s/$class *\*/m_ptr/g;

		# How about a parent pointer?
		if($parents{$class} ne ""){
		    $origargs =~ s/$parents{$class} \*/$parents{$class}/;
		    $origargs =~ s/$parents{$class}/m_$parents{$class}/;
		}

		$fval =~ s/^\*//;
		if($rval ne "void"){
		    print " return";
		}
		print " $fval($origargs); ";
		print "}\n";
	    }
	}
	
	if($creates{$class} ne ""){
	    print "\n";
	    print "private:\n";
	    print "  $key m_ptr;\n";
	}

	if($parents{$class} ne ""){
	    print "  $parents{$class}* m_$parents{$class};\n";
	}
	
	print "};\n\n";
    }
}
