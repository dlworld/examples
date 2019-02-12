ping -i .01 $ip | while read pong; do echo "$(date +%T-%N) $pong "; done > ping-$ip.log &

