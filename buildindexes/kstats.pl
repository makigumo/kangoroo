#!/usr/bin/perl
$dir=$ARGV[0];
open(G,$dir."kkdic.idx");
$kanji=0;
$eheisig=0;
$heisig=0;
$kelem=0;
$bb=0;
$l1=<G>; $l2=<G>; $l3=<G>; <G>;
while( $l1 || $l3 ) {
  ($c)=($l3=~/^(\d+)\D/);
  chop($l3);
  chop($l1);
  chop($l2);
  if ($l3) {
  if ($l1) {
    if ($l1=~/ L\d+ /) {
      ($n)=($l1=~/ L(\d+) /);
      if ($n<=2042) {$eheisig++;} else {$heisig++;}
    } else {
      if ($l2) {$kanji++;} else {$kelem++;}
    }  
  } else {$bb++;}
  }
  $l1=<G>; $l2=<G>; $l3=<G>; <G>;
}  
close(G);
$total=$eheisig+$heisig+$kanji+$kelem+$bb;
print "Statistics: Total=$total EHeisig=$eheisig Heisig=$heisig OtherKanji=$kanji KElem=$kelem BB=$bb\n";
