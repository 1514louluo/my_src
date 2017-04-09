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
	def get_user_info_by_uid(self, uid):
		myres=common.xlresource.resource_singleton()
		myredis=common.xlresource.redis.Redis(connection_pool=myres.redis_pool)
		rkey=service_common.REDIS_MOUMOU_PREFIX+uid
		rvalue = None
		try:
			rvalue = myredis.get(rkey)
		except Exception , e:
			LOG_ERROR('run','myredis.get error, key='+rkey+ ' ' + str(e) +get_vrun_info())
			return '-1'
		return rvalue
	
		#----------------------------------------------------------------------
	def update_user_info_by_uid(self, uid, value):
		myres=common.xlresource.resource_singleton()
		myredis=common.xlresource.redis.Redis(connection_pool=myres.redis_pool)
		rkey=service_common.REDIS_MOUMOU_PREFIX+uid
		try:
			myredis.setex(rkey, value, self.get_remind_secs_from_now2tomorrow())
		except Exception , e:
			LOG_ERROR('run','myredis.get error, key='+rkey+ ' ' + str(e) +get_vrun_info())
			return -1
		return 0
	
	#----------------------------------------------------------------------
	def update_current_userid(self, uid):
		myres=common.xlresource.resource_singleton()
		myredis=common.xlresource.redis.Redis(connection_pool=myres.redis_pool)
		rkey=service_common.REDIS_CURRENT_UID_PREFIX
		try:
			myredis.setex(rkey, uid, self.get_remind_secs_from_now2tomorrow())
		except Exception , e:
			LOG_ERROR('run','myredis.get error, key='+rkey+ ' ' + str(e) +get_vrun_info())
			return -1
		return 0
	
	#----------------------------------------------------------------------
	def get_current_userid(self):
		myres=common.xlresource.resource_singleton()
		myredis=common.xlresource.redis.Redis(connection_pool=myres.redis_pool)
		rkey=service_common.REDIS_CURRENT_UID_PREFIX
		rvalue = None
		try:
			rvalue = myredis.get(rkey)
		except Exception , e:
			LOG_ERROR('run','myredis.get error, key='+rkey+ ' ' + str(e) +get_vrun_info())
			return '-1'
		return rvalue
	