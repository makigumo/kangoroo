#!/usr/bin/perl
$name=$ARGV[0];
$dir=$ARGV[1];
open(G,"$dir$name.idx");
$c="";
$pos=0;
$s=<G>;
while( $s ) {
  chop($s);
  if ($name=~"de") {$cc=substr($s,0,2);} else {$cc=substr($s,0,3);}
  if ($cc && ($c ne $cc)) {$c=$cc;print "$c\t$pos\n";}  
  if ($name=~"dic") {<G>; <G>; <G>;} $pos=tell(G); $s=<G>;
}  
close(G);
