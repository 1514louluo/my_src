#-*-coding:UTF-8-*-


import sys
sys.path.append('./')
import general_common
import general_config

#import gevent
#from gevent import monkey
#gevent.monkey.patch_all()
#import urllib2

import urls

from gevent.coros import BoundedSemaphore

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

sem = BoundedSemaphore(1)
urls_function_dict={}
status_200_ok = '200 OK'
status_404_not_found = '404 NOT FOUND'
string_empty=''
global totalcount
totalcount=0
def application(environ, start_response):
	
	#body = '<p>Hello World</p>'
	pathinfo=environ['PATH_INFO']
	headers = [('Content-Type', 'text/html')]
	if urls_function_dict.has_key(pathinfo)==False:
		#print pathinfo
		start_response(status_404_not_found, headers)
		return string_empty
	start_response(status_200_ok, headers)
	sem.acquire()
	global totalcount
	temp=totalcount
	totalcount=temp+1
	sem.release()
	if totalcount%100==0:
		print totalcount
	return urls_function_dict[pathinfo](environ,start_response)

class httpserver(object):
	def __init__(self):
		print 'httpserver init'
	def start_server(self):
		config=general_config.config_singleton()
		port=config.get_value('httpinfo','http_port','10')
		for item in urls.urlconfigs:
			urls_function_dict[item[0]]=item[1]
		#print len(urls.urlconfigs)
		#for url_key in self.urls_function_dict:
		#	self.urls_function_dict[url_key]('environ', 'start_response')
		general_common.WSGIServer(('', int(port)), application).serve_forever()

		
def fetch(pid):
	while 1==1:
		sem.acquire()
		global totalcount
		temp=totalcount
		totalcount=temp+1
		sem.release()
		if totalcount%100000==0:
			print totalcount
	
	
def asynchronous():
	threads = []
	for i in range(1,500000):
		threads.append(general_common.gevent.spawn(fetch, i))
	general_common.gevent.joinall(threads)
if __name__=='__main__':
	#myserver =httpserver()
	#myserver.start_server()
	asynchronous()
	
	
	
	
	





















