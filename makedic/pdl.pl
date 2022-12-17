#!/usr/bin/perl
while( <STDIN> ) {
  $s=$_;
  ($j)=(m/^([^\t]*\t[^\t]*)\t/);
  if ($j eq $jpre) {if (!$pr) {print "\n$spre";} $pr=1; print $s;} else {$pr="";}
  $jpre=$j;
  $spre=$s;
}  
