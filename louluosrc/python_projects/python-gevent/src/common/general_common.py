#-*-coding:UTF-8-*-
import gevent
from gevent import monkey
gevent.monkey.patch_all()
from gevent.pywsgi import WSGIServer
import sys
reload(sys)
sys.setdefaultencoding('utf8')

import os
#import MySQLdb
import datetime
import time
import ConfigParser
import urllib2

from gevent.lock import BoundedSemaphore

from gevent.queue import Queue

def querystr2dict(querystr):
	query_dict={}
	itemlist = querystr.split('&')
	for item in itemlist:
		kvlist=item.split('=')
		if len(kvlist)!=2:
			continue
		query_dict[kvlist[0]]=kvlist[1]

	return query_dict