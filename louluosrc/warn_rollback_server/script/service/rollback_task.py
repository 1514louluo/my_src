#-*-coding:UTF-8-*-

import sys
sys.path.append('./')
sys.path.append('../')
import general_common
import general_config

import resource
import service_common
import json
import time
import select
import urllib2
from logger import *

rollback_config = []
linkman_config = {}
gevent_threads = []
query_url = ''
rollback_config_table = ''
linkman_config_table = ''
wran_config_table = ''

rollback_type = ['1','2','3']


def make_up_config(rollback_config_data, linkman_config_data):
    global rollback_config, linkman_config
    linkman_config.clear()
    rollback_config = []
    # first for rollback_config #
    for row in rollback_config_data:
        key_map = {}
        key_map['type']        = row[0]
        key_map['server_name'] = row[1]
        key_map['service_name'] = row[2]
        key_map['start_time'] = row[3]
        key_map['end_time'] = row[4]
        key_map['range_start_time']  = row[5]
        key_map['range_end_time']  = row[6]
        key_map['time_sep'] = row[7]
        key_map['success_rate'] = row[8]
        key_map['fail_rate'] = row[9]
        key_map['ifgroup'] = row[10]
        key_map['volumn'] = row[11]
        key_map['linkman'] = row[13]

        value_map = json.loads(row[12])

        elem = []
        elem.append(key_map)
        elem.append(value_map)

        rollback_config.append(elem)
    # second for linkman_config #
    for row in linkman_config_data:
        linkman = row[0]
        mailbox = row[1]
        phone  = row[2]

        value_map = {}
        value_map['mailbox'] = mailbox
        value_map['phone'] = phone
        linkman_config[linkman] = value_map
    

def load_mysql_rollback_config(table_name):
    config = resource_singleton()
    mysql_conn = config.mysql_pool.get()
    
    cursor = mysql_conn.cursor()

    sql = "select type,server_name,service_name,start_time,end_time,range_start_time,range_end_time,time_sep," +
            "success_rate,fail_rate,ifgroup,volumn,parameter_list,linkman " + table_name
    cursor.execute(sql)
    alldata = cursor.fetchall()
    cursor.close()
    conn.close()
    
    return alldata

def load_mysql_linkman_config(table_name):
    config = resource_singleton()
    mysql_conn = config.mysql_pool.get()
    
    cursor = mysql_conn.cursor()

    sql = "select name,mailbox,phone " + table_name
    cursor.execute(sql)
    alldata = cursor.fetchall()
    cursor.close()
    conn.close()
    
    return alldata

def dict_2url(paramdict):
    kvlist=[]
    for key in paramdict:
	#print key,paramdict[key]
	kvlist.append((key+'='+paramdict[key]))
    return '&'.join(kvlist)
	
	
def query(config):
    global query_url
    base = config[0]
    pargam = config[1]
    
    # 设置过滤条件
    filter_list=[]
    for key in pargam:
        filter_dict_1 = {"field":key, "value":pargam[key]}
        filter_list.append(filter_dict_1)

    filter_dict_2 = {"field":"server_name", "value":base['server_name']}
    filter_dict_3 = {"field":"service_name", "value":base['service_name']}
    filter_list.append(filter_dict_2)
    filter_list.append(filter_dict_3)

    #设置group条件
    group_list=[]
    for group_elem in str(base['ifgroup']).split('|'):
        group_dict1={"groupfield":group_elem}
        group_list.append(group_dict1)

    #设置是否需要做count操作
    iscount='1'
	
    str_filter_cond=urllib.quote(str(filter_list).replace("'",'"') )
    str_group_cond=urllib.quote(str(group_list).replace("'",'"') )
	
    param_dict={}
    param_dict['group']=str_group_cond
    param_dict['filter']=str_filter_cond
    param_dict['isneed_count']=iscount
    param_dict['dbname']='test'
    param_dict['tablename']=base['server_name']
    param_dict['starttime']= base['range_start_time']
    param_dict['endtime']=  base['range_end_time']
    request_params=dict_2url(param_dict)
    print str(filter_list)
    print str(group_list)
    response = urllib2.urlopen(query_url+request_params)
    reply_str = response.read()
    print reply_str
    return reply_str

def notify_linkman(config):
    while True
        select.select([],[],[],60)
        mysql_conn = resource_singleton().mysql_pool.get()
        cursor = mysql_conn.cursor()

        sql = "select time, message, linkman, mailbox, status from " + wran_config_table
        cursor.execute(sql)
        alldata = cursor.fetchall()
        cursor.close()
        conn.close()

        for row in alldata:
            status = row[4]
            if '0' == status:
                
    pass

def flush2linkman(config):
    mysql_conn = resource_singleton().mysql_pool.get()
    cursor = mysql_conn.cursor()

    sql = "insert into " + wran_config_table + "(time, message, linkman, mailbox, status) value (%s,%s,%s,%s,%s)"
    cursor.execute(sql)
    cursor.close()
    conn.close()
    
    pass

def parse_query_response(reply_str):
    reply_list = reply_str.split('&')
    reply = {}
    content = ""
    for elem in reply_list:
        tmp = elem.split('=')
        reply[tmp[0]] = tmp[1]
    if '1' == reply['result']:
        content = json.loads(reply['content'])

    return content

def worker(config):
    base = config[0]
    my_type = base['type']
    start_time = long(base['start_time'])
    end_time = long(base['end_time'])
    success_rate = int(base['success_rate'])
    time_sep = long(base['time_sep'])
    volumn = long(base['volumn'])

    while True:
        now = long(time.time())
        if now > end_time or now < start_time:
            return
        select.select([],[],[],time_sep)
        content = parse_query_response(query(config))
        sum = 0
        for adict in content:
            sum += long(adict['count'])
        if '1' == my_type:
            if sum >= volumn:
                flush2linkman(config)
                
        elif '2' == my_type:
            config[1]['result'] = '1'
            content = parse_query_response(query(config))
            success_sum = 0
            for adict in content:
                success_sum += long(adict['count'])
            if success_sum * 100/sum < success_rate:
                flush2linkman(config)
            

def INIT_WARN_ROLLBACK():
    config=general_config.config_singleton()
    rollback_config_table = config.get_value('rollback_config','rollback_config_table','rollback_config')
    linkman_config_table = config.get_value('linkman_config','linkman_config_table','linkman_config')
    query_url = config.get_value('query_url','query_url','http://10.10.10.199')

def start_warn_rollback():
    global rollback_config, linkman_config, gevent_threads, rollback_config_table, linkman_config_table
    
    for thread in gevent_threads:
        gevent.kill(thread)

    rollback_data = load_mysql_rollback_config(rollback_config_table)
    linkman_data = load_mysql_linkman_config(linkman_config_table)
    make_up_config(rollback_data, linkman_data)
    
    for config in rollback_config:
        gevent_threads.append(gevent.spawn(worker, config))
    

def process_update_rollback_config(environ, start_response):
    querystr=environ.get('QUERY_STRING','')
    param_dict=general_common.querystr2dict(querystr)
    flag=param_dict.get('flag','')
    if 'warn_rollback' != flag:
        return produce_result(PARAM_ERROR_ID, [])

    start_warn_rollback()
    return produce_result(SUCCESS_STR, [])










