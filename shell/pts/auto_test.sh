#!/bin/sh
t=$(hostname)-$(date +%Y%m%d) 
mkdir -p Test_Result/$t 
for shell in `ls | grep pts`
do
	./${shell} K_result_${shell}_$t K_des_${shell}_$t
        phoronix-test-suite result-file-to-csv kresult${shell}$t >Test_Result/$t/kresult${shell}$t.csv
done
