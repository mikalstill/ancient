#!/usr/local/bin/perl

use strict;
my($incomment, $origline, $code, $possfunction, $arg, %pointers, $key, $class,
   %creates, %family, %suppress, $temp);

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
    if(!(/INTERNAL/) && 
       (/([a-zA-Z0-9_\*]+) ([a-zA-Z0-9_\*]+) \(([^\(\)]+)\)/)){
	my($rval, $fval, $aval) = ($1, $2, $3);
	$_ = $aval;
	s/ //g;
	foreach $arg (split(/,/)){
	    # If the last character is a * then this is a pointer
	    # ignore some boring pointers which are too common
	    $_ = $arg;
	    if(($arg ne "char*") && ($arg ne "int*") && ($arg ne "void*") && 
	       (/\*$/)){
		$pointers{$arg} = $pointers{$arg}."$rval;$fval;$aval;";
	    }
	}

	# If there is a * in the combination of the return value and the 
	# function name, then this is a creation function
	$_ = "$rval$fval";
	if(($rval ne "char") && ($rval ne "void") && ($rval ne "int") &&
	   (/.*\*.*/)){
	    $creates{$fval} = $creates{$fval}."$rval;";
	}
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
	    $rval = shift(@argsarray);
	    $fval = shift(@argsarray);
	    $aval = shift(@argsarray);
	    
	    $aval =~ s/$class[ \t\*]+[, ]*//;
	    
	    # We only support one parent at the moment
	    if(($creates{$fval} ne "") && ($family{$rval} eq "") &&
	       ($suppress{$rval} eq "")){
		$family{$rval} = $family{$rval}."$class";
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
	print "  C$class();  // todo\n";
	print "  ~C$class(); // todo\n";
	print "\n";
	
	# We need to output all the functions which use one of these pointers
	my($rval, $fval, $aval, $origargs, @argsarray);
	@argsarray = split(/;/, $pointers{$key});
	
	while($argsarray[0] ne ""){
	    $rval = shift(@argsarray);
	    $fval = shift(@argsarray);
	    $aval = $origargs = shift(@argsarray);
	    
	    # Do we have a child relationship because of this function?
	    $aval =~ s/$class[ \t\*]+[, ]*//;
	    if($family{$class} ne ""){
		$temp = $family{$class};
		
		$aval =~ s/$temp[ \t\*]+[, ]*//;
	    }

	    # Do we have a parent relationship because of this function?
	    my($skip);
	    $skip = 0;
	    foreach $temp (split(/[ ,]/, $aval)){
		if($family{$temp} eq "$class"){
		    $skip = 1;
		}
	    }

	    if($skip == 0){
		$_ = $fval;
		s/.*_//;
		print "  $rval $_ (";

		my($argcount);
		$argcount = 0;
		foreach $temp (split/,/, $aval){
		    if($argcount > 0){
			print ", ";
			}
		    print "$temp a$argcount";
		    $origargs =~ s/$temp *\**/a$argcount/;
		    $argcount++;
		}
		
		print ") {";
		$origargs =~ s/$class *\*/m_ptr/g;
		$fval =~ s/^\*//;
		if($rval ne "void"){
		    print " return";
		}
		print " $fval($origargs); ";
		print "}\n";
	    }
	}
	
	print "\n";
	print "private:\n";
	print "  $key m_ptr;\n";
	
	if($family{$class} ne ""){
	    print "  $family{$class}& m_$family{$class}\n";
	}
	
	print "};\n\n";
    }
}
