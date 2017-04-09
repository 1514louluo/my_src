#include "dispatcher.h"

#include "SDConfigurationSingleton.h"
#include <sys/types.h>
#include "service_stat.h"

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
        param = "过滤一般运营配置失败";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(FILT_FAIL_IN_COMMON_BUSINESS_PERIORD, param));
        param = "过滤一般条件配置失败";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(FILT_FAIL_IN_COMMON_COND_PERIORD,param));
        param = "过滤一般返回值配置失败";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(FILT_FAIL_IN_COMMON_RESPONSE_PERIORD, param));
        param = "过滤一般任务处理配置失败";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(FILT_FAIL_IN_COMMON_TASK_PERIORD, param));
        param = "过滤硬性运营配置失败";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(FILT_FAIL_IN_ENTRY_BUSINESS_PERIORD, param));
        param = "过滤固定内容返回值配置失败";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(FILT_FAIL_IN_FIXED_CONTENT_RESPONSE_PERIORD, param));
        param = "过滤限制条件配置失败";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(FILT_FAIL_IN_RESTRICT_COND_PERIORD, param));

        param = "更新配置的类型值错误";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(FLUSH_UPDATE_TYPE_ERROR, param));

        param = "更新配置文件错误";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(FLUSH_FILTER_CONFIG_ERROR, param));

        param = "系统超时";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(INTERNAL_TIMEOUT, param));

        param = "非法请求";
        SDUtility::GBKToUTF8(param);
        m_map_err_info.insert(make_pair(ILLEGAL_ACCESS, param));

        // 登陆注册协议集
        m_map_request_path.insert(make_pair(GAMEBOX_SERVER_BOXSERVER_TOUCHBOXACTION, GAMEBOX_SERVER_BOXSERVER_TOUCHBOXACTION_ID));
        m_map_request_path.insert(make_pair(GAMEBOX_SERVER_BOXSERVER_FLUSHFILTERCONFIG, GAMEBOX_SERVER_BOXSERVER_FLUSHFILTERCONFIG_ID));

}

void dispatcher::init(void)
{
        int result;
        m_str_success_static = "成功";
        SDUtility::GBKToUTF8(m_str_success_static);
        init_server_map();

        /*<==============初始化各个filter==================>*/
        RW_detacher::init_RW_detacher(RW_DETACHER_ALL);
        filter_config::INIT_FILTER_CONFIG();
        filter_config& m_filter_config = Singleton<filter_config>::instance();
        result = m_filter_config.do_update_configs();
        if(-1 == result)
        {
                MY_LOGERROR("failed to update filter config!!!");
                sleep(1);
                exit(0);
                return;
        }

        business_event *bevent = new business_event();
        bevent->init_business_event();

        cond_event *cevent = new cond_event();
        cevent->init_cond_event();

        response_event *revent = new response_event();
        revent->init_response_event();

        task_event *tevent = new task_event();
        tevent->init_task_event();

        m_events.push_back(cevent);
        m_events.push_back(bevent);
        m_events.push_back(revent);
        m_events.push_back(tevent);
        /*<================================================>*/

        // static初始化

        const SDConfiguration& config = SDConfigurationSingleton::get_instance()->get_config();
        m_queue_delay_drop_threashold = config.getInt("request_queue_delay_drop_threashold", 1000);


        m_http_manager.set_parent(this);
        m_http_manager.init_http_manager();

        m_thread_poll_touch_man.init_thread_poll();

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
        result = pthread_create(&threadid, NULL, dispatcher::simple_timer_proc, this);
        if(result == -1)
        {
                MY_LOGERROR("failed to create thread dispatcher::http_response_proc");
                sleep(1);
                exit(0);
                return;
        }

        MY_LOGINFO("service init finished");
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
                        case GAMEBOX_SERVER_BOXSERVER_TOUCHBOXACTION_ID:
                                task = boost::bind(&dispatcher::Process_touchBoxAction, this, prequest);
                                break;
                        case GAMEBOX_SERVER_BOXSERVER_FLUSHFILTERCONFIG_ID:
                                task = boost::bind(&dispatcher::Process_flushFilterConfig, this, prequest);
                                break;

                        default:
                                MY_LOGERROR("process dealing still not supported, cmdpath="<<prequest->m_str_cmdpath<<" serverid="<<serverid);
                                http_response_error(ILLEGAL_ACCESS,prequest);

                }
                //boost::function<void(void)> testtask = boost::bind(&dispatcher::test_process, this, prequest);
                if(task != NULL)
                {

                        m_thread_poll_touch_man.commit_task(task);

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

int dispatcher::http_response(HttpResponseHeaderPacket *phttp_response , int clientfd,
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

void *dispatcher::simple_timer_proc(void *param)
{
        if(param == NULL)
        {
                return (void *)NULL;
        }

        dispatcher *pthis =(dispatcher *)param;
        pthis->timer_stat_doit();

        MY_LOGERROR("dispatcher::simple_timer_proc terminated");
        return (void *)0;
}

void dispatcher::timer_stat_doit(void)
{
        int ret;
        struct timeval secs_sep;
        while(true)
        {
                secs_sep.tv_sec = 43200;
                secs_sep.tv_usec = 0;
                ret = select(0,NULL,NULL,NULL,&secs_sep);
                if(!ret)
                {
                        filter_config& fconfig = Singleton<filter_config>::instance();
                        fconfig.do_flush_redis_ttl_in_unrestrict_cond_config();
                }
                else
                        LOG4CPLUS_ERROR(logger, "select error!!");
        }

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



int dispatcher::touch_core(boost::shared_ptr<HttpRequestPacket> request,  std::map<std::string, std::string> *response_map_param)
{
        int ret = SUCCESS;
        int size = m_events.size();
        for(int i = 0; i < size; i++)
        {
                ret = m_events.at(i)->do_event(request, response_map_param);
                if(SUCCESS != ret) return ret;
        }

        return ret;
}

void dispatcher::Process_flushFilterConfig(boost::shared_ptr<HttpRequestPacket> request)
{
        int result;
        request->m_start_processtime = SDUtility::get_cur_millseconds();
        result = check_request_queued_timeout(request);
        if(result == -1)
        {
                http_response_error(INTERNAL_TIMEOUT, request);
                return;
        }
        std::string update_type = get_value("type", request->mapParam);
        boost::trim(update_type);
        if(update_type.empty() || "flush_config" != update_type)
        {
                http_response_error(PARAM_ERROR,request);
                return;
        }

        filter_config& m_filter_config = Singleton<filter_config>::instance();
        result = m_filter_config.do_update_configs();

        if(result)
        {
                http_response_error(FLUSH_FILTER_CONFIG_ERROR,request);
                return;
        }

        http_response_ok(request);
}

void dispatcher::Process_touchBoxAction(boost::shared_ptr<HttpRequestPacket> request)
{
        int result;
        request->m_start_processtime = SDUtility::get_cur_millseconds();
        result = check_request_queued_timeout(request);
        if(result == -1)
        {
                http_response_error(INTERNAL_TIMEOUT, request);
                return;
        }
        std::map<std::string, std::string >  response_map_param;
        result = touch_core(request, &response_map_param);

        if(SUCCESS == result) http_response_ok_params(request, response_map_param);
        else http_response_error(result, request);
}


int dispatcher::http_response_error(int errcode, boost::shared_ptr<HttpRequestPacket> prequest)
{
        std::map<int, std::string>::iterator iter;
        std::map<std::string, std::string> rtn_map;
        iter = m_map_err_info.find(errcode);
        if(iter == m_map_err_info.end())
        {
                MY_LOGERROR("http_response_error param not supported, drop response error info ");
                return -1;

        }

        rtn_map.insert(make_pair("result", SDUtility::format(errcode)));
        rtn_map.insert(make_pair("errinfo", iter->second));
        HttpResponseHeaderPacket respose;
        respose.encode(HTTP_RESPONSE_STATUS_OK, rtn_map);
        std::string str_body;
        map2string(str_body, rtn_map);
        //MY_LOGDEBUG("http response: "<<str_body);
        http_response(&respose, prequest->m_clientfd, prequest->m_client_tag);

        time_t nowtime = SDUtility::get_cur_millseconds();
        uint64_t processing_periodtime  = nowtime - prequest->m_start_processtime;
//	if (processing_periodtime > 5000)
        if(processing_periodtime > 1000)
        {
                PERFORM_LOG("process time too long, period millsecs="<<processing_periodtime<<
                            " cmd="<<prequest->m_str_GetRequest<<" now="<<nowtime<<"  starttime="<<prequest->m_start_processtime);
        }
        switch(prequest->m_request_type)
        {
                case GAMEBOX_SERVER_BOXSERVER_TOUCHBOXACTION_ID:
                case GAMEBOX_SERVER_BOXSERVER_FLUSHFILTERCONFIG_ID:
                {
   //                     MY_LOGINFO(prequest->m_str_GetRequest<<" $$$$ RESPONSE-->>> "<<str_body<<" $$$ ");
                           SERVICE_LOG(prequest->m_str_GetRequest<<" $$$$ RESPONSE-->>> "<<str_body<<" $$$ ");

                }
                break;

                default:
                {
  //                      MY_LOGINFO(prequest->m_str_GetRequest<<" $$$$ RESPONSE-->>> "<<str_body<<" $$$ ");
   				SERVICE_LOG(prequest->m_str_GetRequest<<" $$$$ RESPONSE-->>> "<<str_body<<" $$$ ");
                }
                break;
        }


        return 0;

        //return http_response_error(errcode, prequest->m_clientfd, prequest->m_client_tag);

}

int dispatcher::http_response_ok(boost::shared_ptr<HttpRequestPacket> prequest)
{
        service_stat *pstat = service_stat::singleton();
        std::map<std::string, std::string> body;
        body.insert(make_pair("result", SDUtility::format(SUCCESS)));
        body.insert(make_pair("errinfo", m_str_success_static));
        HttpResponseHeaderPacket response;
        std::string str_body;
        map2string(str_body, body);
        response.encode(HTTP_RESPONSE_STATUS_OK, body);
        http_response(&response, prequest->m_clientfd, prequest->m_client_tag);

        uint64_t nowtime = SDUtility::get_cur_millseconds();
        uint64_t processing_periodtime  = nowtime - prequest->m_start_processtime;
//	if (processing_periodtime > 5000)
        if(processing_periodtime > 1000)
        {
                PERFORM_LOG("process time too long, period millsecs="<<processing_periodtime<<
                            " cmd="<<prequest->m_str_cmdpath<<" now="<<nowtime<<"  starttime="<<prequest->m_start_processtime);
        }


        switch(prequest->m_request_type)
        {
                case GAMEBOX_SERVER_BOXSERVER_TOUCHBOXACTION_ID:
                case GAMEBOX_SERVER_BOXSERVER_FLUSHFILTERCONFIG_ID:
                {
 //                       MY_LOGINFO(prequest->m_str_GetRequest<<" $$$$ RESPONSE-->>> "<<str_body<<" $$$ ");
 				SERVICE_LOG(prequest->m_str_GetRequest<<" $$$$ RESPONSE-->>> "<<str_body<<" $$$ ");

                }
                break;

                default:
                {
     //                   MY_LOGINFO(prequest->m_str_GetRequest<<" $$$$ RESPONSE-->>> "<<str_body<<" $$$ ");
				SERVICE_LOG(prequest->m_str_GetRequest<<" $$$$ RESPONSE-->>> "<<str_body<<" $$$ ");
                }
                break;
        }



        STU_REQ_RESP_STAT stat;
        stat.cmd_type = prequest->m_request_type;
        stat.requesttime_millsec = prequest->m_addqueue_time_millsec;
        stat.responsetime_millsec = SDUtility::get_cur_millseconds();

        int serverid = server_path2id(prequest->m_str_cmdpath);
        if(serverid == GAMEBOX_SERVER_BOXSERVER_TOUCHBOXACTION_ID ||
           serverid == GAMEBOX_SERVER_BOXSERVER_FLUSHFILTERCONFIG_ID)
        {
                // 对于登陆游戏请求，不加入request response 统计中
                MY_LOGERROR("skip logingame stat req resp");
                return 0;
        }

        pstat->add_req_resp_stat(stat);

        return 0;



}


int dispatcher::http_response_ok_params(boost::shared_ptr<HttpRequestPacket> prequest,
                                        std::map<std::string, std::string> &map_param)
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
        http_response(&response, prequest->m_clientfd, prequest->m_client_tag);
        uint64_t nowtime = SDUtility::get_cur_millseconds();
        uint64_t processing_periodtime  = nowtime - prequest->m_start_processtime;
//	if (processing_periodtime > 5000)
        if(processing_periodtime > 1000)
        {
                PERFORM_LOG("process time too long, period millsecs="<<processing_periodtime<<
                            " cmd="<<prequest->m_str_cmdpath<<" now="<<nowtime<<"  starttime="<<prequest->m_start_processtime);
        }


        switch(prequest->m_request_type)
        {
                case GAMEBOX_SERVER_BOXSERVER_TOUCHBOXACTION_ID:
                case GAMEBOX_SERVER_BOXSERVER_FLUSHFILTERCONFIG_ID:
                {
  //                      MY_LOGINFO(prequest->m_str_GetRequest<<" $$$$ RESPONSE-->>> "<<str_body<<" $$$ ");
   				SERVICE_LOG(prequest->m_str_GetRequest<<" $$$$ RESPONSE-->>> "<<str_body<<" $$$ ");

                }
                break;

                default:
                {
//                        MY_LOGINFO(prequest->m_str_GetRequest<<" $$$$ RESPONSE-->>> "<<str_body<<" $$$ ");
   				SERVICE_LOG(prequest->m_str_GetRequest<<" $$$$ RESPONSE-->>> "<<str_body<<" $$$ ");
                }
                break;
        }


        STU_REQ_RESP_STAT stat;
        stat.cmd_type = prequest->m_request_type;
        stat.requesttime_millsec = prequest->m_addqueue_time_millsec;
        stat.responsetime_millsec = SDUtility::get_cur_millseconds();

        int serverid = server_path2id(prequest->m_str_cmdpath);
        if(serverid == GAMEBOX_SERVER_BOXSERVER_TOUCHBOXACTION_ID ||
           serverid == GAMEBOX_SERVER_BOXSERVER_FLUSHFILTERCONFIG_ID)
        {
                // 对于登陆游戏请求，不加入request response 统计中
                MY_LOGERROR("skip logingame stat req resp");
                return 0;
        }

        pstat->add_req_resp_stat(stat);

        return 0;

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
                            <<prequest->m_str_cmdpath<<"queued time="<<currenttime_millsec - prequest->m_addqueue_time_millsec
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









