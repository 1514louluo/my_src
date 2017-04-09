#include "dispatcher.h"

#include "SDConfigurationSingleton.h"
#include <sys/types.h>
//#include "general_log_client.h"
#include "service_stat.h"
//#include "general_log_service.h"
#include "log_service.h"
IMPL_LOGGER(dispatcher, logger);

#include "cJSON.h"

dispatcher::dispatcher(void)
{



}

dispatcher::~dispatcher(void)
{
}


void dispatcher::init_server_map(void)
{
        //SDUtility::GBKToUTF8()
        std::string param;
        param = "成功";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(SUCCESS, param));
        param = "参数错误";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(PARAM_ERROR, param));
        param = "无效会话";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(INVALID_SESSION, param));
        param = "账号已登录";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(LOGINED ,param));
        param = "小号不存在";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(NOT_EXIST_SMALL, param));
        param = "无效消息";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(INVALID_MESSAGE, param));
        param = "无效身份证信息";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(INVALID_IDCARDNO, param));
        param = "无效AGREE值";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(INVALID_AGREE_VALUE, param));
        param = "托管账户不存在";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(NOT_EXIST_TRUSTEE, param));
        param = "无效注册来源";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(INVALID_REGISTER_SOURCE, param));
        param = "托管账户已存在";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(EXIST_TRUSTEE, param));
        param = "小号已存在";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(EXIST_SMALL, param));
        param = "禁止添加自己的帐号";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(SAME_USER, param));
        param = "小号个数超过限制";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(SMALL_OVER_LIMIT, param));
        param = "对方小号个数超过限制";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(TRUSTEE_SMALL_OVER_LIMIT, param));
        param = "游客不存在";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(NOT_EXIST_TOURISTID, param));
        param = "游客密码错误";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(TOURIST_PWD_ERROR,param));
        param = "游客ID数量受限";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(TOURISTID_LIMIT, param));
        param = "游客ID和游戏ID不匹配";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(TOURISTID_GAMEID_ERROR, param));
        param = "没有用户登录游戏信息";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(NO_USER_LOGININFO,param));
        param = "城市IP受限";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(IP_LIMITED_CITY, param));
        param = "在其他地方登录";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(LOGIN_ORTHER, param));
        param = "无效访问";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(ILLEGAL_ACCESS, param));
        param = "无权限或被禁止";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(NOT_PERMISSION, param));
        param = "缓存异常";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(CACHE_ERROR, param));
        param = "数据库错误";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(DB_ERROR,param));
        param = "系统错误";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(SYSTEM_ERROR, param));
        param = "账号库IO异常";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(ACCOUNT_IO_ERROR, param));
        param = "账号库未知错误";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(ACCOUNT_UNKOWN_ERROR, param));
        param = "账号库参数错误";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(ACCOUNT_PARAM_ERROR, param));
        param = "账号不存在";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(ACCOUNT_USER_NOT_EXIST, param));
        param = "密码错误";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(ACCOUNT_PASS_ERROR, param));
        param = "临时密码错误";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(ACCOUNT_TEMP_PASS_ERROR,param));
        param = "存在于全局黑名单";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(ACCOUNT_EXIST_WHOLE_BLACK_LIST, param));
        param = "存在于黑名单";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(ACCOUNT_EXIST_BLACK_LIST, param));
        param = "未激活";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(ACCOUNT_NOT_EXIST_WHITE_LIST, param));
        param = "用户已存在";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(ACCOUNT_EXIST, param));
        param = "迅雷帐号不存在";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(ACCOUNT_NOT_EXIST, param));
        param = "个人中心服务异常";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(INDIVIDUAL_CENTER_EORROR, param));
        param = "VERIFY_KEY长度错误";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(VERIFY_KEY_LEN_ERROR, param));
        param = "验证码错误";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(VERIFY_CODE_ERROR, param));
        param = "验证码重复验证";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(VERIFY_CODE_REVERIFY, param));
        param = "验证码超时";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(VERIFY_CODE_TIMEOUT, param));
        param = "VERIFY_KEY错误";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(VERIFY_KEY_ERROR, param));
        param = "验证码类型不存在";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(VERIFY_TYPE_ERROR, param));
        param = "验证码类型不匹配";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(VERIFY_TYPE_NOT_EXIST,param));
        param = "验证码服务器错误";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(VERIFY_SERVER_ERROR, param));
        param = "登录服务器IO异常";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(LOGIN_IO_ERROR, param));
        param = "MD5验证错误";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(MD5_VERIFY_ERROR, param));
        param = "登录游戏参数错误";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(LOGIN_GAME_PARAM_ERROR, param));
        param = "由于网络问题或未知问题暂时无法登录游戏，请稍后登录";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(OTHER_SYS_ERROR, param));
        param = "无登录游戏权限";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(LOGIN_GAME_ILLEGAL_ACCESS, param));
        param = "无效LoginURL";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(INVALID_LOGIN_URL, param));
        param = "jump服务错误";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(JUMP_SERVER_ERROR, param));
        param = "jump连接错误";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(JUMP_SERVER_IO, param));
        param = "jumpkey失效";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(JUMP_KEY_FAILED,param));
        param = "jumpkey无效";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(INVALID_JUMP_KEY, param));
        param = "jump服务器内部错误";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(JUMP_SERVER_INNER_ERROR, param));
        param = "帐号不存在";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(JUMP_ACCOUNT_NOT_EXIST, param));
        param = "无效帐号";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(JUMP_INVSLID_ACCOUNT, param));
        param = "用户被锁定";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(USER_LOCKED_OUT, param));
        param = "JUMP会话ID为空";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(JUMP_NULL_SESSIONID,param));
        param = "JUMP服务未知错误";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(JUMP_SERVER_UNKOWN_ERROR, param));
        param = "绑定游客服务未知错误";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(BIND_TOURIST_SERVER_UNKOWN_ERROR, param));
        param = "绑定失败";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(BIND_TOURIST_FAILED, param));
        param = "迅雷账号和游客账号激活过同一款游戏";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(BIND_TOURIST_XLUSER_SAME_GAME, param));
        param = "游客绑定服务异常";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(BIND_TOURIST_SERVER_IO_ERROR, param));
        param = "游戏插件查询服务异常";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(GAME_VIEWER_SERVER_IO_ERROR, param));
        param = "签名信息不正确而被过滤";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(SIGN_FILTER, param));
        param = "游戏插件更新服务异常";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(GAME_PLAY_SERVER_IO_ERROR, param));
        param = "系统消息不存在";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(NOT_EXIST_SYSTEM_MESSAGE, param));
        param = "系统消息已同步外网或被删除，禁止修改";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(NO_UPDATE_SYSTEM_MESSAGE, param));
        param = "没有选择任何消息";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(NO_SELECT_SYSTEM_MESSAGE, param));
        param = "没有任何用户";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(NO_MESSAGE_ADDRESS, param));
        param = "系统回复不存在";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(NOT_EXIST_SYSTEM_REPLY, param));
        param = "系统回复已同步外网或被删除，禁止修改";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(NO_UPDATE_SYSTEM_REPLY, param));
        param = "没有选择任何系统回复";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(NO_SELECT_SYSTEM_REPLY, param));
        param = "反馈信息不存在";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(NOT_EXIST_USER_FEEDBACK, param));
        param = "反馈信息已回复";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(FEEDBACK_REPLYED, param));
        param = "WEBLOGIN无权限访问";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(WEBLOGIN_ILLEGAL_ACCESS, param));
        param = "WEBLOGIN参数错误";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(WEBLOGIN_PARAM_ERROR, param));
        param = "WEBLOGIN服务IO异常";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(WEBLOGIN_IO_ERROR, param));
        param = "WEBLOGIN账号不存在";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(WEBLOGIN_NO_EXIST_USER, param));
        param = "WEBLOGIN账户被冻结";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(WEBLOGIN_USERNAME_FREEZE, param));
        param = "WEBLOGIN密码错误";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(WEBLOGIN_PASSWORD_ERROR, param));
        param = "WEBLOGIN其他错误";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(WEBLOGIN_OTHER_ERROR, param));
        param = "礼包服务IO异常";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(GIFT_SERVER_IO_ERROR, param));
        param = "游客不能领礼包哦，请先绑定一个迅雷帐号！";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(GIFT_NO_FOR_TOURIST, param));
        param = "礼包服务系统错误";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(GIFT_SYSTEM_ERROR, param));
        param = "礼包参数错误";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(GIFT_PARAM_ERROR,param));
        param = "礼包时间戳过期";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(GIFT_TIMESTAMP_OVERTIEM, param));
        param = "礼包签名无效";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(GIFT_INVALID_SIGN, param));
        param = "您不在获奖名单内，无法领取该礼包！";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(GIFT_NOTFOR_GAME,param));
        param = "您来的太晚了，礼包已经发光啦！";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(GIFT_NO_GIFT, param));
        param = "已经领取";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(GIFT_HAS_GOT, param));
        param = "您不在获奖名单内，无法领取该礼包！";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(GIFT_USER_NOT_IN_GIFTLIST,param));

        param = "游戏库游戏已存在";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(GAMELIB_GAME_EXIST, param));

        param = "系统超时";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(INTERNAL_TIMEOUT, param));

        param = "备机房重定向";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(LOGINBOX_REDIRECT, param));


        param = "连接错误mysql!";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(FLUSH_GAMEINFO_ERROR, param));








        // 登陆注册协议集
        m_map_request_path.insert(make_pair(GAMEBOX_SERVER_BOXSERVER_LOGINGAME, GAMEBOX_SERVER_BOXSERVER_LOGINGAME_ID));
        m_map_request_path.insert(make_pair(GAMEBOX_SERVER_BOXSERVER_RAPELOGINGAME, GAMEBOX_SERVER_BOXSERVER_RAPELOGINGAME_ID));
        m_map_request_path.insert(make_pair(GAMEBOX_SERVER_BOXSERVER_SLAVELOGINGAME, GAMEBOX_SERVER_BOXSERVER_SLAVELOGINGAME_ID));
        m_map_request_path.insert(make_pair(GAMEBOX_SERVER_BOXSERVER_FLUSHGAMEINFO, GAMEBOX_SERVER_BOXSERVER_FLUSHGAMEINFO_ID));
        m_map_request_path.insert(make_pair(GAMEBOX_SERVER_BOXSERVER_SPECIALLOGINGAME, GAMEBOX_SERVER_BOXSERVER_SPECIALLOGINGAME_ID));
        m_map_request_path.insert(make_pair(GAMEBOX_SERVER_BOXSERVER_FLUSHGAMEINFOSPE, GAMEBOX_SERVER_BOXSERVER_FLUSHGAMEINFOSPE_ID));

}


void dispatcher::init(void)
{
        int result;
        m_str_success_static = "成功";
        SDUtility::GBKToUTF8(m_str_success_static);
        init_server_map();
        init_map_startsource_promptid();

        // static初始化
        general_log::INIT_GENERAL_LOG();
        libcurlclient::INIT_LIBURLCLIENT();
//	user_service::INIT_USER_SERVICE();
        prevent_service::INIT_PREVENT();
        loginwebgame_service::INIT_LOGINWEBGAME();
        log_service::INIT_LOG_SERVICE();


        const SDConfiguration& config = SDConfigurationSingleton::get_instance()->get_config();
        m_queue_delay_drop_threashold = config.getInt("request_queue_delay_drop_threashold", 1000);
        m_databasepool.init_databasepool();

        master_slave_conf::singleton();
        gameinfo::GetInstance();

        m_http_manager.set_parent(this);
        m_http_manager.init_http_manager();
//	m_thread_poll_man.init_thread_poll();
        m_thread_poll_logingame_man.init_thread_poll();
        m_databasepool.init_databasepool();
        m_memcache_pool.init_memcache_cli_pool();
//	m_loginrealtime.init_login_realtime();
//	m_redis_pool.init_pool();
//	m_ipserver.init_ipserver();
        m_login_track_server.init_login_track_service();

        std::string str_mongodb_serverip = config.getString("mongodb_server_ip","127.0.0.1");
        int mongodb_server_port = config.getInt("mongodb_server_port",0);
        int mongodb_clients_count = config.getInt("mongodb_clients_count",5);
        std::string mongodb_conn_string;
        mongodb_conn_string = str_mongodb_serverip+":"+SDUtility::format(mongodb_server_port);
        for(int i=0; i<mongodb_clients_count; i++)
        {
                mongo_client *pclient = new mongo_client;
                result = pclient->connect(mongodb_conn_string);
                if(result == -1)
                {
                        MY_LOGERROR("failed to connect to mongodb");
                }
                m_mongodb_pool.install_obj(pclient);
        }


        MY_LOGINFO("service init finished");
        pthread_t threadid;
        result = pthread_create(&threadid, NULL, dispatcher::dispatch_http_request_proc, this);
        if(result == -1)
        {
                MY_LOGERROR("failed to create thread dispatcher::dispatch_http_request_proc");
                sleep(1);
                exit(0);
                return;
        }
        result = pthread_create(&threadid, NULL, dispatcher::http_response_proc, this);
        if(result == -1)
        {
                MY_LOGERROR("failed to create thread dispatcher::http_response_proc");
                sleep(1);
                exit(0);
                return;
        }
        result = pthread_create(&threadid, NULL, dispatcher::timer_stat_proc, this);
        if(result == -1)
        {
                MY_LOGERROR("failed to create thread dispatcher::timer_stat_proc");
                sleep(1);
                exit(0);
                return;
        }

        boost::function<void (std::string &)> zmq_pub_trustee_notify_callback =
                boost::bind(&dispatcher::zmq_client_pub_trustee_notify_recv, this, _1);
//	std::string zmq_service_pub_ip=config.getString("zmq_service_pub_ip","0.0.0.0");
//	int zmq_service_pub_port=config.getInt("zmq_service_pub_port", 0);
        //zmq_service_sub_ip=10.10.1.58
        //zmq_service_sub_port=5555
//	m_zmq_service_self_identity=config.getString("zmq_service_self_identity","unkown");
//	m_zmq_router_notify_server_identity=config.getString("zmq_router_notify_server_identity","unkown");
//	result = m_zmq_client_pub_trustee_notify.init_zmq_client(zmq_service_pub_ip, zmq_service_pub_port,
//				m_zmq_service_self_identity, zmq_pub_trustee_notify_callback);
        if(result == -1)
        {
                MY_LOGERROR("failed to init zmq client for message notify");
        }
        std::string compiletime;
        compiletime = compiletime +  __DATE__ + " "+ __TIME__;
        MY_LOGINFO("dispatcher init finished,compiletime:"<<compiletime);


}

void * dispatcher::dispatch_http_request_proc(void *param)
{

        if(param == NULL)
        {
                return (void *)0;
        }
        dispatcher *pthis  = (dispatcher *)param;
        pthis->dispatch_http_request();
        MY_LOGINFO("dispatch_http_request_proc thread terminated");

        return (void *)0;


}

void dispatcher::dispatch_http_request(void)
{



        while(true)
        {
                int result;
                boost::shared_ptr<HttpRequestPacket> prequest;
                result = m_request_queue.dequeue_expand(prequest);
                if(result != 1)
                {
                        continue;
                }



		  general_log log;
                MY_LOGDEBUG("get one request,url="<<prequest->get_request_url());
                int serverid = server_path2id(prequest->m_str_cmdpath);
                if(serverid == -1)
                {
                        MY_LOGERROR("serverid not registered or suported, cmd path="<<prequest->m_str_cmdpath);
                        continue;
                }
                //prequest->LOGDEBUG();
                prequest->m_request_type = serverid;
                boost::function<void(void)> task = NULL;
                switch(serverid)
                {
                        case GAMEBOX_SERVER_BOXSERVER_LOGINGAME_ID:
                                task = boost::bind(&dispatcher::Process_loginGame, this, prequest);
                                break;

                        case GAMEBOX_SERVER_BOXSERVER_RAPELOGINGAME_ID:
                                task = boost::bind(&dispatcher::Process_rapeLoginGame, this, prequest);
                                break;

                        case GAMEBOX_SERVER_BOXSERVER_SPECIALLOGINGAME_ID:
                                task = boost::bind(&dispatcher::Process_specialLoginGame, this, prequest);
                                break;

                        case GAMEBOX_SERVER_BOXSERVER_FLUSHGAMEINFO_ID:
                                task = boost::bind(&dispatcher::Process_flushGameInfo, this, prequest);
                                break;

                        case GAMEBOX_SERVER_BOXSERVER_SLAVELOGINGAME_ID:
                                task = boost::bind(&dispatcher::Process_slaverLoginGame, this, prequest);
                                break;

                        case GAMEBOX_SERVER_BOXSERVER_FLUSHGAMEINFOSPE_ID:
                                task = boost::bind(&dispatcher::Process_flushGameInfoSpe, this, prequest);
                                break;

                        default:
                                MY_LOGERROR("process dealing still not supported, cmdpath="<<prequest->m_str_cmdpath<<" serverid="<<serverid);
                                box_client_response_error(ILLEGAL_ACCESS,prequest, log);

                }
                //boost::function<void(void)> testtask = boost::bind(&dispatcher::test_process, this, prequest);
                if(task != NULL)
                {

                        m_thread_poll_logingame_man.commit_task(task);

                }

        }

}

int dispatcher::server_path2id(std::string &str_path)
{
        std::map<std::string, int>::iterator iter;
        iter = m_map_request_path.find(str_path);
        if(iter == m_map_request_path.end())
        {
                return -1;
        }
        return iter->second;
}


void dispatcher::test_process(boost::shared_ptr<HttpRequestPacket> request)
{

        //pthread_t threaid = pthread_self();
        //MY_LOGINFO("deal test proc");
        //MY_LOGINFO("cmd="<<request->m_str_cmdpath<<" threadid="<<threaid);

        mysql_guard myguard((void *)&m_databasepool);
        int fetch_result = myguard.fetch_mysql_from_pool();
        if(fetch_result != 0)
        {
                return;
        }
        MY_LOGINFO("get one from db pool");
        std::map<std::string, std::string> map_value;
        map_value.insert(make_pair("result11", "1"));
        map_value.insert(make_pair("test", "2"));
        HttpResponseHeaderPacket response;
        response.encode(HTTP_RESPONSE_STATUS_OK, map_value);
        box_client_response(&response, request->m_clientfd, request->m_client_tag);

}

int dispatcher::box_client_response(HttpResponseHeaderPacket *phttp_response , int clientfd,
                                    uint64_t client_unify_tag)
{
        if(phttp_response == NULL)
        {
                return -1;
        }

        boost::shared_ptr< STU_HTTP_RTN > pstu_http_rtn(new STU_HTTP_RTN);
        pstu_http_rtn->clientfd = clientfd;
        pstu_http_rtn->unify_tag = client_unify_tag;
        pstu_http_rtn->str_send = phttp_response->m_str_respond;
        m_queue_response.add_queue(pstu_http_rtn);
        return 0;


}

void * dispatcher::http_response_proc(void *param)
{

        if(param == NULL)
        {
                return (void *)0;
        }

        dispatcher *pthis = (dispatcher *)param;
        while(true)
        {
                int result;
                boost::shared_ptr<STU_HTTP_RTN> pstu_http_rtn;
                result = pthis->m_queue_response.dequeue_expand(pstu_http_rtn);
                if(result != 1)
                {
                        continue;
                }

                pthis->m_http_manager.m_httpserver.send_data(pstu_http_rtn->clientfd, pstu_http_rtn->unify_tag,
                                pstu_http_rtn->str_send);
                pthis->m_http_manager.m_httpserver.close_client(pstu_http_rtn->clientfd, pstu_http_rtn->unify_tag);

        }
}


int dispatcher::get_sessionid_by_gameinternalno(std::string str_gameinternalno, std::string &str_sessionid)
{
        int result = 0;
        memcache_guard memguard(&m_memcache_pool);
        result = memguard.fetch_memcached_client_from_pool();
        if(result == -1)
        {
                return -1;
        }
        memcache_client *pclient = memguard.get_memcache_client();
        if(pclient == NULL)
        {
                return -1;
        }
        std::string gameinternalno_with_prefix;
        gameinternalno_with_prefix = gameinternalno_with_prefix + PREFIX_GID_TO_SESSIONID + str_gameinternalno;
        return pclient->get_value(gameinternalno_with_prefix, str_sessionid);

}

int dispatcher::set_gameinternalno_sessionid_map(std::string &strsessionid, std::string &str_gameinternalno)
{
        if(strsessionid.empty() || str_gameinternalno.empty())
        {
                return -1;
        }

        int result = 0;
        memcache_guard memguard(&m_memcache_pool);
        result = memguard.fetch_memcached_client_from_pool();
        if(result == -1)
        {
                return -1;
        }
        memcache_client *pclient = memguard.get_memcache_client();
        if(pclient == NULL)
        {
                return -1;
        }

        std::string gameinternalno_with_prefix;
        gameinternalno_with_prefix = gameinternalno_with_prefix + PREFIX_GID_TO_SESSIONID + str_gameinternalno;
        return pclient->set_value(gameinternalno_with_prefix, strsessionid);
}

int dispatcher::set_gameinternalno_sessionid_map(std::string &strsessionid, std::string &str_gameinternalno,
                memcache_client *pmem_client)
{
        if(strsessionid.empty() || str_gameinternalno.empty() || pmem_client == NULL)
        {
                return -1;
        }

        std::string gameinternalno_with_prefix;
        gameinternalno_with_prefix = gameinternalno_with_prefix + PREFIX_GID_TO_SESSIONID + str_gameinternalno;
        return pmem_client->set_value(gameinternalno_with_prefix, strsessionid);

}


int dispatcher::remove_sessioninfo(std::string &str_sessionid)
{

        std::string session_with_prefix;
        session_with_prefix = PREFIX_SESSIONID;
        session_with_prefix = session_with_prefix + str_sessionid;
        int result;
        memcache_guard memguard(&m_memcache_pool);
        result = memguard.fetch_memcached_client_from_pool();
        if(result == -1)
        {
                return -1;
        }
        memcache_client *pclient = memguard.get_memcache_client();
        if(pclient == NULL)
        {
                return -1;
        }
        result = pclient->delete_value(session_with_prefix);
        if(result == -1)
        {
                return -1;
        }
        return 0;

}

int dispatcher::set_session_info(std::string &str_sessionid,STU_SESSION_INFO *psessioninfo)
{

        std::string session_with_prefix;
        session_with_prefix = PREFIX_SESSIONID;
        session_with_prefix = session_with_prefix + str_sessionid;
        int fetch_result;
        memcache_guard memguard(&m_memcache_pool);
        fetch_result = memguard.fetch_memcached_client_from_pool();
        if(fetch_result == -1)
        {
                return -1;
        }
        memcache_client *pmemclient = memguard.get_memcache_client();
        std::string str_value;
        /*str_value = str_value + psessioninfo->username + ";";
        str_value = str_value + psessioninfo->password  + ";";
        str_value = str_value + psessioninfo->clientip  + ";";
        str_value = str_value + psessioninfo->gameinternalno  + ";";
        if (psessioninfo->idcardno.empty())
        {
        	str_value = str_value +   + "*;";
        }
        else
        {
        	str_value = str_value + psessioninfo->idcardno  + ";";
        }

        str_value = str_value + psessioninfo->gameid  + ";";
        str_value = str_value + psessioninfo->useloginstate  + ";";
        str_value = str_value + psessioninfo->heartbeaturl  + ";";*/

        session_to_json(str_value, psessioninfo);

        int result;

        uint64_t starttime = SDUtility::get_cur_millseconds();
        result = pmemclient->set_value(session_with_prefix, str_value);
        uint64_t endtime = SDUtility::get_cur_millseconds();
        service_stat *pstat = service_stat::singleton();
        STU_MEMCACHE_STAT stat;
        stat.delay = endtime-starttime;
        stat.rw_type = MEMCACHE_RW_TYPE_READ;
        pstat->add_memcache_stat(stat);

        if(result == -1)
        {
                return -1;
        }
        MY_LOGDEBUG("set sessionid-->sessioninfo finished, key="<<session_with_prefix<<"  value="<<str_value);
        return 0;


}


int dispatcher::get_session_info(std::string &str_sessionid, STU_SESSION_INFO *psessioninfo)
{

        std::string session_with_prefix;
        session_with_prefix = PREFIX_SESSIONID;
        session_with_prefix = session_with_prefix + str_sessionid;
        int fetch_result;
        memcache_guard memguard(&m_memcache_pool);
        fetch_result = memguard.fetch_memcached_client_from_pool();
        if(fetch_result == -1)
        {
                return -1;
        }
        memcache_client *pmemclient = memguard.get_memcache_client();
        std::string str_value;
        uint64_t starttime = SDUtility::get_cur_millseconds();
        fetch_result  = pmemclient->get_value(session_with_prefix,str_value);
        uint64_t endtime = SDUtility::get_cur_millseconds();
        service_stat *pstat = service_stat::singleton();
        STU_MEMCACHE_STAT stat;
        stat.delay = endtime-starttime;
        stat.rw_type = MEMCACHE_RW_TYPE_READ;
        pstat->add_memcache_stat(stat);
        if(fetch_result == -1)
        {
                return -1;
        }

        if(fetch_result == 0)
        {
                return 0;
        }

        //-----------------------针对新旧类型sessionid进行处理---------------------
        std::vector<std::string> vec;
        boost::split(vec, str_value, boost::is_any_of(";"));
        if(vec.size() < 8)
        {
                //json 格式
                //MY_LOGERROR("memcache value error, value="<<str_value);
                //return -1;
                int result = json_to_session(str_value, psessioninfo);
                if(result == -1)
                {
                        return -1;
                }
                return 1;
        }
        else
        {
                psessioninfo->username=vec[0];
                psessioninfo->password=vec[1];
                psessioninfo->clientip=vec[2];
                psessioninfo->gameinternalno=vec[3];
                psessioninfo->idcardno=vec[4];
                psessioninfo->gameid=vec[5];
                psessioninfo->useloginstate=vec[6];
                psessioninfo->heartbeaturl=vec[7];
                if(psessioninfo->idcardno == "*")
                {
                        psessioninfo->idcardno.clear();
                }
        }

        return 1;

}
void dispatcher::Process_flushGameInfo(boost::shared_ptr<HttpRequestPacket> request)
{
	general_log log;
	log.set_log_tag("flushGameInfo");
        int result;
        std::string update_flag = get_value("flag", request->mapParam);
        if(update_flag.empty())
        {
                box_client_response_error(PARAM_ERROR,request, log);
                return;
        }

        std::map<std::string, std::string> map_result;
        map_result.insert(make_pair("result",SDUtility::format(SUCCESS)));
        box_client_response_ok_params(request, map_result, log);

        gameinfo *ginfo = gameinfo::GetInstance();
	 result = GENERAL_OPT_MYSQL_READ(log, "gameinfo_mysql_tname", int, ginfo->get_gameinfo_from_db);
//        result = ginfo->get_gameinfo_from_db();
        if(result)
        {
                MY_LOGERROR("flush Game info error!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        }
        MY_LOGINFO("flush Game info success");

}
void dispatcher::Process_flushGameInfoSpe(boost::shared_ptr<HttpRequestPacket> request)
{
	general_log log;
	log.set_log_tag("flushGameInfoSpe");
        int result;
        std::string update_flag = get_value("flag", request->mapParam);
        if(update_flag.empty())
        {
                box_client_response_error(PARAM_ERROR,request, log);
                return;
        }

        std::map<std::string, std::string> map_result;
        map_result.insert(make_pair("result",SDUtility::format(SUCCESS)));
        box_client_response_ok_params(request, map_result, log);

        gameinfo *ginfo = gameinfo::GetInstance();
 	 result = GENERAL_OPT_MYSQL_READ(log, "gameinfospe_mysql_tname", int, ginfo->get_gameinfo_from_db_4spe);
//        result = ginfo->get_gameinfo_from_db_4spe();
        if(result)
        {
                MY_LOGERROR("flush Game SPE info error!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        }
        MY_LOGINFO("flush Game info SPE success");
}

void dispatcher::Process_specialLoginGame(boost::shared_ptr<HttpRequestPacket> request)
{
	general_log log;
	log.set_log_tag("specialLoginGame");
        int result;
        request->m_start_processtime = SDUtility::get_cur_millseconds();
        result = check_request_queued_timeout(request);
        if(result == -1)
        {
                box_client_response_error(INTERNAL_TIMEOUT, request, log);
                return;
        }

        std::string sessionid = get_value("sessionid",request->mapParam);
        std::string gameid = get_value("gameid",request->mapParam);
        std::string serverid = get_value("serverid",request->mapParam);
        std::string sid = get_value("sid",request->mapParam);
//	std::string loginurl = get_value("loginurl",request->mapParam);

        boost::trim(gameid);
        boost::trim(sid);
        boost::trim(serverid);

        gameinfo *ginfo = gameinfo::GetInstance();
        std::string loginurl;
        if(ginfo->get_gameinfo4special(gameid, serverid, sid, loginurl))
        {
                box_client_response_error(PARAM_ERROR,request, log);
                return;
        }
//	_gameinfo = SDUtility::URLEncode(_gameinfo);
        std::string clientid = get_value("clientid", request->mapParam);
        int clientidint = SDUtility::atouint32(clientid);
        std::string comefrom2 = get_value("comefrom2", request->mapParam);
        std::string comefrom3 = get_value("comefrom3", request->mapParam);
        std::string comefrom = get_value("comefrom",request->mapParam);

        if(sessionid.empty() || gameid.empty() || sid.empty() || loginurl.empty() || serverid.empty())
        {
                box_client_response_error(PARAM_ERROR,request, log);
                return;
        }
        boost::trim(sessionid);
//	boost::trim(gameid);
//	boost::trim(sid);
        boost::trim(loginurl);
//	boost::trim(serverid);

        STU_SESSION_INFO stu_session_info;
        result = get_session_info(sessionid,&stu_session_info);
        if(result == -1)
        {
                box_client_response_error(CACHE_ERROR, request, log);
                return;
        }
        if(result == 0)
        {
                box_client_response_error(INVALID_SESSION, request, log);
                return;
        }
        /*if (stu_session_info.clientip != request->get_client_ip())
        {
        	box_client_response_error(INVALID_SESSION,request);
        	return;
        }*/

        attach_loginfo(&stu_session_info, request.get(), sessionid);

        loginwebgame_service server;
        STU_GAME_RESULT stu_login_result;

        std::string usertype;
        if(stu_session_info.useloginstate == USER_LOGINSTATE_USERNAME)
        {
                usertype = CopartnerId_DOUBLE_MD5;
        }
        else if(stu_session_info.useloginstate == USER_LOGINSTATE_TOURIST)
        {
                usertype = CopartnerId_TOURIST;
        }
        else if(stu_session_info.useloginstate == USER_LOGINSTATE_URLSTATE)
        {
                usertype = CopartnerId_BLANK_PASS;
        }
        else if(stu_session_info.useloginstate == USER_LOGINSTATE_SESSION)
        {
                usertype = CopartnerId_BLANK_PASS;
        }
        else
        {
                MY_LOGERROR("invalid loginstatus, useloginstate="<<stu_session_info.useloginstate);
                box_client_response_error(PARAM_ERROR,request, log);
                return;
        }

        /*
        渠道，comefrom
        安装，comefrom1
        启动， comefrom2
        激活   默认201305172727631722  comefrom3

        登陆游戏来源  默认 201305174721588359

        旧版本
        comefrom    comefrom
        comefrom2   startsource

        */
        //if (comefrom.empty())
        //{
        //	comefrom = stu_session_info.comefrom;
        //}
        // 将startsource转换成promptid
        std::string promptid;
        promptid = get_promptid_by_startsource(stu_session_info.startsource);

        if(promptid.empty())
        {
                // 游戏库的登陆请求
                result = GENERAL_OPT_3RD_SERVICE_READ(log, "visit platform", int, server.loginGame,
                				stu_session_info.username, stu_session_info.password,
                                          stu_session_info.gameinternalno, usertype, gameid, sid, loginurl,
                                          serverid,stu_session_info.clientip, &stu_login_result, clientidint,
                                          stu_session_info.comefrom,    /*游戏库登录的时候传过来的comefrom字段*/
                                          promptid,    /*comefrom1*/
                                          stu_session_info.comefrom,
                                          "",
                                          sessionid+SDUtility::format((uint64_t)time(NULL))
                                         );
 /*               result = server.loginGame(stu_session_info.username, stu_session_info.password,
                                          stu_session_info.gameinternalno, usertype, gameid, sid, loginurl,
                                          serverid,stu_session_info.clientip, &stu_login_result, clientidint,
                                          stu_session_info.comefrom,    //游戏库登录的时候传过来的comefrom字段
                                          promptid,    //comefrom1
                                          stu_session_info.comefrom,
                                          "",
                                          sessionid+SDUtility::format((uint64_t)time(NULL))
                                         );
*/        }
        else
        {

                // 游戏盒子的登陆请求
                if(comefrom2.empty())
                {
                        // 旧版本游戏盒子，1.1.10以前版本，包括1.1.10
                        result = GENERAL_OPT_3RD_SERVICE_READ(log, "visit platform", int, server.loginGame,
                        				stu_session_info.username, stu_session_info.password,
                                                  stu_session_info.gameinternalno, usertype, gameid, sid, loginurl,
                                                  serverid,stu_session_info.clientip, &stu_login_result, clientidint,
                                                  "201305174721588359",    /*旧版本盒子用默认值*/
                                                  stu_session_info.comefrom,
                                                  promptid,                /*comefrom2*/
                                                  "201305172727631722",
                                                  sessionid+SDUtility::format((uint64_t)time(NULL))
                                                 );
  /*                      result = server.loginGame(stu_session_info.username, stu_session_info.password,
                                                  stu_session_info.gameinternalno, usertype, gameid, sid, loginurl,
                                                  serverid,stu_session_info.clientip, &stu_login_result, clientidint,
                                                  "201305174721588359",    //旧版本盒子用默认值
                                                  stu_session_info.comefrom,
                                                  promptid,                //comefrom2
                                                  "201305172727631722",
                                                  sessionid+SDUtility::format((uint64_t)time(NULL))
                                                 );
   */             }
                else
                {
                        // 新版本游戏盒子，1.1.11版本之后，包括1.1.11
                       result = GENERAL_OPT_3RD_SERVICE_READ(log, "visit platform", int, server.loginGame,
                      				 stu_session_info.username, stu_session_info.password,
                                                  stu_session_info.gameinternalno, usertype, gameid, sid, loginurl,
                                                  serverid,stu_session_info.clientip, &stu_login_result, clientidint,
                                                  comefrom,
                                                  stu_session_info.comefrom,    /*comefrom1*/
                                                  comefrom2,
                                                  comefrom3,
                                                  sessionid+SDUtility::format((uint64_t)time(NULL))
                                                 );
  /*                      result = server.loginGame(stu_session_info.username, stu_session_info.password,
                                                  stu_session_info.gameinternalno, usertype, gameid, sid, loginurl,
                                                  serverid,stu_session_info.clientip, &stu_login_result, clientidint,
                                                  comefrom,
                                                  stu_session_info.comefrom,    //comefrom1
                                                  comefrom2,
                                                  comefrom3,
                                                  sessionid+SDUtility::format((uint64_t)time(NULL))
                                                 );
*/                }
        }



        /*if (promptid.empty())
        {
        	result = server.loginGame(stu_session_info.username, stu_session_info.password,
        		stu_session_info.gameinternalno, usertype, gameid, sid, loginurl,
        		serverid,stu_session_info.clientip, &stu_login_result, clientidint,
        		stu_session_info.comefrom, promptid);
        }
        else
        {
        	result = server.loginGame(stu_session_info.username, stu_session_info.password,
        		stu_session_info.gameinternalno, usertype, gameid, sid, loginurl,
        		serverid,stu_session_info.clientip, &stu_login_result, clientidint,
        		promptid,stu_session_info.comefrom);
        }*/


        if(result == -1)
        {
                //box_client_response_error(OTHER_SYS_ERROR, request);
                std::map<std::string, std::string> map_result1;
                map_result1.insert(make_pair("isPrevent","0"));
                map_result1.insert(make_pair("isAdult","0"));
                map_result1.insert(make_pair("loginStatus",stu_login_result.loginStatus));
                map_result1.insert(make_pair("gameLoginStatus",stu_login_result.gameLoginStatus));
                map_result1.insert(make_pair("gameLoginURL",stu_login_result.gameLoginURL));
                map_result1.insert(make_pair("listNum",stu_login_result.listNum));
                map_result1.insert(make_pair("rtnMsg",stu_login_result.rtnMsg));
                map_result1.insert(make_pair("costTime",stu_login_result.costTime));
//		map_result1.insert(make_pair("gameinfo",_gameinfo));

                box_client_response_ok_params(request, map_result1, log);
                return;
        }
        if(result == -2)
        {

                box_client_response_error(LOGINBOX_REDIRECT, request, log);
                return;

        }

        int errcode_result = server.parseLoginGame(SDUtility::atouint32(stu_login_result.loginStatus.c_str(),
                             stu_login_result.loginStatus.size()));
        if(errcode_result != SUCCESS)
        {
                box_client_response_error(errcode_result, request, log);
                return;
        }

        // 保存登陆游戏信息到数据库, 提交到logintrack服务中处理
        STU_LOGINGAME_DB_INFO stu_logingame_db_info;
        stu_logingame_db_info.gameinternalno = stu_session_info.gameinternalno;
        stu_logingame_db_info.gameid = gameid;
        stu_logingame_db_info.serverid = serverid+"_"+sid;
        stu_logingame_db_info.logintime = SDUtility::get_current_time();
        boost::shared_ptr<STU_TRACK_LOGINGAME_INFO> pstu_track_logingame(new STU_TRACK_LOGINGAME_INFO);
        pstu_track_logingame->params = request->m_str_GetRequest+"&uid="+stu_session_info.xlinternalno;
        pstu_track_logingame->stu_logingame_info = stu_logingame_db_info;

        pstu_track_logingame->request = *(request.get());

        pstu_track_logingame->request.mapParam.insert(make_pair("sessionid", sessionid));
        pstu_track_logingame->request.mapParam.insert(make_pair("uid", request->str_log_xlinternalno));
        pstu_track_logingame->request.mapParam.insert(make_pair("gameinternalno", request->str_log_gameinternalno));
        pstu_track_logingame->request.mapParam.insert(make_pair("gameid", gameid));
        pstu_track_logingame->request.mapParam.insert(make_pair("serverid", serverid));
        pstu_track_logingame->request.mapParam.insert(make_pair("sid", sid));
        pstu_track_logingame->request.mapParam.insert(make_pair("result", SDUtility::format(SUCCESS)));
        pstu_track_logingame->request.mapParam.insert(make_pair("loginStatus",stu_login_result.loginStatus));
        pstu_track_logingame->request.mapParam.insert(make_pair("comefrom", comefrom));
        pstu_track_logingame->request.mapParam.insert(make_pair("comefrom2", comefrom2));
        pstu_track_logingame->request.mapParam.insert(make_pair("comefrom3", comefrom3));
        pstu_track_logingame->request.mapParam.insert(make_pair("version", get_value("version",request->mapParam)));

        m_login_track_server.m_queue_logingame.add_queue(pstu_track_logingame);
        int isPrevent = 0;
        int isAdult = 0;
        /*STU_LOGINGAME_DB_INFO stu_logingame_db_info;
        mysql_guard dbguard(&m_databasepool);
        result = dbguard.fetch_mysql_from_pool();
        if (result == -1)
        {
        	box_client_response_error(DB_ERROR, request);
        	return;
        }
        logingame_db_service logingamedbserver(dbguard.get_mysql());
        result = logingamedbserver.add_logingame(&stu_logingame_db_info);
        if (result == -1)
        {
        	box_client_response_error(DB_ERROR, request);
        	return;
        }*/


        // 检查防沉迷
        /*std::vector<STU_PREVENT_INFO> vec_prevent;

        prevent_service preventserver(NULL);
        preventserver.query_prevent(vec_prevent, gameid);


        BOOST_FOREACH(STU_PREVENT_INFO item, vec_prevent)
        {
        	MY_LOGDEBUG("gameid"<<item.gameid<<" isprevent"<<item.is_prevent<<" isadult"<<item.is_adult);
        }
        if (vec_prevent.size() != 0)
        {
        	MY_LOGDEBUG("find gameid in prevent vecter, isaldult="<<vec_prevent[0].is_adult
        		<<" isprevent="<<vec_prevent[0].is_prevent);
        	STU_PREVENT_INFO stu;
        	stu = vec_prevent[0];
        	isAdult = SDUtility::atouint32(stu.is_adult.c_str(), stu.is_adult.size());
        	if (SDUtility::atouint32(stu.is_prevent.c_str(),stu.is_prevent.size()) == PREVENT_STATUS_FORBIDDEN)
        	{
        		MY_LOGDEBUG("game is prevent gameid="<<gameid);
        		if (stu_session_info.idcardno.empty())
        		{
        			MY_LOGDEBUG("find no idcard set PREVENT_STATUS_UNKNOWN");
        			isPrevent = PREVENT_STATUS_UNKNOWN;
        		}
        		else
        		{
        			MY_LOGDEBUG("find idcard set PREVENT_STATUS_ALLOW");
        			isPrevent = PREVENT_STATUS_UNKNOWN;
        		}
        	}
        }
        */

        //MY_LOGDEBUG("gameLoginStatus ="<<stu_login_result.gameLoginStatus);
        if(stu_login_result.gameLoginStatus==WEBGAEM_LOGIN_RTN_GAMELOGINSTATUS_PREVENT)
        {
                if(stu_session_info.idcardno.empty())
                {
                        isPrevent = PREVENT_STATUS_UNKNOWN;
                }
                else
                {
                        isPrevent = PREVENT_STATUS_FORBIDDEN;
                }
        }


        std::map<std::string, std::string> map_result;
        map_result.insert(make_pair("isPrevent",SDUtility::format(isPrevent)));
        map_result.insert(make_pair("isAdult",SDUtility::format(isAdult)));
        map_result.insert(make_pair("loginStatus",stu_login_result.loginStatus));
        map_result.insert(make_pair("gameLoginStatus",stu_login_result.gameLoginStatus));
        map_result.insert(make_pair("gameLoginURL",stu_login_result.gameLoginURL));
        map_result.insert(make_pair("listNum",stu_login_result.listNum));
        map_result.insert(make_pair("rtnMsg",stu_login_result.rtnMsg));

        map_result.insert(make_pair("costTime",stu_login_result.costTime));

//	map_result.insert(make_pair("gameinfo",_gameinfo));

        box_client_response_ok_params(request, map_result, log);

}

void dispatcher::Process_rapeLoginGame(boost::shared_ptr<HttpRequestPacket> request)
{
	general_log log;
	log.set_log_tag("specialLoginGame");
        int result;
        request->m_start_processtime = SDUtility::get_cur_millseconds();
        result = check_request_queued_timeout(request);
        if(result == -1)
        {
                box_client_response_error(INTERNAL_TIMEOUT, request, log);
                return;
        }

        std::string sessionid = get_value("sessionid",request->mapParam);
        std::string gameid = get_value("gameid",request->mapParam);
        boost::trim(gameid);
        gameinfo *ginfo = gameinfo::GetInstance();
        std::string serverid;
        std::string sid;
        std::string loginurl;
        std::string _gameinfo;
        if(ginfo->get_gameinfo(gameid, serverid, sid, loginurl,_gameinfo))
        {
                box_client_response_error(PARAM_ERROR,request, log);
                return;
        }
        _gameinfo = SDUtility::URLEncode(_gameinfo);
        std::string clientid = get_value("clientid", request->mapParam);
        int clientidint = SDUtility::atouint32(clientid);
        std::string comefrom2 = get_value("comefrom2", request->mapParam);
        std::string comefrom3 = get_value("comefrom3", request->mapParam);
        std::string comefrom = get_value("comefrom",request->mapParam);

        if(sessionid.empty() || gameid.empty() || sid.empty() || loginurl.empty() || serverid.empty())
        {
                box_client_response_error(PARAM_ERROR,request, log);
                return;
        }
        boost::trim(sessionid);
//	boost::trim(gameid);
        boost::trim(sid);
        boost::trim(loginurl);
        boost::trim(serverid);


        STU_SESSION_INFO stu_session_info;
        result = get_session_info(sessionid,&stu_session_info);
        if(result == -1)
        {
                box_client_response_error(CACHE_ERROR, request, log);
                return;
        }
        if(result == 0)
        {
                box_client_response_error(INVALID_SESSION, request, log);
                return;
        }
        /*if (stu_session_info.clientip != request->get_client_ip())
        {
        	box_client_response_error(INVALID_SESSION,request);
        	return;
        }*/

        attach_loginfo(&stu_session_info, request.get(), sessionid);

        loginwebgame_service server;
        STU_GAME_RESULT stu_login_result;

        std::string usertype;
        if(stu_session_info.useloginstate == USER_LOGINSTATE_USERNAME)
        {
                usertype = CopartnerId_DOUBLE_MD5;
        }
        else if(stu_session_info.useloginstate == USER_LOGINSTATE_TOURIST)
        {
                usertype = CopartnerId_TOURIST;
        }
        else if(stu_session_info.useloginstate == USER_LOGINSTATE_URLSTATE)
        {
                usertype = CopartnerId_BLANK_PASS;
        }
        else if(stu_session_info.useloginstate == USER_LOGINSTATE_SESSION)
        {
                usertype = CopartnerId_BLANK_PASS;
        }
        else
        {
                MY_LOGERROR("invalid loginstatus, useloginstate="<<stu_session_info.useloginstate);
                box_client_response_error(PARAM_ERROR,request, log);
                return;
        }

        /*
        渠道，comefrom
        安装，comefrom1
        启动， comefrom2
        激活   默认201305172727631722  comefrom3

        登陆游戏来源  默认 201305174721588359

        旧版本
        comefrom    comefrom
        comefrom2   startsource

        */
        //if (comefrom.empty())
        //{
        //	comefrom = stu_session_info.comefrom;
        //}
        // 将startsource转换成promptid
        std::string promptid;
        promptid = get_promptid_by_startsource(stu_session_info.startsource);

        if(promptid.empty())
        {
                // 游戏库的登陆请求
                result = GENERAL_OPT_3RD_SERVICE_READ(log, "visit platform", int, server.loginGame,
                				stu_session_info.username, stu_session_info.password,
                                          stu_session_info.gameinternalno, usertype, gameid, sid, loginurl,
                                          serverid,stu_session_info.clientip, &stu_login_result, clientidint,
                                          stu_session_info.comefrom,    /*游戏库登录的时候传过来的comefrom字段*/
                                          promptid,    /*comefrom1*/
                                          stu_session_info.comefrom,
                                          "",
                                          sessionid+SDUtility::format((uint64_t)time(NULL))
                                         );
 /*               result = server.loginGame(stu_session_info.username, stu_session_info.password,
                                          stu_session_info.gameinternalno, usertype, gameid, sid, loginurl,
                                          serverid,stu_session_info.clientip, &stu_login_result, clientidint,
                                          stu_session_info.comefrom,    //游戏库登录的时候传过来的comefrom字段
                                          promptid,    //comefrom1
                                          stu_session_info.comefrom,
                                          "",
                                          sessionid+SDUtility::format((uint64_t)time(NULL))
                                         );
  */      }
        else
        {

                // 游戏盒子的登陆请求
                if(comefrom2.empty())
                {
                        // 旧版本游戏盒子，1.1.10以前版本，包括1.1.10
                       result = GENERAL_OPT_3RD_SERVICE_READ(log, "visit platform", int, server.loginGame,
                       				stu_session_info.username, stu_session_info.password,
                                                  stu_session_info.gameinternalno, usertype, gameid, sid, loginurl,
                                                  serverid,stu_session_info.clientip, &stu_login_result, clientidint,
                                                  "201305174721588359",    /*旧版本盒子用默认值*/
                                                  stu_session_info.comefrom,
                                                  promptid,                /*comefrom2*/
                                                  "201305172727631722",
                                                  sessionid+SDUtility::format((uint64_t)time(NULL))
                                                 );
      /*                  result = server.loginGame(stu_session_info.username, stu_session_info.password,
                                                  stu_session_info.gameinternalno, usertype, gameid, sid, loginurl,
                                                  serverid,stu_session_info.clientip, &stu_login_result, clientidint,
                                                  "201305174721588359",    //旧版本盒子用默认值
                                                  stu_session_info.comefrom,
                                                  promptid,                //comefrom2
                                                  "201305172727631722",
                                                  sessionid+SDUtility::format((uint64_t)time(NULL))
                                                 );
 */               }
                else
                {
                        // 新版本游戏盒子，1.1.11版本之后，包括1.1.11
                       result = GENERAL_OPT_3RD_SERVICE_READ(log, "visit platform", int, server.loginGame,
                       				stu_session_info.username, stu_session_info.password,
                                                  stu_session_info.gameinternalno, usertype, gameid, sid, loginurl,
                                                  serverid,stu_session_info.clientip, &stu_login_result, clientidint,
                                                  comefrom,
                                                  stu_session_info.comefrom,    /*comefrom1*/
                                                  comefrom2,
                                                  comefrom3,
                                                  sessionid+SDUtility::format((uint64_t)time(NULL))
                                                 );
 /*                       result = server.loginGame(stu_session_info.username, stu_session_info.password,
                                                  stu_session_info.gameinternalno, usertype, gameid, sid, loginurl,
                                                  serverid,stu_session_info.clientip, &stu_login_result, clientidint,
                                                  comefrom,
                                                  stu_session_info.comefrom,    //comefrom1
                                                  comefrom2,
                                                  comefrom3,
                                                  sessionid+SDUtility::format((uint64_t)time(NULL))
                                                 );
*/                }
        }



        /*if (promptid.empty())
        {
        	result = server.loginGame(stu_session_info.username, stu_session_info.password,
        		stu_session_info.gameinternalno, usertype, gameid, sid, loginurl,
        		serverid,stu_session_info.clientip, &stu_login_result, clientidint,
        		stu_session_info.comefrom, promptid);
        }
        else
        {
        	result = server.loginGame(stu_session_info.username, stu_session_info.password,
        		stu_session_info.gameinternalno, usertype, gameid, sid, loginurl,
        		serverid,stu_session_info.clientip, &stu_login_result, clientidint,
        		promptid,stu_session_info.comefrom);
        }*/


        if(result == -1)
        {
                //box_client_response_error(OTHER_SYS_ERROR, request);
                std::map<std::string, std::string> map_result1;
                map_result1.insert(make_pair("isPrevent","0"));
                map_result1.insert(make_pair("isAdult","0"));
                map_result1.insert(make_pair("loginStatus",stu_login_result.loginStatus));
                map_result1.insert(make_pair("gameLoginStatus",stu_login_result.gameLoginStatus));
                map_result1.insert(make_pair("gameLoginURL",stu_login_result.gameLoginURL));
                map_result1.insert(make_pair("listNum",stu_login_result.listNum));
                map_result1.insert(make_pair("rtnMsg",stu_login_result.rtnMsg));

                map_result1.insert(make_pair("costTime",stu_login_result.costTime));
                map_result1.insert(make_pair("gameinfo",_gameinfo));

                box_client_response_ok_params(request, map_result1, log);
                return;
        }
        if(result == -2)
        {

                box_client_response_error(LOGINBOX_REDIRECT, request, log);
                return;

        }

        int errcode_result = server.parseLoginGame(SDUtility::atouint32(stu_login_result.loginStatus.c_str(),
                             stu_login_result.loginStatus.size()));
        if(errcode_result != SUCCESS)
        {
                box_client_response_error(errcode_result, request, log);
                return;
        }

        // 保存登陆游戏信息到数据库, 提交到logintrack服务中处理
        STU_LOGINGAME_DB_INFO stu_logingame_db_info;
        stu_logingame_db_info.gameinternalno = stu_session_info.gameinternalno;
        stu_logingame_db_info.gameid = gameid;
        stu_logingame_db_info.serverid = serverid+"_"+sid;
        stu_logingame_db_info.logintime = SDUtility::get_current_time();
        boost::shared_ptr<STU_TRACK_LOGINGAME_INFO> pstu_track_logingame(new STU_TRACK_LOGINGAME_INFO);
        pstu_track_logingame->params = request->m_str_GetRequest+"&uid="+stu_session_info.xlinternalno;
        pstu_track_logingame->stu_logingame_info = stu_logingame_db_info;

        pstu_track_logingame->request = *(request.get());

        pstu_track_logingame->request.mapParam.insert(make_pair("sessionid", sessionid));
        pstu_track_logingame->request.mapParam.insert(make_pair("uid", request->str_log_xlinternalno));
        pstu_track_logingame->request.mapParam.insert(make_pair("gameinternalno", request->str_log_gameinternalno));
        pstu_track_logingame->request.mapParam.insert(make_pair("gameid", gameid));
        pstu_track_logingame->request.mapParam.insert(make_pair("serverid", serverid));
        pstu_track_logingame->request.mapParam.insert(make_pair("sid", sid));
        pstu_track_logingame->request.mapParam.insert(make_pair("result", SDUtility::format(SUCCESS)));
        pstu_track_logingame->request.mapParam.insert(make_pair("loginStatus",stu_login_result.loginStatus));
        pstu_track_logingame->request.mapParam.insert(make_pair("comefrom", comefrom));
        pstu_track_logingame->request.mapParam.insert(make_pair("comefrom2", comefrom2));
        pstu_track_logingame->request.mapParam.insert(make_pair("comefrom3", comefrom3));
        pstu_track_logingame->request.mapParam.insert(make_pair("version", get_value("version",request->mapParam)));

        m_login_track_server.m_queue_logingame.add_queue(pstu_track_logingame);
        int isPrevent = 0;
        int isAdult = 0;
        /*STU_LOGINGAME_DB_INFO stu_logingame_db_info;
        mysql_guard dbguard(&m_databasepool);
        result = dbguard.fetch_mysql_from_pool();
        if (result == -1)
        {
        	box_client_response_error(DB_ERROR, request);
        	return;
        }
        logingame_db_service logingamedbserver(dbguard.get_mysql());
        result = logingamedbserver.add_logingame(&stu_logingame_db_info);
        if (result == -1)
        {
        	box_client_response_error(DB_ERROR, request);
        	return;
        }*/


        // 检查防沉迷
        /*std::vector<STU_PREVENT_INFO> vec_prevent;

        prevent_service preventserver(NULL);
        preventserver.query_prevent(vec_prevent, gameid);


        BOOST_FOREACH(STU_PREVENT_INFO item, vec_prevent)
        {
        	MY_LOGDEBUG("gameid"<<item.gameid<<" isprevent"<<item.is_prevent<<" isadult"<<item.is_adult);
        }
        if (vec_prevent.size() != 0)
        {
        	MY_LOGDEBUG("find gameid in prevent vecter, isaldult="<<vec_prevent[0].is_adult
        		<<" isprevent="<<vec_prevent[0].is_prevent);
        	STU_PREVENT_INFO stu;
        	stu = vec_prevent[0];
        	isAdult = SDUtility::atouint32(stu.is_adult.c_str(), stu.is_adult.size());
        	if (SDUtility::atouint32(stu.is_prevent.c_str(),stu.is_prevent.size()) == PREVENT_STATUS_FORBIDDEN)
        	{
        		MY_LOGDEBUG("game is prevent gameid="<<gameid);
        		if (stu_session_info.idcardno.empty())
        		{
        			MY_LOGDEBUG("find no idcard set PREVENT_STATUS_UNKNOWN");
        			isPrevent = PREVENT_STATUS_UNKNOWN;
        		}
        		else
        		{
        			MY_LOGDEBUG("find idcard set PREVENT_STATUS_ALLOW");
        			isPrevent = PREVENT_STATUS_UNKNOWN;
        		}
        	}
        }
        */

        //MY_LOGDEBUG("gameLoginStatus ="<<stu_login_result.gameLoginStatus);
        if(stu_login_result.gameLoginStatus==WEBGAEM_LOGIN_RTN_GAMELOGINSTATUS_PREVENT)
        {
                if(stu_session_info.idcardno.empty())
                {
                        isPrevent = PREVENT_STATUS_UNKNOWN;
                }
                else
                {
                        isPrevent = PREVENT_STATUS_FORBIDDEN;
                }
        }


        std::map<std::string, std::string> map_result;
        map_result.insert(make_pair("isPrevent",SDUtility::format(isPrevent)));
        map_result.insert(make_pair("isAdult",SDUtility::format(isAdult)));
        map_result.insert(make_pair("loginStatus",stu_login_result.loginStatus));
        map_result.insert(make_pair("gameLoginStatus",stu_login_result.gameLoginStatus));
        map_result.insert(make_pair("gameLoginURL",stu_login_result.gameLoginURL));
        map_result.insert(make_pair("listNum",stu_login_result.listNum));
        map_result.insert(make_pair("rtnMsg",stu_login_result.rtnMsg));

        map_result.insert(make_pair("costTime",stu_login_result.costTime));
        map_result.insert(make_pair("gameinfo",_gameinfo));

        box_client_response_ok_params(request, map_result, log);

}
void dispatcher::Process_loginGame(boost::shared_ptr<HttpRequestPacket> request)
{
	general_log log;
	log.set_log_tag("specialLoginGame");
        int result;
        request->m_start_processtime = SDUtility::get_cur_millseconds();
        request->m_unixtime_tag_map.insert(make_pair("2_start_process", SDUtility::format(request->m_start_processtime)));
        result = check_request_queued_timeout(request);
        if(result == -1)
        {
                box_client_response_error(INTERNAL_TIMEOUT, request, log);
                return;
        }

        std::string sessionid = get_value("sessionid",request->mapParam);
        std::string gameid = get_value("gameid",request->mapParam);
        std::string sid = get_value("sid",request->mapParam);
        std::string loginurl = get_value("loginurl",request->mapParam);
        std::string serverid = get_value("serverid",request->mapParam);
        std::string clientid = get_value("clientid", request->mapParam);
        int clientidint = SDUtility::atouint32(clientid);
        std::string comefrom2 = get_value("comefrom2", request->mapParam);
        std::string comefrom3 = get_value("comefrom3", request->mapParam);
        std::string comefrom = get_value("comefrom",request->mapParam);
        std::string tokenId = get_value("tokenId", request->mapParam);

        std::string comefrom1 = get_value("comefrom1",request->mapParam);
        std::string bizid = get_value("bizid", request->mapParam);
        std::string clientType = get_value("clientType", request->mapParam);

        if(sessionid.empty() || gameid.empty() || sid.empty() || loginurl.empty() || serverid.empty())
        {
                box_client_response_error(PARAM_ERROR,request, log);
                return;
        }
        boost::trim(sessionid);
        boost::trim(gameid);
        boost::trim(sid);
        boost::trim(loginurl);
        boost::trim(serverid);

        STU_SESSION_INFO stu_session_info;
        result = get_session_info(sessionid,&stu_session_info);
        if(result == -1)
        {
                box_client_response_error(CACHE_ERROR, request, log);
                return;
        }
        if(result == 0)
        {
                box_client_response_error(INVALID_SESSION, request, log);
                return;
        }
        /*if (stu_session_info.clientip != request->get_client_ip())
        {
        	box_client_response_error(INVALID_SESSION,request);
        	return;
        }*/

        attach_loginfo(&stu_session_info, request.get(), sessionid);

        loginwebgame_service server;
        STU_GAME_RESULT stu_login_result;

        std::string usertype;
        if(stu_session_info.useloginstate == USER_LOGINSTATE_USERNAME)
        {
                usertype = CopartnerId_DOUBLE_MD5;
        }
        else if(stu_session_info.useloginstate == USER_LOGINSTATE_TOURIST)
        {
                usertype = CopartnerId_TOURIST;
        }
        else if(stu_session_info.useloginstate == USER_LOGINSTATE_URLSTATE)
        {
                usertype = CopartnerId_BLANK_PASS;
        }
        else if(stu_session_info.useloginstate == USER_LOGINSTATE_SESSION)
        {
                usertype = CopartnerId_BLANK_PASS;
        }
        else
        {
                MY_LOGERROR("invalid loginstatus, useloginstate="<<stu_session_info.useloginstate);
                box_client_response_error(PARAM_ERROR,request, log);
                return;
        }

        if(!bizid.empty() && "1" == bizid)
        {
        	  result = GENERAL_OPT_3RD_SERVICE_READ(log, "visit platform", int, server.loginGame,
			  			stu_session_info.username, stu_session_info.password,
                                          stu_session_info.gameinternalno, usertype, gameid, sid, loginurl,
                                          serverid,stu_session_info.clientip, &stu_login_result, clientidint,
                                          comefrom,
                                          comefrom1,    /*comefrom1*/
                                          comefrom2,
                                          comefrom3,
                                          tokenId);
   /*             result = server.loginGame(stu_session_info.username, stu_session_info.password,
                                          stu_session_info.gameinternalno, usertype, gameid, sid, loginurl,
                                          serverid,stu_session_info.clientip, &stu_login_result, clientidint,
                                          comefrom,
                                          comefrom1,    //comefrom1
                                          comefrom2,
                                          comefrom3,
                                          tokenId);
  */      }
        else
        {
                /*
                渠道，comefrom
                安装，comefrom1
                启动， comefrom2
                激活   默认201305172727631722  comefrom3

                登陆游戏来源  默认 201305174721588359

                旧版本
                comefrom    comefrom
                comefrom2   startsource

                */
                //if (comefrom.empty())
                //{
                //	comefrom = stu_session_info.comefrom;
                //}
                // 将startsource转换成promptid
                std::string promptid;
                promptid = get_promptid_by_startsource(stu_session_info.startsource);
                request->m_unixtime_tag_map.insert(make_pair("3_logingame_request", SDUtility::format(SDUtility::get_cur_millseconds())));
                if(promptid.empty())
                {
                        // 游戏库的登陆请求
                        result = server.loginGame(stu_session_info.username, stu_session_info.password,
                                                  stu_session_info.gameinternalno, usertype, gameid, sid, loginurl,
                                                  serverid,stu_session_info.clientip, &stu_login_result, clientidint,
                                                  stu_session_info.comefrom,    /*游戏库登录的时候传过来的comefrom字段*/
                                                  promptid,    /*comefrom1*/
                                                  stu_session_info.comefrom,
                                                  "",
                                                  tokenId);
                }
                else
                {
                        // 游戏盒子的登陆请求
                        if(comefrom2.empty())
                        {
                                // 旧版本游戏盒子，1.1.10以前版本，包括1.1.10
                                result = GENERAL_OPT_3RD_SERVICE_READ(log, "visit platform", int, server.loginGame,
                                				stu_session_info.username, stu_session_info.password,
                                                          stu_session_info.gameinternalno, usertype, gameid, sid, loginurl,
                                                          serverid,stu_session_info.clientip, &stu_login_result, clientidint,
                                                          "201305174721588359",    /*旧版本盒子用默认值*/
                                                          stu_session_info.comefrom,
                                                          promptid,                /*comefrom2*/
                                                          "201305172727631722",
                                                          tokenId);
    /*                            result = server.loginGame(stu_session_info.username, stu_session_info.password,
                                                          stu_session_info.gameinternalno, usertype, gameid, sid, loginurl,
                                                          serverid,stu_session_info.clientip, &stu_login_result, clientidint,
                                                          "201305174721588359",    //旧版本盒子用默认值
                                                          stu_session_info.comefrom,
                                                          promptid,                //comefrom2
                                                          "201305172727631722",
                                                          tokenId);
    */                    }
                        else
                        {
                                // 新版本游戏盒子，1.1.11版本之后，包括1.1.11
                                 result = GENERAL_OPT_3RD_SERVICE_READ(log, "visit platform", int, server.loginGame,
                                 				stu_session_info.username, stu_session_info.password,
                                                          stu_session_info.gameinternalno, usertype, gameid, sid, loginurl,
                                                          serverid,stu_session_info.clientip, &stu_login_result, clientidint,
                                                          comefrom,
                                                          stu_session_info.comefrom,    /*comefrom1*/
                                                          comefrom2,
                                                          comefrom3,
                                                          tokenId);	
     /*                           result = server.loginGame(stu_session_info.username, stu_session_info.password,
                                                          stu_session_info.gameinternalno, usertype, gameid, sid, loginurl,
                                                          serverid,stu_session_info.clientip, &stu_login_result, clientidint,
                                                          comefrom,
                                                          stu_session_info.comefrom,    //comefrom1
                                                          comefrom2,
                                                          comefrom3,
                                                          tokenId);
      */                  }
                }

                request->m_unixtime_tag_map.insert(make_pair("4_logingame_reply", SDUtility::format(SDUtility::get_cur_millseconds())));
                request->m_costtime = stu_login_result.costTime;

                /*if (promptid.empty())
                {
                	result = server.loginGame(stu_session_info.username, stu_session_info.password,
                		stu_session_info.gameinternalno, usertype, gameid, sid, loginurl,
                		serverid,stu_session_info.clientip, &stu_login_result, clientidint,
                		stu_session_info.comefrom, promptid);
                }
                else
                {
                	result = server.loginGame(stu_session_info.username, stu_session_info.password,
                		stu_session_info.gameinternalno, usertype, gameid, sid, loginurl,
                		serverid,stu_session_info.clientip, &stu_login_result, clientidint,
                		promptid,stu_session_info.comefrom);
                }*/

        }
        if(result == -1)
        {
                //box_client_response_error(OTHER_SYS_ERROR, request);
                std::map<std::string, std::string> map_result1;
                map_result1.insert(make_pair("isPrevent","0"));
                map_result1.insert(make_pair("isAdult","0"));
                map_result1.insert(make_pair("loginStatus",stu_login_result.loginStatus));
                map_result1.insert(make_pair("gameLoginStatus",stu_login_result.gameLoginStatus));
                map_result1.insert(make_pair("gameLoginURL",stu_login_result.gameLoginURL));
                map_result1.insert(make_pair("listNum",stu_login_result.listNum));
                map_result1.insert(make_pair("rtnMsg",stu_login_result.rtnMsg));
                map_result1.insert(make_pair("costTime",stu_login_result.costTime));

                box_client_response_ok_params(request, map_result1, log);
                return;
        }
        if(result == -2)
        {
                box_client_response_error(LOGINBOX_REDIRECT, request, log);
                return;

        }
        int errcode_result = server.parseLoginGame(SDUtility::atouint32(stu_login_result.loginStatus.c_str(),
                             stu_login_result.loginStatus.size()));
        if(errcode_result != SUCCESS)
        {
                box_client_response_error(errcode_result, request, log);
                return;
        }

        // 保存登陆游戏信息到数据库, 提交到logintrack服务中处理
        STU_LOGINGAME_DB_INFO stu_logingame_db_info;
        stu_logingame_db_info.gameinternalno = stu_session_info.gameinternalno;
        stu_logingame_db_info.gameid = gameid;
        stu_logingame_db_info.serverid = serverid+"_"+sid;
        stu_logingame_db_info.logintime = SDUtility::get_current_time();
        boost::shared_ptr<STU_TRACK_LOGINGAME_INFO> pstu_track_logingame(new STU_TRACK_LOGINGAME_INFO);
        pstu_track_logingame->params = request->m_str_GetRequest+"&uid="+stu_session_info.xlinternalno;
        pstu_track_logingame->stu_logingame_info = stu_logingame_db_info;

        pstu_track_logingame->request = *(request.get());

        pstu_track_logingame->request.mapParam.insert(make_pair("sessionid", sessionid));
        pstu_track_logingame->request.mapParam.insert(make_pair("uid", request->str_log_xlinternalno));
        pstu_track_logingame->request.mapParam.insert(make_pair("gameinternalno", request->str_log_gameinternalno));
        pstu_track_logingame->request.mapParam.insert(make_pair("gameid", gameid));
        pstu_track_logingame->request.mapParam.insert(make_pair("serverid", serverid));
        pstu_track_logingame->request.mapParam.insert(make_pair("sid", sid));
        pstu_track_logingame->request.mapParam.insert(make_pair("result", SDUtility::format(SUCCESS)));
        pstu_track_logingame->request.mapParam.insert(make_pair("loginStatus",stu_login_result.loginStatus));
        pstu_track_logingame->request.mapParam.insert(make_pair("comefrom", comefrom));
        pstu_track_logingame->request.mapParam.insert(make_pair("comefrom2", comefrom2));
        pstu_track_logingame->request.mapParam.insert(make_pair("comefrom3", comefrom3));
        pstu_track_logingame->request.mapParam.insert(make_pair("version", get_value("version",request->mapParam)));
        pstu_track_logingame->request.mapParam.insert(make_pair("clientType", clientType));

        m_login_track_server.m_queue_logingame.add_queue(pstu_track_logingame);
        int isPrevent = 0;
        int isAdult = 0;
        /*STU_LOGINGAME_DB_INFO stu_logingame_db_info;
        mysql_guard dbguard(&m_databasepool);
        result = dbguard.fetch_mysql_from_pool();
        if (result == -1)
        {
        	box_client_response_error(DB_ERROR, request);
        	return;
        }
        logingame_db_service logingamedbserver(dbguard.get_mysql());
        result = logingamedbserver.add_logingame(&stu_logingame_db_info);
        if (result == -1)
        {
        	box_client_response_error(DB_ERROR, request);
        	return;
        }*/


        // 检查防沉迷
        /*std::vector<STU_PREVENT_INFO> vec_prevent;

        prevent_service preventserver(NULL);
        preventserver.query_prevent(vec_prevent, gameid);


        BOOST_FOREACH(STU_PREVENT_INFO item, vec_prevent)
        {
        	MY_LOGDEBUG("gameid"<<item.gameid<<" isprevent"<<item.is_prevent<<" isadult"<<item.is_adult);
        }
        if (vec_prevent.size() != 0)
        {
        	MY_LOGDEBUG("find gameid in prevent vecter, isaldult="<<vec_prevent[0].is_adult
        		<<" isprevent="<<vec_prevent[0].is_prevent);
        	STU_PREVENT_INFO stu;
        	stu = vec_prevent[0];
        	isAdult = SDUtility::atouint32(stu.is_adult.c_str(), stu.is_adult.size());
        	if (SDUtility::atouint32(stu.is_prevent.c_str(),stu.is_prevent.size()) == PREVENT_STATUS_FORBIDDEN)
        	{
        		MY_LOGDEBUG("game is prevent gameid="<<gameid);
        		if (stu_session_info.idcardno.empty())
        		{
        			MY_LOGDEBUG("find no idcard set PREVENT_STATUS_UNKNOWN");
        			isPrevent = PREVENT_STATUS_UNKNOWN;
        		}
        		else
        		{
        			MY_LOGDEBUG("find idcard set PREVENT_STATUS_ALLOW");
        			isPrevent = PREVENT_STATUS_UNKNOWN;
        		}
        	}
        }
        */

        //MY_LOGDEBUG("gameLoginStatus ="<<stu_login_result.gameLoginStatus);
        if(stu_login_result.gameLoginStatus==WEBGAEM_LOGIN_RTN_GAMELOGINSTATUS_PREVENT)
        {
                if(stu_session_info.idcardno.empty())
                {
                        isPrevent = PREVENT_STATUS_UNKNOWN;
                }
                else
                {
                        isPrevent = PREVENT_STATUS_FORBIDDEN;
                }
        }


        std::map<std::string, std::string> map_result;
        map_result.insert(make_pair("isPrevent",SDUtility::format(isPrevent)));
        map_result.insert(make_pair("isAdult",SDUtility::format(isAdult)));
        map_result.insert(make_pair("loginStatus",stu_login_result.loginStatus));
        map_result.insert(make_pair("gameLoginStatus",stu_login_result.gameLoginStatus));
        map_result.insert(make_pair("gameLoginURL",stu_login_result.gameLoginURL));
        map_result.insert(make_pair("listNum",stu_login_result.listNum));
        map_result.insert(make_pair("rtnMsg",stu_login_result.rtnMsg));
        map_result.insert(make_pair("costTime",stu_login_result.costTime));

        box_client_response_ok_params(request, map_result, log);

}

void dispatcher::message001totrustee(STU_MESSAGE_0001 *pmsg, STU_TRUSTEE_INFO *ptrustee)
{


        ptrustee->seqid = pmsg->seqid;;
        ptrustee->gameid = pmsg->gameid;
        ptrustee->mastergid = pmsg->mastergid;
        ptrustee->mastername = pmsg->mastername;
        ptrustee->serverid = pmsg->serverid;
        ptrustee->slaveralias = pmsg->slaveralias;
        ptrustee->slavergid = pmsg->slavergid;
        ptrustee->slavername = pmsg->slavername;
        ptrustee->idcardno = pmsg->idcardno;
        ptrustee->masteralias = pmsg->masteralias;
        ptrustee->status = 0;        // 0,为尚未通过申请状态， 1为通过申请状态，trustee表中读取的数据为通过申请状态
        ptrustee->smallsource = 1;  // 1， 通过托管申请方式  0 通过添加小号申请

}




void dispatcher::trustee2json(std::string &str_json, std::vector<STU_TRUSTEE_INFO> &vec_trustee)
{
        str_json = "{\"0\":";
        std::string str_size;
        int i=1;
        str_size = SDUtility::format((uint32_t)vec_trustee.size());
        str_json = str_json + str_size+",";

        BOOST_FOREACH(STU_TRUSTEE_INFO stu, vec_trustee)
        {

                std::string str_count;
                str_count = SDUtility::format(i);
                str_json = str_json + "\""+str_count + "\":";
                i++;
                str_json = str_json+"{";
                char charsequence[40]= {0};
                sprintf(charsequence, "%ld", stu.seqid);
                std::string str_sequence;
                str_sequence = charsequence;

                while(stu.masteralias.find('\\') != std::string::npos)
                {
                        //MY_LOGDEBUG("stu.masteralias="<<stu.masteralias);
                        boost::erase_first(stu.masteralias,"\\");
                }
                while(stu.masteralias.find('"') != std::string::npos)
                {
                        //MY_LOGDEBUG("stu.masteralias="<<stu.masteralias);
                        //stu.masteralias.erase(  stu.masteralias.find('\\') );
                        boost::erase_first(stu.masteralias,"\"");
                }
                while(stu.masteralias.find('&') != std::string::npos)
                {
                        //MY_LOGDEBUG("stu.masteralias="<<stu.masteralias);
                        //stu.masteralias.erase(  stu.masteralias.find('\\') );
                        boost::erase_first(stu.masteralias,"&");
                }

                while(stu.slaveralias.find('\\') != std::string::npos)
                {
                        //stu.slaveralias.erase(  stu.slaveralias.find('\\') );
                        MY_LOGDEBUG("stu.slaveralias="<<stu.slaveralias);
                        boost::erase_first(stu.slaveralias,"\\");
                }
                while(stu.slaveralias.find('"') != std::string::npos)
                {
                        //stu.slaveralias.erase(  stu.slaveralias.find('\\') );
                        //MY_LOGDEBUG("stu.slaveralias="<<stu.slaveralias);
                        boost::erase_first(stu.slaveralias,"\"");
                }
                while(stu.slaveralias.find('&') != std::string::npos)
                {
                        //stu.slaveralias.erase(  stu.slaveralias.find('\\') );
                        //MY_LOGDEBUG("stu.slaveralias="<<stu.slaveralias);
                        boost::erase_first(stu.slaveralias,"&");
                }

                str_json = str_json + "\"trusteeid\":"+str_sequence + ",";
                str_json = str_json + "\"mastergid\":\""+stu.mastergid + "\",";
                str_json = str_json + "\"mastername\":\""+stu.mastername + "\",";
                str_json = str_json + "\"slavergid\":\""+stu.slavergid + "\",";
                str_json = str_json + "\"slavername\":\""+stu.slavername + "\",";
                str_json = str_json + "\"masteralias\":\""+stu.masteralias + "\",";
                str_json = str_json + "\"slaveralias\":\""+stu.slaveralias + "\",";
                str_json = str_json + "\"gameid\":\""+stu.gameid + "\",";

                // 需要处理 serverid 和sid
                if(stu.serverid.empty() || stu.serverid == "_")
                {
                        str_json = str_json + "\"serverid\":\"" + "\",";
                        str_json = str_json + "\"sid\":\"" + "\",";
                }
                else if(stu.serverid.find("_") != std::string::npos)
                {
                        std::vector<std::string> param_vec;
                        boost::split(param_vec, stu.serverid, boost::is_any_of("_"));
                        if(param_vec.size() == 2)
                        {
                                str_json = str_json + "\"serverid\":\""+param_vec[0] + "\",";
                                str_json = str_json + "\"sid\":\""+param_vec[1] + "\",";
                        }
                }
                str_json = str_json + "\"smallsource\":"+SDUtility::format(stu.smallsource) + ",";
                str_json = str_json + "\"status\":"+ SDUtility::format(stu.status) + "},";
        }

        str_json.erase(str_json.size()-1);
        str_json = str_json + "}";

}




int dispatcher::box_client_response_error(int errcode, boost::shared_ptr<HttpRequestPacket> prequest, general_log &log)
{
        service_stat *pstat = service_stat::singleton();
        std::map<int, std::string>::iterator iter;
        std::map<std::string, std::string> rtn_map;
        iter = m_map_err_info.find(errcode);
        if(iter == m_map_err_info.end())
        {
                MY_LOGERROR("box_client_response_error param not supported, drop response error info ");
                return -1;

        }

        rtn_map.insert(make_pair("result", SDUtility::format(errcode)));
        rtn_map.insert(make_pair("errinfo", iter->second));
        HttpResponseHeaderPacket respose;
        respose.encode(HTTP_RESPONSE_STATUS_OK, rtn_map);
        std::string str_body;
        map2string(str_body, rtn_map);
        //MY_LOGDEBUG("http response: "<<str_body);
        box_client_response(&respose, prequest->m_clientfd, prequest->m_client_tag);
log.attach_http_business_log( prequest->mapParam, rtn_map);
        time_t nowtime = SDUtility::get_cur_millseconds();
        uint64_t processing_periodtime  = nowtime - prequest->m_start_processtime;
//	if (processing_periodtime > 5000)
        if(processing_periodtime > 1000)
        {
                PERFORM_LOG("process time too long, period millsecs="<<processing_periodtime<<
                            " cmd="<<prequest->m_str_GetRequest<<" now="<<nowtime<<"  starttime="<<prequest->m_start_processtime);

                log_service logservice;
                logservice.produce_slowlog_log(prequest.get(), rtn_map, nowtime);
        }

        switch(prequest->m_request_type)
        {

                case GAMEBOX_SERVER_BOXSERVER_LOGINGAME_ID:
                case GAMEBOX_SERVER_BOXSERVER_RAPELOGINGAME_ID:
//	case GAMEBOX_SERVER_BOXSERVER_FLUSHGAMEINFO_ID:
                case GAMEBOX_SERVER_BOXSERVER_SLAVELOGINGAME_ID:
                case GAMEBOX_SERVER_BOXSERVER_SPECIALLOGINGAME_ID:
//	case GAMEBOX_SERVER_BOXSERVER_FLUSHGAMEINFOSPE_ID:
                {
                        LOGINGAME_LOG(prequest->m_str_GetRequest<<" $$$$ RESPONSE-->>> "<<str_body
                                      <<" $$$  gameinternalno="<<prequest->str_log_gameinternalno<<
                                      " username="<<prequest->str_log_username<<" sessionid="<<prequest->str_log_sessionid
                                      <<" clientip="<<prequest->get_client_ip()<<"  xlinternalno="<<prequest->str_log_xlinternalno);
                        log_service logservice;
                        logservice.produce_logingame_log(prequest.get(), rtn_map);
                        pstat->add_logingame_result_stat(false);
                }
                break;

                default:
                {
                        SERVICE_LOG(prequest->m_str_GetRequest<<" $$$$ RESPONSE-->>> "<<str_body
                                    <<" $$$  gameinternalno="<<prequest->str_log_gameinternalno<<
                                    " username="<<prequest->str_log_username<<" sessionid="<<prequest->str_log_sessionid
                                    <<" clientip="<<prequest->get_client_ip()<<"  xlinternalno="<<prequest->str_log_xlinternalno);
                }
                break;
        }

log.attach_http_main_log( prequest->m_str_cmdpath, SDUtility::format(uint64_t(time(NULL))), 
		SDUtility::format(processing_periodtime), SDUtility::format(errcode));
log.produce_http_log();
        return 0;

        //return box_client_response_error(errcode, prequest->m_clientfd, prequest->m_client_tag);

}

int dispatcher::box_client_response_ok(boost::shared_ptr<HttpRequestPacket> prequest, general_log &log)
{
        service_stat *pstat = service_stat::singleton();
        std::map<std::string, std::string> body;
        body.insert(make_pair("result", SDUtility::format(SUCCESS)));
        body.insert(make_pair("errinfo", m_str_success_static));
        HttpResponseHeaderPacket response;
        std::string str_body;
        map2string(str_body, body);
        response.encode(HTTP_RESPONSE_STATUS_OK, body);
        box_client_response(&response, prequest->m_clientfd, prequest->m_client_tag);
log.attach_http_business_log( prequest->mapParam, body);
        uint64_t nowtime = SDUtility::get_cur_millseconds();
        uint64_t processing_periodtime  = nowtime - prequest->m_start_processtime;
//	if (processing_periodtime > 5000)
        if(processing_periodtime > 1000)
        {
                PERFORM_LOG("process time too long, period millsecs="<<processing_periodtime<<
                            " cmd="<<prequest->m_str_GetRequest<<" now="<<nowtime<<"  starttime="<<prequest->m_start_processtime);
                log_service logservice;
                logservice.produce_slowlog_log(prequest.get(), body, nowtime);
        }


        switch(prequest->m_request_type)
        {

                case GAMEBOX_SERVER_BOXSERVER_LOGINGAME_ID:
                case GAMEBOX_SERVER_BOXSERVER_RAPELOGINGAME_ID:
//	case GAMEBOX_SERVER_BOXSERVER_FLUSHGAMEINFO_ID:
                case GAMEBOX_SERVER_BOXSERVER_SLAVELOGINGAME_ID:
                case GAMEBOX_SERVER_BOXSERVER_SPECIALLOGINGAME_ID:
//	case GAMEBOX_SERVER_BOXSERVER_FLUSHGAMEINFOSPE_ID:
                {
                        LOGINGAME_LOG(""<<prequest->get_request_url()<<"  RESPONSE-->"<<str_body<<
                                      " $$$  gameinternalno="<<prequest->str_log_gameinternalno<<
                                      " username="<<prequest->str_log_username<<" sessionid="<<prequest->str_log_sessionid
                                      <<" clientip="<<prequest->get_client_ip()<<"  xlinternalno="<<prequest->str_log_xlinternalno);
                        log_service logserver;
                        logserver.produce_logingame_log(prequest.get(), body);
                        pstat->add_logingame_result_stat(true);
                        //MY_LOGDEBUG("produce_logingame_log");
                }
                break;

                default:
                {
                        SERVICE_LOG(prequest->m_str_GetRequest<<" $$$$ RESPONSE-->>> "<<str_body
                                    <<" $$$ gameinternalno="<<prequest->str_log_gameinternalno<<
                                    " username="<<prequest->str_log_username<<" sessionid="<<prequest->str_log_sessionid
                                    <<" clientip="<<prequest->get_client_ip()<<"  xlinternalno="<<prequest->str_log_xlinternalno);
                }
                break;
        }



        STU_REQ_RESP_STAT stat;
        stat.cmd_type = prequest->m_request_type;
        stat.requesttime_millsec = prequest->m_addqueue_time_millsec;
        stat.responsetime_millsec = SDUtility::get_cur_millseconds();
log.attach_http_main_log( prequest->m_str_cmdpath, SDUtility::format(uint64_t(time(NULL))), 
		SDUtility::format(processing_periodtime), SDUtility::format(SUCCESS));
log.produce_http_log();
        int serverid = server_path2id(prequest->m_str_cmdpath);
        if(serverid == GAMEBOX_SERVER_BOXSERVER_LOGINGAME_ID ||
           serverid == GAMEBOX_SERVER_BOXSERVER_SLAVELOGINGAME_ID||
           serverid == GAMEBOX_SERVER_BOXSERVER_RAPELOGINGAME_ID ||
           //	serverid == GAMEBOX_SERVER_BOXSERVER_FLUSHGAMEINFO_ID  ||
           serverid == GAMEBOX_SERVER_BOXSERVER_SPECIALLOGINGAME_ID)
                //	serverid == GAMEBOX_SERVER_BOXSERVER_FLUSHGAMEINFOSPE_ID)
        {
                // 对于登陆游戏请求，不加入request response 统计中
                MY_LOGERROR("skip logingame stat req resp");
                return 0;
        }

        pstat->add_req_resp_stat(stat);

        return 0;



}


int dispatcher::box_client_response_ok_params(boost::shared_ptr<HttpRequestPacket> prequest,
                std::map<std::string, std::string> &map_param, general_log &log)
{
        service_stat *pstat = service_stat::singleton();
        std::map<std::string, std::string> body;
        body.insert(make_pair("result", SDUtility::format(SUCCESS)));
        body.insert(make_pair("errinfo", "ok"));

        std::map<std::string, std::string>::iterator iter;
        for(iter = map_param.begin(); iter!= map_param.end(); iter++)
        {
                body.insert(make_pair(iter->first, iter->second));
        }
        HttpResponseHeaderPacket response;
        response.encode(HTTP_RESPONSE_STATUS_OK, body);
        std::string str_body;
        map2string(str_body, body);
        //MY_LOGDEBUG("http response="<<str_body);
        prequest->m_unixtime_tag_map.insert(make_pair("5_reply_end_process", SDUtility::format(SDUtility::get_cur_millseconds())));
        box_client_response(&response, prequest->m_clientfd, prequest->m_client_tag);
log.attach_http_business_log( prequest->mapParam, body);
        uint64_t nowtime = SDUtility::get_cur_millseconds();
        uint64_t processing_periodtime  = nowtime - prequest->m_start_processtime;
//	if (processing_periodtime > 5000)
        if(processing_periodtime > 1000)
        {
                PERFORM_LOG("process time too long, period millsecs="<<processing_periodtime<<
                            " cmd="<<prequest->m_str_GetRequest<<" now="<<nowtime<<"  starttime="<<prequest->m_start_processtime);

                log_service logservice;
                logservice.produce_slowlog_log(prequest.get(), body, nowtime);
        }


        switch(prequest->m_request_type)
        {
                case GAMEBOX_SERVER_BOXSERVER_LOGINGAME_ID:
                case GAMEBOX_SERVER_BOXSERVER_RAPELOGINGAME_ID:
                //	case GAMEBOX_SERVER_BOXSERVER_FLUSHGAMEINFO_ID:
                case GAMEBOX_SERVER_BOXSERVER_SPECIALLOGINGAME_ID:
                        //	case GAMEBOX_SERVER_BOXSERVER_FLUSHGAMEINFOSPE_ID:
                {
                        std::string strunixtime;
                        std::string logingame_request;
                        std::string logingame_reply;
                        logingame_request = prequest->m_unixtime_tag_map["3_logingame_request"];
                        logingame_reply = prequest->m_unixtime_tag_map["4_logingame_reply"];
                        uint64_t request = SDUtility::atouint64(logingame_request);
                        uint64_t reply = SDUtility::atouint64(logingame_reply);
                        uint64_t diff = reply-request;

                        map2string(strunixtime,prequest->m_unixtime_tag_map);
                        LOGINGAME_LOG(""<<prequest->get_request_url()<<"  RESPONSE-->"<<str_body<<
                                      " $$$  gameinternalno="<<prequest->str_log_gameinternalno<<
                                      " username="<<prequest->str_log_username<<" sessionid="<<prequest->str_log_sessionid
                                      <<" clientip="<<prequest->get_client_ip()<<"  xlinternalno="<<prequest->str_log_xlinternalno<<"  "<<strunixtime<<
                                      "  timetag="<<SDUtility::format(diff)<<
                                      " costime="<<prequest->m_costtime);
                        log_service logservice;
                        logservice.produce_logingame_log(prequest.get(), body);
                        pstat->add_logingame_result_stat(true);
                }
                break;
                case GAMEBOX_SERVER_BOXSERVER_SLAVELOGINGAME_ID:
                {
                        std::string strunixtime;
                        std::string logingame_request;
                        std::string logingame_reply;
                        logingame_request = prequest->m_unixtime_tag_map["3_logingame_request"];
                        logingame_reply = prequest->m_unixtime_tag_map["4_logingame_reply"];
                        uint64_t request = SDUtility::atouint64(logingame_request);
                        uint64_t reply = SDUtility::atouint64(logingame_reply);
                        uint64_t diff = reply-request;

                        map2string(strunixtime,prequest->m_unixtime_tag_map);
                        LOGINGAME_LOG(""<<prequest->get_request_url()<<"  RESPONSE-->"<<str_body<<
                                      " $$$  gameinternalno="<<prequest->str_log_gameinternalno<<
                                      " username="<<prequest->str_log_username<<" sessionid="<<prequest->str_log_sessionid
                                      <<" clientip="<<prequest->get_client_ip()<<"  xlinternalno="<<prequest->str_log_xlinternalno<<"  "<<strunixtime<<
                                      "  timetag="<<SDUtility::format(diff)<<
                                      " costime="<<prequest->m_costtime);
                        log_service logserver;
                        logserver.produce_slavelogingame_log(prequest.get(), body);
                        pstat->add_logingame_result_stat(true);
                }
                break;
                default:
                {
                        SERVICE_LOG(prequest->m_str_GetRequest<<" $$$ RESPONSE-->>> "<<str_body
                                    <<"$$$ gameinternalno="<<prequest->str_log_gameinternalno<<
                                    " username="<<prequest->str_log_username<<" sessionid="<<prequest->str_log_sessionid
                                    <<" clientip="<<prequest->get_client_ip()<<"  xlinternalno="<<prequest->str_log_xlinternalno);
                }
                break;
        }


        STU_REQ_RESP_STAT stat;
        stat.cmd_type = prequest->m_request_type;
        stat.requesttime_millsec = prequest->m_addqueue_time_millsec;
        stat.responsetime_millsec = SDUtility::get_cur_millseconds();
log.attach_http_main_log( prequest->m_str_cmdpath, SDUtility::format(uint64_t(time(NULL))), 
		SDUtility::format(processing_periodtime), SDUtility::format(SUCCESS));
log.produce_http_log();
        int serverid = server_path2id(prequest->m_str_cmdpath);
        if(serverid == GAMEBOX_SERVER_BOXSERVER_LOGINGAME_ID ||
           serverid == GAMEBOX_SERVER_BOXSERVER_SLAVELOGINGAME_ID||
           serverid == GAMEBOX_SERVER_BOXSERVER_RAPELOGINGAME_ID ||
           //	serverid == GAMEBOX_SERVER_BOXSERVER_FLUSHGAMEINFO_ID ||
           serverid == GAMEBOX_SERVER_BOXSERVER_SPECIALLOGINGAME_ID)
                //	serverid == GAMEBOX_SERVER_BOXSERVER_FLUSHGAMEINFOSPE_ID)
        {
                // 对于登陆游戏请求，不加入request response 统计中
                MY_LOGERROR("skip logingame stat req resp");
                return 0;
        }

        pstat->add_req_resp_stat(stat);

        return 0;

}

void dispatcher::attach_loginfo(STU_SESSION_INFO *psession, HttpRequestPacket *prequest ,std::string &strsessionid)
{
        if(psession == NULL  || prequest == NULL)
        {
                return;
        }

        prequest->str_log_gameinternalno = psession->gameinternalno;
        prequest->str_log_username = psession->username;
        prequest->str_log_sessionid = strsessionid;
        prequest->str_log_xlinternalno = psession->xlinternalno;

}

void * dispatcher::timer_stat_proc(void *param)
{

        if(param == NULL)
        {
                return (void *)NULL;
        }

        dispatcher *pthis =(dispatcher *)param;
        pthis->timer_stat_doit();

        MY_LOGERROR("dispatcher::timer_stat_proc terminated");
        return (void *)0;

}

//int dispatcher::update_gamelib_default_config(void)
//{
//	mysql_guard dbguard(&m_databasepool);
//	int result = dbguard.fetch_mysql_from_pool();
//	if (result == -1)
//	{
//		MY_LOGERROR("failed to update gamelib default config");
//		return -1;
//	}
//	result = gamelib_service::update_gamelib_config(dbguard.get_mysql());
//	if (result == -1)
//	{
//		MY_LOGERROR("failed to update gamelib default config");
//		return -1;
//	}
//	return 0;
//}

void dispatcher::timer_stat_doit(void)
{
        service_stat *pstat = service_stat::singleton();

        //update_gamelib_default_config();
        // 游戏库
        //uint64_t update_gamelib_default_config_before=time(NULL);
        //int      update_gamelib_default_config_diff=60;

        std::string today = SDUtility::format_day(time(NULL));

        while(true)
        {
                sleep(2);
                //uint64_t currenttime=time(NULL);
                uint64_t total_client_connection = m_http_manager.m_httpserver.get_total_history_client_count();
                uint64_t current_client_cnnection = m_http_manager.m_httpserver.get_client_count();
                //MY_LOGINFO("total="<<total_client_connection<< "  curent="<<current_client_cnnection);
                pstat->set_total_conn(total_client_connection);
                pstat->set_current_conn(current_client_cnnection);
                /*
                		std::string now_day = SDUtility::format_day(time(NULL));
                		if ( today != now_day)
                		{
                			MY_LOGDEBUG("change day, update messsage");
                			systemmessage_service::load_sysmsg_map();
                			messageaddress_service::load_msg_address_map();
                			today = now_day;
                		}

                */
                //// 游戏库基础配置信息更新到内存，60s更新一次
                //if (currenttime - update_gamelib_default_config_before > update_gamelib_default_config_diff)
                //{
                //	update_gamelib_default_config_before = currenttime;
                //	update_gamelib_default_config();
                //}

        }

}

int dispatcher::check_request_queued_timeout(boost::shared_ptr<HttpRequestPacket> prequest)
{


        // 需要检查队列中请求排队的时间，如果超过一定时间，将丢弃请求，避免导致雪崩效应
        uint64_t currenttime_millsec = SDUtility::get_cur_millseconds();
        prequest->m_dequeue_time_millsec = currenttime_millsec;
        if(currenttime_millsec - prequest->m_addqueue_time_millsec > m_queue_delay_drop_threashold)
        {
                prequest->m_response_time_millsec = currenttime_millsec;
                PERFORM_LOG("request queueed too long, drop process, response error imediately, cmd="
                            <<prequest->m_str_GetRequest<<"queued time="<<currenttime_millsec - prequest->m_addqueue_time_millsec
                            <<" threshold="<<m_queue_delay_drop_threashold);

                return -1;
        }
        else
        {
                //MY_LOGINFO("addsec="<<prequest->m_addqueue_time_millsec<<" dequeuesec="<<
                //	prequest->m_dequeue_time_millsec<<" queued_time="<<
                //	prequest->m_dequeue_time_millsec-prequest->m_addqueue_time_millsec);
                return 0;
        }

}

void dispatcher::prevent2json(std::string &str_json, std::vector<STU_PREVENT_INFO> &vec_prevent)
{
        str_json = str_json+"[";
        if(vec_prevent.size() != 0)
        {
                BOOST_FOREACH(STU_PREVENT_INFO info, vec_prevent)
                {
                        str_json = str_json+"{\"";
                        str_json = str_json+"gameid\":\"";
                        str_json = str_json+info.gameid + "\",";
                        str_json = str_json+"\"isprevent\":";
                        str_json = str_json+info.is_prevent+",";
                        str_json = str_json+"\"isadult\":";
                        str_json = str_json+info.is_adult+"},";
                }
                str_json.erase(str_json.size()-1);
        }
        str_json = str_json+"]";



}

int dispatcher::cache_gamelib_get_vec(std::vector<std::string> &vec ,std::string &str_key, redis_client *pclient)
{
        std::string key;
        key = key+PREFIX_GAMELIB+str_key;
        int result;
        result = pclient->get_value_set(key, vec);
        if(result == -1)
        {
                MY_LOGERROR("dispatcher::cache_gamelib_get_vec   get_value_set failed,key="<<str_key);
                return -1;
        }

        // 如果没有获取到数据，则不需要设置expire时间，直接返回成功
        if(vec.empty())
        {
                return 0;
        }
        result =  pclient->set_expire(key);

        if(result == 1)
        {
                return 0;
        }
        else
        {
                return -1;
        }


}

int dispatcher::cache_gamelib_set_value(std::string &str_key, std::string &str_value, redis_client *pclient)
{
        std::string key;
        key = key+PREFIX_GAMELIB+str_key;
        int result;
        result = pclient->set_value_set(key, str_value);
        if(result == -1)
        {
                return -1;
        }
        return pclient->set_expire(key);

}

int dispatcher::cache_gamelib_del_value(std::string &str_key, std::string &str_value, redis_client *pclient)
{
        std::string key;
        key = key+PREFIX_GAMELIB+str_key;
        return pclient->del_value_set(key,str_value);
}

int dispatcher::Chk18PaperId(const char *sPaperId)
{
        long lSumQT =0;
        //加权因子
        int R[] = {7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2 };
        //校验码
        char sChecker[11]= {'1','0','X', '9', '8', '7', '6', '5', '4', '3', '2'};
        //检验长度
        if(18 != strlen(sPaperId)) return -1;
        //校验数字
        //for (int i=0; i<18; i++)
        //{
        //if ( !isdigit(sPaperId[i]) && !(('X' == sPaperId[i] || 'x' == sPaperId[i]) && 17 == i) )
        //{
        //	return -1;
        //}
        //}
        //验证最末的校验码
        for(int i=0; i<=16; i++)
        {
                lSumQT += (sPaperId[i]-48) * R[i];
        }
        if(sChecker[lSumQT%11] != sPaperId[17])
        {
                return -1;
        }

        return 0;
}

void dispatcher::Process_slaverLoginGame(boost::shared_ptr<HttpRequestPacket> request)
{
	general_log log;
	log.set_log_tag("specialLoginGame");
        int result;
        request->m_start_processtime = SDUtility::get_cur_millseconds();
        result = check_request_queued_timeout(request);
        if(result == -1)
        {
                box_client_response_error(INTERNAL_TIMEOUT, request, log);
                return;
        }

        std::string sessionid = get_value("sessionid",request->mapParam);
        std::string smallid = get_value("smallid", request->mapParam);
        std::string gameid = get_value("gameid", request->mapParam);
        std::string sid = get_value("sid", request->mapParam);
        std::string loginurl = get_value("loginurl", request->mapParam);
        std::string serverid = get_value("serverid", request->mapParam);
        std::string clientid = get_value("clientid", request->mapParam);

        int clientidint = SDUtility::atouint32(clientid);
        if(loginurl.empty())
        {
                gameinfo *ginfo = gameinfo::GetInstance();
                if(ginfo->get_gameinfo4special(gameid, serverid, sid, loginurl))
                {
                        box_client_response_error(PARAM_ERROR,request, log);
                        return;
                }
        }
        if(sessionid.empty() || smallid.empty() || gameid.empty() ||
           sid.empty() || loginurl.empty() || serverid.empty())
        {
                box_client_response_error(PARAM_ERROR, request, log);
                return;
        }
        boost::trim(sessionid);
        boost::trim(smallid);
        boost::trim(gameid);
        boost::trim(sid);
        boost::trim(loginurl);
        boost::trim(serverid);

        std::string comefrom2 = get_value("comefrom2", request->mapParam);
        std::string comefrom3 = get_value("comefrom3", request->mapParam);
        std::string comefrom = get_value("comefrom",request->mapParam);
        std::string version = get_value("version",request->mapParam);


        STU_SESSION_INFO stu_session_info;
        result = get_session_info(sessionid, &stu_session_info);
        if(result == -1)
        {
                box_client_response_error(CACHE_ERROR,request, log);
                return;
        }
        if(result == 0)
        {
                box_client_response_error(INVALID_SESSION, request,log);
                return;
        }

        mysql_guard dbguard(&m_databasepool);
        result = dbguard.fetch_mysql_from_pool();
        if(result == -1)
        {
                box_client_response_error(DB_ERROR, request, log);
                return;
        }

        trustee_service trusteeserver(dbguard.get_mysql());
        STU_TRUSTEE_INFO stu_trustee_info;
	result = GENERAL_OPT_MYSQL_READ(log, "gamebox.trustee", int, trusteeserver.query_record_by_seqid, &stu_trustee_info, SDUtility::atouint64(smallid));
 //       result = trusteeserver.query_record_by_seqid(&stu_trustee_info, SDUtility::atouint64(smallid));
        if(result == -1)
        {
                box_client_response_error(DB_ERROR, request, log);
                return;
        }
        if(result == 0)
        {
                box_client_response_error(NOT_EXIST_SMALL, request, log);
                return;
        }

        if(stu_trustee_info.mastergid != stu_session_info.gameinternalno)
        {
                MY_LOGERROR("Process_slaverLoginGame no permission, stu_trustee_info.mastergid="<<stu_trustee_info.mastergid
                            <<"  stu_session_info.gameinternalno="<<stu_session_info.gameinternalno);
                box_client_response_error(NOT_PERMISSION, request, log);
                return;
        }
        if(stu_trustee_info.gameid != gameid)
        {
                MY_LOGERROR("Process_slaverLoginGame no permission, stu_trustee_info.gameid="<<
                            stu_trustee_info.gameid<<" gameid="<<gameid);
                box_client_response_error(NOT_PERMISSION, request, log);
                return;
        }
        loginwebgame_service loginwebgame_server;
        STU_GAME_RESULT stu_login_result;
        std::string password;
        std::string str_state;
        str_state  = CopartnerId_BLANK_PASS;
        password = "null";

        std::string promptid;
        promptid = get_promptid_by_startsource(stu_session_info.startsource);

        std::map<std::string, std::string> map_cmd;
        map_cmd.insert(make_pair("sessionid",sessionid));
        map_cmd.insert(make_pair("uid",""));
        map_cmd.insert(make_pair("gameinternalno",stu_trustee_info.slavergid));
        map_cmd.insert(make_pair("gameid",gameid));
        map_cmd.insert(make_pair("serverid",serverid));
        map_cmd.insert(make_pair("comefrom",comefrom));
        map_cmd.insert(make_pair("comefrom3",comefrom3));
        map_cmd.insert(make_pair("comefrom2",comefrom2));
        map_cmd.insert(make_pair("version",version));

        if(promptid.empty())
        {
                // 游戏库的登陆请求
                result = GENERAL_OPT_3RD_SERVICE_READ(log, "visit platform", int, loginwebgame_server.loginGame,
                						stu_trustee_info.slavername, password,stu_trustee_info.slavergid,
                                                       str_state, gameid, sid, loginurl, serverid, stu_session_info.clientip,
                                                       &stu_login_result, clientidint,
                                                       stu_session_info.comefrom,    /*游戏库登录的时候传过来的comefrom字段*/
                                                       promptid,    /*comefrom1*/
                                                       stu_session_info.comefrom,
                                                       "",
                                                       sessionid+SDUtility::format((uint64_t)time(NULL))
                                                       );
   /*             result = loginwebgame_server.loginGame(stu_trustee_info.slavername, password,stu_trustee_info.slavergid,
                                                       str_state, gameid, sid, loginurl, serverid, stu_session_info.clientip,
                                                       &stu_login_result, clientidint,
                                                       stu_session_info.comefrom,    //游戏库登录的时候传过来的comefrom字段
                                                       promptid,    //comefrom1
                                                       stu_session_info.comefrom,
                                                       "",
                                                       sessionid+SDUtility::format((uint64_t)time(NULL))
                                                      );
   */
        }
        else
        {
                // 游戏盒子的登陆请求
                if(comefrom2.empty())
                {
                        // 旧版本游戏盒子，1.1.10以前版本，包括1.1.10
                        result = GENERAL_OPT_3RD_SERVICE_READ(log, "visit platform", int, loginwebgame_server.loginGame,
                        					stu_trustee_info.slavername, password,stu_trustee_info.slavergid,
                                                               str_state, gameid, sid, loginurl, serverid, stu_session_info.clientip,
                                                               &stu_login_result, clientidint,
                                                               "201305174721588359",    /*旧版本盒子用默认值*/
                                                               stu_session_info.comefrom,                /*comefrom1*/
                                                               promptid,
                                                               "201305172727631722",
                                                               sessionid+SDUtility::format((uint64_t)time(NULL))
                                                              );
  /*                      result = loginwebgame_server.loginGame(stu_trustee_info.slavername, password,stu_trustee_info.slavergid,
                                                               str_state, gameid, sid, loginurl, serverid, stu_session_info.clientip,
                                                               &stu_login_result, clientidint,
                                                               "201305174721588359",    //旧版本盒子用默认值
                                                               stu_session_info.comefrom,                //comefrom1
                                                               promptid,
                                                               "201305172727631722",
                                                               sessionid+SDUtility::format((uint64_t)time(NULL))
                                                              );
  */
                }
                else
                {
                        // 新版本游戏盒子，1.1.11版本之后，包括1.1.11
                        result = GENERAL_OPT_3RD_SERVICE_READ(log, "visit platform", int, loginwebgame_server.loginGame,
                        						stu_trustee_info.slavername, password,stu_trustee_info.slavergid,
                                                               str_state, gameid, sid, loginurl, serverid, stu_session_info.clientip,
                                                               &stu_login_result, clientidint,
                                                               comefrom,
                                                               stu_session_info.comefrom,    /*comefrom1*/
                                                               comefrom2,
                                                               comefrom3,
                                                               sessionid+SDUtility::format((uint64_t)time(NULL))
                                                              );
   /*                     result = loginwebgame_server.loginGame(stu_trustee_info.slavername, password,stu_trustee_info.slavergid,
                                                               str_state, gameid, sid, loginurl, serverid, stu_session_info.clientip,
                                                               &stu_login_result, clientidint,
                                                               comefrom,
                                                               stu_session_info.comefrom,    //comefrom1
                                                               comefrom2,
                                                               comefrom3,
                                                               sessionid+SDUtility::format((uint64_t)time(NULL))
                                                              );
     */           }
        }

        /*std::string promptid;
        promptid = get_promptid_by_startsource(stu_session_info.startsource);

        if (promptid.empty())
        {
        	result = loginwebgame_server.loginGame(stu_trustee_info.slavername, password,stu_trustee_info.slavergid,
        		str_state, gameid, sid, loginurl, serverid, stu_session_info.clientip,
        		&stu_login_result, clientidint, stu_session_info.comefrom,promptid);
        }
        else
        {
        	result = loginwebgame_server.loginGame(stu_trustee_info.slavername, password,stu_trustee_info.slavergid,
        		str_state, gameid, sid, loginurl, serverid, stu_session_info.clientip,
        		&stu_login_result, clientidint,promptid, stu_session_info.comefrom);
        }*/


        //-------------------------------从这个地方开始，后面的部分和loginGame是一样的-------------------
        if(result == -1)
        {
                //box_client_response_error(OTHER_SYS_ERROR, request);
                map_cmd.insert(make_pair("isPrevent","0"));
                map_cmd.insert(make_pair("isAdult","0"));
                map_cmd.insert(make_pair("loginStatus",stu_login_result.loginStatus));
                map_cmd.insert(make_pair("gameLoginStatus",stu_login_result.gameLoginStatus));
                map_cmd.insert(make_pair("gameLoginURL",stu_login_result.gameLoginURL));
                map_cmd.insert(make_pair("listNum",stu_login_result.listNum));
                map_cmd.insert(make_pair("rtnMsg",stu_login_result.rtnMsg));
                map_cmd.insert(make_pair("costTime",stu_login_result.costTime));

                box_client_response_ok_params(request, map_cmd,log);
                return;
        }

        if(result == -2)
        {
                box_client_response_error(LOGINBOX_REDIRECT, request, log);
                return;
        }

        int errcode_result = loginwebgame_server.parseLoginGame(SDUtility::atouint32(stu_login_result.loginStatus.c_str(),
                             stu_login_result.loginStatus.size()));
        if(errcode_result != SUCCESS)
        {
                box_client_response_error(errcode_result, request, log);
                return;
        }

        // 保存登陆游戏信息到数据库
        STU_LOGINGAME_DB_INFO stu_logingame_db_info;
        stu_logingame_db_info.gameinternalno = stu_session_info.gameinternalno;
        stu_logingame_db_info.gameid = gameid;
        stu_logingame_db_info.serverid = serverid+"_"+sid;
        stu_logingame_db_info.logintime = SDUtility::get_current_time();
        /*	template_guard<mongo_client> mongoguard(&m_mongodb_pool);
        	result = mongoguard.fetch_obj_from_pool();
        	if (result == -1)
        	{
        		box_client_response_error(DB_ERROR, request);
        		return;
        	}
        	//logingame_db_service logingamedbserver(dbguard.get_mysql());
        	logingame_mongodb_service logingamedbserver(mongoguard.get());
        	result = logingamedbserver.add_logingame(&stu_logingame_db_info);
        	if (result == -1)
        	{
        		box_client_response_error(DB_ERROR, request);
        		return;
        	}
        */
        // 检查防沉迷
        std::vector<STU_PREVENT_INFO> vec_prevent;

        prevent_service preventserver(NULL);
        preventserver.query_prevent(vec_prevent, gameid);
        int isPrevent = 0;
        int isAdult = 0;
        if(vec_prevent.size() != 0)
        {
                //MY_LOGDEBUG("find gameid in prevent vecter, isaldult="<<vec_prevent[0].is_adult
                //			<<" isprevent="<<vec_prevent[0].is_prevent);
                STU_PREVENT_INFO stu;
                stu = vec_prevent[0];
                isAdult = SDUtility::atouint32(stu.is_adult.c_str(), stu.is_adult.size());
                if(SDUtility::atouint32(stu.is_prevent.c_str(),stu.is_prevent.size()) == PREVENT_STATUS_FORBIDDEN)
                {
                        //MY_LOGDEBUG("game is prevent gameid="<<gameid);
                        if(stu_session_info.idcardno.empty())
                        {
                                //	MY_LOGDEBUG("find no idcard set PREVENT_STATUS_UNKNOWN");
                                isPrevent = PREVENT_STATUS_UNKNOWN;
                        }
                        else
                        {
                                //	MY_LOGDEBUG("find idcard set PREVENT_STATUS_ALLOW");
                                isPrevent = PREVENT_STATUS_ALLOW;
                        }
                }
        }

        //MY_LOGDEBUG("gameLoginStatus ="<<stu_login_result.gameLoginStatus);
        if(stu_login_result.gameLoginStatus==WEBGAEM_LOGIN_RTN_GAMELOGINSTATUS_PREVENT)
        {
                isPrevent = PREVENT_STATUS_FORBIDDEN;
        }

        map_cmd.insert(make_pair("isPrevent",SDUtility::format(isPrevent)));
        map_cmd.insert(make_pair("isAdult",SDUtility::format(isAdult)));
        map_cmd.insert(make_pair("loginStatus",stu_login_result.loginStatus));
        map_cmd.insert(make_pair("gameLoginStatus",stu_login_result.gameLoginStatus));
        map_cmd.insert(make_pair("gameLoginURL",stu_login_result.gameLoginURL));
        map_cmd.insert(make_pair("listNum",stu_login_result.listNum));
        map_cmd.insert(make_pair("rtnMsg",stu_login_result.rtnMsg));
        map_cmd.insert(make_pair("costTime",stu_login_result.costTime));

        box_client_response_ok_params(request, map_cmd, log);

}

int dispatcher::session_to_json(std::string &str_json, STU_SESSION_INFO *psessioninfo)
{
        //std::string username;
        //std::string password;
        //std::string clientip;
        //std::string gameinternalno;
        //std::string idcardno;
        //std::string gameid;// ???????????id
        //std::string useloginstate;
        //std::string heartbeaturl;
        //std::string comefrom;  // 渠道来源

        cJSON *jsonobj = cJSON_CreateObject();

        cJSON_AddStringToObject(jsonobj,"username",psessioninfo->username.c_str());
        cJSON_AddStringToObject(jsonobj,"password",psessioninfo->password.c_str());
        cJSON_AddStringToObject(jsonobj,"clientip",psessioninfo->clientip.c_str());
        cJSON_AddStringToObject(jsonobj,"gameinternalno",psessioninfo->gameinternalno.c_str());
        cJSON_AddStringToObject(jsonobj,"idcardno",psessioninfo->idcardno.c_str());
        cJSON_AddStringToObject(jsonobj,"gameid",psessioninfo->gameid.c_str());
        cJSON_AddStringToObject(jsonobj,"useloginstate",psessioninfo->useloginstate.c_str());
        cJSON_AddStringToObject(jsonobj,"heartbeaturl",psessioninfo->heartbeaturl.c_str());
        cJSON_AddStringToObject(jsonobj,"comefrom",psessioninfo->comefrom.c_str());
        cJSON_AddStringToObject(jsonobj,"xlinternalno",psessioninfo->xlinternalno.c_str());
        cJSON_AddStringToObject(jsonobj,"startsource",psessioninfo->startsource.c_str());
        cJSON_AddStringToObject(jsonobj,"comefrom2",psessioninfo->comefrom2.c_str());
        cJSON_AddStringToObject(jsonobj,"comefrom3", psessioninfo->comefrom3.c_str());

        //cJSON_AddStringToObject(jsonobj,"username",psessioninfo->username.c_str());
        // 这里添加后续需要支持的内容



        char *pcharjson = cJSON_PrintUnformatted(jsonobj);
        str_json = pcharjson;
        free(pcharjson);
        cJSON_Delete(jsonobj);

        return 0;

}

int dispatcher::json_to_session(std::string &str_json, STU_SESSION_INFO *psessioninfo)
{
        cJSON *jsonobj = cJSON_Parse(str_json.c_str());
        if(jsonobj == NULL)
        {
                MY_LOGERROR("failed to dispatcher::json_to_session");
                return -1;
        }

        cJSON *item=NULL;

        item = cJSON_GetObjectItem(jsonobj, "username");
        if(item != NULL)
        {
                psessioninfo->username = item->valuestring;
        }

        item = cJSON_GetObjectItem(jsonobj, "password");
        if(item != NULL)
        {
                psessioninfo->password = item->valuestring;
        }

        item = cJSON_GetObjectItem(jsonobj, "clientip");
        if(item != NULL)
        {
                psessioninfo->clientip = item->valuestring;
        }

        item = cJSON_GetObjectItem(jsonobj, "gameinternalno");
        if(item != NULL)
        {
                psessioninfo->gameinternalno = item->valuestring;
        }

        item = cJSON_GetObjectItem(jsonobj, "idcardno");
        if(item != NULL)
        {
                psessioninfo->idcardno = item->valuestring;
        }

        item = cJSON_GetObjectItem(jsonobj, "gameid");
        if(item != NULL)
        {
                psessioninfo->gameid = item->valuestring;
        }

        item = cJSON_GetObjectItem(jsonobj, "useloginstate");
        if(item != NULL)
        {
                psessioninfo->useloginstate = item->valuestring;
        }

        item = cJSON_GetObjectItem(jsonobj, "heartbeaturl");
        if(item != NULL)
        {
                psessioninfo->heartbeaturl = item->valuestring;
        }

        item = cJSON_GetObjectItem(jsonobj, "comefrom");
        if(item != NULL)
        {
                psessioninfo->comefrom = item->valuestring;
        }

        item = cJSON_GetObjectItem(jsonobj, "xlinternalno");
        if(item != NULL)
        {
                psessioninfo->xlinternalno = item->valuestring;
        }

        item = cJSON_GetObjectItem(jsonobj, "startsource");
        if(item != NULL)
        {
                psessioninfo->startsource = item->valuestring;
        }

        item = cJSON_GetObjectItem(jsonobj, "comefrom2");
        if(item != NULL)
        {
                psessioninfo->comefrom2 = item->valuestring;
        }

        item = cJSON_GetObjectItem(jsonobj, "comefrom3");
        if(item != NULL)
        {
                psessioninfo->comefrom3 = item->valuestring;
        }

        cJSON_Delete(jsonobj);

        return 0;

}

int dispatcher::remove_cache_gameinternalno(std::string gameinternalno)
{

        std::string gameinternalno_with_prefix;
        gameinternalno_with_prefix = PREFIX_GID_TO_SESSIONID;
        gameinternalno_with_prefix = gameinternalno_with_prefix + gameinternalno;
        int result;
        memcache_guard memguard(&m_memcache_pool);
        result = memguard.fetch_memcached_client_from_pool();
        if(result == -1)
        {
                return -1;
        }
        memcache_client *pclient = memguard.get_memcache_client();
        if(pclient == NULL)
        {
                return -1;
        }
        result = pclient->delete_value(gameinternalno_with_prefix);
        if(result == -1)
        {
                return -1;
        }
        return 0;

}

void dispatcher::init_map_startsource_promptid(void)
{
        int result;
        mysql_config_loader loader;
        CMysql mysql;
        result = mysql.Connect(loader.m_str_mysql_ip.c_str(), loader.m_str_user.c_str(), loader.m_str_password.c_str(),
                               loader.m_str_dbname.c_str(), loader.m_mysql_port);
        if(result == -1)
        {
                return;
        }

        char charmysql[1024]= {0};


        sprintf(charmysql, "select startsource, promptid from gamebox.startsource_promptid");
        try
        {
                mysql.Query(charmysql);
                try
                {
                        mysql.StoreResult();
                }
                catch(CMysqlException& e)
                {
                        MY_LOGERROR("exec sql failed, sql="<<charmysql<<" error="<<e.GetErrMsg());
                        mysql.Close();
                        return;
                }
        }
        catch(CMysqlException& e)
        {
                mysql.Close();
                return;
        }


        int data_len;
        while(mysql.FetchRow())
        {
                std::string startsource;
                std::string promptid;
                char *pdata = NULL;
                pdata					   = mysql.GetFieldBinary("startsource", data_len);
                startsource.append(pdata, data_len);
                pdata					   = mysql.GetFieldBinary("promptid", data_len);
                promptid.append(pdata, data_len);
                m_map_startsource_promptid.insert(make_pair(startsource, promptid));
        }

        mysql.Close();

        std::map<std::string, std::string>::iterator iter;
        for(iter=m_map_startsource_promptid.begin(); iter!=m_map_startsource_promptid.end(); iter++)
        {
                MY_LOGDEBUG("startsource="<<iter->first<<"  promptid="<<iter->second);
        }


}

std::string dispatcher::get_promptid_by_startsource(std::string start_source)
{
        std::map<std::string, std::string>::iterator iter;
        iter = m_map_startsource_promptid.find(start_source);
        if(iter == m_map_startsource_promptid.end())
        {
                return "";
        }
        return iter->second;

}

int dispatcher::cache_set_session_tickout(std::string old_session)
{
        if(old_session.empty())
        {
                return 0;
        }
        std::string session_with_prefix;
        session_with_prefix = PREFIX_SESSION_TICKOUT;
        session_with_prefix = session_with_prefix + old_session;
        int fetch_result;
        memcache_guard memguard(&m_memcache_pool);
        fetch_result = memguard.fetch_memcached_client_from_pool();
        if(fetch_result == -1)
        {
                return -1;
        }
        memcache_client *pmemclient = memguard.get_memcache_client();
        std::string str_value;
        str_value = "1";
        //fetch_result  = pmemclient->set_value(session_with_prefix,str_value);
        fetch_result = pmemclient->set_value_with_expire(session_with_prefix, str_value, 600);
        return 0;


}

void dispatcher::zmq_client_pub_trustee_notify_recv(std::string &data)
{

}

void dispatcher::send_agreetrustee_notify(std::string from_gameinternalno,
                std::string to_gameinternalno, std::string message_type,
                std::string info)
{
        /*path=svr.post.agreetrustee&type=?&info=?from_gameinternalno=?&to_gameinternalno=?
        type:
        agreetrustee   addsmall
        */
        std::map<std::string, std::string> mapparam;
        mapparam.insert(std::make_pair("path", "svr.post.agreetrustee"));
        mapparam.insert(std::make_pair("type", message_type));
        mapparam.insert(std::make_pair("info", info));
        mapparam.insert(std::make_pair("from_gameinternalno", from_gameinternalno));
        mapparam.insert(std::make_pair("to_gameinternalno", to_gameinternalno));
        std::string content;
        map2string(content,mapparam);

        std::map<std::string, std::string> map_message;
        map_message.insert(std::make_pair("msgoriginal","svr"));
        map_message.insert(std::make_pair("from",m_zmq_service_self_identity));
        map_message.insert(std::make_pair("gameinternalno","svr"));
        map_message.insert(std::make_pair("content",content));
        cjson_wrapper wrapper;
        std::string str_json;
        wrapper.format_json_unfromat(map_message, str_json);
        m_zmq_client_pub_trustee_notify.send_data(str_json, m_zmq_router_notify_server_identity);
        MY_LOGDEBUG("send zmq data: "<<str_json<<"   destination="<<m_zmq_router_notify_server_identity);


}

int dispatcher::master_slave_check(void)
{
        master_slave_conf *pconf = master_slave_conf::singleton();
        if(pconf->is_master() == true)
        {
                return 0;
        }
        else
        {
                return -1;
        }

}












