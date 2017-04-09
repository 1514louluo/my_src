#-*-coding:UTF-8-*-


import sys
sys.path.append('./')
sys.path.append('service')
import general_common
import general_config


#step1  please import service .py filename
import service_test1


#step2   
#please config url---> process functions here
urlconfigs=[('/test1',service_test1.process_test1)]
urlconfigs=[('/warn/rollback/update',rollback_task.process_update_rollback_config)]




if __name__=='__main__':
	print 'urls.py'
	





















