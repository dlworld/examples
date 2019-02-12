#!/bin/bash

op=$1

if [ `uname -r | awk -F "-" '{print $1}'` == "2.6.32" ]; then
    thp_dir=/sys/kernel/mm/redhat_transparent_hugepage
else
    thp_dir=/sys/kernel/mm/transparent_hugepage
fi
    
# always disable defrag
echo never > ${thp_dir}/defrag
echo 0 > ${thp_dir}/khugepaged/defrag

if [ "$op"x == "disable"x ]
then
    echo never > ${thp_dir}/enabled
else
    echo always > ${thp_dir}/enabled
fi

