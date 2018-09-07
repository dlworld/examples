#!/bin/bash

vm=$1                #进行在线备份的虚拟机
bkPath=/mnt/dlw   #备份数据存放的路径
sz_disk=20
sz_f=20
sz_i=1
inc_nums=$2
full_nums=$2

echo "test began at `date`"
if [ 0 -eq 1 ]
then
echo "创建磁盘${sz_disk}G，挂载到虚拟机中"
qemu-img create -f qcow2 ${bkPath}/test.qcow2 ${sz_disk}G
virsh attach-device ${vm} ${bkPath}/disk_data.xml --live
sleep 5
#ssh ${remoteip} "mkfs.ext4 /dev/vdb" >/dev/null 2>&1
#ssh ${remoteip} "mount -t ext4 /dev/vdb /mnt"
#ssh ${remoteip} "df -h"
fi


for k in `seq ${full_nums}`
do

echo "准备数据，进行第${k}次全备份,${sz_f}G"
qemu-img remove ks3://${vm}_dlw_bk_193_f_01.qcow2
qemu-img create -f qcow2 ks3://${vm}_dlw_bk_193_f_01.qcow2 ${sz_disk}G
##ssh ${remoteip} "sh /opt/w.sh full ${sz_f}" >/dev/null 2>&1
##ssh ${remoteip} "sync"
time virsh blockbackup ${vm} ${bkPath}/${vm}.qcow2 ks3://${vm}_dlw_bk_193_f_01.qcow2 \
      --mode full --format qcow2 --persistent --wait --timeout 1000000 --verbose

echo "第一次增量备份,${sz_i}G" 
qemu-img remove ks3://${vm}_dlw_bk_193_i_01.qcow2
qemu-img create -f qcow2  -b ks3://${vm}_dlw_bk_193_f_01.qcow2 ks3://${vm}_dlw_bk_193_i_01.qcow2
#ssh ${remoteip} "sh /opt/w.sh inc1 ${sz_i}" >/dev/null 2>&1
#ssh ${remoteip} "sync"
time virsh blockbackup ${vm} ${bkPath}/${vm}.qcow2 ks3://${vm}_dlw_bk_193_i_01.qcow2 \
      --mode incremental --format qcow2 --persistent --wait --timeout 1000000 --verbose

for j in `seq 1 ${inc_nums}`
do
        incx=$(( j+1 ))
        echo "第${incx}次增量备份，${sz_i}G" 
    qemu-img remove ks3://${vm}_dlw_bk_193_i_0${incx}.qcow2
    qemu-img create -f qcow2  -b ks3://${vm}_dlw_bk_193_i_0${j}.qcow2 ks3://${vm}_dlw_bk_193_i_0${incx}.qcow2
    #ssh ${remoteip} "sh /opt/w.sh inc${incx} ${sz_i}" >/dev/null 2>&1
    #ssh ${remoteip} "sync"
    time virsh blockbackup ${vm} ${bkPath}/${vm}.qcow2 ks3://${vm}_dlw_bk_193_i_0${incx}.qcow2 \
            --mode incremental --format qcow2 --persistent --wait --timeout 1000000 --verbose
done

ls -lh *.qcow2

lastN=$(( ${inc_nums}+1 ))
echo "合并后恢复"
qemu-img remove ks3://${vm}_dlw_bk_193_c_02.qcow2
time qemu-img convert -f qcow2 -O qcow2 ks3://${vm}_dlw_bk_193_i_0${lastN}.qcow2 ks3://${vm}_dlw_bk_193_c_02.qcow2 -p
time qemu-img convert -f qcow2 -O qcow2 ks3://${vm}_dlw_bk_193_c_02.qcow2 rc_c_02.qcow2 -p
ls -lh *.qcow2

done

if [ 0 -eq 1 ]
then
echo "挂载恢复的文件到虚拟机上进行验证"
virsh attach-device ${vm} ${bkPath}/disk_c_02.xml --live
#ssh ${remoteip} "lsblk"
#ssh ${remoteip} "mkdir /bk_c_02" >/dev/null 2>&1
#ssh ${remoteip} "mount -t ext4 /dev/vdc /bk_c_02"
#ssh ${remoteip} "df -h"


echo "数据磁盘情况" 
#ssh ${remoteip} "ls -lh /mnt" 
echo "合并增量备份还原磁盘情况" 
#ssh ${remoteip} "ls -lh /bk_c_02"

echo "生成恢复磁盘下的文件md5"
#ssh ${remoteip} "md5sum /bk_c_02/full >/opt/md5.full.bk_c_02"
echo "预期比较结果为相同" 
#ssh ${remoteip} "diff /opt/md5.full.bk_c_02 /mnt/md5.full"
for j in `seq 0 ${inc_nums}`
do
        incx=$(( j+1 ))
    #ssh ${remoteip} "md5sum /bk_c_02/inc${incx} >/opt/md5.inc${incx}.bk_c_02"
    echo "预期比较结果为相同"
    echo "---------------------"
    #ssh ${remoteip} "diff /opt/md5.inc${incx}.bk_c_02 /mnt/md5.inc${incx}"
done

echo "卸载磁盘"
#ssh ${remoteip} "umount /bk_c_02"
#ssh ${remoteip} "umount /mnt"

virsh detach-device ${vm} ${bkPath}/disk_c_02.xml --live
virsh detach-device ${vm} ${bkPath}/disk_data.xml --live
fi


echo "清理文件"
for j in `seq 0 ${inc_nums}`
do
   incx=$(( j+1 ))
   qemu-img remove ks3://${vm}_dlw_bk_193_i_0${incx}.qcow2
done

qemu-img remove ks3://${vm}_dlw_bk_193_f_01.qcow2
qemu-img remove ks3://${vm}_dlw_bk_193_c_02.qcow2
#rm -f ${bkPath}/*.qcow2
echo "test finished at `date`"

