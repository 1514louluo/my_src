#-*-coding:UTF-8-*-
#!/usr/local/python2.6/bin/python


import socket
import struct
import hashlib
import json
import time


def pack_buffer(content):
	mylen=len(content)
	myformat='!HH'+str(mylen)+'s'
	ss = struct.pack(myformat, 0, mylen, content)
	#print myformat, len(ss), ss[1]
	return ss

def recv_package(sock):
	headbytes=sock.recv(4)
	#print headbytes
	version, length=struct.unpack('!HH', headbytes)
	body=sock.recv(length)
	#print body
	return body
	
def jsonstr2dict(jsonstring):
    try:
        dict_obj=json.loads(jsonstring)
        #dict_obj=repr(jsonobj)
        return dict_obj
    except:
        dict_obj={}
        return dict_obj
def dict2str(mydict):
    mystr=json.dumps(mydict)
    return mystr

def dic2jsonstr(dic):
	return str(dic)
	
def string2dic(mystr):
	mydict={}
	mylist=mystr.split('&')
	for item in mylist:
		kvlist=item.split('=')
		if len(kvlist)!=2:
			print 'decode mystr error, mystr=',mystr
			return {}
		mydict[kvlist[0]]=kvlist[1]
	return mydict

def dic2string(dic):
	mylist=[]
	for key in dic:
		mylist.append('='.join([key, dic[key]]))
	return '&'.join(mylist)
	
#客户端申请key
def getkey(sock):
	path='/cli/notify/adapter/randomkey/get'
	contentlist=['path',path]
	content = '='.join(contentlist)
	buffer = pack_buffer(content)
	sock.send(buffer)
	recv_body= recv_package(sock)
	dict_body=string2dic(recv_body)
	print dict_body
	return dict_body['key']
	
#客户端发送认证请求
def send_auth(sock, randomkey):
	privatekey='Bk17dES88ApSJwsO'
	gameinternalno='109834774938943'
	key=randomkey
	md5before=gameinternalno+key+privatekey
	md5after=hashlib.md5(md5before).hexdigest()
	print 'md5=',md5after,'md5before',md5before
	paramdic={}
	paramdic['path']='/cli/notify/adapter/auth/post'
	paramdic['seqid']='12345'
	paramdic['key']=randomkey
	paramdic['sign']=md5after
	paramdic['gameinternalno']=gameinternalno
	content=dic2string(paramdic)
	print content
	buffer=pack_buffer(content)
	sock.send(buffer)
	auth_body=recv_package(sock)
	print auth_body
	

#客户端上报属性信息
def property_report(sock):
	property_dic={}
	property_dic['clienttype']='gamebox'
	property_dic['version']='1.1.15'
	property_value=dic2jsonstr(property_dic)
	#print property_value
	paramdic={}
	paramdic['path']='/cli/notify/adapter/property/post'
	paramdic['seqid']='9899789'
	paramdic['property']=property_value
	content=dic2string(paramdic)
	print content
	buffer=pack_buffer(content)
	sock.send(buffer)
	
	
def rt_property_report(sock):
	property_type='rt_gameinfos'
	property_value='[{"gameid":"000075","serverid":"100","playtime":"10000"}, {"gameid":"000085","serverid":"200","playtime":"20000"}]'
	paramdic={}
	paramdic['path']='/cli/notify/adapter/rt_property/post'
	paramdic['seqid']='1231232'
	paramdic['property_type']=property_type
	paramdic['property_value']=property_value
	content=dic2string(paramdic)
	print content
	buffer=pack_buffer(content)
	sock.send(buffer)
	
	
if __name__=='__main__':
	#print 'client_simulator.py'
	#content='hello=xxxx'
	#buffer=pack_buffer(content)
	#print buffer
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.connect(('10.10.1.58', 6800))
	mykey = getkey(sock)
	send_auth(sock, mykey)
	property_report(sock)
	time.sleep(10)
	rt_property_report(sock)
	time.sleep(10)
	
	
	
	
	
	
	
	
	
	
	