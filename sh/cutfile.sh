#!/bin/bash

today=""
yesterday=

currentTime=$(date "+%Y-%m-%d %H:%M:%S")
today=$(echo $currentTime | awk '{print today=$1}')
yesterday=$today

echo "============"
echo "$today"
echo $yesterday

#while true
cat $1 | while read line
do
	#echo $line
	today=$(echo $currentTime | awk '{print $1}')
	#today=$(echo $line | awk '{print $1}')
	if [ $today == $yesterday ]; then
		#echo $line >> $yesterday.log
		echo $currentTime "[Status][MqttConnect]" >> mqtt_test_$today.log
	else
		yesterday=$today
		#echo $line >> $today.log
		echo $currentTime "[Status][MqttConnect]" >> mqtt_test_$today.log
	fi
done
