
#pragma once



typedef struct _STU_MSG_INFO
{
	int fd;
	uint64_t tag;
	std::string tokenid;
	std::string content_path;
	std::string content;
}STU_MSG_INFO;



#define  PREFIX_SESSIONID         "01"
#define  PREFIX_GID_TO_SESSIONID  "02"
#define  PREFIX_GAMELIB           "03"
#define  PREFIX_EXIST_USER        "04"
#define  PREFIX_EXIST_GAMEUSER    "05"
#define  PREFIX_SESSION_TICKOUT   "06"   // 标识该sessionid被踢掉了
#define	 PREFIX_SESSION_ONLINE_INFO "07"  // 标识用户在线时长信息  存储在redis中，盒子后台没有使用，心跳服务器有使用

#define  PREFIX_RANDOMKEY          "randomkey"

// sesson信息结构体
typedef struct _STU_SESSION_INFO
{
	std::string username;
	std::string password;
	std::string clientip;
	std::string gameinternalno;
	std::string idcardno;
	std::string gameid;// ???????????id
	std::string useloginstate;
	std::string heartbeaturl;
	std::string comefrom;  // 渠道来源
	std::string xlinternalno; // xl内部id
	std::string startsource;  // 启动来源
}STU_SESSION_INFO;








