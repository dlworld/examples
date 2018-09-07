#!/bin/sh

start=$1
end=$2
debug=$3

for i in `seq $((start)) $((end))`
do
        qemu-img create -f qcow2 -b centos7.base backup$i.qcow2 20G
        cp backup.xml backup${i}.xml
        sed -ie "s/backup/backup$i/g" backup${i}.xml
        if [ $debug ]
        then
                sed -ie "s/qemu-kvm/qemu-kvm-debug/g" backup${i}.xml
        fi

        j=`echo "obase=16;$i" | bc`
        if [ $((i)) -gt 15 ]
        then
                sed -ie "s/fea50/fea$j/g" backup${i}.xml
        else
                sed -ie "s/fea50/fea5$j/g" backup${i}.xml
        fi

        virsh undefine backup${i}
        virsh define backup${i}.xml
done

