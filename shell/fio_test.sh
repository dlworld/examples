#!/bin/bash

TEST_DIR=fio-test-`hostname`-`date +%m%d%H%M`
FIO=fio
ENGINE=libaio
FILENAME=/dev/vdb
#LOGS=./logs/
SIZE=50g
RUNTIME=300
JOBS=4

drop_cache() {
    echo 3 > /proc/sys/vm/drop_caches
}

# $1 read/write
# $2 blocksize
# $3 iodepth
run_fio() {
    drop_cache
    $FIO -filename=$FILENAME \
         -ioengine=$ENGINE \
         -ramp_time=10 \
         -direct=1 \
         -rw=$1 \
         -bs=$2 \
         -size=$SIZE \
         -runtime=$RUNTIME \
         -group_reporting \
         -numjobs=$JOBS \
         -iodepth=$3 \
         -write_iops_log=$1-$2.`hostname` \
         -write_bw_log=$1-$2.`hostname` \
         -thread \
         -stonewall \
         -rwmixread=70 \
         -name=$1_$2 
    sleep 5
}

read_64k() {
    run_fio read 64k 64
}

write_64k() {
    run_fio write 64k 64
}




randread_4k() {
    run_fio randread 4k 64
}

randwrite_4k() {
    run_fio randwrite 4k 16
}

readwrite_4k() {
    run_fio readwrite 4k 64
}

readwrite_64k() {
    run_fio readwrite 64k 64
}


mkdir -p $TEST_DIR
pushd $TEST_DIR
read_64k
write_64k
randread_4k
randwrite_4k
readwrite_4k
readwrite_64k
popd

