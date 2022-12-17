#!/usr/bin/perl
while( <STDIN> ) {
  $s=$_;
  ($freq,$f,$k,$p,$t,$r)=(m/^([^\t]*)\t([^\t]*)\t([^\t]*)\t([^\t]*)\t([^\t]*)\t(.*)/);
  if ($p eq "a") {$f="...$f";} elsif ($p eq "z") {$f="$f...";}
  if ($k) {$j="$k [$f]";} else {$j=$f;}
  print "$j/$t$r\n";
}
