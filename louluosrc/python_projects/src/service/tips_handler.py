#-*-coding:UTF-8-*-
from common import general_common
from common import general_config
from service.service_common import *
from common.logger import *
from datagram import account_server
from cache import cache
from db import db
import json
import urllib
import httplib
import memcache
import hashlib

import sys 
reload(sys) 
sys.setdefaultencoding('utf8') 

config=general_config.config_singleton()
#memcache_config = config.get_value('memcache', 'memcache_config','10.1.3.46:13900')
memcache_config = config.get_value('memcache', 'memcache_config','10.10.10.199:13900')
#platform_url = config.get_value('url', 'goldcardlevel_url','dy.niu.xunlei.com')
platform_url = config.get_value('url', 'goldcardlevel_url','10.10.10.202')
dos_url = config.get_value('url', 'dos_url','dq2.niu.xunlei.com')



def check_sessionid(sessionid, uid):
	if len(sessionid)==0:
		return -1
	mc = memcache.Client([memcache_config],debug=0)
	values =  mc.get("01"+sessionid)
	if values==None:
		return -1
	else:
		try:
			adict = json.loads(values)
		except:
			LOG_ERROR('run', 'json parse error!!' + get_vrun_info())
			return -1
		if uid == adict['xlinternalno']:
			return 0
	return 1

def parse_json(input, item):
	try:
		ajson = json.loads(input)
	except:
		return str(-2)
	if None == ajson['data']:
		if 1 == int(ajson['rtn']) and 'vip' == item:
			return str(0)
		return str(-1)
	elif 0 == int(ajson['rtn']):
		return str(ajson['data'][item])
	else:
		return str(-1)

def parse_json_4dos(input):
	try:
		ajson = json.loads(input)
	except:
		return str(-2)
	if 0 == int(ajson['rtn']):
		if None == ajson['data']:
			return str(-1)
		return str(ajson['data']['id'])
	else:
		return str(-1)


def visit_dos_service(uid, sceneId):
	url_params_string = "userId="+uid+"&sceneId="+sceneId
	conn = httplib.HTTPConnection(dos_url)
	conn.request("GET", "/getSceneClusterId?"+url_params_string)
	r = conn.getresponse()
	buf = r.read()
	print buf
	return parse_json_4dos(buf)


	
def visit_glodcardlevel_interface(uid):
	url_params_string = "uid="+uid
	conn = httplib.HTTPConnection(platform_url)
	conn.request("GET", "/vip/getVipInfo.do?"+url_params_string)
	r = conn.getresponse()
	buf = r.read()
	print buf
	return parse_json(buf[9:-1], 'vip')

def visit_niuxgoldcoupon_interface(uid, days):
	url_params = []
	url_params.append("expireDays="+str(days))
	url_params.append("userId="+uid)
	url_params_string = '&'.join(url_params)
	url_params.append("resptype=plain")
	key_info = 'dlfaoenlwerflifslfe'
		
	sign_val = hashlib.md5(url_params_string+key_info).hexdigest()
	url_params.append("sign="+sign_val)
	url_params_string = '&'.join(url_params)

	conn = httplib.HTTPConnection(platform_url)
	conn.request("GET", "/league/getexpirewarncash.do?"+url_params_string)
	r = conn.getresponse()
	buf = r.read()
	print buf
	
	return parse_json(buf, 'sumMoney')

def process_conf_update(environ, start_response):
	querystr=environ.get('QUERY_STRING','')
	LOG_INFO('run', 'get one request! request = ' + querystr + get_vrun_info())
	param_dict=general_common.querystr2dict(querystr)
	
	callback=param_dict.get('callback', '')
	
	dbconf = db.db_conf()
	scenes = dbconf.update_confs()
	
	return produce_result({'result':'1'}, environ, callback)
	
def process_general_tips(environ, start_response):
	querystr=environ.get('QUERY_STRING','')
	LOG_INFO('run', 'get one request! request = ' + querystr + get_vrun_info())
	param_dict=general_common.querystr2dict(querystr)

	result_list = []
	business=param_dict.get('business', '')
	callback=param_dict.get('callback', '')
	uid =param_dict.get('uid', '')
	sessionid=param_dict.get('sessionid', '')
	if 0 == len(business) or 0 == len(sessionid) or 0 == len(uid):
		LOG_ERROR('run', 'param error !!! ' + get_vrun_info())
		response_dict = {}
		response_dict['result'] = PARAM_ERROR_ID
		response_dict['errinfo'] = dict_errcode[PARAM_ERROR_ID]
		response_dict['content'] = {}
		result_list.append(response_dict)
		return produce_result(result_list, environ, callback)
	
	'''check session'''
	'''this is box'''
	if 0 == len(callback):
		result_int = check_sessionid(sessionid, uid)
		'''this is web'''
	else:
		result_int = account_server.check_web_sessionid(sessionid, uid)
	if -1 == result or 1 == result:
		LOG_ERROR('run', 'check sessionid error!!! wrong sessionid sessionid = ' + sessionid + get_vrun_info())
		response_dict = {}
		response_dict['result'] = INVALID_SESSION_ID
		response_dict['errinfo'] = dict_errcode[INVALID_SESSION_ID]
		response_dict['content'] = {}
		result_list.append(response_dict)
		return produce_result(result_list, environ, callback)
	
	
	

	'''1rd check if match business tag'''
	dbconf = db.db_conf()
	scenes = dbconf.query_business_conf(business)
	if 0 == len(scenes):
		LOG_ERROR('run', 'no found business id!!! ' + get_vrun_info())
		response_dict = {}
		response_dict['result'] = NO_FOUND_BUSINESS_ID
		response_dict['errinfo'] = dict_errcode[NO_FOUND_BUSINESS_ID]
		response_dict['content'] = {}
		result_list.append(response_dict)
		return produce_result(result_list, environ, callback)
	
	'''2rd handle each scene'''
	if_jumpout_loop = '0'
	for scene in scenes['scene_tag'].split('|'):
		if 0 == len(scene):
			continue
		scene_rps_map = {'result':SUCCESS_ID,'errinfo':dict_errcode[SUCCESS_ID],'content':{}}
		confs_map = dbconf.query_scene_conf(scene)
		print str(confs_map) + "======"
		'''3rd handle cond'''
		restrict_cond = confs_map.get('restrict_cond_tag','')
		print "restrict_cond" + restrict_cond
		for cond_tag in restrict_cond.split('|'):
			if 0 == len(cond_tag):
				continue
			cond_map = dbconf.query_restrict_cond_conf(cond_tag)
			if 0 == len(cond_map):
				LOG_ERROR('run', 'wrong config!!! no found restrict tag of scene in restrict cond table! with scene = '+ str(scene) + get_vrun_info())
				if_jumpout_loop = CONF_ERROR_ID
				break
			'''先看配置的restrict cond key是否出现在请求中'''
			isexist = param_dict.get(cond_map['key'], '')
			if 0 == len(isexist):
				LOG_ERROR('run', 'filt restrict cond error! with scene = '+ str(scene) + get_vrun_info())
				if_jumpout_loop = FILT_LIMIT_ID
				break
			'''再看配置的restrict cond value是否为空，若不为空是否跟请求中的key对应的值是否相同'''
			if 0 < len(cond_map['value']):
				if cond_map['value'] != isexist:
					LOG_ERROR('run', 'filt restrict cond error! with scene = '+ str(scene) + get_vrun_info())
					if_jumpout_loop = FILT_LIMIT_ID 
					break
					
		'''如果restrict cond验证出现问题，直接跳出循环，执行下一个场景'''
		if '0' != if_jumpout_loop:
			scene_rps_map['result'] = if_jumpout_loop
			scene_rps_map['errinfo'] = dict_errcode[if_jumpout_loop]
			result_list.append(scene_rps_map)
			if_jumpout_loop = '0'
			continue
		'''开始unrestict cond'''
		unrestrict_cond = confs_map.get('unrestrict_cond_tag','')
		print "unrestrict_cond" + unrestrict_cond
		for cond_tag in unrestrict_cond.split('|'):
			if 0 == len(cond_tag):
				continue
			cond_map = dbconf.query_unrestrict_cond_conf(cond_tag)
			if 0 == len(cond_map):
				LOG_ERROR('run', 'wrong config!!! no found unrestrict tag of scene in unrestrict cond table! with scene = '+ str(scene) + get_vrun_info())
				if_jumpout_loop = CONF_ERROR_ID
				break
			'''开始访问聚类服务'''
			if 'clusterid' == cond_map['cond_key']:
				uid = param_dict.get('uid', '')
				if 0 == len(uid):
					LOG_ERROR('run', 'bad request!!! no found uid! with scene = '+ str(scene) + get_vrun_info())
					if_jumpout_loop = PARAM_ERROR_ID
					break
				clusterid = visit_dos_service(uid, business)
				if None == clusterid:
					LOG_ERROR('run', 'clusterid is not match!!!'+ get_vrun_info())
					if_jumpout_loop = CLUSTER_NO_MATCH_ID
					break
				elif '-2' == clusterid:
					LOG_ERROR('run', 'json parse error!!!'+ get_vrun_info())
					if_jumpout_loop = JSON_OPT_ERROR_ID
					break
				elif '-1' == clusterid:
					LOG_ERROR('run', 'clusterid unknow error!!!'+ get_vrun_info())
					if_jumpout_loop = CLUSTER_NO_MATCH_ID
					break
				else:
					scene_rps_map['clusterid'] = clusterid
		
		'''如果unrestrict cond验证出现问题，直接跳出循环，执行下一个场景'''
		if '0' != if_jumpout_loop:
			scene_rps_map['result'] = if_jumpout_loop
			scene_rps_map['errinfo'] = dict_errcode[if_jumpout_loop]
			result_list.append(scene_rps_map)
			if_jumpout_loop = '0'
			continue
	
	
		'''4rd handle response'''
		common_rtn = confs_map.get('common_rtn_tag','')
		print "common_rtn" + common_rtn
		for rtn_tag in common_rtn.split('|'):
			if 0 == len(rtn_tag):
				continue
			rtn_map = dbconf.query_common_rtn_conf(rtn_tag)
			if 0 == len(rtn_map):
				LOG_ERROR('run', 'wrong config!!! no found common rtn tag of scene in common rtn table! with scene = '+ str(scene) + get_vrun_info())
				if_jumpout_loop = CONF_ERROR_ID
				break
			rtn_key = rtn_map['content_key']
			# vip等级
			if 'grade' == rtn_key:
				uid = param_dict.get('uid','')
				if 0 == len(uid):
					LOG_ERROR('run', 'visit wrong http request, bad pargam!' + get_vrun_info())
					if_jumpout_loop = PARAM_ERROR_ID
					break
				goldcardlevel = visit_glodcardlevel_interface(uid)
				if 0 > float(goldcardlevel):
					LOG_ERROR('run', 'visit goldcardlevel url error!!! goldcardlevel = ' + str(goldcardlevel) + get_vrun_info())
					if_jumpout_loop = VISIT_PLATFORM_ERROR_ID
					break
				if goldcardlevel != rtn_map['content_cond']:
					LOG_ERROR('run', 'filt limit!!! goldcardlevel = ' + str(goldcardlevel) + get_vrun_info())
					if_jumpout_loop = FILT_LIMIT_ID
					break
				scene_rps_map['content'] = rtn_map['content_value']
				scene_rps_map['content']['clusterid'] = scene_rps_map.get('clusterid', '')
				
			# 代金券
			elif 'dayno' == rtn_key:
				uid = param_dict.get('uid','')
				if 0 == len(uid):
					LOG_ERROR('run', 'visit wrong http request, bad pargam!' + get_vrun_info())
					if_jumpout_loop = PARAM_ERROR_ID
					break
				dayno = rtn_map['content_cond']
				if 0 == len(dayno):
					LOG_ERROR('run', 'config error no dayno in common content table, scene = ' + str(scene) + get_vrun_info())
					if_jumpout_loop = CONF_ERROR_ID 
					break
				overdue = visit_niuxgoldcoupon_interface(uid, dayno)
				if 0 > float(overdue):
					LOG_ERROR('run', 'visit niuxgoldcoupon url error!!! overdue = ' + str(overdue) + get_vrun_info())
					if_jumpout_loop = VISIT_PLATFORM_ERROR_ID
					break
				elif 0 == float(overdue):
					LOG_ERROR('run', 'no coupon overdue !!! overdue = ' + str(overdue) + get_vrun_info())
					if_jumpout_loop = FILT_LIMIT_ID 
					break
				scene_rps_map['content'] = rtn_map['content_value']
				scene_rps_map['content']['clusterid'] = scene_rps_map.get('clusterid', '')
				
			elif 'gameid' == rtn_key:
				gameid = param_dict.get('gameid','')
				if 0 == len(gameid):
					LOG_ERROR('run', 'visit wrong http request, bad pargam!' + get_vrun_info())
					if_jumpout_loop = PARAM_ERROR_ID
					break
				if gameid != rtn_map['content_cond']:
					LOG_ERROR('run', 'no this gameid !!! gameid = ' + str(gameid) + get_vrun_info())
					if_jumpout_loop = FILT_LIMIT_ID 
					break
				scene_rps_map['content'] = rtn_map['content_value']
				scene_rps_map['content']['clusterid'] = scene_rps_map.get('clusterid', '')
	
			elif 'clusterid' == rtn_key:
				clusterid_value = rtn_map['content_cond']
				if 0 == len(clusterid_value):
					LOG_ERROR('run', 'config error no dayno in common content table, scene = ' + str(scene) + get_vrun_info())
					if_jumpout_loop = CONF_ERROR_ID 
					break
				clusterid_in_rps = scene_rps_map.get('clusterid', '')
				if clusterid_value != clusterid_in_rps:
					LOG_ERROR('run', 'clusterid is not match!!!' + str(clusterid_in_rps) + get_vrun_info())
					if_jumpout_loop = FILT_LIMIT_ID 
					break
				scene_rps_map['content'] = rtn_map['content_value']
				scene_rps_map['content']['clusterid'] = scene_rps_map.get('clusterid', '')
	
				
		
			
		'''如果common content 验证出现问题，直接跳出循环，执行下一个场景'''
		if '0' != if_jumpout_loop:
			scene_rps_map['result'] = if_jumpout_loop
			scene_rps_map['errinfo'] = dict_errcode[if_jumpout_loop]
			result_list.append(scene_rps_map)
			if_jumpout_loop = '0'
			continue
	

	
		fixed_rtn = confs_map.get('fixed_rtn_tag', '')
		print "fixed_rtn" + fixed_rtn
		for rtn_tag in fixed_rtn.split('|'):
			if 0 == len(rtn_tag):
				continue
			rtn_map = dbconf.query_fixed_rtn_conf(rtn_tag)
			if 0 == len(rtn_map):
				LOG_ERROR('run', 'wrong config!!! no found fixed rtn tag of scene in fixed rtn table! with scene = '+ str(scene) + get_vrun_info())
				if_jumpout_loop = CONF_ERROR_ID
				break
			scene_rps_map['content'][rtn_map['content_key']] = rtn_map['content_value']
			
		'''如果fixed content 验证出现问题，直接跳出循环，执行下一个场景'''
		if '0' != if_jumpout_loop:
			scene_rps_map['result'] = if_jumpout_loop
			scene_rps_map['errinfo'] = dict_errcode[if_jumpout_loop]
			result_list.append(scene_rps_map)
			if_jumpout_loop = '0'
			continue
	

	
		'''5rd handle task'''
		task = confs_map.get('task_conf_tag','')
		print "task" + task
		for task_tag in task.split('|'):
			if 0 == len(task_tag):
				continue
			task_map = dbconf.query_task_conf(task_tag)
			if 0 == len(task_map):
				LOG_ERROR('run', 'wrong config!!! no found task tag of scene in task table! with scene = '+ str(scene) + get_vrun_info())
				if_jumpout_loop = CONF_ERROR_ID
				break
			opt  = task_map['task_limit'].get('opt','')
			parameter = task_map['task_limit'].get('parameter','')
			days = task_map['task_limit'].get('days','')
			''''''
			if 'goldcardlevel' == opt:
				uid = param_dict.get('uid','')
				if 0 == len(uid):
					LOG_ERROR('run', 'visit wrong http request, bad pargam!' + get_vrun_info())
					if_jumpout_loop = PARAM_ERROR_ID
					break
				rcache = cache.cache_service()
				result = rcache.exist_and_insert_4goldcardlevel(uid, uid)
				if -1 == result:
					LOG_ERROR('run', 'resis error!!! ' + get_vrun_info())
					if_jumpout_loop = REDIS_ERROR_ID
					break
				elif 1 == result:
					LOG_ERROR('run', 'filt limit !!! ' +  get_vrun_info())
					if_jumpout_loop = FILT_LIMIT_ID
					break
#
			elif 'niuxgoldcoupon' == opt:
				uid = param_dict.get('uid','')
				if 0 == len(uid):
					LOG_ERROR('run', 'visit wrong http request, bad pargam!' + get_vrun_info())
					if_jumpout_loop = PARAM_ERROR_ID
					break
				rcache = cache.cache_service()
				result = rcache.exist_and_insert_4niuxgoldcoupon(uid, uid)
				if -1 == result:
					LOG_ERROR('run', 'resis error!!! ' + get_vrun_info())
					if_jumpout_loop = REDIS_ERROR_ID
					break
				elif 1 == result:
					LOG_ERROR('run', 'filt limit !!! ' +  get_vrun_info())
					if_jumpout_loop = FILT_LIMIT_ID
					break
			
			elif 'gameidtips' == opt:
				pid = param_dict.get('pid','')
				if 0 == len(pid):
					LOG_ERROR('run', 'visit wrong http request, bad pargam!' + get_vrun_info())
					if_jumpout_loop = PARAM_ERROR_ID
					break
				rcache = cache.cache_service()
				result = rcache.exist_and_insert(pid, pid)
				if -1 == result:
					LOG_ERROR('run', 'resis error!!! ' + get_vrun_info())
					if_jumpout_loop = REDIS_ERROR_ID
					break
				elif 1 == result:
					LOG_ERROR('run', 'filt limit !!! ' +  get_vrun_info())
					if_jumpout_loop = FILT_LIMIT_ID
					break
	
						
			elif 'common' == opt:
				param_value = param_dict.get('parameter','')
				if 0 == len(param_value):
					LOG_ERROR('run', 'visit wrong http request, bad pargam!' + get_vrun_info())
					if_jumpout_loop = PARAM_ERROR_ID
					break
				rcache = cache.cache_service()
				try:
					dayno_int = long(dayno)
				except:
					LOG_ERROR('run', 'dayno is error!!! ' + get_vrun_info())
					if_jumpout_loop = CONF_ERROR_ID
					break
				if dayno_int < 1:	
					LOG_ERROR('run', 'dayno is error!!! ' + get_vrun_info())
					if_jumpout_loop =  CONF_ERROR_ID
					break
				rcache = cache.cache_service()
				result = rcache.common_exist_and_insert(business, parameter, param_value, dayno, dayno_int)
				if -1 == result:
					LOG_ERROR('run', 'resis error!!! ' + get_vrun_info())
					if_jumpout_loop =  REDIS_ERROR_ID
					break
				elif 1 == result:
					LOG_ERROR('run', 'filt limit !!! ' +  get_vrun_info())
					if_jumpout_loop = FILT_LIMIT_ID
					break
	
		'''如果fixed content 验证出现问题，直接跳出循环，执行下一个场景'''
		if '0' != if_jumpout_loop:
			scene_rps_map['result'] = if_jumpout_loop
			scene_rps_map['errinfo'] = dict_errcode[if_jumpout_loop]
			result_list.append(scene_rps_map)
			if_jumpout_loop = '0'
			continue
	

	
		result_list.append(scene_rps_map)
	return produce_result(result_list, environ, callback)





