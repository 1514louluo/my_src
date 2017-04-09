APP_INCLUDE=-I/home/louluo/nfs/my_svn/lockfree_struct/liblfds/src -I/home/louluo/nfs/my_svn/lockfree_struct/liblfds/src/inc -I/home/louluo/nfs/my_svn/lockfree_struct/liblfds/src/lfds611_freelist -I/home/louluo/nfs/my_svn/lockfree_struct/liblfds/src/lfds611_liblfds -I/home/louluo/nfs/my_svn/lockfree_struct/liblfds/src/lfds611_queue -I/home/louluo/nfs/my_svn/lockfree_struct/liblfds
all:
	 @cd /home/louluo/nfs/my_svn/lockfree_struct/liblfds/src; make -f makefile.sub MAKEFILE_CONF_PATH="/home/louluo/nfs/my_svn/lockfree_struct/liblfds/build/makefile.conf" APP_OBJ_PATH="/home/louluo/nfs/my_svn/lockfree_struct/liblfds/obj" APP_DBG=0 CXX="gcc" FORM="40;35;1" APP_INCLUDE="${APP_INCLUDE}" -s
	 @cd /home/louluo/nfs/my_svn/lockfree_struct/liblfds/src/inc; make -f makefile.sub MAKEFILE_CONF_PATH="/home/louluo/nfs/my_svn/lockfree_struct/liblfds/build/makefile.conf" APP_OBJ_PATH="/home/louluo/nfs/my_svn/lockfree_struct/liblfds/obj" APP_DBG=0 CXX="gcc" FORM="40;35;1" APP_INCLUDE="${APP_INCLUDE}" -s
	 @cd /home/louluo/nfs/my_svn/lockfree_struct/liblfds/src/lfds611_freelist; make -f makefile.sub MAKEFILE_CONF_PATH="/home/louluo/nfs/my_svn/lockfree_struct/liblfds/build/makefile.conf" APP_OBJ_PATH="/home/louluo/nfs/my_svn/lockfree_struct/liblfds/obj" APP_DBG=0 CXX="gcc" FORM="40;35;1" APP_INCLUDE="${APP_INCLUDE}" -s
	 @cd /home/louluo/nfs/my_svn/lockfree_struct/liblfds/src/lfds611_liblfds; make -f makefile.sub MAKEFILE_CONF_PATH="/home/louluo/nfs/my_svn/lockfree_struct/liblfds/build/makefile.conf" APP_OBJ_PATH="/home/louluo/nfs/my_svn/lockfree_struct/liblfds/obj" APP_DBG=0 CXX="gcc" FORM="40;35;1" APP_INCLUDE="${APP_INCLUDE}" -s
	 @cd /home/louluo/nfs/my_svn/lockfree_struct/liblfds/src/lfds611_queue; make -f makefile.sub MAKEFILE_CONF_PATH="/home/louluo/nfs/my_svn/lockfree_struct/liblfds/build/makefile.conf" APP_OBJ_PATH="/home/louluo/nfs/my_svn/lockfree_struct/liblfds/obj" APP_DBG=0 CXX="gcc" FORM="40;35;1" APP_INCLUDE="${APP_INCLUDE}" -s
	 @cd /home/louluo/nfs/my_svn/lockfree_struct/liblfds/build; make -f makefile.bin MAKEFILE_CONF_PATH="/home/louluo/nfs/my_svn/lockfree_struct/liblfds/build/makefile.conf" APP_OBJ_PATH="/home/louluo/nfs/my_svn/lockfree_struct/liblfds/obj" APP_DBG=0 CXX="gcc" FORM="40;35;1" APP_TARGET="/home/louluo/nfs/my_svn/lockfree_struct/liblfds/bin/my_app" -s
clean:
	 rm -f /home/louluo/nfs/my_svn/lockfree_struct/liblfds/bin/*
	 rm -f /home/louluo/nfs/my_svn/lockfree_struct/liblfds/lib/*
	 rm -f /home/louluo/nfs/my_svn/lockfree_struct/liblfds/obj/*
distclean:
	 rm -f /home/louluo/nfs/my_svn/lockfree_struct/liblfds/src/makefile.sub*
	 rm -f /home/louluo/nfs/my_svn/lockfree_struct/liblfds/src/inc/makefile.sub*
	 rm -f /home/louluo/nfs/my_svn/lockfree_struct/liblfds/src/lfds611_freelist/makefile.sub*
	 rm -f /home/louluo/nfs/my_svn/lockfree_struct/liblfds/src/lfds611_liblfds/makefile.sub*
	 rm -f /home/louluo/nfs/my_svn/lockfree_struct/liblfds/src/lfds611_queue/makefile.sub*
	 rm -f /home/louluo/nfs/my_svn/lockfree_struct/liblfds/makefile
	 rm -f /home/louluo/nfs/my_svn/lockfree_struct/liblfds/bin/*
	 rm -f /home/louluo/nfs/my_svn/lockfree_struct/liblfds/lib/*
	 rm -f /home/louluo/nfs/my_svn/lockfree_struct/liblfds/obj/*
install:
