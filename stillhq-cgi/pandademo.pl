#!/usr/bin/perl

# Make a demo PDF using some simple biblio information and an image

use strict;

my($applicant, $id, $address, $PANDASCRIPT, $INPUTIMAGE, $image, $imagetype);
$applicant="Nerk enterprises P/L";
$id=1234567;
$address="12 Wibble Way, Wallaroo";

# Print out the PDF header for the web server so that the browser can work out what to do
print "Content-type: application/pdf\n\n";

# Start PandaScript
open(PANDASCRIPT, "| /usr/local/bin/pandascript") || die "Could not start pandascript ($!)";
print PANDASCRIPT "output \"-\"\nnewpage\n";

# Determine if we are using a PNG or a TIFF
open(INPUTIMAGE, "< ./$id.1") || die "Could not open input image ($!)";
$image=<INPUTIMAGE>;
close(INPUTIMAGE);

if((137 == ord(substr($image, 0, 1))) && (80 == ord(substr($image, 1, 1))) && (78 == ord(substr($image, 2, 1))) && (71 == ord(substr($image, 3, 1)))){
    $imagetype = "PNGIMG";
}
else{
    $imagetype = "TIFFIMG";
}

# Now build a simple little PDF file
print PANDASCRIPT "setfontmode 0";
print PANDASCRIPT "setcharacterspacing 0.0";
print PANDASCRIPT "setwordspacing 0.0";
print PANDASCRIPT "sethorizontalscaling 0.87";
print PANDASCRIPT "setleading 16.0";
print PANDASCRIPT "textbox 10 10 100 100 \"Applicant: $applicant\"\n";
print PANDASCRIPT "textbox 25 10 100 100 \"Trade Mark Number: $id\"\n";
print PANDASCRIPT "textbox 40 10 100 100 \"Address: $address\"\n";
print PANDASCRIPT "imagebox 100 100 324 324 \"$id.1\" $imagetype\n";
print PANDASCRIPT "close\n";

# Done
