#!/bin/bash

file="vm.list"


function vm_pin()
{
	uuid=$1
	domname=`virsh dominfo $uuid | grep Name | awk '{print $2}'`
	vcpunum=`virsh vcpucount $domname | grep current | grep live | awk '{print $3}'`
	echo "start cpupin $domname $uuid"
	let vcpunum--
	pincpu=`lscpu | grep "NUMA node0 CPU" | awk -F ':' '{print $2}'`

	#echo $domname
	#echo $vcpunum
	#echo $pincpu

	for i in `seq 0 $vcpunum`
	do
		virsh vcpupin $domname $i $pincpu > /dev/null
	done

	virsh vcpupin $domname
}

for i in `cat $file`
do
	virsh dominfo $i > /dev/null 2>&1
	if [ $? -eq 0 ]; then
		vm_pin $i
	fi
done
