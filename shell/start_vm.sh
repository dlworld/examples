#!/bin/bash

/home/dlw/Sources/qemu/x86_64-softmmu/qemu-system-x86_64 --enable-kvm -m 1024 \
    -drive file=~/images/cirros-0.3.5-x86_64-disk.img,if=none,id=drive-virtio-disk0,format=qcow2,cache=none \
    -device virtio-blk-pci,scsi=off,drive=drive-virtio-disk0,id=virtio-disk0,bootindex=1 \
    -qmp tcp:localhost:4444,server \
    -monitor stdio
