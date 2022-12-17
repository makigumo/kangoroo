#!/usr/bin/perl

$space = "\xe3\x80\x80";
$komma = "\xe3\x80\x81";
$punkt = "\xe3\x80\x82";
$kauf = "\xe3\x80\x8c";
$kzu = "\xe3\x80\x8d";

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
    if ($f=~/^\.\.\./) {$f=~s/^\.\.\.//; $pp="a";}
    elsif ($f=~/\.\.\.$/) {$f=~s/\.\.\.$//; $pp="z";} else {$pp="";}
    
    $fclean=$f;
    $fclean =~ s/\s//g;
    $fclean =~ s/$space//g;
    $fclean =~ s/$komma//g;
    $fclean =~ s/$punkt//g;
    $fclean =~ s/$kauf//g;
    $fclean =~ s/$kzu//g;
    
    
    $k=~s/\s*\[[^\]]+\]\s*/ /g;
    $k=~s/[\(\),]/;/g;
    @tok = split(/;/,$k);
    $fff=0;$k="";
    for ($i=0;$i<=$#tok;$i++) {
      $t=($tok[$i]);
      $t=~s/^\s*//;
      $t=~s/\s*$//;
      $tclean=$t;
      $tclean =~ s/^\.\.\.//g;
      $tclean =~ s/\.\.\.$//g;
      $tclean =~ s/\s//g;
      $tclean =~ s/$space//g;
      $tclean =~ s/$komma//g;
      $tclean =~ s/$punkt//g;
      $tclean =~ s/$kauf//g;
      $tclean =~ s/$kzu//g;
      if ($t && ($tclean ne $fclean)) {
        if ($t=~/[\x80-\xff]/) {
          if ($fff==0) {$k=$t;} elsif ($fff==1) {$k.=" ($t";} elsif ($fff>=2) {$k.=",$t";}
          $fff++;
        } else {$fff=-1000;$k="";$f="";}  
      } elsif ($t && ($tclean eq $fclean) && !$pp) {
        if ($t=~/^\.\.\./) {$pp="a";} elsif ($t=~/\.\.\.$/) {$pp="z";}
      }
    }
    if ($fff>1) {$k.=")";}
  }  
}  
$r=~s/\[(\d+)\]/(\1)/g;
if ($k || $f) {print "$f\t$k\t$pp\t\t[D]$r\n";}
