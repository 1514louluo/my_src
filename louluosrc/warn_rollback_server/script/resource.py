#-*-coding:UTF-8-*-


import general_common

import general_config
global _resource_singleton
_resource_singleton=None
import redis
import pymongo
import MySQLdb


m_log_task = None


class resource_module(object):
	def __init__(self):
		print 'resource_module init'
		self.redis_pool=None
		self.mongo_connection=None
		self.mysql_pool=None
		self.init_redis()
		self.init_mongodb()
		self.init_mysql()

	def init_redis(self):
		config=general_config.config_singleton()
		redisip=config.get_value('redis_client','redis_client_ip','127.0.0.1')
		redisport=config.get_value('redis_client','redis_client_port','0')
		print redisip, redisport
		self.redis_pool = redis.ConnectionPool(host=redisip, port=int(redisport), db=0)
		#print self.redis_pool.host, self.redis_pool.port
	
	def init_mongodb(self):
		config=general_config.config_singleton()
		mongo_ip=config.get_value('mongodb_client','mongodb_client_ip','127.0.0.1')
		mongo_port=config.get_value('mongodb_client','mongodb_client_port','0')
		print 'mongodb', mongo_ip,mongo_port
		self.mongo_connection=pymongo.Connection(mongo_ip,int(mongo_port))  
		
	def init_mysql(self):
		config=general_config.config_singleton()
		mysql_client_ip=config.get_value('mysql_client','mysql_client_ip','127.0.0.1')
		mysql_client_port=config.get_value('mysql_client','mysql_client_port','0')
		mysql_pool_count=int(config.get_value('mysql_client','mysql_pool_count','3'))
		username=config.get_value('mysql_client','username','root')
		passwd=config.get_value('mysql_client','passwd','0000')
		dbname=config.get_value('mysql_client','dbname','0000')
		self.mysql_pool = general_common.Queue()
		for icount in range(0,mysql_pool_count,1 ):
			conn=MySQLdb.connect(host=mysql_client_ip,user=username,passwd=passwd,db="stat_gamebox",charset="utf8",port=int(mysql_client_port))
			#cursor = conn.cursor() 
			self.mysql_pool.put(conn)
			#print icount
	
def resource_singleton():
	global _resource_singleton
	if  not _resource_singleton:
		_resource_singleton = resource_module()
		#print 'init _resource_singleton'
		return _resource_singleton
	else:
		#print '_resource_singleton already inited'
		return _resource_singleton
		

		
		
if __name__=='__main__':
	print 'resource.py'
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
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	