#!/usr/bin/perl
while( <STDIN> ) {
  $s=$_;
  ($f,$k,$p,$t,$r)=(m/^([^\t]*)\t([^\t]*)\t([^\t]*)\t([^\t]*)\t(.*)/);
  if ($fpre && ($f ne $fpre)) {output();}
  if ($r=~/\[D\]/ && $r=~/\[E\]/) {print $s;} 
  elsif ($r=~/\[D\]/) {push(@Dk,$k); push(@Dp,$p); push(@Dt,$t); push(@Dr,$r);}
  else {push(@Ek,$k); push(@Et,$t); push(@Ep,$p); push(@Er,$r);}
  $fpre=$f;
}
output();

sub output {
  for ($i=0;$i<=$#Ek;$i++) {
    $fit=-1;
    for ($j=0;$j<=$#Dk && $fit<0;$j++) {
      if (!$Ek[$i] && !$Dk[$j]) {$fit=$j;} else {
        $kk=$Dk[$j];
        $kk=~s/\./ /g;
        $kk=~s/[\(\)]/,/g;
        @tok = split(/,/,$kk);
        for ($ii=0;$ii<=$#tok;$ii++) { 
          $tt=($tok[$ii]);
          $tt=~s/^\s+//;$tt=~s/\s+$//;
          if ($Ek[$i] eq $tt) {$fit=$j;}
        }  
      }
    }
    if ($fit>=0) {
      $ere=$Er[$i];
      if ($Dr[$fit]=~/\[E\]/) {$ere=~s/^\[E\]//;}
      $Ek[$i]="XxXxX"; $Dt[$fit]=$Et[$i]; $Dr[$fit].=$ere;
      if ($Dr[$fit]=~/\/\(P\)\//) {$Dr[$fit]=~s/\/\(P\)\//\//g; $Dr[$fit].="(P)\/";}
    }    
  }
  for ($i=0;$i<=$#Dk;$i++) {print "$fpre\t$Dk[$i]\t$Dp[$i]\t$Dt[$i]\t$Dr[$i]\n";}
  for ($i=0;$i<=$#Ek;$i++) {if ($Ek[$i] ne "XxXxX") {print "$fpre\t$Ek[$i]\t$Ep[$i]\t$Et[$i]\t$Er[$i]\n";}}
  @Dk=(); @Dp=(); @Dt=(); @Dr=();
  @Ek=(); @Ep=(); @Et=(); @Er=();
}
