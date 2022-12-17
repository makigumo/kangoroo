#!/usr/bin/perl
$name=$ARGV[0];
$dir=$ARGV[1];
open(G,"$dir$name.idx");
$s=<G>;
while( <G> ) {
  ($first,$rest)= (/(\S+)(\t.*)/);
  $rest=~s/\S//g;
  if (length($rest)>=999) {print "$first\n";}
}  
close(G);
