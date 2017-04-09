#-*-coding:UTF-8-*-

from service import service_common
import common.xlresource
from common.logger  import *
import time
########################################################################
class cache_service(object):
	"""实现对缓存的业务处理逻辑"""

	#----------------------------------------------------------------------
	def __init__(self):
		"""Constructor"""
	
	
	def get_remind_secs_from_now2tomorrow(self):
		now_struct = time.localtime()
		now = time.mktime(now_struct)
		pararms = (now_struct.tm_year, now_struct.tm_mon,
			       now_struct.tm_mday, 23, 
			       59, 59,now_struct.tm_wday, now_struct.tm_yday, now_struct.tm_isdst)		
		
		tomorrow = time.mktime(pararms)
		time_sep = long(tomorrow - now)
		return time_sep
	#----------------------------------------------------------------------
	def exist_and_insert(self, key, value):
		"""检查是否弹tips，返回值0表示可以弹， 返回值1表示已经弹过， 返回值-1表示redis出错"""
		myres=common.xlresource.resource_singleton()
		myredis=common.xlresource.redis.Redis(connection_pool=myres.redis_pool)
		rkey=service_common.REDIS_TIPS_PREFIX+key
		rvalue = None
		try:
			rvalue = myredis.get(rkey)
		except Exception , e:
			result = False
			LOG_ERROR('run','myredis.get error, key='+rkey+ ' ' + str(e) +get_vrun_info())
			return -1
		#print myredis.set('hello1','hello1')
		if None == rvalue:
			'''No this key!'''
			try:
				myredis.setex(rkey, value, self.get_remind_secs_from_now2tomorrow())
			except Exception , e:
				LOG_ERROR('run','myredis.set error, key='+rkey+ ' ' + str(e) +get_vrun_info())
				return -1
			return 0
		else:
			return 1
		
	def exist_and_insert_4gametime(self, key, value):
		"""检查是否弹tips，返回值0表示可以弹， 返回值1表示已经弹过， 返回值-1表示redis出错"""
		myres=common.xlresource.resource_singleton()
		myredis=common.xlresource.redis.Redis(connection_pool=myres.redis_pool)
		rkey=service_common.REDIS_GAMETIMETIPS_PREFIX+key+value
		rvalue = None
		try:
			rvalue = myredis.get(rkey)
		except Exception , e:
			result = False
			LOG_ERROR('run','myredis.get error, key='+rkey+ ' ' + str(e) +get_vrun_info())
			return -1
		#print myredis.set('hello1','hello1')
		if None == rvalue:
			'''No this key!'''
			try:
				myredis.setex(rkey, value, self.get_remind_secs_from_now2tomorrow())
			except Exception , e:
				LOG_ERROR('run','myredis.set error, key='+rkey+ ' ' + str(e) +get_vrun_info())
				return -1
			return 0
		else:
			return 1
		
	def exist_and_insert_4goldcardlevel(self, key, value):
		"""检查是否弹tips，返回值0表示可以弹， 返回值1表示已经弹过， 返回值-1表示redis出错"""
		myres=common.xlresource.resource_singleton()
		myredis=common.xlresource.redis.Redis(connection_pool=myres.redis_pool)
		rkey=service_common.REDIS_GOLDCARDLEVELTIPS_PREFIX+key
		rvalue = None
		try:
			rvalue = myredis.get(rkey)
		except Exception , e:
			result = False
			LOG_ERROR('run','myredis.get error, key='+rkey+ ' ' + str(e) +get_vrun_info())
			return -1
		#print myredis.set('hello1','hello1')
		if None == rvalue:
			'''No this key!'''
			try:
				myredis.setex(rkey, value, self.get_remind_secs_from_now2tomorrow())
			except Exception , e:
				LOG_ERROR('run','myredis.set error, key='+rkey+ ' ' + str(e) +get_vrun_info())
				return -1
			return 0
		else:
			return 1
		
	def exist_and_insert_4niuxgoldcoupon(self, key, value):
		"""检查是否弹tips，返回值0表示可以弹， 返回值1表示已经弹过， 返回值-1表示redis出错"""
		myres=common.xlresource.resource_singleton()
		myredis=common.xlresource.redis.Redis(connection_pool=myres.redis_pool)
		rkey=service_common.REDIS_NIUXGOLDCOUPONTIPS_PREFIX+key
		rvalue = None
		try:
			rvalue = myredis.get(rkey)
		except Exception , e:
			result = False
			LOG_ERROR('run','myredis.get error, key='+rkey+ ' ' + str(e) +get_vrun_info())
			return -1
		#print myredis.set('hello1','hello1')
		if None == rvalue:
			'''No this key!'''
			try:
				myredis.setex(rkey, value, self.get_remind_secs_from_now2tomorrow())
			except Exception , e:
				LOG_ERROR('run','myredis.set error, key='+rkey+ ' ' + str(e) +get_vrun_info())
				return -1
			return 0
		else:
			return 1
		
	def common_exist_and_insert(self, prefix, param, key, value, dayno):
		"""检查是否弹tips，返回值0表示可以弹， 返回值1表示已经弹过， 返回值-1表示redis出错"""
		myres=common.xlresource.resource_singleton()
		myredis=common.xlresource.redis.Redis(connection_pool=myres.redis_pool)
		rkey=service_common.REDIS_COMMON_PREFIX+prefix+param+key
		rvalue = None
		try:
			rvalue = myredis.get(rkey)
		except Exception , e:
			result = False
			LOG_ERROR('run','myredis.get error, key='+rkey+ ' ' + str(e) +get_vrun_info())
			return -1
		#print myredis.set('hello1','hello1')
		if None == rvalue:
			'''No this key!'''
			try:
				myredis.setex(rkey, value, self.get_remind_secs_from_now2tomorrow() + (dayno - 1)*86400)
			except Exception , e:
				LOG_ERROR('run','myredis.set error, key='+rkey+ ' ' + str(e) +get_vrun_info())
				return -1
			return 0
		else:
			return 1
		
	
