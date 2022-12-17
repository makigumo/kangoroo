#!/bin/bash
sed -f sonderz.sed wadoku0.utf8 >wadoku1.utf8
sed -f sonderz.sed edict0.utf8 | grep -v "←" | grep -v "↑" | grep -v "↓" | grep -v "→" >edict1.utf8
cat wadoku1.utf8 | ./wadoku2tab.pl | ./sort.pl | ./rdentries.pl | ./wrest2e.pl >wadoku2.utf8
cat edict1.utf8 | ./edict2tab.pl | ./sort.pl | ./rdee.pl >edict2.utf8
cp wadoku2.utf8 dic0.utf8
cat edict2.utf8 >>dic0.utf8 
cat dic0.utf8 | grep -v "^|" | grep -v "^[0-9A-Za-z]" | ./sort.pl >dic1.utf8

cat dic1.utf8 | ./mergebasic.pl | ./mergerest.pl | ./sort.pl >dic0.utf8
cat dic0.utf8 | ./addwf.pl | ./rsort.pl >dic1.utf8
cat dic1.utf8 | ./etab2edict.pl > dedict.utf8

cat edict2.utf8 | grep -v "^|" | grep -v "^[0-9A-Za-z]" | ./sort.pl >dic0.utf8
cat dic0.utf8 | ./addwf.pl | ./rsort.pl >dic1.utf8
cat dic1.utf8 | ./etab2eedict.pl > edict.utf8

rm dic0.utf8
rm dic1.utf8
rm wadoku1.utf8
rm wadoku2.utf8
rm edict1.utf8
rm edict2.utf8

cat kanjidic.utf8 | ./filterkdic.pl | ./sort.pl > kdic.utf8
