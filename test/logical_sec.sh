#!/bin/bash
#rm *.t
for i in `seq 3 5`;
do
    ./logic.py $i |& tee $i.t
done

awk 'BEGIN {err=0}; /errors/ {err+=$3}; END {printf "END with %d total errors\n", err; if (err==0) {printf "Fucking great job guys\n"}}' *.t
rm *.t
