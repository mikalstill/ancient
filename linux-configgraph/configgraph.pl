#!/usr/bin/perl

use strict;
my($depth, $condition, $temp, $hashref);

$depth = 0;
$condition = "";

$hashref = process("$ARGV[0]/arch/i386/config.in", $hashref);
dumphash($hashref, 0);










###########################################################################

sub process(){
    my($filename, $hashref) = @_;
    local(*INPUT);

    print STDERR "Processing $filename\n";
    open INPUT, "< $filename";
    while(<INPUT>){
	chomp;

	if(/^[ \t]*\#/){
	    # ...comment...
	}
	elsif(/^[ \t]*$/){
	    # Blank lines...
	}
	
	##########
	# Menus -- we display these to give an idea of parsing progress
	elsif(/^[ \t]*mainmenu_name[ \t](.+)/){
#	    print STDERR depthIndent().getString($1)."\n";
	    $depth = 1;
	}
	elsif(/^[ \t]*mainmenu_option next_comment/){
	    $depth++;
	}
	elsif(/^[ \t]*comment[ \t](.+)/){
#	    print depthIndent().getString($1)."\n";
	    $depth++;
	}
	elsif(/^[ \t]*endmenu/){
	    $depth -= 2;
	}
	
	##########
	# Booleans    
	elsif(/^[ \t]*bool[ \t]+(.+)[ \t]+(.+)/){
	    $hashref = addstatement($hashref, "bool\t$condition\t$2\ty,n\t".getString($1));
	}
	elsif(/^[ \t]*dep_bool[ \t]+(.+)[ \t]+(.+)[ \t]+(.+)/){
	    #todo condition();
#	    print "Dependant boolean: ".getString($1)." $2 on $3\n";
	}
	elsif(/^[ \t]*define_bool[ \t]+(.+)[ \t]+(.+)/){
	    $hashref = addstatement($hashref, "define_bool\t$condition\t$1\t$2");
	}

	#########
	# Mbooleans
	elsif(/^[ \t]*dep_mbool[ \t]+(.+)[ \t]+(.+)[ \t]+(.+)/){
	    #todo condition();
#	    print "Dependant mboolean: ".getString($1)." $2 on $3\n";
	}
	
	#########
	# Ints
	elsif(/^[ \t]*int[ \t]+(.+)[ \t]+(.+)[ \t]+(.+)/){
	    $hashref = addstatement($hashref, "int\t$condition\t$2\t$3,anyint\t".getString($1));
	}
	elsif(/^[ \t]*dep_int[ \t]+(.+)[ \t]+(.+)/){
	    #todo condition();
#	    print "$1 = $2\n";
	}
	elsif(/^[ \t]*define_int[ \t]+(.+)[ \t]+(.+)/){
	    $hashref = addstatement($hashref, "define_int\t$condition\t$1\t$2");
	}

	#########
	# Hex
	elsif(/^[ \t]*hex[ \t]+(.+)[ \t]+(.+)[ \t]+(.+)/){
	    $hashref = addstatement($hashref, "hex\t$condition\t$2\t$3,anyhex\t".getString($1));
	}
	elsif(/^[ \t]*dep_hex[ \t]+(.+)[ \t]+(.+)/){
	    #todo condition();
#	    print "$1 = $2\n";
	}
	elsif(/^[ \t]*define_hex[ \t]+(.+)[ \t]+(.+)/){
	    $hashref = addstatement($hashref, "define_hex\t$condition\t$1\t$2");
	}

	#########
	# Strings
	elsif(/^[ \t]*string[ \t]+(.+)[ \t]+(.+)[ \t]+(.+)/){
	    $hashref = addstatement($hashref, "string\t$condition\t$2\t$3,anystring\t".getString($1));
	}
	elsif(/^[ \t]*dep_string[ \t]+(.+)[ \t]+(.+)/){
	    #todo condition();
#	    print "$1 = $2\n";
	}
	elsif(/^[ \t]*define_string[ \t]+(.+)[ \t]+(.+)/){
	    $hashref = addstatement($hashref, "define_string\t$condition\t$1\t$2");
	}
	
	#########
	# Tristates
	elsif(/^[ \t]*tristate[ \t]+(.+)[ \t]+(.+)/){
	    $hashref = addstatement($hashref, "tristate\t$condition\t$2\ty,n,m\t".getString($1));
	}
	elsif(/^[ \t]*dep_tristate[ \t]+(.+)[ \t]+(.+)[ \t]+(.+)/){
	    #todo condition();
#	    print "Dependant tristate: ".getString($1)." $2 on $3\n";
	}
	elsif(/^[ \t]*define_tristate[ \t]+(.+)[ \t]+(.+)/){
	    $hashref = addstatement($hashref, "define_tristate\t$condition\t$1\t$2");
	}
	
	##########
	# Conditionals
	elsif(/^[ \t]*if \[ \"(.+)\" ([!=]+) \"(.+)\" \][ \t]*;/){
	    if($condition ne ""){
		$condition = "$condition;$1 $2 $3";
	    }
	    else{
		$condition = "$1 $2 $3";
	    }
	}
	elsif(/^[ \t]*fi/){
	    $condition =~ s/;[^;]*$//;
	}
	elsif(/^[ \t]*else/){
	    $condition =~ s/;[^;]*$//;
	    $condition = "$condition;else";
	}
	
	##########
	# Process other files
	elsif(/^[ \t]*source[ \t](.+)/){
#	    print "\t\t\t[Sourced file: $ARGV[0]/$1]\n";
	    process("$ARGV[0]/$1");
#	    print "\t\t\t[End sourced file: $ARGV[0]/$1]\n";
	}
	
	##########
	# Choices
	elsif(/^[ \t]*choice[ \t](.+)[ \t]+\\/){
	    #todo condition();
#	    print "Choice: ".getString($1)."\n";
	}
	elsif(/[ \t]+[\"]?([^ \t]*)[ \t]+(.+)[ \t]+(\\)/){
	    foreach $temp (split/\//, $1){
		#todo condition();
#	    print "$temp: $2\n";	    
	    }
	}
	elsif(/[ \t]+([^ \t]*)[ \t]+(.+)\"[ \t]+(.+)/){
	    foreach $temp (split/\//, $1){
		#todo condition();
#		print "$temp: $2\n";	    
	    }
	    #todo condition();
#	    print "Default: $3\n";
	}
	
	##########
	# Unknown lines
	else{
#	    print STDERR "Unknown line: $_";
	}
    }

    return $hashref;
}

sub getString(){
    my($input) = @_;
    my($temp);
    
    if(/\"(.*)\"/){
	# Double quoted strings have no substitution
	return $1;
    }
    elsif(/\'(.*)\'/){
	# Substitution required
	# todo: subst
	$temp = $1;
	$temp =~ s/^[ \t]+//;
	return $temp;
    }
    else{
#	print STDERR "Unknown string format: $input\n";
	return $input;
    }
}

sub depthIndent(){
    my($count, $string);

    $string = "";
    for($count; $count < $depth; $count++){
	$string = "$string ";
    }
    return $string;
}

sub addstatement(){
    my($hash_ref, $string) = @_;
    my(@cols, @conditions, $newstring, $re1, $re2, $re3);

    # Break the line into it's columns
    @cols = split(/\t/, $string);
    @conditions = split(/;/, @cols[1]);

    if($conditions[0] eq ""){
	$newstring = $hash_ref->{"END"};
	$hash_ref->{"END"} = "$newstring!$string";
    }
    else{
	print STDERR "Processing condition \"$conditions[0]\"\n";
	if($conditions[0] =~ /([^!= \t]*)[ \t]*([!=]+)[ \t]*(.*)/){
	    print STDERR "[$1][$2][$3]\n";
	    $re1 = $1;
	    $re2 = $2;
	    $re3 = $3;

	    if($re1 =~ /\$CONFIG_(.*)/){
		print STDERR "Condition based on $1\n";
	    }
	}


	splice(@conditions, 0, 1);
	$newstring = "$cols[0]\t".join(/;/, @conditions)."\t$cols[2]\t$cols[3]\t$cols[4]";
	$hash_ref->{$conditions[0]} = addstatement($hash_ref->{$conditions[0]}, $newstring);
    }

    return $hash_ref;
}

sub dumphash()
{
    my($hash_ref, $indent) = @_;
    my($key, $count);

    foreach $key (keys %$hash_ref){
	for($count = 0; $count < $indent; $count++){
	}
	
	if($key ne "END"){
	    # todo: These if's fix a bug in the tree which causes and empty key... I can't find it
	    if($key ne ""){
		print "<table border=1 width=\"100%\"><tr><td bgcolor=\"0000FF\">$key";
		print "&nbsp;</td></tr><tr><td>";
	    }
	    dumphash($hash_ref->{$key}, $indent + 1);
	    if($key ne ""){
		print "</td></tr></table><BR><BR>";
	    }
	}
	else{
	    $hash_ref->{$key} =~ s/!/<BR>\n/g;
	    print $hash_ref->{$key};
	}
    }
}
