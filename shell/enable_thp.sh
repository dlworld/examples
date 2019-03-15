#!/bin/bash

op=$1

if [ `uname -r | awk -F "-" '{print $1}'` == "2.6.32" ]; then
    thp_dir=/sys/kernel/mm/redhat_transparent_hugepage
else
    thp_dir=/sys/kernel/mm/transparent_hugepage
fi
    
# always disable defrag

if [ "$op"x == "disable"x ]
then
    echo "disable thp..."
    echo never > ${thp_dir}/enabled
    echo never > ${thp_dir}/defrag
    echo 0 > ${thp_dir}/khugepaged/defrag
else
    echo "enable thp..."
    echo always > ${thp_dir}/enabled
    echo always > ${thp_dir}/defrag
    echo 1 > ${thp_dir}/khugepaged/defrag
fi

echo "done"

