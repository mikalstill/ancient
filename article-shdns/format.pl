#!/usr/bin/perl

use strict;
my(%tag, $level, $document, $remainder, $cmd, $args, $pre, $count, $external,
  $EXTERNAL, $sidebarcount);
$sidebarcount = 1;

# NOOPs
$tag{"!DOCTYPE"} = "NOOP";
$tag{"article"} = "NOOP";
$tag{"/article"} = "NOOP";
$tag{"title"} = "NOOP";
$tag{"para"} = "NOOP";
$tag{"/sect1"} = "NOOP";
$tag{"/sect2"} = "NOOP";
$tag{"/sect3"} = "NOOP";
$tag{"/sect4"} = "NOOP";
$tag{"/sect5"} = "NOOP";
$tag{"execute-/cmd"} = "NOOP";
$tag{"execute-args"} = "NOOP";
$tag{"/execute"} = "NOOP";
$tag{"cmd"} = "NOOP";
$tag{"input"} = "NOOP";
$tag{"/listitem"} = "NOOP";

# Tags which do something have an action which is exec'ed
$tag{"article"} = "\$level = \"article\";";
$tag{"sect1"} = "\$level = \"sect1\";";
$tag{"sect2"} = "\$level = \"sect2\";";
$tag{"sect3"} = "\$level = \"sect3\";";
$tag{"sect4"} = "\$level = \"sect4\";";
$tag{"sect5"} = "\$level = \"sect5\";";

$tag{"itemizedlist"} = "print \"[list]\n\";";
$tag{"/itemizedlist"} = "print \"[endlist]\n\n\";";
$tag{"listitem"} = "print \"[dot] \"; \$level = \"listitem\";";

$tag{"quote"} = "\$remainder=\"\$pre\\\"\$remainder\"";
$tag{"/quote"} = "\$remainder=\"\$pre\\\"\$remainder\"";

$tag{"programlisting"} = "print \"[cw]\n\";";
$tag{"/programlisting"} =  "print \"\$pre\n[ecw]\n\n\";";

$tag{"sidebar"} = "\$external = \"\"; \$level = \"sidebar\";";
$tag{"/sidebar"} = "open SIDEBAR, \"> sidebar-\$sidebarcount\";
                    print SIDEBAR \"\$external\";
                    close SIDEBAR;
                    print \"[Please insert the sidebar file named sidebar-\$sidebarcount here]\\n\\n\";
                    \$sidebarcount++;
                    \$level = \"exsidebarhack\"";

$tag{"emphasis"} = "\$remainder=\"\$pre&lt;i&gt;\$remainder\"";
$tag{"/emphasis"} = "\$remainder=\"\$pre&lt;/i&gt;\$remainder\"";

$tag{"command"} = "\$remainder=\"\$pre&lt;b&gt;\$remainder\"";
$tag{"/command"} = "\$remainder=\"\$pre&lt;/b&gt;\$remainder\"";

#####
$tag{"execute"} = "\$level = \"execute\";";
$tag{"execute-/cmd"} = "if(\$pre eq \"eqimg\"){
                         print \"[Please insert the image \";
                         }
                       elsif(\$pre eq \"code2db\"){
                         print \"[Please insert the code file named \";
                         }
                       elsif(\$pre eq \"cat\"){
                         print \"[Please insert the file named \";
                         \$level = \"catexecute\";
                         }
                       else{
                         print \"UNKNOWN EXECUTE COMMAND (\$pre)\n\"
                         }";
$tag{"execute-/args"} = "print \"\$pre here]\n\n\";";
$tag{"execute-/input"} = "print \"\$pre here]\n\n\";";
$tag{"catexecute-/args"} = "print \"\$pre here]\";";
$tag{"catexecute-/input"} = "print \"\$pre here]\";";

#####
$tag{"sidebar-/title"} = 
                  "\$external = \"\$external\$pre\n\";
                   for(\$count=0;\$count < length(\$pre); \$count++){
                     if(\$level eq \"sect1\"){
                       \$external = \"\$external=\";
                       }
                     else{
                       \$external = \"\$external-\";
                       }
                     }
                   \$external = \"\$external\n\n\";";

$tag{"/title"} =  "print \"\$pre\n\";
                   for(\$count=0;\$count < length(\$pre); \$count++){
                     if(\$level eq \"sect1\"){
                       print \"=\";
                       }
                     else{
                       print \"-\";
                       }
                     }
                   print \"\n\n\";";

#####
$tag{"sidebar-/para"} = "\$external = \"\$external\$pre\n\n\";";

$tag{"/para"} =  "print \"\$pre\n\n\";";


$document = "";
while(<>){
  $count = -1;
  while($count != length($_)){
    $count = length($_);
    s/\n$//;
    s/\r$//;
    }

  if($_ ne ""){
    $document = "$document $_";
  }
}

while($document ne ""){
  if($document =~ /([^<]*)<([^>]*)>(.*)/){
#    print "* $2\n";
    $pre = $1;
    $cmd = $2;
    $remainder = $3;

    $pre =~ s/^[ \t]*//;
    $cmd =~ s/ .*//;
    $args = substr($2, length($cmd));
#    print "*\tlevel = $level\n";
#    print "*\tpre = $pre\n";
#    print "*\tcmd = [$cmd]\n";
#    print "*\targs = [$args]\n";

    if($tag{"$level-$cmd"} ne ""){
#      print "\n******* Executing $level-$cmd ***********\n";
      eval $tag{"$level-$cmd"};
    }
    elsif($tag{$cmd} ne ""){
#      print "\n******* Executing $cmd ($level) ***********\n";
      eval $tag{$cmd};
    }
    elsif($tag{$cmd} eq "NOOP"){
    }
    else{
      print "** $cmd: No command string for this tag (level $level)\n";
    }
    $document = $remainder;
  }
}
