#!/usr/bin/perl

use strict;
my($depth, $condition, $temp);

$depth = 0;
$condition = "NONE";
while(<STDIN>){
    if(/^\#/){
	# ...comment...
    }
    elsif(/^[ \t]*$/){
	# Blank lines...
    }

    ##########
    # Menus    
    elsif(/^[ \t]*mainmenu_name[ \t](.*)/){
	depthIndent();
	print getString($1)."<BR>\n";
	$depth = 1;
    }
    elsif(/^[ \t]*mainmenu_option next_comment/){
	$depth++;
	print "<BR>\n";
    }
    elsif(/^[ \t]*comment[ \t](.*)/){
	depthIndent();
	print getString($1)."<BR>\n";
	$depth++;
    }
    elsif(/^[ \t]*endmenu/){
	$depth -= 2;
    }

    ##########
    # Booleans    
    elsif(/^[ \t]*define_bool[ \t]+(.*)[ \t]+(.*)/){
	depthIndent();
	markCondition();
	if($2 eq "y"){
	    print "<FONT COLOR=\"green\">";
	}
	else{
	    print "<FONT COLOR=\"red\">";
	}
	print "$1 = $2</FONT><BR>\n";
    }
    elsif(/^[ \t]*bool[ \t]+(.*)[ \t]+(.*)/){
	depthIndent();
	markCondition();
	print getString($1)." ($2)<BR>\n";
    }
    elsif(/^[ \t]+dep_bool[ \t]+(.*)[ \t]+(.*)[ \t]+(.*)/){
	depthIndent();
	markCondition();
	print "Dependant boolean: ".getString($1)." $2 on $3<BR>\n";
    }

    elsif(/^[ \t]*define_int[ \t]+(.*)[ \t]+(.*)/){
	depthIndent();
	markCondition();
	print "$1 = $2<BR>\n";
    }

    elsif(/^[ \t]*tristate[ \t]+(.*)[ \t]+(.*)/){
	depthIndent();
	markCondition();
	print "tristate: ".getString($1)." ($2)<BR>\n";
    }
    elsif(/^[ \t]*dep_tristate[ \t]+(.*)[ \t]+(.*)[ \t]+(.*)/){
	depthIndent();
	markCondition();
	print "Dependant tristate: ".getString($1)." $2 on $3<BR>\n";
    }

    ##########
    # Conditionals
    elsif(/^[ \t]*if \[ \"(.*)\" ([!=]+) \"(.*)\" \][ \t]*;/){
	$condition = "$condition;$1 $2 $3";
    }
    elsif(/^[ \t]*fi/){
	$condition =~ s/;[^;]*$//;
    }
    elsif(/^else/){
	$condition =~ s/;[^;]*$//;
	$condition = "$condition;else";
    }

    ##########
    # Process other files
    elsif(/^[ \t]*source[ \t](.*)/){
	# todo
    }

    ##########
    # Choices
    elsif(/^[ \t]*choice[ \t](.*)[ \t]+\\/){
	depthIndent();
	markCondition();
	print "Choice: ".getString($1)."<BR>\n";
	$depth++;
    }
    elsif(/[ \t]+[\"]?([^ \t]*)[ \t]+(.*)[ \t]+(\\)/){
	foreach $temp (split/\//, $1){
	    depthIndent();
	    markCondition();
	    print "$temp: $2<BR>\n";	    
	}
    }
    elsif(/[ \t]+([^ \t]*)[ \t]+(.*)\"[ \t]+(.*)/){
	foreach $temp (split/\//, $1){
	    depthIndent();
	    markCondition();
	    print "$temp: $2<BR>\n";	    
	}
	depthIndent();
	markCondition();
	print "<B>Default: $3</B><BR><BR>\n";
	$depth--;
    }

    ##########
    # Unknown lines
    else{
	print STDERR "Unknown line: $_";
    }
}

##############################################################################
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
	print STDERR "Unknown string format: $input\n";
	return $input;
    }
}

sub depthIndent(){
    my($count);

    for($count; $count < $depth; $count++){
	print "&nbsp;&nbsp;&nbsp;&nbsp;";
    }
}

sub markCondition(){
    if($condition ne ""){
	print "<FONT COLOR=\"orange\">$condition: </FONT>";
    }
}
