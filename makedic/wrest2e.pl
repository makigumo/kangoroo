#!/usr/bin/perl
while( <STDIN> ) {
  $s=$_;
  ($j,$r)=(m/^([^\t]*\t[^\t]*\t[^\t]*)\t\t(.*)/);
  $r=~s/; /\//g;
  $r=~s/\s*\/+\s*/\//g;
#   $r=~s/\. /\//g;
  $r=~s/\.$/\//;
  if (!$r=~/\/$/) {$r.="/";}
  print "$j\t\t$r\n";
}  
