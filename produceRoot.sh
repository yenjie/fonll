#!/bin/bash

echo "{">$1.C
echo "   TFile *inf = new TFile(\"$1.root\",\"recreate\");">>$1.C
for i in `seq 1 9`;
do
   echo $i
   g=$(($i+5))
   if [ "$i" == "1" ] 
   then 
        g=2
   fi
   echo $g
   
   cat $1|awk -v g=$g -v i=$i '
   BEGIN{print "   TH1D *h"i"= new TH1D(\"h"i"\",\"\",100,0,100);"}
   {print "   h"i"->Fill("$1","$g");"}
   END{print "   h"i"->Write();"}
   '>>$1.C

done

echo "}">>$1.C

root -l -q -b $1.C
