#-*-coding:UTF-8-*-


import sys

import urllib2

import time  
import thread  
def timer(no, interval):  
	cnt = 0  
	while cnt<10:
		try:
			response = urllib2.urlopen('http://10.10.9.9:10000/test1?')
			result = response.read() 
		except:
			print 'error'
	thread.exit_thread()  
	
	
def test(): #Use thread.start_new_thread() to create 2 new threads  
	thread.start_new_thread(timer, (1,1))  
	thread.start_new_thread(timer, (2,2))  
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (1,1))  
	thread.start_new_thread(timer, (2,2))  
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (1,1))  
	thread.start_new_thread(timer, (2,2))  
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	thread.start_new_thread(timer, (2,2)) 
	time.sleep(1000000)
	
	

if __name__=='__main__':
	print 'urls.py'
	test()





















