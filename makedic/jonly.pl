#!/usr/bin/perl
# $hiraa = "\xe3\x81\x81";
# $kataa = "\xe3\x82\xa1";
# $katavu = "\xe3\x83\xb4";
while( <STDIN> ) {
  ($j)=(m/^([^\t]*\t[^\t]*)\t/);
  print "$j\n";
}  
