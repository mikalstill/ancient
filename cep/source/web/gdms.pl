#!/usr/bin/perl

# GDMS web broker
# Copyright (C) Michael Still             2002
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#  
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#  
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

# This is the broker between the GDMS code and the CGI server. It's role is to
# parse templates, execute requests, and cache results for speedy processing
# next time that request is seen...
use strict;
use CGI;

my($result, $command, $TEMP);

# Variables set by the config file
my($templates, $datasets, $commandentry, $discommandentry, $selectstart, $selectentry, $selectend, $plotcache, $tmpdir, $rooturl, $ploturl, $gdms, $temp);

# Setup the CGI module
$result = new CGI();
print $result->header;

# Read in the config file
eval `cat gdms.config` or die "GDMS web could not read it's config file: $@";
print STDERR "Working directory is: ".`pwd`;
$temp = $gdms;
$temp =~ s/\/.*$//;
print STDERR "Changing to: $temp\n";

# Determine what page we are accessing
$command = $result->param('command');
if($command eq ""){
    $command = "main";
}

# Determine if the GDMS script for this page exists already (

print processTemplate("$templates/$command.html");
exit;



###############################################################################
# Find the template file, and then parse it
sub processTemplate(){
    my($file) = @_;
    my($pre, $post, $cmd, $output, $len, $line);

    # This local usage is used to make the TEMPLATE filehandle local to this
    # subroutine...
    local *TEMPLATE;
    print STDERR "Processing template file: $file\n";

    $output = "";
    open TEMPLATE, "< $file" or
	die "GMDS web could not open the template file $file";
    while(<TEMPLATE>){
	# Repeatedly process a line until there are not more template commands
	$line = $_;
	$len = -1;

	while($len != length($line)){
	    $len = length($_);

	    # todo: there must be a better way of doing this...
	    $_ = $line;
	    if(/(.*)&{([^%]*)};(.*)/){
		$pre = $1;
		$cmd = $2;
		$post = $3;
		
		if($cmd eq "commands"){
		    # List the available commands
		    $line = $pre.getCommands().$post;
		}
		elsif($cmd eq "dataset"){
		    # The name of the current dataset
		    $line = $pre.$result->param('dataset').$post;
		}
		elsif($cmd eq "datasets"){
		    # List the datasets in the dataset directory
		    my($temptotal, $tempfile);
		    $temptotal = "";

		    print STDERR "Getting datasets from $datasets\n";
		    open TEMP, "find $datasets -type f -name \"*.dat1\" |";
		    while(<TEMP>){
			my($linecount);
			$tempfile = $_;
			$tempfile =~ s/$datasets\/*//;
			$tempfile =~ s/.dat1\n$//;

			$linecount = `cat $datasets/$tempfile.dat1 | wc -l`;
			$temptotal = $temptotal.
			    substHTML($selectentry, 
				      "<a href=\"$rooturl?command=main&dataset=$tempfile\">$tempfile<\/a> ".
				      "($linecount lines)");
		    }
		    close TEMP;

		    $line = $pre.$temptotal.$post;
		}
		elsif($cmd eq "motd"){
		    # Output a message of the day
		    $line = $pre.processTemplate("$templates/motd.html").$post;
		}
		elsif($cmd eq "northplot"){
		    # A plot in the X direction
		    print STDERR "Plotting in x direction\n";
		    $line = $pre.generateAndLink("x").$post;
		}
		elsif($cmd eq "eastplot"){
		    # A plot in the Y direction
		    $line = $pre.generateAndLink("y").$post;
		}
		elsif($cmd eq "upplot"){
		    # A plot in the Z direction
		    $line = $pre.generateAndLink("z").$post;
		}
	    }
	}
	
	# And now we can print out the resultant line
	$output = $output.$line;
    }

    return $output;
}

# Determine what commands should be available at this time
sub getCommands(){
    my($output, $temp);

    $output = "";
    if($result->param('dataset') eq ""){
	# Dataset open / close
	$output = $output.substHTML($commandentry, "<a href=\"$rooturl?command=open\">Open<\/a>");
	$output = $output.substHTML($discommandentry, "Close");
	
	# Plotting
	$output = $output.substHTML($discommandentry, "Plot");
    }
    else{
	# Datset open / close
	$output = $output.substHTML($discommandentry, "Open");
	$output = $output.substHTML($commandentry, "<a href=\"$rooturl?command=main\">Close<\/a>");

	# Plotting
	if($result->param('command') eq "plot"){
	    $output = $output.substHTML($discommandentry, "Plot");
	}
	else{
	    $output = $output.substHTML($commandentry, "<a href=\"$rooturl?command=plot&dataset=".
					$result->param('dataset')."\">Plot</a>");
	}
    }

    return $output;
}

# Substitute into the HTML stub from the config file
sub substHTML(){
    my($html, $insert) = @_;
    my($temp);

    $temp = $html;
    $temp =~ s/%s/$insert/;
    return $temp;
}

# This subroutine deals with generating plots as required and then outputs
# the HTML needed to link to that image
sub generateAndLink(){
    my($dir) = @_;
    my($file, $unique);
    local *COMMANDS;

    print STDERR "Started generateAndLink()\n";

    # Generate the filename
    $unique = "$$-".time()."-".rand();
    $file = "$plotcache/".$result->param('dataset')."-$dir.png";

    print STDERR "Filename is: $file\n";

    if(! -f $file){
	# We need to generate the image
	print STDERR "Plot cache miss for ".$result->param('dataset')." ($dir)\n";
	open COMMANDS, "> $tmpdir/gdms-$unique.cmd" or 
	    die "Could not open temporary file $tmpdir/gdms-$unique.cmd\n";
	print COMMANDS "open $datasets/".$result->param('dataset')."\n";
	print COMMANDS "plot $dir $file\n";
	close COMMANDS;

	# Execute the gdms main program with this command script
	`$gdms -b $tmpdir/gdms-$unique.cmd` or 
	    die "GDMS execution error for: $gdms -b $tmpdir/gdms-$unique.cmd";
	print STDERR "Return code as $?\n";
    }
    
    # Now link to that image
    return "<img src=\"$ploturl/".$result->param('dataset')."-$dir.png\">";
}
