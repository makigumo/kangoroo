#!/usr/bin/perl

$dir=$ARGV[0];

$rep = "\xE3\x80\x85";
$akana = "\xE3\x81\x81";
$afwr = "\xEF\xBC\xA1";
$ichi = "\xe4\xb8\x80";
$space = "\xe3\x80\x80";
$komma = "\xe3\x80\x81";
$punkt = "\xe3\x80\x82";
$kauf = "\xe3\x80\x8c";
$kzu = "\xe3\x80\x8d";
$wo = "\xe3\x82\x92";

open(F, $dir."dic.utf8");
$pos=0;

%de=();
%kana=();
%kanji=();
%ji=();

$count=0;

while( <F> ) {
  $s=$_;  
  if (($count++)%1000==0) {($entry) = ($s=~/^(\S+)\s/); print "--- $entry ($pos)\n";}
  %currentde=();
  %currentkana=();
  %currentkanji=();
  %currentji=();  
  $s =~ s/[^a-zA-Z0-9\x80-\xff]/ /g;
  $s =~ s/$wo/ $wo /g;
  $s =~ s/$space/ /g;
  $s =~ s/$komma/ /g;
  $s =~ s/$punkt/ /g;
  $s =~ s/$kauf/ /g;
  $s =~ s/$kzu/ /g;
  $s =~ s/\s+/ /g;
  $s =~ s/^\s//;$s =~ s/\s$//;    
  @tok = split(/ /,$s);
  for ($i=0;$i<=$#tok;$i++) {
    $t=lc($tok[$i]);
    $bKanji = ""; $bDE = ""; $bSkip = "";
    if (substr($t,0,1) lt "\x80") {
      $bDE=1;
      if (length($t)<2) {$bSkip=1;}
      for ($j=0;$j<length($t);$j++) { if (substr($t,$j,1) ge "\x80") {$bSkip=1;}}
    } else {
      if ((substr($t,0,3) lt $akana) || (substr($t,0,3) ge $afwr)) {$bSkip=1;}
      for ($j=0;$j<length($t);$j+=3) {
        $cj = substr($t,$j,3);
        if (($cj lt $rep) || ($cj ge $afwr)) {$bSkip=1;} 
        elsif ($cj ge $ichi) {$bKanji=1;}
      }      
    }
    if (!$bSkip && $t) {
      if ($bDE) {$currentde{$t}=1;}
      elsif ($bKanji) {$currentkanji{$t}=1;}
      else {$currentkana{$t}=1;}
    }
  }
  
  $s =~ s/[^\x80-\xff]//g;
  for ($j=0;$j<length($s);$j+=length($cj)) {
    $cj = utf8substr($s,$j);
    if (($cj ge $ichi) && ($cj lt $afwr)) {$currentji{$cj}=1;}
  }  
  
  foreach $t (keys (%currentde)) { if ($de{$t} eq "") {$de{$t}=$pos;} else {
    $sp=$de{$t}; $sp=~s/\S//g; if (length($sp)<998) {$de{$t}.=" $pos";}
  } } 
  foreach $t (keys (%currentkanji)) { if ($kanji{$t} eq "") {$kanji{$t}=$pos;} else {
    $sp=$kanji{$t}; $sp=~s/\S//g; if (length($sp)<998) {$kanji{$t}.=" $pos";}
  } } 
  foreach $t (keys (%currentkana)) { if ($kana{$t} eq "") {$kana{$t}=$pos;} else {
    $sp=$kana{$t};  $sp=~s/\S//g; if (length($sp)<998) {$kana{$t}.=" $pos";}
  } }
  foreach $t (keys (%currentji)) { if ($ji{$t} eq "") {$ji{$t}=$pos;} else {
    $sp=$ji{$t}; $sp=~s/\S//g; if (length($sp)<998) {$ji{$t}.=" $pos";}
  } }

  $pos=tell(F);
}
close(F);

open(F, ">".$dir."de.idx");
foreach $key (sort (keys(%de))) {
   print F "$key\t$de{$key}\n";
}
close(F);

open(F, ">".$dir."kana.idx");
foreach $key (sort (keys(%kana))) {
   print F "$key\t$kana{$key}\n";
}
close(F);

open(F, ">".$dir."kanji.idx");
foreach $key (sort (keys(%kanji))) {
   print F "$key\t$kanji{$key}\n";
}
close(F);

open(F, "kdic.utf8");
open(G, "linkdata.utf8");
open(H, ">".$dir."kkdic.idx");
$bReadG=1;
while( 1 ) {
  $l1=<F>;
  $l1 =~ s/\s+$//;
  if ($bReadG) {
    $g=<G>;
    if (!$g) {last;}
    $l3=<G>;
    $l4=<G>;
    $bReadG="";
  }  
  if ($l1) {
    $k=substr($l1,0,3);
    ($k2)=($g=~/^\d+ (.*)/);
    if ($k eq $k2) {print H "$l1\n$ji{$k}\n$l3$l4"; $bReadG=1;} else {print H "$l1\n$ji{$k}\n\n\n";}
  } else
  {
    print H "\n\n".$l3.$l4;
    $bReadG=1;
  }
}
close(F);
close(G);
close(H);


sub utf8substr {
  my ($s,$j) = @_;
  if (checkutf8(substr($s,$j,1))) {return substr($s,$j,1);}
  elsif (checkutf8(substr($s,$j,2))) {return substr($s,$j,2);}
  return substr($s,$j,3);
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
