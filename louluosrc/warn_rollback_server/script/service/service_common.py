#-*-coding:UTF-8-*-


import sys
sys.path.append('./')
sys.path.append('../')
import general_common
import general_config
from logger import *
#import gevent
#from gevent import monkey
#gevent.monkey.patch_all()
#import urllib2

import sys 
reload(sys) 
sys.setdefaultencoding('utf8') 


SUCCESS_ID				='1'
PARAM_ERROR_ID			='2'
TIME_ERROR_ID			='3'
SYSTEM_ERROR_ID			='4'


SUCCESS_STR				='ok'
PARAM_ERROR_STR			='参数错误'
TIME_ERROR_STR			='时间参数错误'
SYSTEM_ERROR_STR		='系统错误'



dict_errcode={}
#dict_errcode[SUCCESS_ID]=SUCCESS_STR
#dict_errcode[PARAM_ERROR_ID]=PARAM_ERROR_STR
#dict_errcode[TIME_ERROR_ID]=TIME_ERROR_STR
#dict_errcode[SYSTEM_ERROR_ID]=SYSTEM_ERROR_STR


dict_errcode[SUCCESS_ID]=SUCCESS_STR
dict_errcode[PARAM_ERROR_ID]=PARAM_ERROR_STR
dict_errcode[TIME_ERROR_ID]=TIME_ERROR_STR
dict_errcode[SYSTEM_ERROR_ID]=SYSTEM_ERROR_STR


logtype_service='service'

def produce_result(errcode, paramdict):
	kvlist=[]
	kvlist.append('result='+errcode)
	kvlist.append('errinfo='+dict_errcode[errcode])
	
	for key in paramdict:
		#print key,paramdict[key]
		kvlist.append((key+'='+paramdict[key]))
	return '&'.join(kvlist)

def log_track(path, qeurystring, errcode, paramdict):
	kvlist=[]
	kvlist.append('result='+errcode)
	kvlist.append('errinfo='+dict_errcode[errcode])
	for key in paramdict:
		#print key,paramdict[key]
		kvlist.append((key+'='+paramdict[key]))
	replystr='&'.join(kvlist)
	logstr=str(path)+str(qeurystring)+'   RESPONSE---> '+replystr
	#print 'export log  logstr',logstr
	#print type(logstr)
	LOG_INFO(logtype_service,logstr+get_vrun_info())
	#LOG_INFO(logtype_service,logstr)
	
	

if __name__=='__main__':
	print 'service_test1'
	mydict={}
	mydict['hello']='xxxxxx'
	print produce_result(SUCCESS_ID,mydict)
	





















