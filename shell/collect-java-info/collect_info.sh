#!/bin/bash

app=java
log_dir=`hostname`-`date +%F-%H-%M-%S`-logs

echo "Create log directory: " $log_dir
mkdir $log_dir

echo "Collect system info..."
cat /etc/system-release > ${log_dir}/system-info.log
echo 'Kernel: ' `uname -r` >> ${log_dir}/system-info.log
java -version >> ${log_dir}/system-info.log
echo '#### Running Processes: ' >> ${log_dir}/system-info.log
ps -aux  >> ${log_dir}/system-info.log

cp /var/log/messages ${log_dir}
cp /var/log/cron ${log_dir}

for p in `pgrep $app`
do
  echo "Collect info of pid " $p
  top -H -d 1 -n 3 -b > ${log_dir}/${p}-top.log
  jstack -l $p > ${log_dir}/${p}-jstack.log
  pstack  $p > ${log_dir}/${p}-pstack.log
done

echo "Run test program: gettimeofday"
time ./monotanic > ${log_dir}/time.log
echo "Run test program: rdtsc"
time ./rdtsc > ${log_dir}/rdtsc.log

echo "Achieve logs..."
tar czvf ${log_dir}.tar.gz ${log_dir}
