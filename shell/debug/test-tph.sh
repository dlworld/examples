#!/bin/bash

instance=instance-0000a986
save_file=instance-0000a986.dump-20190123
log=`hostname`-`uname -r`.log

virsh destroy ${instance}
echo never > /sys/kernel/mm/transparent_hugepage/enabled
sleep 60
virsh restore ${save_file}
pid=`pgrep qemu-kvm`
sleep 30
echo "\ntransparent_hugepage never" >> ${log}
timeout -s 2 10 perf stat -e 'kvm:*' -p $pid  2>>${log}
top -b -n 1 -p $pid >> ${log}

virsh destroy ${instance}
echo always > /sys/kernel/mm/transparent_hugepage/enabled
sleep 60
virsh restore ${save_file}
pid=`pgrep qemu-kvm`
sleep 30
echo "\ntransparent_hugepage always" >> ${log}
timeout -s 2 10 perf stat -e 'kvm:*' -p $pid  2>>${log}
top -b -n 1 -p $pid >> ${log}

