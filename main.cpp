#include "apns_gene.h"


int main()
{
    INIT_LOGGER_BY_CONF("../conf/log4cplus.conf"); //日志初始化

    configref &config = Singleton<configref>::instance();
    config.load("../conf/gene.conf");              //初始化配置文件

    int modid = config.getInt("modid", 380481);
    int cmd = config.getInt("cmdid", 851968);
    float timeout = (float)config.getInt("timeout", 2);
    int threadnb = config.getInt("threadnum", 10);
    int moduleid = config.getInt("moduleid", 0);
    int maxqueuesize = config.getInt("queuesize", 10);
    int queuetimeout = config.getInt("queuetimeout", 1);

    CAppleApnsPushMulClient &apnsclt = Singleton<CAppleApnsPushMulClient>::instance();
    apnsclt.init(modid, cmd, timeout, threadnb, frommoduleid, maxqueuesize, queuetimeout); //初始化apnsclient

    ApnsGene apnsgene;                 //初始化gene

    int ret = 0;
    int selecttimeout = config.getInt("", );
    struct timeval secs_sep;
    while(true)
    {
        secs_sep.tv_sec = selecttimeout;
        secs_sep.tv_usec = 0;
        ret = select(0,NULL,NULL,NULL,&secs_sep);
        if(!ret)
        {
            apnsgene.handle_mysql();
        }
        else
            cout<<"select error!!"<<endl;

    }
    return 0;
}
