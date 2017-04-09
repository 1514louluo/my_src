
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
#define  PREFIX_SESSION_TICKOUT   "06"   // ��ʶ��sessionid���ߵ���
#define	 PREFIX_SESSION_ONLINE_INFO "07"  // ��ʶ�û�����ʱ����Ϣ  �洢��redis�У����Ӻ�̨û��ʹ�ã�������������ʹ��

#define  PREFIX_RANDOMKEY          "randomkey"

// sesson��Ϣ�ṹ��
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
	std::string comefrom;  // ������Դ
	std::string xlinternalno; // xl�ڲ�id
	std::string startsource;  // ������Դ
}STU_SESSION_INFO;








