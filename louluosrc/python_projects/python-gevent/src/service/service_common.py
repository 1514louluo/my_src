#-*-coding:UTF-8-*-


import sys
from common import general_common
from common import general_config
from common.logger import *
#import gevent
#from gevent import monkey
#gevent.monkey.patch_all()
#import urllib2

import memcache
import json
import sys 
reload(sys) 
sys.setdefaultencoding('utf8') 


REDIS_TIPS_PREFIX='tips221'
REDIS_GAMETIMETIPS_PREFIX='gametime223'
REDIS_GOLDCARDLEVELTIPS_PREFIX='goldcardlevel223'
REDIS_NIUXGOLDCOUPONTIPS_PREFIX='niuxgoldcoupon223'
REDIS_COMMON_PREFIX='common'

SUCCESS_ID						='1'
PARAM_ERROR_ID					='2'
FILT_LIMIT_ID					='3'
REDIS_ERROR_ID					='4'
NO_FOUND_GAMEID_ID				='5'
JSON_OPT_ERROR_ID				='6'
NO_FOUND_GOLDCARDLEVEL_ID		='7'
NO_FOUND_NIUXGOLDCOUPON_ID		='8'
INVALID_SESSION_ID				='9'
VISIT_PLATFORM_ERROR_ID			='10'
NO_FOUND_BUSINESS_ID			='11'
CONF_ERROR_ID					='12'
CLUSTER_NO_MATCH_ID				='13'


SUCCESS_STR							='ok'
PARAM_ERROR_STR						='参数错误'
FILT_LIMIT_STR						='tips弹出限制'
REDIS_ERROR_STR						='redis缓存错误'
NO_FOUND_GAMEID_STR					='gameid未找到'
JSON_OPT_ERROR_STR					='JSON解析错误'
NO_FOUND_GOLDCARDLEVEL_STR			='glodcardlevel未找到'
NO_FOUND_NIUXGOLDCOUPON_STR			='niuxgoldcoupon配置出错'
INVALID_SESSION_STR					='无效的sessionid'
VISIT_PLATFORM_ERROR_STR			='访问平台url出错'
NO_FOUND_BUSINESS_STR				='没有找到业务号'
CONF_ERROR_STR						='配置出错'
CLUSTER_NO_MATCH_STR				='聚类ID不匹配'



dict_errcode={}

dict_errcode[SUCCESS_ID]=SUCCESS_STR
dict_errcode[PARAM_ERROR_ID]=PARAM_ERROR_STR
dict_errcode[FILT_LIMIT_ID]=FILT_LIMIT_STR
dict_errcode[REDIS_ERROR_ID]=REDIS_ERROR_STR
dict_errcode[NO_FOUND_GAMEID_ID]=NO_FOUND_GAMEID_STR
dict_errcode[JSON_OPT_ERROR_ID]=JSON_OPT_ERROR_STR
dict_errcode[NO_FOUND_GOLDCARDLEVEL_ID]=NO_FOUND_GOLDCARDLEVEL_STR
dict_errcode[NO_FOUND_NIUXGOLDCOUPON_ID]=NO_FOUND_NIUXGOLDCOUPON_STR
dict_errcode[INVALID_SESSION_ID]=INVALID_SESSION_STR
dict_errcode[VISIT_PLATFORM_ERROR_ID]=VISIT_PLATFORM_ERROR_STR
dict_errcode[NO_FOUND_BUSINESS_ID]=NO_FOUND_BUSINESS_STR
dict_errcode[CONF_ERROR_ID]=CONF_ERROR_STR
dict_errcode[CLUSTER_NO_MATCH_ID]=CLUSTER_NO_MATCH_STR



def produce_result(paramdict, environ, callback):
	try:
		replystr = json.dumps(paramdict, ensure_ascii=False)
	except:
		LOG_ERROR('run','wrong response' + str(paramdict) + get_vrun_info())
		
	if 0 != len(callback):
		replystr = callback + '(' + replystr + ')'
	LOG_INFO('run','   RESPONSE---> ' + replystr + get_vrun_info())
	
	logstr=environ.get('PATH_INFO', '') + '?' + environ.get('QUERY_STRING','') + '   RESPONSE---> '+ replystr
	LOG_INFO('service',logstr+get_vrun_info())
	
	return replystr


