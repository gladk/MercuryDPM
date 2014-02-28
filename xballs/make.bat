#!/bin/tcsh
switch($HOSTTYPE) 
case rs6000: 
   set EXT = ibm
   breaksw
case sun4:   
   set EXT = sun
   breaksw
case alpha:   
   set EXT = dec
   breaksw
case i386-linux:   
   set EXT = linux
   breaksw
default: 
   echo unknown hardware $HOSTTYPE
   exit
   breaksw
endsw

if ( ! -f make.$EXT\.log ) /bin/rm *.o
/bin/rm make.*.log
make -f make.$EXT
echo compiled on a $HOSTTYPE $EXT > make.$EXT\.log
cp xballs xballs.$EXT
