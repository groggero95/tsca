#
# Copyright (C) EURECOM, Telecom Paris
#
# Prof. Renaud PACALET          <renaud.pacalet@telecom-paristech.fr>
# Alberto ANSELMO               <Alberto.Anselmo@eurecom.fr>
# Simone Alessandro CHIABERTO	<Simone-Alessandro.Chiaberto@eurecom.fr>
# Fausto CHIATANTE              <Fausto.Chiatante@eurecom.fr>
# Giulio ROGGERO                <Giulio.Roggero@eurecom.fr>
#
# This file must be used under the terms of the CeCILL. This source
# file is licensed as described in the file COPYING, which you should
# have received as part of this distribution. The terms are also
# available at:
# http://www.cecill.info/licences/Licence_CeCILL_V1.1-US.txt 
#

#!/bin/bash
#rm *.t
for i in `seq 3 5`;
do
    ./logic.py $i |& tee $i.t
done

awk 'BEGIN {err=0}; /errors/ {err+=$3}; END {printf "END with %d total errors\n", err; if (err==0) {printf "Fucking great job guys\n"}}' *.t
rm *.t
