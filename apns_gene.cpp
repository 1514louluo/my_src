#include "apns_gene.h"
IMPL_LOGGER(ApnsGene, logger);

ApnsGene::ApnsGene()
{
    configref &config = Singleton<configref>::instance();
    m_strfilepre = config.getString("fileprefix", "");
    m_stralldt = config.getString("alldt", "");
    m_host = config.getString("host", "");
    m_port = config.getInt("port", 3306);
    m_db = config.getString("db", "");
    m_user = config.getString("user", "");
    m_passwd = config.getString("passwd", "");

    for (int i=0;i<m_threadnb; i++)
    {
      pthread_t tid;
      int result = pthread_create(&tid, NULL, ApnsGene::do_in_thread, this);
      if (result == -1)
      {
        exit(0);
      }
      m_vec_tid.push_back(tid);
    }
}

ApnsGene::~ApnsGene()
{
}

int ApnsGene::handle_mysql()
{
    MysqlClient mysql_client;
    if ( !mysql_client.connect(m_host, m_user, m_passwd, m_db, m_port))
    {
      LOG_ERROR_FMT("ERROR: connect to mysql server failed: host=%s" , m_host.c_str());
      return -1;
    }
    mysql_client.query("set names utf8;");

    unsigned int now = (unsigned int)time(NULL);
    char szquery[1024];
    snprintf(szquery, 1024, "select * from MusicCommonSettings.t_apns_gene "
                            "where Ftips_status=1 "
                            "and UNIX_TIMESTAMP(Fstart_time) >= %u "
                            "and UNIX_TIMESTAMP(Fstart_time) <= %u", now-5, now+5);
    string sql = szquery; //更新状态 结束
    MysqlData mysqldata;
    if ( !mysql_client.query(sql, mysqldata) )
    {
      LOG_ERROR( "ERROR: " << mysql_client.error() << ", sql: " << sql );
      return -1;
    }
    if(0 == mysqldata.rows()) return  1; //如果为空返回1
    for (size_t i=0; i<mysqldata.rows(); i++)
    {
        MysqlDataRow rowdata=mysqldata.get_row(i);
        APNSMSG msg;
        msg.id = atoi(rowdata["Ftips_id"].c_str());
        msg.type = atoi(rowdata["Ftype"].c_str());
        msg.filetype = atoi(rowdata["Fpackage_type"].c_str());
        msg.files = rowdata["Fpackage_name"];
        msg.msg = rowdata["Fcontent"];

        push_back(msg);
   }

    return 0;
}

int ApnsGene::push_back(APNSMSG &apnsmg)
{
    return m_queue.addqueue(apnsmg);
}

void *ApnsGene::do_in_thread(void *arg)
{
    ApnsGene *pthis = (ApnsGene *)arg;
    pthis->_do();
    return NULL;
}

void ApnsGene::_do()
{
    //设置取消点,这里是立即取消
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);

    CTmemApi cmemuinapi;
    int iRet = cmemuinapi.SetConfFile();
    if (iRet < 0)
    {
      LOG_ERROR_FMT("[%s:%d]SetConfFile fail.file[%s] iRet:%d,msg:[%s]",__FILE__,__LINE__,strTmp.c_str(),iRet, cmemuinapi.GetLastError());
      return -__LINE__;
    }
    CTmemApi cmemudidapi;
    iRet = cmemudidapi.SetConfFile();
    if (iRet < 0)
    {
      LOG_ERROR_FMT("[%s:%d]SetConfFile fail.file[%s] iRet:%d,msg:[%s]",__FILE__,__LINE__,strTmp.c_str(),iRet, cmemudidapi.GetLastError());
      return -__LINE__;
    }

    while(1)
    {
        APNSMSG msg;
        msg.id = 0;
        msg.files = "";
        msg.msg = "";
        msg.type = 0;
        msg.filetype = 0;
        int ret = m_queue.dequeue(msg);
        if(1 != ret) continue;
        if(!msg.files.empty() && !msg.msg.empty() && 0 < msg.type && 0 < msg.filetype && 0 < msg.id)
        {
            //here parse all files
            updatedb(msg, 2); //扭转状态为进行中
            parse_line_each_file(cmemuinapi, cmemudidapi, msg);
            updatedb(msg, 3); //扭转状态为执行结束
        }
    }
}

//最好不超过100
int ApnsGene::uin2dt(CTmemApi &cmemuinapi, vector<string> &vecuin, vector<string> &vecdt)
{
    vector<ssdasn::TKeyNode> vecKeyNode;
    vecKeyNode.clear();
    for (unsigned int i = 0; i < vecuin.size(); ++i)
    {
        ssdasn::TKeyNode tKeyNode;
        tKeyNode.key = vecuin[i];
        vecKeyNode.push_back(tKeyNode);
    }

    int iRet = cmemuinapi.GetList(vecKeyNode, SPP_APNS_GENE_MOD, DEVICE_TOKEN_TMEM);

    if (iRet < 0)
    {
        LOG_ERROR_FMT("get token failed, error msg = %s", cmemuinapi.GetLastError());
        return -1;
    }

    for (unsigned int i = 0; i < vecKeyNode.size(); ++i)
    {
        ssdasn::TKeyNode tKeyNode = vecKeyNode[i];
        if (0 == tKeyNode.retcode)
        {
            fetchdt(tKeyNode.data, vecdt);
        }
    }
    return 0;
}

int ApnsGene::udid2dt(CTmemApi &cmemudidapi, vector<string> &vecudid, vector<string> &vecdt)
{
    vector<ssdasn::TKeyNode> vecKeyNode;
    vecKeyNode.clear();
    for (unsigned int i = 0; i < vecudid.size(); ++i)
    {
        ssdasn::TKeyNode tKeyNode;
        tKeyNode.key = vecudid[i];
        vecKeyNode.push_back(tKeyNode);
    }

    int iRet = cmemudidapi.GetList(vecKeyNode, SPP_APNS_GENE_MOD, DEVICE_TOKEN_TMEM);

    if (iRet < 0)
    {
        LOG_ERROR_FMT("get token failed, error msg = %s", cmemudidapi.GetLastError());
        return -1;
    }

    for (unsigned int i = 0; i < vecKeyNode.size(); ++i)
    {
        ssdasn::TKeyNode tKeyNode = vecKeyNode[i];
        if (0 == tKeyNode.retcode)
        {
            fetchdt(tKeyNode.data, vecdt);
        }
    }
    return 0;
}

int ApnsGene::fetchdt(const string &strToken, vector<string> &vecdt)
{
    //获取失败返回-1，否则返回0
    string::size_type iStartpos = 0;
    string::size_type iEndpos = 0;
    string::size_type iFound = 0;
    string strTmpPiece = "";
    string strTmpToken = "";
    string strTmpTimestamp = "";
    while(1)
    {
        iEndpos = strToken.find(';', iStartpos);

        if(iEndpos != string::npos)
        {
            strTmpPiece = strToken.substr(iStartpos, iEndpos - iStartpos);
        }else
        {
            strTmpPiece = strToken.substr(iStartpos);
        }

        iFound = strTmpPiece.find_first_of(',');

        if(iFound != string::npos)
        {
            strTmpToken = strTmpPiece.substr(0, iFound);
            strTmpTimestamp = strTmpPiece.substr(iFound + 1);

            vecdt.push_back(strTmpToken);
//            break;
        }

        if(iEndpos == string::npos)
        {
            break;
        }else
        {
            iStartpos = iEndpos + 1;
        }
    }

    return 0;
}

int ApnsGene::split_by_char(vector<string> &out, string &in, char sep)
{
    int len = in.size();
    const char *begin = in.c_str();
    const char *current_elem = begin;
    const char *current_index = begin;

    while(current_index - begin < len)
    {
        if(sep == *current_index)
        {
            string elem = string(current_elem, current_index - current_elem);
            if(!elem.empty())
                out.push_back(elem);
            current_elem = current_index + 1;
        }
        ++current_index;
    }
    string elem = string(current_elem, current_index - current_elem);
    if(!elem.empty())
        out.push_back(elem);
    return 0;
}

int ApnsGene::push_batch(CTmemApi &cmemuinapi, CTmemApi &cmemudidapi, vector<string> &vecline, APNSMSG &apnsmsg)
{
    switch(apnsmsg.filetype)
    {
    case FILE_TYPE_UDID:
    {
        vector<string> vecdt;
        ret = udid2dt(cmemudidapi, vecline, vecdt);
        if(0 > ret)
        {
            LOG_ERROR_FMT("udid2dt error!");
        }
        else
        {
            send2apns(vecdt, strmsg);
        }
        break;
    }
    case FILE_TYPE_UIN:
    {
        vector<string> vecdt;
        ret = uin2dt(cmemuinapi, vecline, vecdt);
        if(0 > ret)
        {
            LOG_ERROR_FMT("udid2dt error!");
        }
        else
        {
            send2apns(vecdt, strmsg);
        }

        break;
    }
    case FILE_TYPE_DT:
    default:
    {
        send2apns(vecline, strmsg);
        break;
    }
    }
    return 0;
}

int ApnsGene::updatedb(APNSMSG &apnsmsg, int status)
{
    MysqlClient mysql_client;
    if ( !mysql_client.connect(m_host, m_user, m_passwd, m_db, m_port))
    {
      LOG_ERROR_FMT("ERROR: connect to mysql server failed: host=%s" , m_host.c_str());
      return -1;
    }
    mysql_client.query("set names utf8;");

    char szquery[1024];
    snprintf(szquery, 1024, "update MusicCommonSettings.t_apns_gene set Ftips_status = %d where Ftips_id=%u;", status, apnsmsg.id);
    string sql = szquery; //更新状态
    if ( !mysql_client.query(sql) )
    {
      LOG_ERROR( "ERROR: " << mysql_client.error() << ", sql: " << sql );
      return -1;
    }
    return 0;
}

int ApnsGene::parse_line_each_file(CTmemApi &cmemuinapi, CTmemApi &cmemudidapi, APNSMSG &apnsmsg)
{
    if(PUSH_TYPE_ALL == apnsmsg.type)
    {
        apnsmsg.files = m_stralldt;
        apnsmsg.filetype = FILE_TYPE_DT;
    }

    vector<string> vecfname;
    split_by_char(vecfname, apnsmsg.files, ';');

    int ret = 0;
    for(unsigned int i = 0; i < vecfname.size(); ++i)
    {
        string path = m_strfilepre + vecfname[i];
        FILE *_fp = fopen(path.c_str(), "r");
        if(NULL == _fp)
        {
            LOG_ERROR_FMT("error file path = %s!", path.c_str());
            continue;
        }
        vector<string> vecline;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
        if (m_fp == NULL) continue;
        while ((read = getline(&line, &len, _fp)) != -1)
        {
            string strdt(line, len);
            if(!strdt.empty())
                vecline.push_back(strdt);

            if(1000 <= vecline.size())
            {
               push_batch(cmemuinapi, cmemudidapi, vecline, apnsmsg);
               vecline.clear();
            }
        }

        if(vecline.empty())
            push_batch(cmemuinapi, cmemudidapi, vecline, apnsmsg);
        if(_fp) close(_fp);
    }

    return 0;
}

int send2apns(vector<string> &vecdt, string &strmsg)
{
    CAppleApnsPushMulClient &apnsclt = Singleton<CAppleApnsPushMulClient>::instance();
    for(unsigned int i = 0; i < vecdt.size(); ++i)
    {
        apnsclt.PackSimpleMessage(vecdt[i].c_str(), strmsg.c_str());
    }
    return 0;
}



