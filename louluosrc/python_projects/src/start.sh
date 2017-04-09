echo "start server!"
nohup python general_tips.py > ../log/shellrun.log |rotatelogs  ../log/apachelog.log.%Y%m%d 100M  &
