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

# This is the broker between the GDMS code and the CGI server. It's 
# role is to parse templates, execute requests, and cache results for 
# speedy processing next time that request is seen...
use strict;
use CGI;

my($result, $command, $TEMP);

# Variables set by the config file
my($templates, $datasets, $matrices, $commandentry, $discommandentry, 
   $selectstart, $selectentry, $selectend, $plotcache, $tmpdir, 
   $rooturl, $ploturl, $gdms, $temp);

# Setup the CGI module
$result = new CGI();

# Read in the config file
eval `cat gdms.config` or die 
    "GDMSweb $$: Could not read config file: $@";
$temp = $gdms;
$temp =~ s/\/.*$//;

# Determine what page we are accessing
$command = $result->param('command');
if($command eq ""){
    $command = "main";
}

# Some pages just do some processing and then immediately redirect to 
# another page. I need to deal with these before I can display the 
# page headers
print STDERR "GDMSweb $$: Processing $command\n";
print STDERR "GDMSweb $$: Dataset is permanent\n" 
    if($result->param('intype') eq "");
print STDERR "GDMSweb $$: Dataset is temporary\n" 
    if($result->param('intype') ne "");
 
if($command eq "lsprocess"){
    $temp = "$rooturl?dataset=".$result->param('dataset')."-ls".
	$result->param('reweight');

    # If we are using manual reweighting, then we need to get more 
    # information
    if($result->param('reweight') ne "auto"){
	$temp = $temp."&command=lsreweight&dir=north";
    }
    else{
	$temp = $temp."&command=lsselect&data=".
	    $result->param('dataset')."-ls".
	    $result->param('reweight')."-data&residuals=".
	    $result->param('dataset')."-ls".
	    $result->param('reweight')."-residuals";
	
	# Do the processing here
	performLs("auto");
    }

    if($result->param('desc') ne ""){
	$temp = $temp."&desc=".$result->param('desc');
    }
    $temp = $temp."&intype=temp";

    # And now bounce of to the real page
    print STDERR "GDMSweb $$: Redirecting after automatic LS to: $temp\n";
    print $result->redirect($temp);
    exit;
}
elsif($command eq "lsprocesstwo"){
    $temp = "$rooturl?dataset=".$result->param('dataset')."-ls".
	$result->param('reweight');

    $temp = $temp."&command=lsselect&data=".$result->param('dataset').
	"-ls".$result->param('reweight')."-data&residuals=".
	$result->param('dataset')."-ls".
	$result->param('reweight')."-residuals";
    
    # Do the processing here
    performLs($result->param('lsargs'));

    if($result->param('desc') ne ""){
	$temp = $temp."&desc=".$result->param('desc');
    }
    $temp = $temp."&intype=temp";

    # And now bounce of to the real page
    print STDERR "GDMSweb $$: Redirecting after manual LS to: $temp\n";
    print $result->redirect($temp);
    exit;
}
elsif($command eq "windowprocess"){
    $temp = "$rooturl?dataset=".$result->param('dataset')."-win".
	$result->param('wtype')."&command=plot";
    if($result->param('desc') ne ""){
	$temp = $temp."&desc=".$result->param('desc');
    }
    $temp = $temp."&intype=temp";

    # Do the processing here
    performWindow();

    # And now bounce of to the real page
    print STDERR "GDMSweb $$: Redirecting after window to: $temp\n";
    print $result->redirect($temp);
    exit;
}
elsif($command eq "interpprocess"){
    $temp = "$rooturl?dataset=".$result->param('dataset')."-interp".
	$result->param('itype')."&command=plot";
    if($result->param('desc') ne ""){
	$temp = $temp."&desc=".$result->param('desc');
    }
    $temp = $temp."&intype=temp";

    # Do the processing here
    performInterpolation();

    # And now bounce of to the real page
    print STDERR "GDMSweb $$: Redirecting after interpolation to: $temp\n";
    print $result->redirect($temp);
    exit;
}
elsif($command eq "fftprocess"){
    $temp = "$rooturl?dataset=".$result->param('dataset').
	"-fft&command=plot";
    if($result->param('desc') ne ""){
	$temp = $temp."&desc=".$result->param('desc');
    }
    $temp = $temp."&intype=temp";

    # Do the processing here
    performFFT();

    # And now bounce of to the real page
    print STDERR "GDMSweb $$: Redirecting after FFT to: $temp\n";
    print $result->redirect($temp);
    exit;
}

# Determine if the GDMS script for this page exists already
print $result->header;
print processTemplate("$templates/$command.html");
exit;



######################################################################
# Find the template file, and then parse it
sub processTemplate(@args){
    my($file) = @_;
    my($pre, $post, $cmd, $output, $len, $line);

    # This local usage is used to make the TEMPLATE filehandle local 
    # to this subroutine...
    local *TEMPLATE;
    print STDERR "GDMSweb $$: Processing template file: $file\n";

    $output = "";
    if(!open TEMPLATE, "< $file"){
	print STDERR "GDMSweb $$: $file not found\n";
	open TEMPLATE, "< $templates/commandnotfound.html";
    }
    while(<TEMPLATE>){
	# Repeatedly process a line until there are not more template 
	# commands
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
		    if($result->param('desc') eq ""){
			$line = $pre.$result->param('dataset').$post;
		    }
		    else{
			$line = $pre.$result->param('desc').$post;
		    }
		}
		elsif($cmd eq "datasets"){
		    # List the datasets in the dataset directory
		    my($temptotal, $tempfile);
		    $temptotal = "";

		    open TEMP, "find $datasets -type f -name \"*.dat1\" |";
		    while(<TEMP>){
			my($linecount);
			$tempfile = $_;
			$tempfile =~ s/$datasets\/*//;
			$tempfile =~ s/.dat1\n$//;

			$linecount = `cat $datasets/$tempfile.dat1 | wc -l`;
			$temptotal = $temptotal.
			    substHTML($selectentry, 
				      "<a href=\"$rooturl?".
				      "command=plot&".
				      "dataset=$tempfile\">".
				      $tempfile."<\/a> ".
				      "($linecount lines)");
		    }
		    close TEMP;
		    
		    $line = $pre.$temptotal.$post;
		}
		elsif($cmd eq "motd"){
		    # Output a message of the day
		    $line = $pre.processTemplate("$templates/motd.html").
			$post;
		}
		elsif($cmd eq "northplot"){
		    # A plot in the X;
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
		elsif($cmd eq "lsparams"){
		    # Output a HTML form for the parameters of a LS 
		    # regression
		    $line = $pre.generateForm("lsprocess", 
					      $result->param('dataset'), 
					      "$templates/lsparams.html").
						  $post;
		}
		elsif($cmd eq "lsselect"){
		    $line = $pre."<a href=\"$rooturl?dataset=".
			$result->param('data').
			"&command=plot&intype=temp\">Data</a><br>".
			"<a href=\"$rooturl?dataset=".
			$result->param('residuals').
			"&command=plot&intype=temp\">Residuals</a>".
			$post;
		}
		elsif($cmd eq "lsreweightdir"){
		    $line = $pre.$result->param('dir').$post;
		}
		elsif($cmd eq "winparams"){
		    # Output a HTML form for the parameters of a 
		    # windowing operation
		    $line = $pre.generateForm("windowprocess", 
					      $result->param('dataset'), 
					      "$templates/winparams.html").
						  $post;
		}
		elsif($cmd eq "interpparams"){
		    # Output a HTML form for the parameters of a
		    # interpolation operation
		    $line = $pre.generateForm("interpprocess", 
					      $result->param('dataset'), 
					      "$templates/interpparams.html").
						  $post;
		}
		elsif($cmd eq "matrices"){
		    # List the datasets in the dataset directory
		    my($temptotal, $next);
		    $temptotal = "";

		    if($result->param('dir') eq "north"){
			$next = "command=lsreweight&dir=east";
		    }
		    elsif($result->param('dir') eq "east"){
			$next = "command=lsreweight&dir=up";
		    }
		    else{
			$next = "command=lsprocesstwo&lsreweight=manual";
		    }

		    open TEMP, "find $matrices -type f -name \"*.mat\" |";
		    while(<TEMP>){
			chomp;
			s/$matrices//;

			$temptotal = $temptotal.
			    substHTML($selectentry, 
				      "<a href=\"$rooturl?$next&lsargs=".
				      $result->param('lsargs').
				      "$matrices/$_,\">$_<\/a>");
		    }
		    close TEMP;
		    
		    $line = $pre.$temptotal.$post;
		}
	    }
	}
	
	# And now we can print out the resultant line
	$output = $output.$line;
    }

    return $output;
}

# Determine what commands should be available at this time
sub getCommands(@args){
    my($output, $temp);

    $output = "";
    # If there is no current dataset
    if($result->param('dataset') eq ""){
	# Dataset open
	$output = $output.substHTML($commandentry, 
				    "<a href=\"$rooturl?command=open\">Open<\/a>");
	
	# Maths
	$output = $output.substHTML($discommandentry, 
				    "Least squares");
	$output = $output.substHTML($discommandentry, "Windowing");
	$output = $output.substHTML($discommandentry, "Interpolation");
	$output = $output.substHTML($discommandentry, "FFT");
    }
    else{
	# Datset open
	$output = $output.substHTML($commandentry, 
				    "<a href=\"$rooturl?command=open\">Open<\/a>");

	# Maths
	$temp = "<a href=\"$rooturl?command=ls&dataset=".
	    $result->param('dataset');
	$temp = $temp."&intype=temp" 
	    if($result->param('intype') ne "");
	$temp = $temp."\">Least squares</a>";
	$output = $output.substHTML($commandentry, $temp);

	$temp = "<a href=\"$rooturl?command=window&dataset=".
	    $result->param('dataset');
	$temp = $temp."&intype=temp" 
	    if($result->param('intype') ne "");
	$temp = $temp."\">Windowing</a>";
	$output = $output.substHTML($commandentry, $temp);

	$temp = "<a href=\"$rooturl?command=interp&dataset=".
	    $result->param('dataset');
	$temp = $temp."&intype=temp" 
	    if($result->param('intype') ne "");
	$temp = $temp."\">Interpolation</a>";
	$output = $output.substHTML($commandentry, $temp);

	$temp = "<a href=\"$rooturl?command=fftprocess&dataset=".
	    $result->param('dataset');
	$temp = $temp."&intype=temp" 
	    if($result->param('intype') ne "");
	$temp = $temp."\">FFT</a>";
	$output = $output.substHTML($commandentry, $temp);
    }

    return $output;
}

# Substitute into the HTML stub from the config file
sub substHTML(@args){
    my($html, $insert) = @_;
    my($temp);

    $temp = $html;
    $temp =~ s/%s/$insert/;
    return $temp;
}

# This subroutine deals with generating plots as required and then 
# outputs the HTML needed to link to that image
sub generateAndLink(@args){
    my($dir) = @_;
    my($file, $unique);
    local *COMMANDS;

    # Generate the filename
    $unique = "$$-".time()."-".rand();
    $file = "$plotcache/".$result->param('dataset')."-$dir.png";

    if(! -f $file){
	# We need to generate the image
	print STDERR "GDMSweb $$: Plot cache miss for ".
	    $result->param('dataset')." ($dir)\n";
	open COMMANDS, "> $tmpdir/gdms-$unique.cmd" or 
	    die "Could not open temporary file $tmpdir/gdms-$unique.cmd\n";

	# Open the dataset. It might be in a temporary location
	if($result->param('intype') ne "temp"){
	    print COMMANDS "open $datasets/".
		$result->param('dataset')."\n";
	}
	else{
	    print COMMANDS "open $tmpdir/".
		$result->param('dataset')."\n";
	}

	print COMMANDS "plot $dir $file\n";
	close COMMANDS;

	# Execute the gdms main program with this command script
	`$gdms -b $tmpdir/gdms-$unique.cmd`;
	print STDERR "GDMSweb $$: GDMS execute return code is $?\n";
    }
    
    # Now link to that image
    return "<img src=\"$ploturl/".$result->param('dataset').
	"-$dir.png\">";
}

# Generate a HTML form for the required paramers
sub generateForm(@args){
    my($command, $dataset, $passedform) = @_;
    my($resultstring, $DUMPFILE);

    # Start the form
    $resultstring = "<form method=\"post\" action=\"$rooturl\" ";
    $resultstring = $resultstring.
	"enctype=\"application/x-www-form-urlencoded\">";
    $resultstring = $resultstring.
	"<input type=\"hidden\" name=\"command\" value=\"$command\" />";
    $resultstring = $resultstring.
	"<input type=\"hidden\" name=\"dataset\" value=\"$dataset\" />";
    if($result->param('intype') ne ""){
	print STDERR "Continuing temporary tag\n";
	$resultstring = $resultstring.
	    "<input type=\"hidden\" name=\"intype\" value=\"temp\" />";
    }

    $resultstring = $resultstring.processTemplate("$passedform");
    
    # End the form
    $resultstring = $resultstring."<BR><BR><div align=\"center\">";
    $resultstring = $resultstring.
	"<input type=\"submit\" name=\"commit\" value=\" OK \" /></div></form>";

    return $resultstring;
}

# Create a dataset in the temp location which is the output of a LS 
# regression
sub performLs(){
    my($lsargs) = @_;
    my($unique);
    local *COMMANDS;

    print STDERR "GDMSweb $$: Performing LS regression\n";
    $unique = "$$-".time()."-".rand();
    open COMMANDS, "> $tmpdir/gdms-$unique.cmd" or 
	die "Could not open temporary file $tmpdir/gdms-$unique.cmd\n";
    
    # Open the dataset. It might be in a temporary location
    if($result->param('intype') ne "temp"){
	print COMMANDS "open $datasets/".$result->param('dataset')."\n";
    }
    else{
	print COMMANDS "open $tmpdir/".$result->param('dataset')."\n";
    }
    
    print COMMANDS "ls $lsargs $tmpdir/".$result->param('dataset')."-ls".
	$result->param('reweight')."-data $tmpdir/".
	$result->param('dataset')."-ls".
	$result->param('reweight')."-residuals\n";
    close COMMANDS;
    
    # Execute the gdms main program with this command script
    `$gdms -b $tmpdir/gdms-$unique.cmd`;
    print STDERR "GDMSweb $$: GDMS return code is $?\n";
}

# Create a dataset in the temp location which is the output of a 
# windowing operation
sub performWindow(){
    my($unique);
    local *COMMANDS;

    print STDERR "GDMSweb $$: Performing window operation\n";
    $unique = "$$-".time()."-".rand();
    open COMMANDS, "> $tmpdir/gdms-$unique.cmd" or 
	die "Could not open temporary file $tmpdir/gdms-$unique.cmd\n";
    
    # Open the dataset. It might be in a temporary location
    if($result->param('intype') ne "temp"){
	print COMMANDS "open $datasets/".$result->param('dataset')."\n";
    }
    else{
	print COMMANDS "open $tmpdir/".$result->param('dataset')."\n";
    }
    
    print COMMANDS "window ".$result->param('wtype')." ".
	$result->param('wsize')." ".$result->param('woverlap').
	" $tmpdir/".
	$result->param('dataset')."-win".$result->param('wtype')."\n";
    close COMMANDS;
    
    # Execute the gdms main program with this command script
    `$gdms -b $tmpdir/gdms-$unique.cmd`;
    print STDERR "GDMSweb $$: GDMS return code is $?\n";
}

# Create a dataset in the temp location which is the output of 
# interpolation operation
sub performInterpolation(){
    my($unique);
    local *COMMANDS;

    print STDERR "GDMSweb $$: Performing interpolation\n";
    $unique = "$$-".time()."-".rand();
    open COMMANDS, "> $tmpdir/gdms-$unique.cmd" or 
	die "Could not open temporary file $tmpdir/gdms-$unique.cmd\n";
    
    # Open the dataset. It might be in a temporary location
    if($result->param('intype') ne "temp"){
	print COMMANDS "open $datasets/".$result->param('dataset')."\n";
    }
    else{
	print COMMANDS "open $tmpdir/".$result->param('dataset')."\n";
    }
    
    print COMMANDS "interp ".$result->param('itype')." ".
	$result->param('isamplerate')." $tmpdir/".
	$result->param('dataset')."-interp".$result->param('itype')."\n";
    close COMMANDS;
    
    # Execute the gdms main program with this command script
    `$gdms -b $tmpdir/gdms-$unique.cmd`;
    print STDERR "GDMSweb $$: GDMS return code is $?\n";
}

# Create a dataset in the temp location which is the output of the FFT
sub performFFT(){
    my($unique);
    local *COMMANDS;

    print STDERR "GDMSweb $$: Performing FFT\n";
    $unique = "$$-".time()."-".rand();
    open COMMANDS, "> $tmpdir/gdms-$unique.cmd" or 
	die "Could not open temporary file $tmpdir/gdms-$unique.cmd\n";
    
    # Open the dataset. It might be in a temporary location
    if($result->param('intype') ne "temp"){
	print COMMANDS "open $datasets/".$result->param('dataset')."\n";
    }
    else{
	print COMMANDS "open $tmpdir/".$result->param('dataset')."\n";
    }
    
    print COMMANDS "fft $tmpdir/".
	$result->param('dataset')."-fft\n";
    close COMMANDS;
    
    # Execute the gdms main program with this command script
    `$gdms -b $tmpdir/gdms-$unique.cmd`;
    print STDERR "GDMSweb $$: GDMS return code is $?\n";
}
