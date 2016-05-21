#!/bin/bash

dirs="/opt/glance/images/remotefs /opt/nfs/iso/remotefs  /opt/template"

for dir in $dirs:
do
    device=`mount | grep $dir | cut -d ' ' -f 1`
    if [ $device ]
    then
        echo 'umount ' $device
        umount $device
        umount $device 2> /dev/null
    fi
done

