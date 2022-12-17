#!/usr/bin/perl
while( <STDIN> ) {
  $s=$_;
  if ($s=~/\\n/) {$s=~s/\\n//g; $r.=" $s"; $r=~s/\s*$//;} else {
    $r=~s/\[(\d+)\]/(\1)/g;
    if ($k || $f) {print "$f\t$k\t$pp\t\t[D]$r\n";}  
    ($k,$f,$r)= ($s=~/^([^\t]+)\t([^\t]+)\t(.*)/);
    $r=~s/^\s*//;
    $r=~s/\s*$//;
    
    $f=~s/\s*\[[^\]]+\]\s*/ /g;
    $f=~s/^\s*//;
    $f=~s/\s*$//;
    $ff=$f;
    if ($f=~/^\.\.\./) {$f=~s/^\.\.\.//; $pp="a";}
    elsif ($f=~/\.\.\.$/) {$f=~s/\.\.\.$//; $pp="z";} else {$pp="";}
    
    $k=~s/\s*\[[^\]]+\]\s*/ /g;
    $k=~s/[\(\),]/;/g;
    @tok = split(/;/,$k);
    $fff=0;$k="";
    for ($i=0;$i<=$#tok;$i++) {
      $t=($tok[$i]);
      $t=~s/^\s*//;
      $t=~s/\s*$//;
      if ($t && ($t ne $ff)) {
        if ($t=~/[\x80-\xff]/) {
          if ($fff==0) {$k=$t;} elsif ($fff==1) {$k.=" ($t";} elsif ($fff>=2) {$k.=",$t";}
          $fff++;
        } else {$fff=-1000;$k="";$f="";}  
      }
    }
    if ($fff>1) {$k.=")";}
  }  
}  
$r=~s/\[(\d+)\]/(\1)/g;
if ($k || $f) {print "$f\t$k\t$pp\t\t[D]$r\n";}
