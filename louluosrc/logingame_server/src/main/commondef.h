

#ifndef __COMMONDEF__
#define __COMMONDEF__   1


#include "stdint.h"
#include <map>
#include <string>
//typedef std::map<std::string, std::string> param_type;

typedef struct _STU_REQUEST_OBJ 
{
	int clientfd;
	uint64_t client_fd_tag;
	std::map<std::string, std::string> map_;

};

enum enum_login_state
{
	enum_login_state_username=0,
	enum_login_state_logincookie=1,
	enum_login_state_tourist=2,
};




//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
// 登陆注册协议集

#define  GAMEBOX_SERVER_BOXSERVER_LOGINGAME "/boxserver/loginGame"               // 登陆游戏
#define  GAMEBOX_SERVER_BOXSERVER_RAPELOGINGAME "/boxserver/rapeLoginGame"               // 推新登陆游戏
#define  GAMEBOX_SERVER_BOXSERVER_SLAVELOGINGAME "/boxserver/slaverLoginGame"      // 小号登陆游戏
#define  GAMEBOX_SERVER_BOXSERVER_FLUSHGAMEINFO   "/boxserver/flushGameInfo"         //提示更新数据库，为了推新登陆游戏         
#define  GAMEBOX_SERVER_BOXSERVER_SPECIALLOGINGAME   "/boxserver/specialLoginGame"         //
#define  GAMEBOX_SERVER_BOXSERVER_FLUSHGAMEINFOSPE   "/boxserver/flushGameInfoSpe"         //提示更新数据库，为了推新登陆游戏         
//---------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------

// 登陆注册协议集
#define  GAMEBOX_SERVER_BOXSERVER_LOGINGAME_ID       0                           // 登陆游戏
#define  GAMEBOX_SERVER_BOXSERVER_SLAVELOGINGAME_ID  1                           // 小号登陆游戏
#define  GAMEBOX_SERVER_BOXSERVER_RAPELOGINGAME_ID       2                           // 推新登陆游戏
#define  GAMEBOX_SERVER_BOXSERVER_FLUSHGAMEINFO_ID       3                           //提示更新数据库，为了推新登陆游戏       
#define  GAMEBOX_SERVER_BOXSERVER_SPECIALLOGINGAME_ID  4
#define  GAMEBOX_SERVER_BOXSERVER_FLUSHGAMEINFOSPE_ID   5

enum enum_err_code
{
	SUCCESS=1, 
	PARAM_ERROR=2, 
	INVALID_SESSION=3, 
	LOGINED=4, 
	NOT_EXIST_SMALL=11, 
	INVALID_MESSAGE=12,
	INVALID_IDCARDNO=13,
	INVALID_AGREE_VALUE=14, 
	NOT_EXIST_TRUSTEE=15, 
	INVALID_REGISTER_SOURCE=16, 
	EXIST_TRUSTEE=17, 
	EXIST_SMALL=18, 
	SAME_USER=19, 
	SMALL_OVER_LIMIT=20, 
	TRUSTEE_SMALL_OVER_LIMIT=21, 
	NOT_EXIST_TOURISTID=22, 
	TOURIST_PWD_ERROR=23,
	TOURISTID_LIMIT=24, 
	TOURISTID_GAMEID_ERROR=25, 
	NO_USER_LOGININFO=26,
	IP_LIMITED_CITY=93, 
	LOGIN_ORTHER=94, 
	ILLEGAL_ACCESS=95, 
	NOT_PERMISSION=96, 
	CACHE_ERROR=97, 
	DB_ERROR=98,
	SYSTEM_ERROR=99, 
	ACCOUNT_IO_ERROR=100, 
	ACCOUNT_UNKOWN_ERROR=101,
	ACCOUNT_PARAM_ERROR=102, 
	ACCOUNT_USER_NOT_EXIST=103, 
	ACCOUNT_PASS_ERROR=104, 
	ACCOUNT_TEMP_PASS_ERROR=105,
	ACCOUNT_EXIST_WHOLE_BLACK_LIST=106, 
	ACCOUNT_EXIST_BLACK_LIST=107, 
	ACCOUNT_NOT_EXIST_WHITE_LIST=109,  
	ACCOUNT_EXIST=110, 
	ACCOUNT_NOT_EXIST=111,
	INDIVIDUAL_CENTER_EORROR=112, 
	VERIFY_KEY_LEN_ERROR=201, 
	VERIFY_CODE_ERROR=202, 
	VERIFY_CODE_REVERIFY=203,
	VERIFY_CODE_TIMEOUT=204, 
	VERIFY_KEY_ERROR=205, 
	VERIFY_TYPE_ERROR=206,
	VERIFY_TYPE_NOT_EXIST=207,
	VERIFY_SERVER_ERROR=208, 
	LOGIN_IO_ERROR=300, 
	MD5_VERIFY_ERROR=301, 
	LOGIN_GAME_PARAM_ERROR=302, 
	OTHER_SYS_ERROR=303,
	LOGIN_GAME_ILLEGAL_ACCESS=304,
	INVALID_LOGIN_URL=305, 
	JUMP_SERVER_ERROR=401, 
	JUMP_SERVER_IO=402, 
	JUMP_KEY_FAILED=403,
	INVALID_JUMP_KEY=404, 
	JUMP_SERVER_INNER_ERROR=405, 
	JUMP_ACCOUNT_NOT_EXIST=406, 
	JUMP_INVSLID_ACCOUNT=407, 
	USER_LOCKED_OUT=408, 
	JUMP_NULL_SESSIONID=409,
	JUMP_SERVER_UNKOWN_ERROR=410, 
	BIND_TOURIST_SERVER_UNKOWN_ERROR=501, 
	BIND_TOURIST_FAILED=502, 
	BIND_TOURIST_XLUSER_SAME_GAME=503, 
	BIND_TOURIST_SERVER_IO_ERROR=504, 
	GAME_VIEWER_SERVER_IO_ERROR=600, 
	SIGN_FILTER=601, 
	GAME_PLAY_SERVER_IO_ERROR=602, 
	NOT_EXIST_SYSTEM_MESSAGE=701, 
	NO_UPDATE_SYSTEM_MESSAGE=702, 
	NO_SELECT_SYSTEM_MESSAGE=703, 
	NO_MESSAGE_ADDRESS=704, 
	NOT_EXIST_SYSTEM_REPLY=705, 
	NO_UPDATE_SYSTEM_REPLY=706, 
	NO_SELECT_SYSTEM_REPLY=707, 
	NOT_EXIST_USER_FEEDBACK=708, 
	FEEDBACK_REPLYED=709, 
	WEBLOGIN_ILLEGAL_ACCESS=801, 
	WEBLOGIN_PARAM_ERROR=802, 
	WEBLOGIN_IO_ERROR=803, 
	WEBLOGIN_NO_EXIST_USER=804, 
	WEBLOGIN_USERNAME_FREEZE=805, 
	WEBLOGIN_PASSWORD_ERROR=806, 
	WEBLOGIN_OTHER_ERROR=807, 
	GIFT_SERVER_IO_ERROR=901, 
	GIFT_NO_FOR_TOURIST=902, 
	GIFT_SYSTEM_ERROR=903, 
	GIFT_PARAM_ERROR=904,
	GIFT_TIMESTAMP_OVERTIEM=905, 
	GIFT_INVALID_SIGN=906, 
	GIFT_NOTFOR_GAME=907,
	GIFT_NO_GIFT=908, 
	GIFT_HAS_GOT=909, 
	GIFT_USER_NOT_IN_GIFTLIST=910,
	GAMELIB_GAME_EXIST=1000,
	INTERNAL_TIMEOUT=2000,
	LOGINBOX_REDIRECT=2001,
	FLUSH_GAMEINFO_ERROR=2002,
	
};

// 游戏登陆服务器返回的防沉迷返回
#define  WEBGAEM_LOGIN_RTN_GAMELOGINSTATUS_PREVENT  "97"

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
	// 后续新增
	std::string comefrom2;
	std::string comefrom3;   
}STU_SESSION_INFO;


// trustee表结构字段信息
typedef struct _STU_TRUSTEE_INFO
{
	uint64_t seqid;
	std::string mastergid;
	std::string mastername;
	std::string masteralias; //大号对小号的命名，并不是大号自己的昵称
	std::string slavergid;
	std::string slavername;
	std::string slaveralias;
	std::string gameid;
	std::string serverid;
	int			status;	// 0:正在授理托管，1:已完成托管
	std::string idcardno;
	int         smallsource;  // 小号来源，0，直接添加小号   1 通过托管申请
}STU_TRUSTEE_INFO;

typedef struct _STU_MESSAGE_0001
{
	uint64_t seqid;
	std::string mastergid;
	std::string mastername;
	std::string slavergid;
	std::string slavername;
	std::string slaveralias;
	std::string gameid;
	std::string serverid;
	int			status;
	std::string updatetime;
	std::string masteralias;////大号对小号的命名，并不是大号自己的昵称
	std::string idcardno;
}STU_MESSAGE_0001;
  
// 定义gamebox.message_0001表中status字段取值
enum enum_TrusteeStatus 
{
	TrusteeStatus_INITIALIZED=1, 
	TrusteeStatus_REQUESTED=2, 
	TrusteeStatus_DELAY_REQUEST=3, 
	TrusteeStatus_RETRY_REQUEST=4, 
	TrusteeStatus_REQUEST_FAIL=5, 
	TrusteeStatus_RESPONSE=6, 
	TrusteeStatus_RETRY_RESPONSE=7, 
	TrusteeStatus_RESPONSE_FAIL=8,
};

// 登陆返回结果
typedef struct _STU_LOGIN_RESULT
{
	int result;
	std::string errorinfo;
	std::string gameinternalno;
	std::string xlinternalno;
	std::string oldusername;
	long		digitusername;
	std::string bindemail;
	std::string bindphone;
	std::string bindcard;
	std::string truename;
	std::string nickname;
	std::string idcardno;
	std::string stateresult;
	std::string loginstate;
	int			accountresult;
	int			account;
	int			xldegree;
	int			vipresult;
	bool		isuservip;
	int			uservip;
	std::string registtime;
	int			loginType;
	std::string usrname;
}STU_LOGIN_RESULT;


// 游客表结构
typedef struct _STU_TOURIST
{
	uint64_t seqid;
	std::string touristid;
	std::string mac;
}STU_TOURIST;


// 记录实时登陆信息
typedef struct _STU_LOGIN_REALTIME 
{

	uint64_t seqid;
	std::string sessionid;
	std::string gameinternalno;
	std::string clientip;
	std::string heartbeaturl;
	std::string createtime;
	int status;    // 0  插入  1 update  2 删除
}STU_LOGIN_REALTIME;


#define  LOGIN_RT_STATUS_INSERT   0
#define  LOGIN_RT_STATUS_UPDATE   1
#define  LOGIN_RT_STATUS_DELETE   2

#define  PREFIX_SESSIONID         "01"
#define  PREFIX_GID_TO_SESSIONID  "02"
#define  PREFIX_GAMELIB           "03"
#define  PREFIX_EXIST_USER        "04"
#define  PREFIX_EXIST_GAMEUSER    "05"
#define  PREFIX_SESSION_TICKOUT   "06"   // 标识该sessionid被踢掉了
#define	 PREFIX_SESSION_ONLINE_INFO "07"  // 标识用户在线时长信息  存储在redis中，盒子后台没有使用，心跳服务器有使用

// 标记登陆类型
#define  USER_LOGINSTATE_USERNAME     "0"  // 用户名密码登陆
#define  USER_LOGINSTATE_URLSTATE     "1"  // 登陆态登陆
#define  USER_LOGINSTATE_TOURIST      "2"  // 游客登陆
#define  USER_LOGINSTATE_SESSION      "3"  // sessionid登陆

// 登陆服务器登陆类型标识
#define  CopartnerId_DOUBLE_MD5   "0101"   // 用户名密码登陆
#define  CopartnerId_BLANK_PASS   "0100"   // 登陆态登陆
#define  CopartnerId_XL_CLIENT    "0002"   // 
#define  CopartnerId_TOURIST      "1003"   // 游客登陆
#define  CopartnerId_WEBTOURIST   "1004"

// 标记登陆盒子历史记录
typedef struct _STU_LOGINBOX_HIS
{
	uint64_t    seqid;
	std::string username;
	std::string gameinternalno;
	std::string clientip;
	std::string clientcity;
    int days;
	std::string lasttime;
	std::string logintime;
}STU_LOGINBOX_HIS;


typedef struct _STU_PREVENT_INFO
{
	uint64_t    seqid;
	std::string gameid;
	std::string is_prevent;
	std::string is_adult;
}STU_PREVENT_INFO;


typedef struct _STU_GAMELIB_INFO
{
	std::string gameinternalno;
	std::string gameid;
	std::string serverid;
	std::string sid;

}STU_GAMELIB_INFO;


typedef struct _STU_CITY_IP
{
	uint64_t start_ip;
	uint64_t end_ip;
	std::string str_province;
	std::string city;
}STU_CITY_IP;


// 登陆游戏返回结果
typedef struct _STU_GAME_RESULT
{
	std::string  loginStatus;//:迅雷帐号登陆状态
	std::string  gameLoginStatus;//:游戏合作商返回登陆状态
	std::string  gameLoginURL;//:游戏合作方登陆URL
	std::string  listNum;//:排队人数
	std::string  rtnMsg;//:返回信息
	std::string  costTime;  // 游戏登陆服务器返回的处理时间
}STU_GAME_RESULT;


typedef struct _STU_LOGINGAME_DB_INFO
{
	long seqid;
	std::string gameinternalno;
	std::string gameid;
	std::string serverid;
	std::string logintime;
}STU_LOGINGAME_DB_INFO;


typedef struct _STU_SYSTEM_MESSAGE
{
	uint64_t seqid;
	std::string messageid;
	std::string message;
	int receivetype;
	std::string start;
	std::string end;
	int status;
	std::string updatetime;
}STU_SYSTEM_MESSAGE;


// receivetype：接收类型 0 ：全部用户，1：迅雷用户，2：游客，3：指定帐号
#define  SYS_MSG_RCV_TYPE_ALL				0
#define  SYS_MSG_RCV_TYPE_XLUSER			1
#define  SYS_MSG_RCV_TYPE_TOURIST			2
#define  SYS_MSG_RCV_TYPE_SPECIAL_USERS	    3

// usertype:用户类型 0，迅雷帐号，1，游客帐号（批量添加用户时要按usertype分类，一批帐号的usertype要相同）
#define  SYS_MSG_USERTYPE_XL                0
#define  SYS_MSG_USERTYPE_TOURSIT           1

#define  SYS_MSG_STATUS_INITED              0
#define  SYS_MSG_STATUS_PUBLISHED           1
#define  SYS_MSG_STATUS_DELETED             2


//feedbacktype：反馈类型，0：问题反馈，1：回复反馈,2：系统反馈
#define  FEEDBACK_TYPE_FEEDBACK                  0
#define  FEEDBACK_TYPE_FEEDBACK_REPLY            1
#define  FEEDBACK_TYPE_SYS_FEEDBACK              2


// 映射messageadress表结构
typedef struct _STU_MSG_ADDRESS
{
	uint64_t seqid;
	std::string messageid;
	int	usertype;
	std::string gameinternalno;
	std::string username;

}STU_MSG_ADDRESS;

// 映射systemtouserreply表结构
typedef struct _STU_SYS2USERREPLY
{
	uint64_t seqid;
	std::string messageid;
	std::string message;
	int	usertype;
	std::string gameinternalno;
	std::string username;
	std::string updatetime;
	int status;	
	uint64_t feedbackid;
}STU_SYS2USERREPLY;

#define  SYSTEM_TO_USERREPLY_STATUS_ADDED    0
#define  SYSTEM_TO_USERREPLY_STATUS_RELEASED  1
#define  SYSTEM_TO_USERREPLY_STATUS_DELETED  2

// 映射 messagerecordN 表结构
typedef struct _STU_MESSAGE_RECORD
{
	std::string recordid;
	std::string messageid;
	std::string message;
	int	messagetype;
	int	usertype;
	std::string gameinternalno;
	std::string username;
	std::string updatetime;
	int	status;
}STU_MESSAGE_RECORD;

#define MSG_RECORD_USERTYPE_TOURIST  1
#define MSG_RECORD_USERTYPE_USER     0

// 映射messagefeedback表结构
typedef struct _STU_MESSAGE_FEEDBACK
{
	uint64_t seqid;
	std::string messageid;
	std::string message;
	int	usertype;
	std::string gameinternalno;
	std::string username;
	std::string updatetime;
	int	status;
	std::string recordid;
}STU_MESSAGE_FEEDBACK;

#define FEEDBACK_STATUS_NOT_REPLYED            0
#define FEEDBACK_STATUS_REPLYED                1
#define FEEDBACK_STATUS_PREPAIR_TO_REPLY       2



// 针对系统消息，回复给客户端，需要进行json编码的结构体
typedef struct _STU_SYSTEM_MESSAGE_JSON
{
	std::string messageid;
	std::string message;
	std::string start;
	std::string end;

}STU_SYSTEM_MESSAGE_JSON;




typedef struct _STU_GAMEVIEW_GAMEINFO
{
	std::string gameid;
	std::string gamename;
	std::string svrid;
	std::string svruid;
	std::string svrname;
	std::string role;
	std::string lasttime;
}STU_GAMEVIEW_GAMEINFO;

#define  PREVENT_STATUS_FORBIDDEN   1
#define  PREVENT_STATUS_UNKNOWN     2
#define  PREVENT_STATUS_ALLOW       0

typedef struct _STU_ZMQ_INFO_IDENTITY
{
	std::string identity;
	std::string cmd_type;  // heartbeat  data
	std::string routing_destination; 
	std::string data;
}STU_ZMQ_INFO_IDENTITY;


typedef struct _STU_ZMQ_INFO
{
	std::string cmd_type;  // heartbeat  data
	std::string destination; 
	std::string data;
}STU_ZMQ_INFO;


//#define  ZMQ_CMD_TYPE_REG       "reg"
#define	 ZMQ_CMD_TYPE_HEARTBEAT "heartbeat"
#define  ZMQ_CMD_TYPE_DATA      "data"
#define  ZMQ_CMD_TYPE_CONFIG    "config"


#define  ZMQ_ERROR   zmq_strerror(zmq_errno())

#define  PATH_CTL_CMD_ADDQUEUE       "/xlmq/addqueue"
#define  PATH_CTL_CMD_DELQUEUE       "/xlmq/delqueue"
#define  PATH_CTL_CMD_LISTQUEUE       "/xlmq/listqueue"
#define  PATH_CTL_CMD_STATQUEUE		 "/xlmq/statqueue"

#define  PATH_CTL_CMD_ADDQUEUE_ID      0
#define  PATH_CTL_CMD_DELQUEUE_ID      1
#define  PATH_CTL_CMD_LISTQUEUE_ID     2
#define  PATH_CTL_CMD_STATQUEUE_ID     3

#define RC_OK_ID           "1"
#define RC_ERR_ID          "2"

#define RC_OK_INFO         "success"
#define RC_ERR_INFO         "error"

#endif


