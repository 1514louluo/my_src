#-*-coding:UTF-8-*-
import sys 
reload(sys) 
sys.setdefaultencoding('utf8') 
import gevent
from common.logger import *
from common import general_common
from common import general_config
from common import xlresource
from db import db

from http import httpserver

import os

if __name__=='__main__':
	while True:
		try:
			unsure_pid = os.fork()
			if 0 == unsure_pid: # 子进程
				break
			else:               #  父进程
				try:
					os.waitpid(unsure_pid, 0)
				except OSError,e:
					print 'waitpid error!! you need reboot server again!!'
					sys.exit(1)
				print 'child process exit, restart child process'
		except OSError,e:
			print 'fork error！！ you need reboot server again!!'
			sys.exit(1)
	
	logger_init('../conf/logger.conf', ['root','service','run'])
	xlresource.resource_singleton()
	dbconf = db.db_conf()
	dbconf.update_confs()
	
	print 'service', '<===============service begin!!!=================> '
	LOG_INFO('run', '<===============service begin!!!=================> ' + get_vrun_info())
	LOG_INFO('service', '<===============service begin!!!=================> ' + get_vrun_info())
	mysvr=httpserver.httpserver()
	mysvr.start_server()
