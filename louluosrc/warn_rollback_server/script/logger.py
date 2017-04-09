# -*- coding:utf-8 -*-

import general_common
import logging
from logging import config
import gevent
from gevent.event import Event
from gevent.queue import Queue

#---------------------------------#
# log_task = [type, level, msg]
# type: which logger
# level:  INFO | DEBUG | ERROR | WARN | CRITICAL
# msg : log main msg
#---------------------------------#
m_log_task = None
m_evt = None
m_loggers = {}       # {'name':logger}
m_thread_ctx = None
	
def log_opt_info(type, msg):
	global m_loggers
	m_loggers[type].info(msg) 

def log_opt_error(type, msg):
	global m_loggers
	m_loggers[type].error(msg) 

def log_opt_warn(type, msg):
	global m_loggers
	m_loggers[type].warn(msg) 

def log_opt_debug(type, msg):
	global m_loggers
	m_loggers[type].debug(msg) 

def log_opt_critical(type, msg):
	global m_loggers
	m_loggers[type].critical(msg) 

def process_log_loop():
	global m_log_task,m_evt,m_loggers
	callback_func = {
			'INFO'     :  log_opt_info,
			'DEBUG'    :  log_opt_debug, 
			'ERROR'    :  log_opt_error, 
			'WARN'     :  log_opt_warn, 
			'CRITICAL' :  log_opt_critical
			}
	while True:
	    m_evt.wait()
	    m_evt.clear()
	    while not m_log_task.empty():
                log_task = m_log_task.get()
		logger_type = log_task[0]
		log_level = log_task[1]
		log_msg = log_task[2]

		callback_func[log_level](logger_type, log_msg)



def logger_init(path, items):
	global m_log_task,m_evt,m_loggers, m_thread_ctx

	logging.config.fileConfig(path)

	for elem in items:
            m_loggers[elem] = logging.getLogger(elem)

        m_evt = Event()
        m_log_task = Queue()

	m_thread_ctx = gevent.spawn(process_log_loop)

		
def LOG_INFO(type, msg):
	global m_log_task,m_evt
	m_log_task.put_nowait([type, 'INFO', msg])
	m_evt.set()

def LOG_WARN(type, msg):
	global m_log_task,m_evt
	m_log_task.put_nowait([type,'WARN', msg])
	m_evt.set()

def LOG_ERROR(type, msg):
	global m_log_task,m_evt
	m_log_task.put_nowait([type,'ERROR',msg])
	m_evt.set()

def LOG_DEBUG(type, msg):
	global m_log_task,m_evt
	m_log_task.put_nowait([type,'DEBUG',msg])
	m_evt.set()

def LOG_CRITICAL(type, msg):
	global m_log_task,m_evt
	m_log_task.put_nowait([type,'CRITICAL',msg])
	m_evt.set()
	
