#!/bin/bash

op=$1
pid=$2
data=$2

if [ $op == "record" ]
then
        perf kvm --guestkallsyms=./c7-3.10.0-693.5.2.kallsyms \
                --guestmodules=./c7-3.10.0-693.5.2.modules \
                --guestvmlinux=./vmlinux \
                record -a -p $pid
elif [ $op == "report" ]
then
        perf kvm --guestkallsyms=./c7-3.10.0-693.5.2.kallsyms \
                --guestmodules=./c7-3.10.0-693.5.2.modules \
                --guestvmlinux=./vmlinux \
                report -i $data
fi

