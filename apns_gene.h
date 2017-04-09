/*
 * #################################################################################################
 *	Copyright (C) 2015 luolin <1514louluo@sina.com>
 *	All rights reserved.
 *	Author: luolin
 *	Date: 2015-07-17
 *	License: MIT
 *	Desc: apns gene
 * #################################################################################################
 */
#ifndef APNS_GENE_H
#define APNS_GENE_H
#include "tmllog4cplus/log4cpluslogger.h"
#include "tmsspp_apple_apns_push/spp_apple_apns_push_mul_client.h"
#include "Singleton.h"
#include "configref.h"

#define FILE_TYPE_DT 0
#define FILE_TYPE_UIN 1
#define FILE_TYPE_UDID 2

#define PUSH_TYPE_ALL 1
#define PUSH_TYPE_PKG 2

typedef struct
{
    unsigned int   id;
    int    type; //            1:全量          2:号码包方式
    int    filetype; //文件类型 0:devicetoken  1:uin  2:udid
    string files;
    string msg;
}APNSMSG;

class ApnsGene
{
public:
    //
    ApnsGene();
    ~ApnsGene();

    //对外接口 mysql 查询 如果找到合适的就放到管道进行推送
    int handle_mysql();

private:
    //thread
    static void *do_in_thread(void *arg);
    void _do();

    //convert to dt
    int uin2dt(CTmemApi &cmemuinapi, vector<string> &vecuin, vector<string> &vecdt);
    int udid2dt(CTmemApi &cmemudidapi, vector<string> &vecudid, vector<string> &vecdt);
    int fetchdt(const string &strToken, vector<string> &vecdt);
    int split_by_char(vector<string> &out, string &in, char sep);

    //parse file every line
    int parse_line_each_file(CTmemApi &cmemuinapi, CTmemApi &cmemudidapi, APNSMSG &apnsmsg);

    //send
    int push_batch(CTmemApi &cmemuinapi, CTmemApi &cmemudidapi, vector<string> &vecline, APNSMSG &apnsmsg);
    int send2apns(CAppleApnsPushMulClient &apnsclt, vector<string> &vecdt, string &strmsg);

    //update mysql status
    int updatedb(APNSMSG &apnsmsg, int status);

    //push back to queue
    int push_back(APNSMSG &apnsmg);

private:

    BlockQueue<APNSMSG> m_queue;
    vector<pthread_t>  m_vec_tid;

    //从conf 里面读取  start
    string m_strfilepre;
    string m_stralldt;

    string m_host;
    string m_user;
    string m_passwd;
    string m_db;
    int m_port;
    //从conf 里面读取  end

    DECL_LOGGER(logger);   //定义logger
};

#endif // APNS_GENE_H
