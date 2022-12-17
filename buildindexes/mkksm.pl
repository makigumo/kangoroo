#!/usr/bin/perl
$dir=$ARGV[0];
open(G,$dir."kkdic.idx");
@meta=();
$pos=0;
$l1=<G>; <G>; $l3=<G>; <G>;
while( $l1 || $l3 ) {
  ($c)=($l3=~/^(\d+)\D/);
  if ($c) {push(@meta,"$c\t$pos\n");}
  $pos=tell(G); $l1=<G>; <G>; $l3=<G>; <G>;
}  
close(G);
@meta=sort {
     ($pa)=($a=~/^(\d+)\t/);
     ($pb)=($b=~/^(\d+)\t/);
     return $pa<=>$pb;
   } @meta;
print @meta;
