#!/bin/bash

IP="10.0.1.88"
#IP="1.92.1.101"
#set -x

rtt_base=100
while (1>0)
do
    ping -c 1 $IP -W 1 > tmp-$IP
    loss=`cat tmp-$IP|grep "100% packet loss"`
    if [[ "$loss" != "" ]]
    then
        echo `date`+"pkt loss" >> loss.log-$IP
        mtr $IP -r -c 10 >> mtr.log-$IP &
    else
        rtt=`cat tmp-$IP|grep "from $IP"|awk '{print $7}'|awk -F "=" '{print $2}'|awk -F "." '{print $1}'`
        echo $rtt
        if [ ${rtt} -gt ${rtt_base} ]
        then 
            echo `date`+"pkt rtt > $rtt_base rtt=$rtt" >> loss.log-$IP 
            mtr $IP -r -c 10 >> mtr.log-$IP &
        else
            echo `date`+"ping monitor ok" >> success.log-$IP
        fi
        sleep 1
    fi
done

