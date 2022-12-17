#!/usr/bin/perl

open(F, "wordfreq/wordfreq_ck.utf8");

%wf=();

while( <F> ) {
  $s=$_;
  $s=~s/^\s*//;
  $s=~s/\s*$//;    
  if ($s && substr($s,0,1) ne "#") {
    ($k,$f)=($s=~/^(\S+)\s(\d+)/);
    $wf{$k}=$f;
  }
}
close(F);

while( <STDIN> ) {
  $s=$_;
  ($f,$j)=(m/^([^\t]*)\t([^\t]*)\t/);
  if (!$j) {$j=$f;}
  $j=~s/[\(\),\.\s]/ /g;
  $j=~s/\s+/ /g;
  $j=~s/^\s*//;
  $j=~s/\s*$//;    
  $f=0;
  $c="A";
  if ($s=~/\/\(P\)\//) {$c="B";}
  @tok = split(/ /,$j);
  for ($i=0;$i<=$#tok;$i++) {
    $t=($tok[$i]);
    if ($t && $wf{$t} && $wf{$t}>$f) {$f=$wf{$t};}
  }
  while (length($f)<7) {$f="0".$f;}
  print "$c$f\t$s";
}  
