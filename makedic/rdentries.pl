#!/usr/bin/perl
while( <STDIN> ) {
  $s=$_;
  ($j,$p,$r)=(m/^([^\t]*\t[^\t]*)\t([^\t]*)\t\t(.*)/);
  if ($j eq $jpre) 
  {
    $r=~s/^\[\w\]/\//; 
    if ($ppre) {$p=$ppre;}
    if (substr($j,0,3) eq "...") {$r=$rpre;}
    elsif (substr($r,1,7) eq substr($rpre,3,7)) {$r=$rpre;}
    elsif (substr($r,0,8) eq "/{Persoe") {$r=$rpre;} 
    elsif (substr($r,0,8) eq "/{Famili") {$r=$rpre;} 
    elsif (substr($r,0,8) eq "/{Ortsn.") {$r=$rpre;} 
    elsif (substr($r,0,8) eq "/{Firmen") {$r=$rpre;} 
    else {$r="$rpre$r";}
  } else {if ($jpre) {print "$jpre\t$ppre\t\t$rpre\n";}}
  $jpre=$j;
  $rpre=$r;
  $ppre=$p;
}  
print "$jpre\t$ppre\t\t$rpre\n";
