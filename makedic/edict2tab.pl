#!/usr/bin/perl
$hiraa = "\xe3\x81\x81";
$kataa = "\xe3\x82\xa1";
$katavu = "\xe3\x83\xb4";
while( <STDIN> ) {
  ($j,$r)= (m/^([^\/]+)\/(.*)/);
  $j=~s/\s+$//;
  $r=~s/^\s+//;
  $r=~s/\s+$//;
  ($k,$f)= ($j=~/(\S+)\s*\[(\S+)\]/);
  if (!$f) {
    $k=$j; $f=$j;
    for ($i=0;$i<length($f);$i++) {
      if (substr($f,$i,3) ge $kataa && substr($f,$i,3) le $katavu) {
        $d=64*(ord(substr($f,$i+1,1))-ord(substr($kataa,1,1)))+ord(substr($f,$i+2,1))-ord(substr($kataa,2,1));
        substr($f,$i,3)=$hiraa;
        if ($d<=62) {substr($f,$i+2,1)=chr(129+$d);} else 
          { substr($f,$i+1,1)=chr(130); substr($f,$i+2,1)=chr(128+$d-63); }
      } 
    }
    if ($f eq $k) {$k="";}
  }  
  ($t,$e)= ($r=~/^(\([a-z][^\)]*\))\s*(\S.*)/);
  if (!$t) {$e=$r;}
  print "$f\t$k\t\t$t\t[E]$e\n";  
}  
