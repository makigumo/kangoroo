#!/usr/bin/perl
$dir=$ARGV[0];
open(G,$dir."dic.utf8");
$com=1;
$pos=0;
$s=<G>;
while( $s ) {
  if ($com && !($s=~/\/\(P\)\//)) {$com="";print "$pos\n";}
  $pos=tell(G); $s=<G>;
}  
print tell(G)."\n";
close(G);
