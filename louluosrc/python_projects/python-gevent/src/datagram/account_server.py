#!/bin/env python
# -*- coding:utf-8 -*-
from common import general_common
from common import general_config 
from datagram import datagram_factory
from common.logger import *
import gevent
from gevent.socket import socket

#variaties reading from config file:

config=general_config.config_singleton()
check_web_sessionid_host = config.get_value('web_sessionid','web_host','127.0.0.1')
check_web_sessionid_port = config.get_value('web_sessionid','web_port','8088' )


def check_web_sessionid(sessionid, uid):
	global check_web_sessionid_host,check_web_sessionid_port
	req = {}
	req['request'] = 'getusrinfor'
	req['sessionid'] = str(sessionid)
	req['userid'] = ''
	resp = []
	sock = socket(gevent.socket.AF_INET, gevent.socket.SOCK_STREAM)
	try:
		req_data = datagram_factory.pack(req)
		sock.connect((check_web_sessionid_host, int(check_web_sessionid_port)))
		sock.send(req_data)
		resp_data = sock.recv(1024)
		resp = datagram_factory.unpack(resp_data)
		LOG_INFO('run', 'resp: ' + str(resp) + get_vrun_info())
		check_result = resp[0]['result']
		sock.close()
		if check_result[0] == '5':
			LOG_ERROR('run', 'web sessiond server can not reached!!! ' +  get_vrun_info())
			return -1
		elif check_result[0] == '4':
			LOG_ERROR('run', 'visit web sessionid server error!!!' + get_vrun_info())
			return -1
		elif check_result == '200':
			if uid == resp[0]['userno']:
				return 0
			else:
				LOG_ERROR('run', 'invalid visit!!!' + get_vrun_info())
				return -1
	except Exception, e:
		sock.close()
		LOG_ERROR('run', 'visit except!!! ' + str(e) + get_vrun_info())
		return -1

'''
import sys
sys.path.append('..')
if __name__=='__main__':
	import os
	os.chdir('../')
	check_web_sessionid_host = '10.10.41.31'
	check_web_sessionid_port = '6001'
	gevent.spawn(check_web_sessionid,'3625A37AD5BB6BDAA411A54E411522C7F2743A52F06B897257452E7B2623BF288B425E8D4E6DA0EC94BBDC25DFD4BEA908DC7A3D3B77232BC5C7C59432598A19', '' )	
	gevent.sleep(seconds=10)
'''
