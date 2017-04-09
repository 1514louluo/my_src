#-*-coding:UTF-8-*-


import sys
sys.path.append('./')
sys.path.append('./service')
import general_common
import general_config
import rollback_task

import httpserver
import resource
from  logger  import *
#import gevent
#from gevent import monkey
#gevent.monkey.patch_all()
#import urllib2





'''
def fetch(pid):
	
	#with gevent.Timeout(7, False) as timeout:
	print 'pid ',pid,'start'
	response = general_common.urllib2.urlopen('http://www.163.com')
	result = response.read()
	#json_result = json.loads(result)
	#datetime = json_result['datetime']

	#print('Process %s: %s' % (pid, datetime))
	print 'pid ',pid,'finished'
	
	

def synchronous():
	for i in range(1,10):
		fetch(i)

def asynchronous():
	threads = []
	for i in range(1,10):
		threads.append(general_common.gevent.spawn(fetch, i))
	general_common.gevent.joinall(threads)

print('Synchronous:')
#synchronous()

print('Asynchronous:')
asynchronous()



'''


if __name__=='__main__':
	logger_init('../conf/logger.conf', ['root','example01'])
	resource.resource_singleton()
	rollback_task.start_warn_rollback()
	mysvr=httpserver.httpserver()
	mysvr.start_server()
	





















