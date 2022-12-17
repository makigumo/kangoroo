#!/usr/bin/perl
# $hiraa = "\xe3\x81\x81";
# $kataa = "\xe3\x82\xa1";
# $katavu = "\xe3\x83\xb4";
while( <STDIN> ) {
  $s=$_;
  while (length($s)>0) {
  ($c) = ($s=~/^([\x00-\x7E]|[\xC2-\xDF][\x80-\xBF]|\xE0[\xA0-\xBF][\x80-\xBF]|[\xE1-\xEC\xEE\xEF][\x80-\xBF]{2}|\xED[\x80-\x9F][\x80-\xBF]|\xF0[\x90-\xBF][\x80-\xBF]{2}|[\xF1-\xF3][\x80-\xBF]{3}|\xF4[\x80-\x8F][\x80-\xBF]{2})/);
  print "$c\n";
  $s=substr($s,length($c),length($s)-length($c));
  }
}  
