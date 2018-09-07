#!/bin/bash

op=$1
start=$2
end=$3

if [ $op == 'destroy' ]
then
        pkill -9 ks3_backup.sh
        pkill -9 virsh
fi

for i in `seq $start $end`
do
        virsh $op backup$i
done

if [ $op == 'start' ]
then
        for i in `seq $start $end`
        do
                ./ks3_backup.sh backup$i 10 100 &
        done
fi

