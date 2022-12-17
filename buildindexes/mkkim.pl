#!/usr/bin/perl
$code=$ARGV[0];
$dir=$ARGV[1];
open(G,$dir."kkdic.idx");
@meta=();
$pos=0;
$s=<G>;
while( $s ) {
  ($c)=($s=~/\s$code(\d+)\s/);
  if ($c) {push(@meta,"$c\t$pos\n");}
  <G>; <G>; <G>; $pos=tell(G); $s=<G>;
}  
close(G);
@meta=sort {
     ($pa)=($a=~/^(\d+)\t/);
     ($pb)=($b=~/^(\d+)\t/);
     return $pa<=>$pb;
   } @meta;
print @meta;
