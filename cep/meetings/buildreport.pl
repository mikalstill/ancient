#!/usr/bin/perl -w

use strict;

my (@list) = `ls week*sgml`;
my ($file);
my ($highest, $count) = (0, 0);

if(stat( "output.sgml")){
    print "output.sgml already exists. To generate a new report please remove it.\n";
    exit(1);
}

open(HEADER, "meetings-header.sgml") || die "Can't open meetings-header.sgml\n";

open(OUT, ">>output.sgml" ) || die "can't open output.sgml\n";

while(<HEADER>){
    print OUT $_;
}

close(HEADER);

foreach $file (@list){
   $file=~m/[0-9]+/;

    if($& > $highest){
        $highest = $&;
    }
}
    
for ($count = 1; $count <= $highest; $count++){
    if(stat("week$count.sgml")){
        open(FILE, "week$count.sgml") || die "can't open week$count.sgml\n";
        while(<FILE>){
            print OUT $_;
        }
        close(FILE);
    }
    else{
        print "missing report for week $count\n";
    }
}

open(FOOTER, "meetings-footer.sgml") || die "Can't open meetings-footer.sgml\n";

while(<FOOTER>){
    print OUT $_;
}

close (FOOTER);
close(OUT);

`docbook2ps output.sgml 2> output.errs`;
`ps2pdf output.ps`;

my $errlen=`cat output.errs | wc -l | tr -d " "`;
if ($errlen > 0){
    print "*** There were $errlen errors generating the meetings report\n";
}
  

   
    
