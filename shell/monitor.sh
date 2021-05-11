#!/usr/bin/env bash
#date,pid,tcpnum,cpu,mem,stime

CSVFILE="monitor.csv"
readonly port=xxxxx
readonly exename=xxx
date=$(date +%s)
tcpnum=$(netstat -nat|grep -i ${port}|wc -l)
topstr=$(top -bn 1 | grep -w ${exename}|tr -s ' ')
pid=$(echo $topstr|cut -d ' ' -f 1)
cpu=$(echo $topstr|cut -d ' ' -f 9)
memory=$(echo $topstr|cut -d ' ' -f 10)
runtime=$(ps -q $pid -o etime=)

printf "%s,%s,%s,%s,%s,%s\n" $date $pid $tcpnum $cpu $memory $runtime >> $CSVFILE
