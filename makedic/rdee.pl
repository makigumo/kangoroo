#!/usr/bin/perl
while( <STDIN> ) {
  $s=$_;
  ($j,$t,$r)=(m/^([^\t]*\t[^\t]*)\t\t([^\t]*)\t(.*)/);
  if ($j eq $jpre) 
  {
    if ($rpre=~/\/\(P\)\//) {$r=$rpre;$t=$tpre;} elsif ($r=~/\/\(P\)\//) {} else {
      $r=~s/^\[\w\]//; 
      $r="$rpre$r";
    }
  } else {if ($jpre) {print "$jpre\t\t$tpre\t$rpre\n";}}
  $jpre=$j;
  $rpre=$r;
  $tpre=$t;
}  
print "$jpre\t\t$tpre\t$rpre\n";
