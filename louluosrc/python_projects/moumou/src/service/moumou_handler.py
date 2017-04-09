#-*-coding:UTF-8-*-
from common import general_common
from common import general_config
from service.service_common import *
from common.logger import *
from cache import cache
import json
import random

import sys 
reload(sys) 
sys.setdefaultencoding('utf8') 

def process_moumou_user(environ, start_response):
	querystr=environ.get('QUERY_STRING','')
	LOG_INFO('run', 'get one request! request = ' + querystr + get_vrun_info())
	param_dict=general_common.querystr2dict(querystr)
	
	#userid=xxxxx&imei=xxxx&imsi=xxxx&build=xxxxx;
	userid=param_dict.get('userid', '')
	imei=param_dict.get('imei', '')
	imsi=param_dict.get('imsi', '')
	build=param_dict.get('build', '')
	
	userinfo = {}
	rcache = cache.cache_service()
	if 0 == len(userid):
		#generate new userid
		uid = rcache.get_current_userid()
		if '-1' == uid:
			LOG_ERROR('run', 'resis error!!! ' + get_vrun_info())
			return produce_result({'errcode':REDIS_ERROR_ID}, environ)
		elif None == uid:
			#no uid generate, begin with 1000
			uid = '999'
		uid = str(int(uid) + 1)
		result = rcache.update_current_userid(uid)
		if -1 == result:
			LOG_ERROR('run', 'resis error!!! ' + get_vrun_info())
			return produce_result({'errcode':REDIS_ERROR_ID}, environ)
		
		#{"userid":"","balance":"","income":"0","expense":""}
		userinfo['userid'] = uid
		balance_rnd = random.randint(0,1000)
		expense_rnd = random.randint(0,1000)
		income = balance_rnd + expense_rnd
		
		userinfo['balance'] = str(balance_rnd)
		userinfo['income'] = str(income) 
		userinfo['expense'] = str(expense_rnd)
		userinfo['imei'] = str(imei)
		userinfo['imsi'] = str(imsi)
		userinfo['build'] = str(build)
		try:
			userinfo_str = json.dumps(userinfo, ensure_ascii=False)
		except:
			LOG_ERROR('run','json dumps error! userinfo = ' + str(userinfo) + get_vrun_info())
			return produce_result({'errcode':JSON_OPT_ERROR_ID}, environ)
		
		result = rcache.update_user_info_by_uid(uid, userinfo_str)
		if -1 ==result:
			LOG_ERROR('run', 'resis error!!! ' + get_vrun_info())
			return produce_result({'errcode':REDIS_ERROR_ID}, environ)
		
	else:
		# check uid
		uinfo = rcache.get_user_info_by_uid(userid)
		if -1 == uinfo:
			LOG_ERROR('run', 'resis error!!! ' + get_vrun_info())
			return produce_result({'errcode':REDIS_ERROR_ID}, environ)
		elif None == uinfo:
			LOG_ERROR('run', 'this userid is not exist!!! userid =  '+ userid + get_vrun_info())
			return produce_result({'errcode':NO_FOUND_UID}, environ)
		
		try:
			userinfo = json.loads(uinfo)
		except:
			LOG_ERROR('run','json loads error! uinfo = ' + str(uinfo) + get_vrun_info())
			return produce_result({'errcode':JSON_OPT_ERROR_ID}, environ)
		
		if imei != userinfo['imei'] or imsi != userinfo['imsi'] or build != userinfo['build']:
			LOG_ERROR('run','wrong params ' + get_vrun_info())
			return produce_result({'errcode':PARAM_ERROR_ID}, environ)
		
	
	#{"errcode:"","errinfo":"","data":{"userid":"","balance":"","income":"0","expense":""}}
	return produce_result({'errcode':SUCCESS_ID,'data':userinfo}, environ)
	


