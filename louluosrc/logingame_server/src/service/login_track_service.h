#pragma once


#include "mycommon.h"
#include "cmysql.h"
#include "block_queue.h"
#include "SDLogger.h"
#include "redis_client.h"
#include "commondef.h"
#include "HttpRequestPacket.h"
#include "mongo_client.h"
typedef struct _STU_TRACK_LOGINBOX_INFO
{
	std::string params;
	STU_LOGINBOX_HIS stu_loginbox_his;
}STU_TRACK_LOGINBOX_INFO;

typedef struct _STU_TRACK_LOGINGAME_INFO
{
	std::string params;
	STU_LOGINGAME_DB_INFO stu_logingame_info;
	HttpRequestPacket request;
}STU_TRACK_LOGINGAME_INFO;

// 针对登陆盒子请求和登陆游戏请求，记录日志，作为后续统计的原始数据
class login_track_service
{
public:
	login_track_service(void);
	~login_track_service(void);

	void init_login_track_service(void);

	block_queue< boost::shared_ptr<STU_TRACK_LOGINGAME_INFO> > m_queue_logingame;

	block_queue< boost::shared_ptr<STU_TRACK_LOGINBOX_INFO> > m_queue_loginbox;
	
	int get_loginbox_cache(std::string gameinternalno);
	int set_loginbox_cache(std::string gameinternalno);

	int get_logingame_cache(std::string gameinternalno);
	int set_logingame_cache(std::string gameinternalno);

private:
	DECL_LOGGER(logger);


	redis_client *m_predclient_loginbox;
	CMysql m_mysql_loginbox;

	redis_client *m_predclient_logingame;
	//CMysql m_mysql_logingame;
	mongo_client m_mongoclient;


	static void *dealwith_logibox_proc(void *param);
	void dealwith_loginbox_doit(void);

	static void *dealwith_logingame_proc(void *param);
	void dealwith_logingame_doit(void);

	void track_loginbox(STU_TRACK_LOGINBOX_INFO *pstu_loginbox);
	void track_logingame(STU_TRACK_LOGINGAME_INFO *pstu_logingame);
	
	
	int get_loginbox_db(std::string gameinternalno);

	
	int get_logingame_db(std::string gameinternalno);

};
