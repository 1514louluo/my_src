#-*-coding:UTF-8-*-
from common import singleton
from common import xlresource
from common import general_config
from common.logger  import *
from gevent.lock import BoundedSemaphore
import sys 
import json
reload(sys) 
sys.setdefaultencoding('utf8') 

########################################################################
class db_conf(object):
	"""这是一个单例"""
	__metaclass__ = singleton.singleton

	#----------------------------------------------------------------------
	def __init__(self):
		"""Constructor"""
		config=general_config.config_singleton()
		self.m_table_scene=config.get_value('mysql_table','table_scene','')
		self.m_table_business=config.get_value('mysql_table','table_business','')
		self.m_table_restrict_cond=config.get_value('mysql_table','table_restrict_cond','')
		self.m_table_unrestrict_cond=config.get_value('mysql_table','table_unrestrict_cond','')
		self.m_table_task=config.get_value('mysql_table','table_task','')
		self.m_table_fixed_content_response=config.get_value('mysql_table','table_fixed_content_response','')
		self.m_table_common_content_response=config.get_value('mysql_table','table_common_content_response','')
		
		self.m_scene_conf = {}
		self.m_business_conf = {}
		self.m_restrict_cond_conf = {}
		self.m_unrestrict_cond_conf = {}
		self.m_task_conf = {}
		self.m_fixed_content_response_conf = {}
		self.m_common_content_response_conf = {}		
		
		self.m_tips_lock=BoundedSemaphore(1)
	
	def update_confs(self):
		self.m_tips_lock.acquire()
		
		self.m_scene_conf.clear()
		self.m_business_conf.clear()
		self.m_restrict_cond_conf.clear()
		self.m_unrestrict_cond_conf.clear()
		self.m_task_conf.clear()
		self.m_fixed_content_response_conf.clear()
		self.m_common_content_response_conf.clear()
		
		self.update_restrict_cond_conf()
		self.update_unrestrict_cond_conf()
		self.update_task_conf()
		self.update_fixed_content_response_conf()
		self.update_common_content_response_conf()
		self.update_scene_conf()
		self.update_business_conf()
		
		self.m_tips_lock.release()
		
	def update_scene_conf(self):	
		myres=xlresource.resource_singleton()
		mysql_conn=myres.mysql_pool.get()

		alldata = ()

		try:
			mysql_conn.ping(True)
		except Exception,e:
			myres.mysql_pool.put(mysql_conn)
			return None

		cursor = mysql_conn.cursor()
		sql = 'select unitag,restrict_cond_tag,unrestrict_cond_tag,task_conf_tag,fixed_rtn_tag,common_rtn_tag from ' + self.m_table_scene
		try:
			cursor.execute(sql)
			alldata = cursor.fetchall()
		except Exception, e:
			LOG_ERROR('service', 'cursor.execute sql= ' + sql + get_vrun_info())

		cursor.close()
		myres.mysql_pool.put(mysql_conn)

		for row in alldata:
			key_map = {}
			key_map['restrict_cond_tag'] = str(row[1])
			key_map['unrestrict_cond_tag'] = str(row[2])
			key_map['task_conf_tag'] = str(row[3])
			key_map['fixed_rtn_tag'] = str(row[4])
			key_map['common_rtn_tag'] = str(row[5])
			
			self.m_scene_conf[str(row[0])] = key_map

	
	def update_business_conf(self):	
		myres=xlresource.resource_singleton()
		mysql_conn=myres.mysql_pool.get()

		alldata = ()

		try:
			mysql_conn.ping(True)
		except Exception,e:
			myres.mysql_pool.put(mysql_conn)
			return None

		cursor = mysql_conn.cursor()
		sql = 'select business_name,scene_tag from ' + self.m_table_business
		try:
			cursor.execute(sql)
			alldata = cursor.fetchall()
		except Exception, e:
			LOG_ERROR('service', 'cursor.execute sql= ' + sql + get_vrun_info())

		cursor.close()
		myres.mysql_pool.put(mysql_conn)

		for row in alldata:
			key_map = {}
			key_map['scene_tag'] = str(row[1])
			
			self.m_business_conf[str(row[0])] = key_map


	def update_restrict_cond_conf(self):	
		myres=xlresource.resource_singleton()
		mysql_conn=myres.mysql_pool.get()

		alldata = ()

		try:
			mysql_conn.ping(True)
		except Exception,e:
			myres.mysql_pool.put(mysql_conn)
			return None

		cursor = mysql_conn.cursor()
		sql = 'select unitag,cond_key,cond_value from ' + self.m_table_restrict_cond
		try:
			cursor.execute(sql)
			alldata = cursor.fetchall()
		except Exception, e:
			LOG_ERROR('service', 'cursor.execute sql= ' + sql + get_vrun_info())

		cursor.close()
		myres.mysql_pool.put(mysql_conn)

		for row in alldata:
			key_map = {}
			key_map['cond_key'] = str(row[1])
			key_map['cond_value'] = str(row[1])
			
			self.m_restrict_cond_conf[str(row[0])] = key_map


	def update_unrestrict_cond_conf(self):	
		myres=xlresource.resource_singleton()
		mysql_conn=myres.mysql_pool.get()

		alldata = ()

		try:
			mysql_conn.ping(True)
		except Exception,e:
			myres.mysql_pool.put(mysql_conn)
			return None

		cursor = mysql_conn.cursor()
		sql = 'select unitag,cond_key,cond_value from ' + self.m_table_unrestrict_cond
		try:
			cursor.execute(sql)
			alldata = cursor.fetchall()
		except Exception, e:
			LOG_ERROR('service', 'cursor.execute sql= ' + sql + get_vrun_info())

		cursor.close()
		myres.mysql_pool.put(mysql_conn)

		for row in alldata:
			key_map = {}
			key_map['cond_key'] = str(row[1])
			key_map['cond_value'] = str(row[1])
			
			self.m_unrestrict_cond_conf[str(row[0])] = key_map


	def update_task_conf(self):	
		myres=xlresource.resource_singleton()
		mysql_conn=myres.mysql_pool.get()

		alldata = ()

		try:
			mysql_conn.ping(True)
		except Exception,e:
			myres.mysql_pool.put(mysql_conn)
			return None

		cursor = mysql_conn.cursor()
		sql = 'select unitag,task_limit from ' + self.m_table_task
		try:
			cursor.execute(sql)
			alldata = cursor.fetchall()
		except Exception, e:
			LOG_ERROR('service', 'cursor.execute sql= ' + sql + get_vrun_info())

		cursor.close()
		myres.mysql_pool.put(mysql_conn)

		for row in alldata:
			key_map = {}
			try:
				key_map['task_limit'] = json.loads(str(row[1]))
			except:
				key_map['task_limit'] = {}
			
			self.m_task_conf[str(row[0])] = key_map


	def update_fixed_content_response_conf(self):	
		myres=xlresource.resource_singleton()
		mysql_conn=myres.mysql_pool.get()

		alldata = ()

		try:
			mysql_conn.ping(True)
		except Exception,e:
			myres.mysql_pool.put(mysql_conn)
			return None

		cursor = mysql_conn.cursor()
		sql = 'select unitag,content_key,content_value from ' + self.m_table_fixed_content_response
		try:
			cursor.execute(sql)
			alldata = cursor.fetchall()
		except Exception, e:
			LOG_ERROR('service', 'cursor.execute sql= ' + sql + get_vrun_info())

		cursor.close()
		myres.mysql_pool.put(mysql_conn)

		for row in alldata:
			key_map = {}
			key_map['content_key'] = str(row[1])
			key_map['content_value'] = str(row[2])
			
			self.m_fixed_content_response_conf[str(row[0])] = key_map


	def update_common_content_response_conf(self):	
		myres=xlresource.resource_singleton()
		mysql_conn=myres.mysql_pool.get()

		alldata = ()

		try:
			mysql_conn.ping(True)
		except Exception,e:
			myres.mysql_pool.put(mysql_conn)
			return None

		cursor = mysql_conn.cursor()
		sql = 'select unitag,content_cond,content_key,content_value from ' + self.m_table_common_content_response
		try:
			cursor.execute(sql)
			alldata = cursor.fetchall()
		except Exception, e:
			LOG_ERROR('service', 'cursor.execute sql= ' + sql + get_vrun_info())

		cursor.close()
		myres.mysql_pool.put(mysql_conn)

		for row in alldata:
			key_map = {}
			key_map['content_cond'] = str(row[1])
			key_map['content_key'] = str(row[2])
			try:
				key_map['content_value'] = json.loads(str(row[3]))
			except:
				key_map['content_value'] = {}
			
			self.m_common_content_response_conf[str(row[0])] = key_map
			
			
	def query_business_conf(self, unitag):
		self.m_tips_lock.acquire()
		scenes_map = self.m_business_conf.get(unitag, {})
		self.m_tips_lock.release()
		return scenes_map
	
	def query_scene_conf(self, unitag):
		self.m_tips_lock.acquire()
		conf_map = self.m_scene_conf.get(unitag, {})
		self.m_tips_lock.release()
		return conf_map

	def query_restrict_cond_conf(self, unitag):
		self.m_tips_lock.acquire()
		cond_map = self.m_restrict_cond_conf.get(unitag, {})
		self.m_tips_lock.release()
		return cond_map
	
	def query_unrestrict_cond_conf(self, unitag):
		self.m_tips_lock.acquire()
		cond_map = self.m_unrestrict_cond_conf.get(unitag, {})
		self.m_tips_lock.release()
		return cond_map


	def query_task_conf(self, unitag):
		self.m_tips_lock.acquire()
		task_map = self.m_task_conf.get(unitag, {})
		self.m_tips_lock.release()
		return task_map
	
	def query_fixed_rtn_conf(self, unitag):
		self.m_tips_lock.acquire()
		rtn_map = self.m_fixed_content_response_conf.get(unitag, {})
		self.m_tips_lock.release()
		return rtn_map
	
	def query_common_rtn_conf(self, unitag):	
		self.m_tips_lock.acquire()
		rtn_map = self.m_common_content_response_conf.get(unitag, {})
		self.m_tips_lock.release()
		return rtn_map
