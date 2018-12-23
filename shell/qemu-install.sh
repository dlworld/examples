#!/bin/bash

op=$1
new_qemu_version=60.10.1.2.ksyun
#old_qemu_version=60.10.4.0.ksyun
old_qemu_version=9.4.ksyun
#old_qemu_version=7.ksyun

if [ $op == "upgrade" ]
then
yum install qemu-kvm-common-1.5.3-${new_qemu_version}.el6.x86_64 \
                           qemu-kvm-tools-1.5.3-${new_qemu_version}.el6.x86_64 \
                           qemu-img-1.5.3-${new_qemu_version}.el6.x86_64 \
                           qemu-kvm-1.5.3-${new_qemu_version}.el6.x86_64
elif [ $op == "downgrade" ]
then
yum downgrade qemu-kvm-common-1.5.3-${old_qemu_version}.el6.x86_64 \
                                     qemu-kvm-tools-1.5.3-${old_qemu_version}.el6.x86_64 \
                                     qemu-img-1.5.3-${old_qemu_version}.el6.x86_64 \
                                     qemu-kvm-1.5.3-${old_qemu_version}.el6.x86_64
fi

