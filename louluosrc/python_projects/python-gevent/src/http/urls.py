#-*-coding:UTF-8-*-
from service import tips_handler

urlconfigs=[('/boxserver/tips/general/handle', tips_handler.process_general_tips),
            ('/boxserver/tips/general/conf/update', tips_handler.process_conf_update)
           ]



