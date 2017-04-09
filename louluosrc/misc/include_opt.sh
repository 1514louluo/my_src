#!/bin/bash

################################################################################
#	author: louluo                                                         #
#	date: 2013-11-29                                                       #
#	function: autobuilder reference	                                       #
################################################################################
APP_SRC_PATH=$1
DEFAULT_INCLUDE_PATH=("/usr/include" "/usr/local/include")

function find_all_dir_in_src(){
	path=$1
	find ${path} -type d | grep -E -v "\.svn"
}

function find_all_file_in_src(){
	path=$1
	find ${path} -type f | grep -E ".+[h|c|cpp|cc]$"
}

function find_all_include_in_one_file(){
	file_name=$1
	cat ${file_name} | grep -E -o "[\/|\*]*#[\s]*include.+\.h[p]{0,2}" | grep -E -v "^\/" | grep -E -o "[a-zA-Z_][a-zA-Z0-9_-//]+.h[p]{0,2}"
}

raw_dirs=($(find_all_dir_in_src ${APP_SRC_PATH}))
raw_files=($(find_all_file_in_src ${APP_SRC_PATH}))
files=()

for elem in ${raw_files[@]}
do
	files=(${files[@]} ${elem})
done

all_include_file=()
for elem in ${files[@]}
do
	all_include_file=(${all_include_file[@]} $(find_all_include_in_one_file ${elem}))
done

#数组去重#
all_include_file=($(awk -vRS=' ' '!a[$1]++' <<< ${all_include_file[@]}))

##
# 第一阶段：先在src目录下面检查所有头文件是否存在 
##

include_file_case_1=()
for elem in ${all_include_file[@]}
do
	file_name=${elem##*\/}
	dir_path=${elem}
	find ${APP_SRC_PATH} -name "${file_name}" | grep -E -q "[a-zA-Z0-9]+"
	if [ $? -gt 0 ]; then
		include_file_case_1=(${include_file_case_1[@]}  ${elem})
	fi
done

##
# 第二阶段：然后检查环境路径和编译器默认路径是否找到头文件
##

include_file_case_2=()
for elem in ${include_file_case_1[@]}
do
	flag=0
	for default_include_elem in ${DEFAULT_INCLUDE_PATH[@]}
	do
		file_name=${elem##*\/}
		match_path=$(find ${default_include_elem} -name "${file_name}")
		if [[ ${match_path} =~ "${elem}" ]]; then
			let flag+=1
			break
		fi
	done
	if [ ${flag} -eq 0 ]; then
		echo ${elem}
		include_file_case_2=(${include_file_case_2[@]} ${elem})
	fi
done

echo -e "\n"

for elem in ${include_file_case_2[@]}
do
	echo ${elem}
done

##
# 第三阶段：找到各个头文件路径和dlib slib所在的路径，供用户选择
##

include_file_case_3=()
