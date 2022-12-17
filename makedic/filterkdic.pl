#!/usr/bin/perl
while( <STDIN> ) {
  ($k,$r,$eng)= (m/^(\S\S\S)\s+([^\{]*)(\{.*)$/);  
  if ($k) {
    ($heisig) = ($r=~/\s(L\d+\s)/);
    ($stroke) = ($r=~/\s(S\d+\s)/);
    $i=1;
    while (checkutf8(substr($r,$i,9999))) {$i++;}
    $kana=substr($r,$i-1,9999);
    $kana =~ s/T1\s+(.+)\s+T2\s+(.+)\s+$/[名:\1] [R:\2] /;
    $kana =~ s/T1\s+(.+)\s+$/[名:\1] /;
    $kana =~ s/T2\s+(.+)\s+$/[R:\1] /;
    $eng=~s/}\s*{/, /g;
    $eng=~s/{//g;
    $eng=~s/}\s*//g;
    print "$k $heisig$stroke$kana$eng\n";
  }  
} 
 
sub checkutf8 {
  my ($s) = @_;
  return ($s =~ m/^(
     [\09\0A\0D\x20-\x7E]               # ASCII
   | [\xC2-\xDF][\x80-\xBF]             # non-overlong 2-byte
   |  \xE0[\xA0-\xBF][\x80-\xBF]        # excluding overlongs
   | [\xE1-\xEC\xEE\xEF][\x80-\xBF]{2}  # straight 3-byte
   |  \xED[\x80-\x9F][\x80-\xBF]        # excluding surrogates
   |  \xF0[\x90-\xBF][\x80-\xBF]{2}     # planes 1-3
   | [\xF1-\xF3][\x80-\xBF]{3}          # planes 4-15
   |  \xF4[\x80-\x8F][\x80-\xBF]{2}     # plane 16
  )*$/x);
}
