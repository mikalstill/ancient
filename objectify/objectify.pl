#!/usr/local/bin/perl

use strict;
my($incomment, $origline, $code, $possfunction, $arg, %pointers, $key, $class,
   $temp, %creates);

$incomment = 0;
$code = "";

while(<STDIN>){
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
    if(/([a-zA-Z0-9_\*]+) ([a-zA-Z0-9_\*]+) \(([^\(\)]+)\)/){
	$temp = "$1;$2;$3;";
	$_ = $3;
	s/ //g;
	foreach $arg (split(/,/)){
	    # If the last character is a * then this is a pointer
	    # ignore some boring pointers which are too common
	    $_ = $arg;
	    if(($arg ne "char*") && ($arg ne "int*") && ($arg ne "void*") && 
	       (/\*$/)){
		$pointers{$arg} = $pointers{$arg}.$temp;
	    }
	}

	# If there is a * in the combination of the return value and the 
	# function name, then this is a creation function
	$_ = "$temp";
	s/;[^;]*$//;
	s/;[^;]*$//;
	s/;//;
	if(/.*\*.*/){
	    print "$_ CREATES\n";
	    
	}
    }
}

# We now know the commonly passed pointers
foreach $key (keys %pointers){
    $class = $key;
    $class =~ s/\*$//;

    print "class C$class\n{\n";
    print "public:\n";
    print "  C$class();  // todo\n";
    print "  ~C$class(); // todo\n";
    print "\n";
    
    # We need to output all the functions which use one of these pointers
    print "  // Functions which take a $class as an arguement\n";

    my($rval, $fval, $aval, @argsarray);
    @argsarray = split(/;/, $pointers{$key});
    
    while($argsarray[0] ne ""){
	$rval = shift(@argsarray);
	$fval = shift(@argsarray);
	$aval = shift(@argsarray);

	$aval =~ s/$class[ \t\*]+[, ]*//;

	print "  $rval $fval ($aval);\n";
    }

    print "\n";
    print "private:\n";
    print "  $key m_ptr;\n";
    print "};\n\n";
}
