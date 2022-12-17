#!/usr/bin/perl
while( <STDIN> ) {
  $s=$_;
  ($j,$p,$t,$r)=(m/^([^\t]*\t[^\t]*)\t([^\t]*)\t([^\t]*)\t(.*)/);
  if ($j eq $jpre) 
  {
    if ($ppre) {$p=$ppre;}
    if ($r=~/\[E\]/) {$r=$rpre.$r;} else {$r=$r.$rpre;}
  } else {if ($jpre) {print "$jpre\t$ppre\t$tpre\t$rpre\n";}}
  $jpre=$j;
  $rpre=$r;
  $tpre=$t;
  $ppre=$p;
}  
print "$jpre\t$ppre\t$tpre\t$rpre\n";
