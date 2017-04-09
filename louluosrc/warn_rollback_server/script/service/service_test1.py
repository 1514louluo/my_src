#-*-coding:UTF-8-*-


import sys
sys.path.append('./')
sys.path.append('../')
import general_common
import general_config

#import gevent
#from gevent import monkey
#gevent.monkey.patch_all()
#import urllib2


import resource

from logger import *

def process_test1(environ, start_response):
	#print environ, start_response
	myres=resource.resource_singleton()
	myredis=resource.redis.Redis(connection_pool=myres.redis_pool)
	myredis.set('hello1','hello1')
	myvalue = myredis.get('hello1')
	db=myres.mongo_connection.test
	collection=db.logingame
	#mongo_collection = resource.mongo_connection.test.logingame
	cursor = collection.find()
	count=0
	for doc in cursor:
		#print doc, type(doc)
		count=count+1
	#print count
	#LOG_INFO('root', 'process_test1')
	
	mysqldbconn=myres.mysql_pool.get()
	cursor=mysqldbconn.cursor()
	sql='select * from gamebox.trustee'
	cursor.execute(sql)
	myrow=0
	for row in cursor.fetchall():
		myrow=myrow+1
	print 'get data from mysql finished'
	general_common.time.sleep(20)
	myres.mysql_pool.put(mysqldbconn)
	return str(myrow)
	#return 'hello1'
	
	
	'''
	config = resource_singleton()
	#pool = redis.ConnectionPool(host='10.10.10.199', port=6379, db=0)
	#r = redis.Redis(connection_pool=pool)
	#r.set('foo', 'bar')
	#print r.get('foo')
	db=config.mongo_connection.test
	#print db
	collection=db.logingame
	#print collection
	cursor = collection.find()
	#cursor = collection.find({'gameinternalno':'20121120103300671'})
	#cursor = collection.find_one()
	#print cursor
	count=0
	for doc in cursor:
		#print doc, type(doc)
		count=count+1
	print count
	'''




if __name__=='__main__':
	print 'service_test1'
	





















