#!/bin/bash
###############SOME STAT###################
SYS_LIBS_CACHE='/etc/ld.so.cache'
#################SOME user define#######################
APP_NAME='my_app'
APP_VERSION=''
APP_INSTALL_PATH=''
APP_STAT=1
APP_DBG=0
APP_MULCORE=0
CCACHE=0
CXX='gcc'
FORM='40;35;1'
#################SOME PATH#######################
APP_ROOT=$(pwd)
if [ ! -d ${APP_ROOT}/build ]; then
	echo "No build dir find in your root dir!"
	exit;
fi
MAKEFILE_BASE_PATH=${APP_ROOT}/build
MAKEFILE_SUB_PATH=${APP_ROOT}/build/makefile.sub
MAKEFILE_SUB_SHARED_PATH=${APP_ROOT}/build/makefile.sub.shared
MAKEFILE_CONF_PATH=${APP_ROOT}/build/makefile.conf
MAKEFILE_SHARED_PATH=${APP_ROOT}/build/makefile.shared
MAKEFILE_STATIC_PATH=${APP_ROOT}/build/makefile.static
MAKEFILE_OBJ_PATH=${APP_ROOT}/build/makefile.bin
# process command line options, shift 相当于左移,自然$#就减1
while test $# -ge 1
do
case "$1" in
    -h* | --help)
      echo -e "\033[40;35;1m #---------------------------------------------------------------------------------------#\033[0m"
      echo -e "\033[40;35;1m #                                                                                       #\033[0m"
      echo -e "\033[40;35;1m #     #     #   #  ######   ###   ####    #   #    #     #       ####    #####    ####  #\033[0m"
      echo -e "\033[40;35;1m #    ##    #   #    #     #   #  #   #   #   #    #     #       #   #   #        #   #  #\033[0m"
      echo -e "\033[40;35;1m #   # #   #   #    #     #   #  ####    #   #    #     #       #   #   #####    ####    #\033[0m"
      echo -e "\033[40;35;1m #  ####  #   #    #     #   #  #   #   #   #    #     #       #   #   #        # #      #\033[0m"
      echo -e "\033[40;35;1m # #   #  ###     #      ###   ####     ###     #     ######  ####    #####    #   #     #\033[0m"
      echo -e "\033[40;35;1m #                                                                                       #\033[0m"
      echo -e "\033[40;35;1m #---------------------------------------------------------------------------------------#\033[0m"

      echo '  configure [--name or -n] [--version or -v] [--prefix or -p] [--verbose] [--ccache] [--compiler or -c] [--mulcore or -j] [--stat or -s] [--form or -f]' 
      echo '  --name     -n  :  decide what name you want to define! default is "my_app" ' 
      echo '  --version  -v  :  decide which version for your app! default is empty ' 
      echo '  --perfix   -p  :  decide where you want to install! default is no install ' 
      echo '  --verbose      :  display detail infos on compile command! default is no verbose ' 
      echo '  --ccache       :  set up ccache support! ccache can speed up the repeat-compile greately! if set up,please install ccache first! default is no support!' 
      echo '  --compiler -c  :  default is gcc, now only support:[gcc | clang], please don not use other compilers! ' 
      echo '  --mulcore  -j  :  decide how many tasks you want to compile at the same time! default is "1" ' 
      echo '  --stat     -s  :  compile-stats has 3 stats: BIN(binary), STATIC(.a), SHARED(.so) ' 
      echo -e '			DO NOTHING,here same as BIN ONLY	: 000 or 0 '
      echo -e '			BIN only             			: 001 or 1 '
      echo -e '			SHARED only          			: 010 or 2 '
      echo -e '			SHARED and BIN       			: 011 or 3 '
      echo -e '			STATIC only          			: 100 or 4 '
      echo -e '			STATIC and BIN 	     			: 101 or 5 '
      echo -e '			STATIC and SHARED    			: 110 or 6 '
      echo -e '			STATIC and SHARED and BIN   		: 111 or 7 '
      echo '  --form     -f  :  set echo style, form just like: foreground-color;background-color;action   '
      echo '                    default style is 40;35;1   ,if you want set form, please read define just as follow:'
      echo '                    --background-color define:'
      echo -e '					  40: black       -- \033[40m hello,world! \033[0m'
      echo -e '					  41: dark red    -- \033[41m hello,world! \033[0m'
      echo -e '					  42: green       -- \033[42m hello,world! \033[0m'
      echo -e '					  43: yellow      -- \033[43m hello,world! \033[0m'
      echo -e '					  44: blue        -- \033[44m hello,world! \033[0m'
      echo -e '					  45: purple      -- \033[45m hello,world! \033[0m'
      echo -e '					  46: dark green  -- \033[46m hello,world! \033[0m'
      echo -e '					  47: white       -- \033[47m hello,world! \033[0m'
      echo '                    --foreground-color define:'
      echo -e '					  30: black       -- \033[30m hello,world! \033[0m'
      echo -e '					  31: dark red    -- \033[31m hello,world! \033[0m'
      echo -e '					  32: green       -- \033[32m hello,world! \033[0m'
      echo -e '					  33: yellow      -- \033[33m hello,world! \033[0m'
      echo -e '					  34: blue        -- \033[34m hello,world! \033[0m'
      echo -e '					  35: purple      -- \033[35m hello,world! \033[0m'
      echo -e '					  36: dark green  -- \033[36m hello,world! \033[0m'
      echo -e '					  37: white       -- \033[37m hello,world! \033[0m'
      echo '                    --action     define:'
      echo -e '					  0: no action       -- \033[0m hello,world! \033[0m'
      echo -e '					  1: high light      -- \033[1m hello,world! \033[0m'
      echo -e '					  2: half high light -- \033[2m hello,world! \033[0m'
      echo -e '					  4: underline       -- \033[4m hello,world! \033[0m'
      echo -e '					  5: blink           -- \033[5m hello,world! \033[0m'
      echo -e '					  7: reverse display -- \033[7m hello,world! \033[0m'
      echo -e '					  8: hide background -- \033[8m hello,world! \033[0m'
        exit 0 ;;
    -p*=* | --prefix=*)                APP_INSTALL_PATH=`echo $1 | sed 's/.*=//'`; shift ;;
    -n*=* | --name=*)                  APP_NAME=`echo $1 | sed 's/.*=//'`;shift ;;
    -c*=* | --compiler=*)              CXX=`echo $1 | sed 's/.*=//'`;shift ;;
    -f*=* | --form=*)                  FORM=`echo $1 | sed 's/.*=//'`;shift ;;
    -j*=* | --mulcore=*)               APP_MULCORE=`echo $1 | sed 's/.*=//'`;shift ;;
    -s*=* | --stat=*)                  APP_STAT=`echo $1 | sed 's/.*=//'`;shift ;;
    -v*=* | --version=*)               APP_VERSION=`echo $1 | sed 's/.*=//'`;shift ;;
    -p* | --prefix)                    APP_INSTALL_PATH="$2"; shift; shift ;;
    -n* | --name)                      APP_NAME="$2"; shift; shift ;;
    -c* | --compiler)                  CXX="$2"; shift; shift ;;
    -f* | --form)                      FORM="$2"; shift; shift ;;
    -j* | --mulcore)                   APP_MULCORE="$2"; shift; shift ;;
    -s* | --stat)                      APP_STAT="$2"; shift; shift ;;
    -v* | --version)                   APP_VERSION="$2"; shift; shift ;;
    --verbose)                         APP_DBG=1; shift ;;
    --ccache)                          CCACHE=1; shift ;;
    *)
      echo "unknown option: $1" 
      echo "$0 --help for help" 
      exit ;;
    esac
done

###########################################

echo -e "\033[40;35;1m #---------------------------------------------------------------------------------------#\033[0m"
echo -e "\033[40;35;1m #                                                                                       #\033[0m"
echo -e "\033[40;35;1m #     #     #   #  ######   ###   ####    #   #    #     #       ####    #####    ####  #\033[0m"
echo -e "\033[40;35;1m #    ##    #   #    #     #   #  #   #   #   #    #     #       #   #   #        #   #  #\033[0m"
echo -e "\033[40;35;1m #   # #   #   #    #     #   #  ####    #   #    #     #       #   #   #####    ####    #\033[0m"
echo -e "\033[40;35;1m #  ####  #   #    #     #   #  #   #   #   #    #     #       #   #   #        # #      #\033[0m"
echo -e "\033[40;35;1m # #   #  ###     #      ###   ####     ###     #     ######  ####    #####    #   #     #\033[0m"
echo -e "\033[40;35;1m #                                                                                       #\033[0m"
echo -e "\033[40;35;1m #---------------------------------------------------------------------------------------#\033[0m"
echo "*********************"
echo -e "\033[40;32;1m Check if CONFIG args valid!\033[0m"

echo -e "APP_NAME=${APP_NAME}"
echo -e "APP_VERSION=${APP_VERSION}"
echo -e "APP_INSTALL_PATH=${APP_INSTALL_PATH}"
echo -e "CXX=${CXX}"
case "${CXX}" in
    gcc|clang)   ;;
    *)
      echo -e "\033[40;31;1m ERROR   : Unknown option for --compile or -c, this only support [gcc | clang]!!!\033[0m"
      echo -e "\033[40;31;1m 	       Please configure --help or -h for help!!!\033[0m"
      exit ;;
    esac

echo -e "FORM=${FORM}"
case "${FORM}" in
    4[0-7]\;3[0-7]\;[0124578])   ;;
    *)
      echo -e "\033[40;31;1m ERROR   : Unknown option for --form or -f, this support \"40~47;30~37;[0124578]\" !!!\033[0m"
      echo -e "\033[40;31;1m 	       Please configure --help or -h for help!!!\033[0m"
      exit ;;
    esac

echo -e "APP_STAT=${APP_STAT}"
case "${APP_STAT}" in
    [1-7] | [0-1][0-1][0-1])   ;;
    *)
      echo -e "\033[40;31;1m ERROR   : Unknown option for -s or -t, this support \"[1-7] | [0-1][0-1][0-1]\" !!!\033[0m"
      echo -e "\033[40;31;1m 	       Please configure --help or -h for help!!!\033[0m"
      exit ;;
    esac

echo -e "APP_DBG=${APP_DBG}"
case "${APP_DBG}" in
    [01])   ;;
    *)
      echo -e "\033[40;31;1m ERROR   : Unknown option for --verbose, this support \"[0-1]\" !!!\033[0m"
      echo -e "\033[40;31;1m 	       Please configure --help or -h for help!!!\033[0m"
      exit ;;
    esac

echo -e "CCACHE=${CCACHE}"
case "${CCACHE}" in
    [01])   ;;
    *)
      echo -e "\033[40;31;1m ERROR   : Unknown option for --ccache, this support \"[0-1]\" !!!\033[0m"
      echo -e "\033[40;31;1m 	       Please configure --help or -h for help!!!\033[0m"
      exit ;;
    esac

if [ ${CCACHE} -eq 1 ]; then
	CXX="ccache ${CXX}"
fi

echo "*********************"
###########################################
echo -e "\033[40;32;1m Check the Dirs!!\033[0m"
if [ ! -d ${APP_ROOT}/src ]; then
  echo -e "\033[40;31;1m ERROR   : No src dir find in your root dir!!!\033[0m"
  exit;
fi
echo "*********************"

if [ ! -d ${APP_ROOT}/obj ]; then
  mkdir -p ${APP_ROOT}/obj
fi

if [ ! -d ${APP_ROOT}/bin ]; then
  mkdir -p ${APP_ROOT}/bin
fi

if [ ! -d ${APP_ROOT}/lib ]; then
  mkdir -p ${APP_ROOT}/lib
fi

#if [ ! -d ${APP_ROOT}/deps ]; then
#  mkdir -p ${APP_ROOT}/deps
#fi

if [ ! -d ${APP_ROOT}/include ]; then
  mkdir -p ${APP_ROOT}/include
fi

if [ ! -d ${APP_ROOT}/conf ]; then
  mkdir -p ${APP_ROOT}/conf
fi

if [ ! -d ${APP_ROOT}/log ]; then
  mkdir -p ${APP_ROOT}/log
fi

APP_SRC_PATH=${APP_ROOT}/src
APP_OBJ_PATH=${APP_ROOT}/obj
APP_BIN_PATH=${APP_ROOT}/bin
APP_LIB_PATH=${APP_ROOT}/lib
#APP_DEPS_PATH=${APP_ROOT}/deps
APP_INCLUDE_PATH=${APP_ROOT}/include
APP_CONF_PATH=${APP_ROOT}/conf
APP_LOG_PATH=${APP_ROOT}/log

VALID_SRC_DIRS=()
APP_INCLUDE=()
###################################
########check lib is exist#########
echo -e "\033[40;32;1m Check if DLIBS SLIBS and INCLUDE valid!\033[0m"
DLIBS=($(sed -n '/-[Ll]/p' ${MAKEFILE_CONF_PATH} | sed -e '/#/d' -e '/:/d'-e 's/.*=//' -e 's/\\//' -e 's/\r//' -e 's/\n//'))
echo "===================="
index=0
len=${#DLIBS[*]}
while [ $index -lt $len ]
do
  if [ -z ${DLIBS[$index]} ]; then
    let index++
    continue
  fi
  echo ${DLIBS[$index]} | grep -q '^-L'
  if [ $? -eq 0 ]; then
    path="$(echo "${DLIBS[$index]}" | sed 's/-L//')"
    let index=index+1

    echo ${DLIBS[$index]} | grep -q '\-l'
    if [ $? -eq 0 ]; then
      libname="$(echo "${DLIBS[$index]}" | sed -e 's/-l//')"
      ls ${path} | grep -q "${libname}*.so"

      if [ $? -eq 0 ]; then
        echo -e "SUCCESS : find ${DLIBS[$index]} in ${path}!"
      else
        echo -e "\033[40;31;1mERROR   : find no ${DLIBS[$index]} in ${path}!\033[0m"
	echo -e "you should try follow path:" 
	echo -e "$(locate "${libname}.so")"
        exit 0
      fi  
      let index=index+1
    else
      echo -e "\033[40;33;1mWRANNING : NO so file in this '-L' ${path}\033[0m"
    fi
  else  
    echo ${DLIBS[$index]} | grep -q '\-l'
    if [ $? -eq 0 ]; then
      libname="lib$(echo "${DLIBS[$index]}" | sed -e 's/-l//')"
            grep -q ${libname} ${SYS_LIBS_CACHE}
            if [ $? -eq 0 ]; then
                    echo -e "SUCCESS : find ${DLIBS[$index]} in system env!"
            else
                    echo -e "\033[40;31;1mERROR   : find no ${DLIBS[$index]} in system env!\033[0m"
		    echo -e "you should try follow path:" 
		    echo -e "$(locate "${libname}.so")"
                    exit 0
            fi
    fi
    let index++
  fi
done
echo "===================="

index=0
SLIBS=($(sed -n '/\.a/p' ${MAKEFILE_CONF_PATH} | sed -e '/#/d' -e '/:/d' -e 's/.*=//' -e 's/\\//' -e 's/\r//' -e 's/\n//'))
len=${#SLIBS[*]}
while [ $index -lt $len ]
do
  if [ -z ${SLIBS[$index]} ]; then
    let index++
    continue
  fi
  if [ -f ${SLIBS[$index]} ]; then
    echo -e "SUCCESS : find ${SLIBS[$index]}!"
  else
    echo -e "\033[40;31;1mERROR   : find no ${SLIBS[$index]}!\033[0m"
    echo -e "you should try follow path:" 
#    echo -e "${SLIBS[$index]}"
    libname="$(echo "${SLIBS[$index]}" | sed -e 's/.*\///')"
    echo -e "$(locate ${libname})"
    exit 0
  fi  
  let index=index+1
done
echo "===================="

index=0
CINCLUDE=($(sed -n '/-I/p' ${MAKEFILE_CONF_PATH} | sed -e '/#/d' -e 's/.*=//' -e 's/-I//' -e 's/\\//' -e 's/\r//' -e 's/\n//'))
len=${#CINCLUDE[*]}
while [ $index -lt $len ]
do
  if [ -z ${CINCLUDE[$index]} ]; then
    let index++
    continue
  fi
  if [ -d ${CINCLUDE[$index]} ]; then
    echo -e "SUCCESS : ${CINCLUDE[$index]} is valid!"
  else
    echo -e "\033[40;31;1mERROR   : ${CINCLUDE[$index]} is no valid!\033[0m"
    exit 0
  fi  
  let index=index+1
done
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>#
############cp makefile.subs to sub_dirs#################
echo "*********************"
echo -e "\033[40;32;1m Distribute makefile.sub!!\033[0m"
ALL_SRC_DIRS=($(find ${APP_SRC_PATH} -type d -name '[a-z]*' -print))

for elem in ${ALL_SRC_DIRS[@]}
do
	echo ${elem} | grep -q '[.]' 
	if [ $? -gt 0 ]; then
		ls ${elem} | grep -q '.[h|cpp|cc|c]$'
		if [ $? -eq 0 ]; then
			if [ -f ${elem}/makefile.sub ]; then
				diff ${elem}/makefile.sub ${MAKEFILE_SUB_PATH}
				if [ $? -gt 0 ]; then	
					cp ${MAKEFILE_SUB_PATH} ${elem}/makefile.sub
				fi
			else
				cp ${MAKEFILE_SUB_PATH} ${elem}/makefile.sub
			fi
			if [ -f ${elem}/makefile.sub.shared ]; then
				diff ${elem}/makefile.sub.shared ${MAKEFILE_SUB_SHARED_PATH}
				if [ $? -gt 0 ]; then	
					cp ${MAKEFILE_SUB_SHARED_PATH} ${elem}/makefile.sub.shared
				fi
			else
				cp ${MAKEFILE_SUB_SHARED_PATH} ${elem}/makefile.sub.shared
			fi
	
			VALID_SRC_DIRS=(${VALID_SRC_DIRS[@]} ${elem})
		fi
	fi	
done

APP_ROOT_IS_EXIST_IN_INCLUDE=0
APP_SRC_PATH_IS_EXIST_IN_INCLUDE=0
for elem in ${VALID_SRC_DIRS[@]}
do
	APP_INCLUDE=(${APP_INCLUDE[@]} "-I${elem}")
	if [ ${elem} == ${APP_ROOT} ]; then
		APP_ROOT_IS_EXIST_IN_INCLUDE=1
	fi
	if [ ${elem} == ${APP_SRC_PATH} ]; then
		APP_SRC_PATH_IS_EXIST_IN_INCLUDE=1
	fi
done
if [ ${APP_ROOT_IS_EXIST_IN_INCLUDE} -eq 0 ]; then
	APP_INCLUDE=(${APP_INCLUDE[@]} "-I${APP_ROOT}")
fi
if [ ${APP_SRC_PATH_IS_EXIST_IN_INCLUDE} -eq 0 ]; then
	APP_INCLUDE=(${APP_INCLUDE[@]} "-I${APP_SRC_PATH}")
fi
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>#
echo "*********************"
echo -e "\033[40;32;1m Generate makefile!!\033[0m"
########################generate makefile################################
echo "APP_INCLUDE=${APP_INCLUDE[@]}" > ${APP_ROOT}/makefile
####################<<ALL>>############################
echo "all:" >> ${APP_ROOT}/makefile
if [ ${#APP_STAT} -eq 3 ]; then
	tmp=0	
	let tmp+=${APP_STAT:0:1}*4
	let tmp+=${APP_STAT:1:1}*2
	let tmp+=${APP_STAT:2:1}
	APP_STAT=${tmp}
	if [ ${APP_STAT} -eq 0 ]; then
		APP_STAT=1
	fi
fi
#whether build to BIN?
BUILD_BIN=0
case "${APP_STAT}" in
    [1357]) 
	BUILD_BIN=1

	for elem in ${VALID_SRC_DIRS[@]}
do
        echo ${elem} | grep -q '[.]'
        if [ $? -gt 0 ]; then
                obj_cmd="\t @cd ${elem}; make -f makefile.sub MAKEFILE_CONF_PATH=\"${MAKEFILE_CONF_PATH}\" APP_OBJ_PATH=\"${APP_OBJ_PATH}\" APP_DBG=${APP_DBG} CXX=\"${CXX}\" FORM=\"${FORM}\" APP_INCLUDE=\"\${APP_INCLUDE}\""
                if [ ${APP_MULCORE} -gt 0 ]; then
                        obj_cmd="${obj_cmd} -j${APP_MULCORE}"
                fi
                if [ ${APP_DBG} -eq 0 ]; then
                        obj_cmd="${obj_cmd} -s"
                fi

                echo -e ${obj_cmd}>>${APP_ROOT}/makefile
        fi
done

	ld_cmd="\t @cd ${MAKEFILE_BASE_PATH}; make -f makefile.bin MAKEFILE_CONF_PATH=\"${MAKEFILE_CONF_PATH}\" APP_OBJ_PATH=\"${APP_OBJ_PATH}\" APP_DBG=${APP_DBG} CXX=\"${CXX}\" FORM=\"${FORM}\" APP_TARGET=\"${APP_BIN_PATH}/${APP_NAME}\""
	if [ ${APP_DBG} -eq 0 ]; then
        	ld_cmd="${ld_cmd} -s"
	fi

	echo -e ${ld_cmd}>>${APP_ROOT}/makefile;;
    *)
    esac

#whether build to SHARED?
case "${APP_STAT}" in
    [2367]) 
	for elem in ${VALID_SRC_DIRS[@]}
do
        echo ${elem} | grep -q '[.]'
        if [ $? -gt 0 ]; then
                obj_cmd="\t @cd ${elem}; make -f makefile.sub.shared MAKEFILE_CONF_PATH=\"${MAKEFILE_CONF_PATH}\" APP_OBJ_PATH=\"${APP_OBJ_PATH}\" APP_DBG=${APP_DBG} CXX=\"${CXX}\" FORM=\"${FORM}\" APP_INCLUDE=\"\${APP_INCLUDE}\" APP_ROOT=\"${APP_ROOT}\""
                if [ ${APP_MULCORE} -gt 0 ]; then
                        obj_cmd="${obj_cmd} -j${APP_MULCORE}"
                fi
                if [ ${APP_DBG} -eq 0 ]; then
                        obj_cmd="${obj_cmd} -s"
                fi

                echo -e ${obj_cmd}>>${APP_ROOT}/makefile
        fi
done

	fmt_cmd="\t @cd ${MAKEFILE_BASE_PATH}; make fmt -f makefile.shared MAKEFILE_CONF_PATH=\"${MAKEFILE_CONF_PATH}\" APP_OBJ_PATH=\"${APP_OBJ_PATH}\" APP_DBG=${APP_DBG} CXX=\"${CXX}\" FORM=\"${FORM}\" APP_LIB_PATH=\"${APP_LIB_PATH}\" APP_ROOT=\"${APP_ROOT}\""
	shared_cmd="\t @cd ${MAKEFILE_BASE_PATH}; make -f makefile.shared MAKEFILE_CONF_PATH=\"${MAKEFILE_CONF_PATH}\" APP_OBJ_PATH=\"${APP_OBJ_PATH}\" APP_DBG=${APP_DBG} CXX=\"${CXX}\" FORM=\"${FORM}\" APP_LIB_PATH=\"${APP_LIB_PATH}\" APP_ROOT=\"${APP_ROOT}\""
	location_cmd="\t @cd ${MAKEFILE_BASE_PATH}; make location -f makefile.shared MAKEFILE_CONF_PATH=\"${MAKEFILE_CONF_PATH}\" APP_OBJ_PATH=\"${APP_OBJ_PATH}\" APP_DBG=${APP_DBG} CXX=\"${CXX}\" FORM=\"${FORM}\" APP_LIB_PATH=\"${APP_LIB_PATH}\" APP_ROOT=\"${APP_ROOT}\""
	if [ ${APP_DBG} -eq 0 ]; then
        	fmt_cmd="${fmt_cmd} -s"
        	shared_cmd="${shared_cmd} -s"
        	location_cmd="${location_cmd} -s"
	fi

	echo -e ${fmt_cmd}>>${APP_ROOT}/makefile
	echo -e ${shared_cmd}>>${APP_ROOT}/makefile
	echo -e ${location_cmd}>>${APP_ROOT}/makefile;;
    *)
    esac

#whether build to STATIC?
case "${APP_STAT}" in
    [4567]) 
	if [ ${BUILD_BIN} -eq 0 ]; then
		for elem in ${VALID_SRC_DIRS[@]}
		do
        		echo ${elem} | grep -q '[.]'
        		if [ $? -gt 0 ]; then
                		obj_cmd="\t @cd ${elem}; make -f makefile.sub MAKEFILE_CONF_PATH=\"${MAKEFILE_CONF_PATH}\" APP_OBJ_PATH=\"${APP_OBJ_PATH}\" APP_DBG=${APP_DBG} CXX=\"${CXX}\" FORM=\"${FORM}\" APP_INCLUDE=\"\${APP_INCLUDE}\""
                		if [ ${APP_MULCORE} -gt 0 ]; then
                        		obj_cmd="${obj_cmd} -j${APP_MULCORE}"
                		fi
                		if [ ${APP_DBG} -eq 0 ]; then
                        		obj_cmd="${obj_cmd} -s"
                		fi

                		echo -e ${obj_cmd}>>${APP_ROOT}/makefile
        		fi
		done
	fi
	fmt_cmd="\t @cd ${MAKEFILE_BASE_PATH}; make fmt -f makefile.static MAKEFILE_CONF_PATH=\"${MAKEFILE_CONF_PATH}\" APP_OBJ_PATH=\"${APP_OBJ_PATH}\" APP_DBG=${APP_DBG} CXX=\"${CXX}\" FORM=\"${FORM}\" APP_LIB_PATH=\"${APP_LIB_PATH}\" APP_ROOT=\"${APP_ROOT}\""
	static_cmd="\t @cd ${MAKEFILE_BASE_PATH}; make -f makefile.static MAKEFILE_CONF_PATH=\"${MAKEFILE_CONF_PATH}\" APP_OBJ_PATH=\"${APP_OBJ_PATH}\" APP_DBG=${APP_DBG} CXX=\"${CXX}\" FORM=\"${FORM}\" APP_LIB_PATH=\"${APP_LIB_PATH}\" APP_ROOT=\"${APP_ROOT}\""
	location_cmd="\t @cd ${MAKEFILE_BASE_PATH}; make location -f makefile.static MAKEFILE_CONF_PATH=\"${MAKEFILE_CONF_PATH}\" APP_OBJ_PATH=\"${APP_OBJ_PATH}\" APP_DBG=${APP_DBG} CXX=\"${CXX}\" FORM=\"${FORM}\" APP_LIB_PATH=\"${APP_LIB_PATH}\" APP_ROOT=\"${APP_ROOT}\""
	if [ ${APP_DBG} -eq 0 ]; then
        	fmt_cmd="${fmt_cmd} -s"
        	static_cmd="${static_cmd} -s"
        	location_cmd="${location_cmd} -s"
	fi

	echo -e ${fmt_cmd}>>${APP_ROOT}/makefile
	echo -e ${static_cmd}>>${APP_ROOT}/makefile
	echo -e ${location_cmd}>>${APP_ROOT}/makefile;;
    *)
    esac

#########################<<clean>>##############################

echo "clean:" >> ${APP_ROOT}/makefile
echo -e "\t rm -f ${APP_ROOT}/bin/*" >>${APP_ROOT}/makefile
echo -e "\t rm -f ${APP_ROOT}/lib/*" >>${APP_ROOT}/makefile
echo -e "\t rm -f ${APP_ROOT}/obj/*" >>${APP_ROOT}/makefile

#########################<<distclean>>##############################
echo "distclean:" >> ${APP_ROOT}/makefile
for elem in ${VALID_SRC_DIRS[@]}
do
	echo -e "\t rm -f ${elem}/makefile.sub*" >>${APP_ROOT}/makefile
done
echo -e "\t rm -f ${APP_ROOT}/makefile" >>${APP_ROOT}/makefile
echo -e "\t rm -f ${APP_ROOT}/bin/*" >>${APP_ROOT}/makefile
echo -e "\t rm -f ${APP_ROOT}/lib/*" >>${APP_ROOT}/makefile
echo -e "\t rm -f ${APP_ROOT}/obj/*" >>${APP_ROOT}/makefile

#########################<<install>>##############################
echo "install:" >> ${APP_ROOT}/makefile
if [ ! -z ${APP_VERSION} ]; then
	echo -e "\t \$(foreach elem,\$(wildcard ${APP_LIB_PATH}/*),\$(shell mv \$(elem) \$(elem).${APP_VERSION}))">>${APP_ROOT}/makefile
fi
if [ ! -z ${APP_INSTALL_PATH} ]; then
	echo -e "\t mkdir -p ${APP_INSTALL_PATH} ">>${APP_ROOT}/makefile
	echo -e "\t cp -r ${APP_BIN_PATH} ${APP_LIB_PATH} ${APP_CONF_PATH} ${APP_LOG_PATH} ${APP_INCLUDE_PATH} ${APP_INSTALL_PATH}">>${APP_ROOT}/makefile
fi

#########################<<add x for makefile>>##############################
chmod +x ${APP_ROOT}/makefile
echo "*********************"
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>#
