#!/bin/bash
svn_dirs=($(find ./ -name ".svn"))
for elem in ${svn_dirs[@]}
do
	echo ${elem}
	rm -rf ${elem}
done
