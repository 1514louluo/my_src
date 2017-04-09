#include "general_log.h"
IMPL_LOGGER(general_log, logger);
std::string general_log::m_zmq_log_destination;
std::string general_log::m_server_name;
std::string general_log::m_server_ip;
std::string general_log::m_server_port;

general_log::general_log() {}
general_log::~general_log() {}

static bool json_array_append(std::string &old_array, std::string &elem)
{
        if(old_array.empty())
        {
                old_array = "[]";
        }
        std::map<std::string, std::string>::iterator iter;
        cJSON* JsonObj=cJSON_Parse(old_array.c_str());

        if(!JsonObj)
        {
                return false;
        }

        if(cJSON_Array != JsonObj->type)
        {
                cJSON_Delete(JsonObj);
                return false;
        }

        cJSON *pjsonobj = cJSON_Parse(elem.c_str());
        cJSON_AddItemToArray(JsonObj, pjsonobj);

        char *pdata = cJSON_PrintUnformatted(JsonObj);
        old_array = pdata;
        free(pdata);

        cJSON_Delete(JsonObj);
        return true;
}

static int format_json_unfromat(std::map<std::string, std::string> &mapparam, std::string &str_json)
{
        cJSON *pjsonobj = cJSON_CreateObject();
        std::map<std::string, std::string>::iterator iter;
        for(iter=mapparam.begin(); iter != mapparam.end(); iter++)
        {
                cJSON_AddStringToObject(pjsonobj, iter->first.c_str(), iter->second.c_str());
        }
        char *pdata = cJSON_PrintUnformatted(pjsonobj);
        str_json = pdata;
        free(pdata);
        cJSON_Delete(pjsonobj);
        return 0;
}

void general_log::set_log_tag(std::string service_name)
{

        m_http_main_log_map.insert(make_pair("server_name", m_server_name));

        m_http_main_log_map.insert(make_pair("server_ip", m_server_ip));

        m_http_main_log_map.insert(make_pair("server_port", m_server_port));

        m_http_main_log_map.insert(make_pair("service_name", service_name));

}

void general_log::attach_http_business_log(std::map<std::string, std::string> &input, std::map<std::string, std::string> &output)
{
        if(!m_http_business_log.empty() && !input.empty())
                m_http_business_log.append("&");
        map2string(m_http_business_log, input);
        if(!m_http_business_log.empty() && !output.empty())
                m_http_business_log.append("&");
        map2string(m_http_business_log, output);
}
void general_log::attach_http_other_type_log(int type, int opt_type,  std::string info, long cost_time)/*ОўГо*/
{
        std::string the_type;
        std::string the_opt_type;
        switch(type)
        {
                case LOG_REDIS:
                        the_type = "redis";
                        break;
                case LOG_MEMCACHE:
                        the_type = "memcache";
                        break;
                case LOG_MYSQL:
                        the_type = "mysql";
                        break;
                case LOG_MONGO:
                        the_type = "mongo";
                        break;
                case LOG_SERVER_3RD:
                        the_type = "3rd_service";
                        break;
                default:
                        break;
        }

        switch(opt_type)
        {
                case OPT_READ:
                        the_opt_type = "read";
                        break;
                case OPT_WRITE:
                        the_opt_type = "write";
                        break;
                case OPT_NONE:
                        the_opt_type = "none";
                        break;
                default:
                        break;
        }

        std::map<std::string, std::string> maprt;
        maprt.insert(make_pair("type",the_type));
        maprt.insert(make_pair("opt_type",the_opt_type));
        maprt.insert(make_pair("info",info));
        maprt.insert(make_pair("cost_time",SDUtility::format(int(cost_time))));

        std::string json_str;
        format_json_unfromat(maprt, json_str);

        switch(type)
        {
                case LOG_REDIS:
                case LOG_MEMCACHE:
                        json_array_append(m_http_cache_log, json_str);
                        break;
                case LOG_MYSQL:
                case LOG_MONGO:
                        json_array_append(m_http_db_log, json_str);
                        break;
                case LOG_SERVER_3RD:
                        json_array_append(m_http_3rd_service_log, json_str);
                        break;
                default:
                        break;
        }

}
void general_log::attach_http_main_log(std::string cmd_path, std::string start_time, std::string process_time, std::string result)
{

        m_http_main_log_map.insert(make_pair("cmd_path", cmd_path));

        m_http_main_log_map.insert(make_pair("start_time", start_time));


        m_http_main_log_map.insert(make_pair("process_time", process_time));

        m_http_main_log_map.insert(make_pair("result", result));


        map2string(m_http_main_log,  m_http_main_log_map);

        if(!m_http_main_log.empty() && !m_http_business_log.empty())
                m_http_main_log.append("&");
        m_http_main_log = m_http_main_log + m_http_business_log;
}

void general_log::set_stat_log_tag(std::string service_name)
{

        m_stat_log_map.insert(make_pair("server_name", m_server_name));

        m_stat_log_map.insert(make_pair("server_ip", m_server_ip));

        m_stat_log_map.insert(make_pair("server_port", m_server_port));

        m_stat_log_map.insert(make_pair("service_name", service_name));

        m_stat_log_map.insert(make_pair("start_time", SDUtility::format(uint64_t(time(NULL)))));

}

void general_log::attach_stat_log(std::string key, std::string value)
{

        m_stat_log_map.insert(make_pair(key, value));
}

void general_log::produce_http_log(void)
{
        msg_codec  msg;
        if(!m_http_db_log.empty())
                msg.setData("db", m_http_db_log);
        if(!m_http_cache_log.empty())
                msg.setData("cache", m_http_cache_log);
        if(!m_http_3rd_service_log.empty())
                msg.setData("service_3rd", m_http_3rd_service_log);
        if(!m_http_main_log.empty())
                msg.setData("main", m_http_main_log);
        std::string send_data = msg.getSendData();
        if(send_data.empty()) return;
        send_data = "general_log_business?" + send_data;

        zmq_client_pub &zmq_pub = Singleton<zmq_client_pub>::instance();
        zmq_pub.send_data(send_data, m_zmq_log_destination);

}
void general_log::produce_stat_log(void)
{
        map2string(m_stat_log, m_stat_log_map);
        msg_codec  msg;
        if(!m_stat_log.empty())
                msg.setData("stat", m_stat_log);

        std::string send_data = msg.getSendData();
        if(send_data.empty()) return;
        send_data = "general_log_stat?" + send_data;

        zmq_client_pub &zmq_pub = Singleton<zmq_client_pub>::instance();
        zmq_pub.send_data(send_data, m_zmq_log_destination);
}

void general_log::zmq_pub_func(std::string &data) {}
void general_log::INIT_GENERAL_LOG(void)
{
        const SDConfiguration& config = SDConfigurationSingleton::get_instance()->get_config();
        m_zmq_log_destination = config.getString("zmq_log_destination", " ");
        m_server_name= config.getString("server_name", " ");
        m_server_ip= config.getString("server_ip", " ");
        m_server_port= SDUtility::format(config.getInt("main_server_port", 2222));

        std::string zmq_pub_ip = config.getString("zmq_pub_ip", "127.0.0.1");
        int zmq_pub_port = config.getInt("zmq_pub_port", 4444);
        std::string zmq_pub_identity=config.getString("zmq_pub_identity","unkown");

        boost::function<void (std::string &)> zmq_pub_callback = boost::bind(&general_log::zmq_pub_func, _1);
        zmq_client_pub &zmq_pub = Singleton<zmq_client_pub>::instance();
        int  result = zmq_pub.init_zmq_client(zmq_pub_ip, zmq_pub_port, zmq_pub_identity, zmq_pub_callback);
        if(-1 == result)
        {
                MY_LOGERROR("failed to init zmq pub!!!");
        }
}







