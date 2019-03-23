#!/bin/bash
for i in `seq 0 5`;
do
    ./logic.py $i |& tee $i.txt
done

cat *.txt | awk 'BEGIN {err=0}; NR%3 == 0 {err+=$3}; END {printf "END with %d total errors\n", err; if (err==0) {printf "Fucking great job guys\n"}}'
rm *.txt
