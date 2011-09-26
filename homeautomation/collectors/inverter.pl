#!/usr/bin/perl -w
# inverter.pl - polls data from the RS232 port on CMS-2000 inverters
# and outputs to text.
# Should also work with similar inverters, eg Orion et al.
# (c) 2010 jinba @ jinba-ittai.net
# License: GPL v2

$|=1; # don't let Perl buffer I/O

use Device::SerialPort;

# serial port parameters
$port = "/dev/inverter"; 
$baud = "9600";


# output parsed data to this text file
$logfile="/var/www/inverter.txt";

# This appears to tell the inverter to start
# the init handshake back from the beginning.
$resetstring="aaaa010000000004000159";

# First step of the init handshake - request the serial
$getserial="aaaa010000000000000155";

# The inverter responds with its serial number
# with a packet starting with this: 
$serialregex="aaaa0000010000800a";

# We reply back to the inverter with a string that looks like this
# which includes the serial number again (minus the spaces)
# aaaa0100000000010b serialnum 01 checksum";
# eg on mine:
# aaaa0100000000010b30393039424f3034353001038e";
$confirmserial="aaaa0100000000010b";
# checksum of the above, plus the trailing 01
$confirmsum=hex(162);

# The inverter replies again with a packet starting with this
$confserregex="aaaa000101000081";

# This is the packet we send on a regular basis
# polling for data:
$requestdata="aaaa010000010102000159";

# The data packet starts with this:
$dataregex="aaaa00010100018228";


# Open the serial port
$serial = Device::SerialPort->new ($port, 0, '') || die "Can\'t open $port: $!";
$serial->baudrate($baud) || die 'fail setting baudrate, try -b option';
$serial->parity("none") || die 'fail setting parity to none';
$serial->databits(8) || die 'fail setting databits to 8';
$serial->stopbits(1) || die 'fail setting stopbits to 1';
$serial->handshake("none") || die 'fail setting handshake to none';
$serial->datatype('raw') || die 'fail setting datatype raw';
$serial->write_settings || die 'could not write settings';
$serial->error_msg(1); # use built-in error messages
$serial->user_msg(1);

$serial->read_char_time(0);     # don't wait for each character
$serial->read_const_time(1000); # 1 second per unfulfilled "read" call

# function to turn the raw data from the inverter into a string of hex
sub fromdata() {
    my $pstring=shift;
    my $hstring=unpack ("H*",$pstring);
    return $hstring;
}

# and turn hex back into raw data for transmission to the inverter
sub todata() {
  my $hstring=shift;
  my $pstring = pack (qq{H*},qq{$hstring});
  return $pstring;
}

# read any response received by the inverter
sub readbuf() {
  my $buf="";
  my ($count,$saw)=$serial->read(255); # will read _up to_ 255 chars
  if ($count > 0) { 
    $buf=$saw;
  }
  return $buf;
}

# grab the serial number from the input data
# and use it to create the response packet, including checksum 
sub calcserial() {
  my $datastring=shift;
  my $serstrtxt=substr($datastring, 18, 20); # serial # is byte 9 through 18
  my $serstr=&todata($serstrtxt);
  my $sersum=unpack ("%C*",$serstr); # calculate the checksum of the serial #
  print "checksum $sersum\n";
  
  my $sertext=&fromdata($serstr); # the serial number in hex
  my $checksum=sprintf ("%04x ", $confirmsum+$sersum); # the checksum in hex
  # and join it all together to get the response
  my $responsetext=$confirmserial .  $sertext . "01" . $checksum;
  return($responsetext);
}

# initial handshake is done here
sub initialise() {
  # step 1: request the serial number
  $resp=&todata($getserial);
  my $gotserial=0;
  my $string;
  my $loopcount=0;
  do {
    print "sending serial request: $getserial\n";
    $serial->write($resp);
    $string=&fromdata(&readbuf());
    print "seen: $string\n";
    if ($string =~ /^$serialregex/) {
      $gotserial=1 
    } else {
      sleep 5;  
      if ($loopcount eq 2) { 
        print "no serial number after 2 tries. sending reset: $resetstring\n";
        my $poke=&todata($resetstring);
        $serial->write($poke);
        sleep 1;
        $loopcount=0;
      }
    }
    $loopcount++;
  } until ($gotserial);
  print "Serial found: $string\n";
  
  # Now for step 2: confirm the serial number
  $serialresponse=&calcserial($string); 
  print".. responding with $serialresponse\n";
  $resp=&todata($serialresponse);
  my $gotresponse=0;
  my $string2;
  $loopcount=0;
  do {
    sleep 2;
    print "confirming serial number: $confirmserial\n";
    $serial->write($resp);
    $string2=&fromdata(&readbuf());
    print "seen: $string2\n";
    if ($string2 =~ /^$confserregex/) {
      $gotresponse=1 
    } else {
      if ($loopcount eq 2) { 
        print "no confirmation after 2 tries. starting again\n";
        &initialise;
      }
      sleep 30; 
    }
    $loopcount++;
  } until ($gotresponse);
  print "Response found: $string2\n";
}

sub parsedata() {
  print "parsing data...\n";
  my $data=shift;
  my @d = (split //, $data);
  $temp=(hex(&fromdata($d[9]) . &fromdata($d[10]))) / 10;
  $Vdc=(hex(&fromdata($d[11]) . &fromdata($d[12]))) / 10;
  $Iac=(hex(&fromdata($d[13]) . &fromdata($d[14]))) / 10;
  $Vac=(hex(&fromdata($d[15]) . &fromdata($d[16]))) / 10;
  $Fac=(hex(&fromdata($d[17]) . &fromdata($d[18]))) / 100;
  $Pac=hex(&fromdata($d[19]) . &fromdata($d[20]));
  $kWh=(hex(&fromdata($d[25]) . &fromdata($d[26]))) /10;
  $run=hex(&fromdata($d[29]) . &fromdata($d[30]));
  $mod=hex(&fromdata($d[31]) . &fromdata($d[32]));

  print "temp: $temp deg C\n";
  print "Vdc: $Vdc V\n";
  print "Iac: $Iac A\n";
  print "Vac: $Vac V\n";
  print "Fac: $Fac Hz\n";
  print "Pac: $Pac W\n";
  print "kWh: $kWh kWh\n";
  print "run: $run hours\n";
  print "mod: $mod\n";

  my $now=time;
  open (LOGFILE, ">$logfile");
  print LOGFILE "$now,$temp,$Vdc,$Iac,$Vac,$Fac,$Pac,$kWh,$run,$mod\n";
  close (LOGFILE);
  #system("/usr/bin/rrdtool", "update", "$rrdfile", "$now,$temp,$Vdc,$Iac,$Vac,$Fac,$Pac,$mod");
  sleep 15;
}

print "Starting up...\n";
#&initialise; # temp

my $pollcount=0;

# The main loop starts here
while (1) {
  sleep 1; ######
  $pollcount++;
  if ($pollcount % 20 == 0) {
    &initialise;
  }
  # Request Data
  $resp=&todata($requestdata);
  print "sending $requestdata, poll $pollcount\n";
  $serial->write($resp);

  my $buffer=&readbuf();
  next if $buffer eq ""; # restart loop on empty buffer

  my $hex=&fromdata($buffer);
  print "hex: $hex\n";

  if ($hex =~ /^$dataregex/) {
    $pollcount=0;
    &parsedata($buffer);
  }
}    

