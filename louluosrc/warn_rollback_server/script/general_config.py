#-*-coding:UTF-8-*-
import sys
sys.path.append('./')

import general_common



global _config_singleton
_config_singleton = None

config_file='../conf/server.conf'

class config_module(object):
	def __init__(self):
		self.config_dict={}
		self.config = general_common.ConfigParser.ConfigParser()
		self.config.readfp(open(config_file,"rb"))
	
	#def get():
	def get_value(self,section, field, default):
		if self.config.has_option(section,field)==False:
			print 'WARN section=',section,' field=',field,'  not exist, use default=',default
			return default
		if self.config_dict.has_key(section+field)==False:
			value = self.config.get(section,field)
			self.config_dict[section+field]=value
			return value
		else:
			return self.config_dict[section+field]
		
		
		
		
def config_singleton():
	global _config_singleton
	if  not _config_singleton:
		_config_singleton = config_module()
		print 'init config_singleton'
		return _config_singleton
	else:
		print 'config_singleton already inited'
		return _config_singleton
	
	#return config_module()

if __name__=='__main__':
	#print 'general_config'
	myconfig=config_singleton()
	myconfig1=config_singleton()
	myconfig2=config_singleton()















