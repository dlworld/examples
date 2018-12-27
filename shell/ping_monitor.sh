#!/bin/bash
IP="120.92.142.101"
#IP="1.92.1.101"
#set -x
rtt_base=100
while (1>0)
do
    ping -c 1 $IP -W 1 > tmp
    loss=`cat tmp|grep "100% packet loss"`
    if [[ "$loss" != "" ]]
    then
        echo `date`+"pkt loss" >> loss.log
        mtr $IP -r -c 10 >> mtr.log &
    else
        rtt=`cat tmp|grep "from $IP"|awk '{print $7}'|awk -F "=" '{print $2}'|awk -F "." '{print $1}'`
        echo $rtt
        if [ ${rtt} -gt ${rtt_base} ]
        then 
            echo `date`+"pkt rtt > $rtt_base rtt=$rtt" >> loss.log 
            mtr $IP -r -c 10 >> mtr.log &
        else
            echo `date`+"ping monitor ok" >> success.log
        fi
        sleep 1
    fi
done
