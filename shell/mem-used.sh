#/bin/bash
RSS=0
VM=0
for PROC in `ls /proc/|grep "^[0-9]"`
do
  if [ -f /proc/$PROC/statm ]; then
      TEP1=`cat /proc/$PROC/statm | awk '{print ($1)}'`
      TEP2=`cat /proc/$PROC/statm | awk '{print ($2)}'`
      echo $PROC $TEP1 $TEP2
      RSS=`expr $RSS + $TEP2`
      VM=`expr $VM + $TEP1`
  fi
done
RSS=`expr $RSS \* 4`
VM=`expr $VM \* 4`

echo "APP total" "RSS:" $RSS"KB", "VM:" $VM"KB",

SLAB=`cat /proc/meminfo | grep Slab | awk '{print ($2)}'`
STACK=`cat /proc/meminfo | grep KernelStack | awk '{print ($2)}'`
echo "Kernel Slab: " $SLAB"KB", "Stack: " $STACK"KB"

