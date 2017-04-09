echo "start server!"
nohup python moumou.py > ../log/shellrun.log |rotatelogs  ../log/apachelog.log.%Y%m%d 100M  &
