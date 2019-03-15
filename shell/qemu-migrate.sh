#!/bin/bash

inst=$1
tgt=$2

virsh migrate --live --p2p --persistent --copy-storage-inc --auto-converge --verbose $inst qemu+tcp://$2/system

