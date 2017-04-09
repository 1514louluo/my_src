import httplib
import json
dos_url = 'dq2.niu.xunlei.com'

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

print visit_dos_service('101606','box_login')
