#!/bin/bash

pid_array=($(ps aux|grep "python general_tips.py"|grep -v "grep" | awk -F " " '{print $2}'))

for pid in ${pid_array[@]}
do
{
	echo "kill -9 ${pid}"
	kill -9 ${pid}
} &
done
wait


echo "stop finish!"
