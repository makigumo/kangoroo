#!/usr/bin/perl
# $hiraa = "\xe3\x81\x81";
# $kataa = "\xe3\x82\xa1";
# $katavu = "\xe3\x83\xb4";
while( <STDIN> ) {
  $s=$_;
  $b=checksmallutf8($s);
  if (!$b) {
    ($c) = ($s=~/([\xC2-\xDF][\x80-\xBF]|\xE0[\xA0-\xBF][\x80-\xBF]|[\xE1\xEA\xEB\xEC\xEE\xEF][\x80-\xBF]{2}|\xED[\x80-\x9F][\x80-\xBF]|\xF0[\x90-\xBF][\x80-\xBF]{2}|[\xF1-\xF3][\x80-\xBF]{3}|\xF4[\x80-\x8F][\x80-\xBF]{2})/);
    if (substr($c,0,1) ge "\x80") {print "$c\n";}
  }
}  

sub checksmallutf8 {
  my ($s) = @_;
  return ($s =~ m/^(
     [\t\09\0A\0D\x20-\x7E]               # ASCII
   | [\xE2-\xE9][\x80-\xBF]{2}  # straight 3-byte
  )*$/x);
}

sub checkutf8 {
  my ($s) = @_;
  return ($s =~ m/^(
     [\t\0A\0D\x20-\x7E]               # ASCII
   | [\xC2-\xDF][\x80-\xBF]             # non-overlong 2-byte
   |  \xE0[\xA0-\xBF][\x80-\xBF]        # excluding overlongs
   | [\xE1-\xEC\xEE\xEF][\x80-\xBF]{2}  # straight 3-byte
   |  \xED[\x80-\x9F][\x80-\xBF]        # excluding surrogates
   |  \xF0[\x90-\xBF][\x80-\xBF]{2}     # planes 1-3
   | [\xF1-\xF3][\x80-\xBF]{3}          # planes 4-15
   |  \xF4[\x80-\x8F][\x80-\xBF]{2}     # plane 16
  )*$/x);
}
