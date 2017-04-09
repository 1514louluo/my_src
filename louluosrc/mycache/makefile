APP_INCLUDE=-I/home/louluo/nfs/my_svn/mycache/src -I/home/louluo/nfs/my_svn/mycache/src/cache -I/home/louluo/nfs/my_svn/mycache/src/include -I/home/louluo/nfs/my_svn/mycache/src/log -I/home/louluo/nfs/my_svn/mycache/src/mem -I/home/louluo/nfs/my_svn/mycache/src/struct -I/home/louluo/nfs/my_svn/mycache
APP_DATE	:=$(shell date '+%Y%m%d%H')
all:
	 @cd /home/louluo/nfs/my_svn/mycache/src; make -f makefile.sub -s MAKEFILE_COMM_PATH="/home/louluo/nfs/my_svn/mycache/build/makefile.comm" APP_OBJ_PATH="/home/louluo/nfs/my_svn/mycache/obj" APP_DBG=0 CXX="gcc" FORM="40;35;1" APP_INCLUDE="${APP_INCLUDE}"
	 @cd /home/louluo/nfs/my_svn/mycache/src/cache; make -f makefile.sub -s MAKEFILE_COMM_PATH="/home/louluo/nfs/my_svn/mycache/build/makefile.comm" APP_OBJ_PATH="/home/louluo/nfs/my_svn/mycache/obj" APP_DBG=0 CXX="gcc" FORM="40;35;1" APP_INCLUDE="${APP_INCLUDE}"
	 @cd /home/louluo/nfs/my_svn/mycache/src/include; make -f makefile.sub -s MAKEFILE_COMM_PATH="/home/louluo/nfs/my_svn/mycache/build/makefile.comm" APP_OBJ_PATH="/home/louluo/nfs/my_svn/mycache/obj" APP_DBG=0 CXX="gcc" FORM="40;35;1" APP_INCLUDE="${APP_INCLUDE}"
	 @cd /home/louluo/nfs/my_svn/mycache/src/log; make -f makefile.sub -s MAKEFILE_COMM_PATH="/home/louluo/nfs/my_svn/mycache/build/makefile.comm" APP_OBJ_PATH="/home/louluo/nfs/my_svn/mycache/obj" APP_DBG=0 CXX="gcc" FORM="40;35;1" APP_INCLUDE="${APP_INCLUDE}"
	 @cd /home/louluo/nfs/my_svn/mycache/src/mem; make -f makefile.sub -s MAKEFILE_COMM_PATH="/home/louluo/nfs/my_svn/mycache/build/makefile.comm" APP_OBJ_PATH="/home/louluo/nfs/my_svn/mycache/obj" APP_DBG=0 CXX="gcc" FORM="40;35;1" APP_INCLUDE="${APP_INCLUDE}"
	 @cd /home/louluo/nfs/my_svn/mycache/src/struct; make -f makefile.sub -s MAKEFILE_COMM_PATH="/home/louluo/nfs/my_svn/mycache/build/makefile.comm" APP_OBJ_PATH="/home/louluo/nfs/my_svn/mycache/obj" APP_DBG=0 CXX="gcc" FORM="40;35;1" APP_INCLUDE="${APP_INCLUDE}"
	 @cd /home/louluo/nfs/my_svn/mycache/build; make -f makefileobj -s MAKEFILE_COMM_PATH="/home/louluo/nfs/my_svn/mycache/build/makefile.comm" APP_OBJ_PATH="/home/louluo/nfs/my_svn/mycache/obj" APP_DBG=0 CXX="gcc" FORM="40;35;1" APP_TARGET="/home/louluo/nfs/my_svn/mycache/bin/my_app"
clean:
	 rm -f /home/louluo/nfs/my_svn/mycache/obj/*
distclean:
	 rm -f /home/louluo/nfs/my_svn/mycache/src/makefile.sub
	 rm -f /home/louluo/nfs/my_svn/mycache/src/cache/makefile.sub
	 rm -f /home/louluo/nfs/my_svn/mycache/src/include/makefile.sub
	 rm -f /home/louluo/nfs/my_svn/mycache/src/log/makefile.sub
	 rm -f /home/louluo/nfs/my_svn/mycache/src/mem/makefile.sub
	 rm -f /home/louluo/nfs/my_svn/mycache/src/struct/makefile.sub
	 rm -f /home/louluo/nfs/my_svn/mycache/bin/* /home/louluo/nfs/my_svn/mycache/obj/*
	 rm -f /home/louluo/nfs/my_svn/mycache/makefile
install:
	 mv /home/louluo/nfs/my_svn/mycache/bin/my_app /home/louluo/nfs/my_svn/mycache/bin/my_app_${APP_DATE}
	 echo "./my_app_${APP_DATE} daemon">/home/louluo/nfs/my_svn/mycache/bin/start.sh
	 echo "killall -9 my_app_${APP_DATE}">/home/louluo/nfs/my_svn/mycache/bin/stop.sh
	 echo "./stop.sh;sleep 1;./start.sh">/home/louluo/nfs/my_svn/mycache/bin/restart.sh
	 chmod +x /home/louluo/nfs/my_svn/mycache/bin/*.sh
