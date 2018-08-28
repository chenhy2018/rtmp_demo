#########################################################################
# File Name: transform_log.sh
# Author: chenhy
# mail: chenhongye2000@163.com
# Created Time: 2018年08月24日 星期五 15时16分21秒
#########################################################################
#!/bin/bash

if [ $# != 1 ]; then 
	exit 1
fi

log_file_prefix_mqtt="rtmp_demo_mqtt"
log_file_prefix_rtmp="rtmp_demo_rtmp"
log_file_suffix=".log"
#modules_mqttbeat="[MQHB]"
modules_mqttbeat="MQHB"
modules_rtmp="RTMPHB"
func_rtmp_h264="RtmpH264Send"
func_rtmp_audio="RtmpAudioSend"

log_file_mqtt="rtmp_demo_mqtt.log"
log_file_rtmp=$log_file_prefix_rtmp
log_file_rtmp_h264="rtmp_demo_rtmp_h264.log"

trans_file1="test_trans.log1"
trans_file2="test_trans.log2"

# if [ -f $trans_file1 ]; then
# 	$(truncate -s 0 $trans_file1)
# fi
# 
# #format convert
# cat $1 | while read line
# do
# 	echo $line
# #	trans_str1=$(echo $line | sed 's/ \[ / \[/')
# 	trans_str1=$(echo $line | sed 's/ \[ / /')
# 	echo $trans_str1
# #	trans_str2=$(echo $trans_str1 | sed 's/ \]\[ /\] \[/g')
# 	trans_str2=$(echo $trans_str1 | sed 's/ \]\[ / /g')
# 	echo $trans_str2
# #	trans_str3=$(echo $trans_str2 | sed 's/ \]/\]/')
# 	trans_str3=$(echo $trans_str2 | sed 's/ \]//')
# 	echo $trans_str3
# 	echo $trans_str3 >> $trans_file1
# 
# done

#parse module
cat $trans_file1 | while read record
do
	echo $record
	filed_module=$(echo $record | awk '{print $3}')
	if [ "$filed_module" == "$modules_mqttbeat" ]; then
		echo $record >> $log_file_mqtt
	elif [ "$filed_module" == "$modules_rtmp" ]; then
		filed_func=$(echo $record | awk '{print $4}')
		if [ "$filed_func" == "$func_rtmp_h264" ]; then
			echo $record >> $log_file_rtmp_h264
		fi
	fi
done
