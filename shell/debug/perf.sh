#!/bin/bash

op=$1
pid=$2
data=$2
kernel=3.10.0-693.5.2.el7.x86_64
vm_ip=192.168.122.201

if [ $3 ]
then    
    kernel=$3
fi

function usage()
{
    echo "$0 record qemu-pid [vm-kernel-version]"
    echo "$0 report perf-data [vm-kernel-version]"
}

function prepare_kallsyms()
{
    ssh $vm_ip "cat /proc/kallsyms" > ./kallsyms-${kernel}
    ssh $vm_ip "cat /proc/modules" > ./modules-${kernel}
    scp $vm_ip:/usr/lib/debug/usr/lib/modules/${kernel}/vmlinux vmlinux-${kernel}
}

if [ -ne kallsyms-${kernel} ] 
then
    prepare_kallsyms
fi

if [ $op == "record" ]
then
        perf kvm --guestkallsyms=./kallsyms-${kernel} \
                --guestmodules=./modules-${kernel} \
                --guestvmlinux=./vmlinux-${kernel} \
                record -a -p $pid
        cp perf.data.guest perf.data.guest-`date +%m-%d-%T`
elif [ $op == "stat" ]
then
     timeout -s 2 10 perf stat -e 'kvm:*' -p $pid
elif [ $op == "report" ]
then
        perf kvm --guestkallsyms=./kallsyms-${kernel} \
                --guestmodules=./modules-${kernel} \
                --guestvmlinux=./vmlinux-${kernel} \
                report -i $data
fi

