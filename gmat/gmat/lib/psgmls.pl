# -*- Perl -*-
#
# This library parses the ESIS output from (n)sgmls
#
# $Id: psgmls.pl,v 1.1.1.1 2002-07-15 21:33:31 root Exp $
#
#
######################################################################

package sgmlsParse;

sub dispatch {
    local($_) = @_;

    $DISPATCH_COUNT++;
    return if $IGNORE_ALL_MARKUP;

    &sgmlsESIS::perl_startElement($1),    return           
	if /^\((.*)$/;

    &sgmlsESIS::perl_endElement($1),      return           
	if /^\)(.*)$/;

    &perl_process_data($1),               return           
	if /^\-(.*)$/;

    &sgmlsESIS::perl_referenceEntity($1), return           
	if /^\&(.*)$/;

    &perl_process_pi($1),              return           
	if /^\?(.*)$/;
							   
    &sgmlsESIS::perl_attribute($1, "IMPLIED",  ""), return if /^A(\S+) IMPLIED/;
    &sgmlsESIS::perl_attribute($1, "CDATA",    $2), return if /^A(\S+) CDATA (.*)$/;
    &sgmlsESIS::perl_attribute($1, "NOTATION", $2), return if /^A(\S+) NOTATION (.*)$/;
    &sgmlsESIS::perl_attribute($1, "ENTITY",   $2), return if /^A(\S+) ENTITY (.*)$/;
    &sgmlsESIS::perl_attribute($1, "TOKEN",    $2), return if /^A(\S+) TOKEN (.*)$/;
    
    &sgmlsESIS::perl_dataAttribute($1, $2, $3), return  if /^D(\S+) (\S+) (.*)$/;
    
    &sgmlsESIS::perl_linkAttribute($1, "IMPLIED",  ""), return if /^a(\S+) IMPLIED/;
    &sgmlsESIS::perl_linkAttribute($1, "CDATA",    $2), return if /^a(\S+) CDATA (.*)$/;
    &sgmlsESIS::perl_linkAttribute($1, "NOTATION", $2), return if /^a(\S+) NOTATION (.*)$/;
    &sgmlsESIS::perl_linkAttribute($1, "ENTITY",   $2), return if /^a(\S+) ENTITY (.*)$/;
    &sgmlsESIS::perl_linkAttribute($1, "TOKEN",    $2), return if /^a(\S+) TOKEN (.*)$/;
    
    &sgmlsESIS::perl_defineNotation($1), return        if /^N(.*)$/;

    &sgmlsESIS::perl_defineExternalEntity($1, $2, $3), return  if /^E(\S+) (\S+) (.*)$/;
    &sgmlsESIS::perl_defineInternalEntity($1, $2, $3), return  if /^I(\S+) (\S+) (.*)$/;

    &sgmlsESIS::perl_defineSubdocEntity($1), return    if /^S(.*)$/;

    &sgmlsESIS::perl_sysid($1), return                 if /^s(.*)$/;
    &sgmlsESIS::perl_pubid($1), return                 if /^p(.*)$/;
						       
    &sgmlsESIS::perl_file($1), return                  if /^f(.*)$/;
						       
    &sgmlsESIS::perl_startSubdoc($1), return           if /^\{(.*)$/;
    &sgmlsESIS::perl_endSubdoc($1),   return           if /^\}(.*)$/;
						       
    &sgmlsESIS::perl_location($1, $2), return          if /^L(\d+) (.+)$/;
    &sgmlsESIS::perl_location($1, ""), return          if /^L(\d+)$/;
						       
    &sgmlsESIS::perl_appinfo($1), return               if /^\#(.*)$/;
						       
    &sgmlsESIS::perl_conforming(), return              if /^C/;

    die "Unexpected (n)sgmls output: $_\n";
}

sub perl_process_data {
    local($_) = @_;
    my(@data) = ();

    while (/\\\|(.*?)\\\|/) {
	push(@data, $`);
	push(@data, "\\|$1\\|");
	$_ = $';
    }
    push (@data, $_) if $_ ne "";

    foreach $_ (@data) {
	if (/\\\|(.*?)\\\|/) {
	    &sgmlsESIS::perl_sdata($1);
	} else {
	    s/\\\\/\001/g;	# Holding pen \\ is a literal slash
	    s/\\n/\n/g;
	    s/\001/\\/g;	# Put back literal slashes as "\".
	    &sgmlsESIS::perl_data($_);
	}
    }
}

sub perl_process_pi {
    local($_) = @_;
    my($pi) = "";
    my($ch);

    # Make sure we translate "\" "n" into "\n"
    # And "\" "\" into "\"
    # But, avoid turning "\\n" into "\n"!
    while (/\\(.)/) {
	$pi .= $`;
	$ch = $1;
	$_ = $';

	if ($ch eq "\\") {
	    $pi .= "\\";
	} elsif ($ch eq "n") {
	    $pi .= "\n";
	} else {
	    $pi .= "\\$ch";
	}
    }
    $pi .= $_;

    &sgmlsESIS::perl_pi($pi);
}

1;
