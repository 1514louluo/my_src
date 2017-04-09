#include "filter_config.h"
IMPL_LOGGER(filter_config, logger);
std::string filter_config::m_table_restrict_cond_config;
std::string filter_config::m_table_unrestrict_cond_config;
std::string filter_config::m_table_business_config;
std::string filter_config::m_table_fixed_content_response_config;
int filter_config::m_time_stamp;

std::string filter_config::do_find_in_fixed_content_config(std::string &key, std::string &value)
{
        std::string index;
        m_rwlock.rdlock();
        std::map<std::string, boost::shared_ptr<fixed_content_response_config> >::iterator iter;
        iter = m_fixed_content_config.find(key);
        if(iter != m_fixed_content_config.end())
        {
                boost::shared_ptr<fixed_content_response_config>  tmp = iter->second;
                index = tmp->m_type;
                value = tmp->m_estimated_value;
        }
        else
        {
                LOG4CPLUS_ERROR(logger, "business regular_rtn_content_conf is not in fixed_content_conf!!!");
                index = "";
        }
        m_rwlock.unlock();
        return index;
}
std::string filter_config::do_find_in_restrict_cond_config(std::string &key, std::string &value)
{
        boost::shared_ptr<restrict_cond_config> tmp;
        m_rwlock.rdlock();
        for(std::map<std::string, boost::shared_ptr<restrict_cond_config> >::const_iterator eiter = m_restrict_cond_config.begin(); eiter != m_restrict_cond_config.end(); ++eiter)
        {
                tmp = eiter->second;
                if(value == tmp->m_estimated_value && key == tmp->m_control_identity)
                {
                        m_rwlock.unlock();
                        return tmp->m_uniq_tag;
                }
        }
        m_rwlock.unlock();

        return "";
}

void filter_config::do_flush_redis_ttl_in_unrestrict_cond_config(void)
{
        m_rwlock.rdlock();
        if(!m_unrestrict_cond_config.empty())
        {
                for(std::map< std::string, boost::shared_ptr<unrestrict_cond_config> >::const_iterator eiter = m_unrestrict_cond_config.begin(); eiter != m_unrestrict_cond_config.end(); ++eiter)
                {
                        boost::shared_ptr<unrestrict_cond_config> config = eiter->second;
                        std::string key = REDIS_HEAD_FOR_BUSINESS + config->m_uniq_tag + config->m_control_identity;

                        if(-1 == RW_detacher::do_redis_expire(key, 86400))
                        {
                                MY_LOGERROR("redis error! expire = " <<key<<" 86400");
                        }
                }
        }
        m_rwlock.unlock();
}

std::string filter_config::do_find_in_unrestrict_cond_config(std::string &key, std::string &value)
{
        int result;
        boost::shared_ptr<unrestrict_cond_config> tmp;
        m_rwlock.rdlock();
        for(std::map<std::string, boost::shared_ptr<unrestrict_cond_config> >::const_iterator eiter = m_unrestrict_cond_config.begin(); eiter != m_unrestrict_cond_config.end(); ++eiter)
        {
                tmp = eiter->second;

                if(key == tmp->m_control_identity)
                {
                        std::string key =  REDIS_HEAD_FOR_UNRESTRICT_COND + tmp->m_uniq_tag + tmp->m_control_identity;
                        result = RW_detacher::do_redis_sismember(key, value);
                        if(-1 == result)
                        {
                                MY_LOGERROR("redis error!!! sismember "<<key<<" "<<value);
                                m_rwlock.unlock();
                                return "";
                        }
                        else if(1 == result)
                        {
                                m_rwlock.unlock();
                                return tmp->m_uniq_tag;
                        }
                }
        }
        m_rwlock.unlock();

        return "";
}


int filter_config::do_filt_time_limit_in_business_config(std::vector<std::string> &vconfigid)
{
        time_t now = time(NULL);
        m_rwlock.rdlock();
        for(std::map<std::string, boost::shared_ptr<business_config> >::const_iterator eiter = m_business_config.begin(); eiter != m_business_config.end(); ++eiter)
        {
                boost::shared_ptr<business_config> bconfig = eiter->second;
                std::string _configid = bconfig->m_configid;

                std::string _valid_start_unixtime = bconfig->m_valid_start_unixtime;
                time_t time_limit = (time_t)SDUtility::atouint64(_valid_start_unixtime);
                if(!_valid_start_unixtime.empty())
                {
                        if(time_limit > now || time_limit + m_time_stamp < now)
                        {
//                                LOG4CPLUS_WARN(logger, "now is no in start_unixtime ~ start_unixtime + "<<m_time_stamp);
                                continue;
                        }
                }
                vconfigid.push_back(_configid);
        }
        m_rwlock.unlock();
        if(vconfigid.empty() && !m_business_config.empty())
                return -1;
        return 0;
}

int filter_config::do_filt_input_limit(std::map<std::string, std::string> *response_map_param, std::vector<std::string> &vconfigid)
{
        std::string regular_conf_value = get_value("regular_conf", response_map_param);
        std::string unregular_conf_value = get_value("unregular_conf", response_map_param);
        response_map_param->clear();

        std::set<std::string> set1, set2;
        std::string atoken = "|";
        stringsplit2set(regular_conf_value, atoken, set1);
        stringsplit2set(unregular_conf_value, atoken, set2);


        int flag = 0;
        std::vector<std::string> new_vconfigid;

        int vconfigid_size = vconfigid.size();
        m_rwlock.rdlock();
        for(int i = 0; i < vconfigid_size; i++)
        {
                std::map<std::string,  boost::shared_ptr<business_config> >::iterator iter;
                iter = m_business_config.find(vconfigid.at(i));
                if(iter != m_business_config.end())
                {
                        boost::shared_ptr<business_config> bconfig = iter->second;
                        std::string _regular_conf = bconfig->m_regular_conf;
                        std::string _unregular_conf =  bconfig->m_unregular_conf;
                        if(!_regular_conf.empty() && !regular_conf_value.empty())
                        {
                                std::vector<std::string> ars;
                                stringsplit(_regular_conf, atoken, ars);
                                int size = ars.size();
                                int i = 0;
                                for(; i < size; i++)
                                {
                                        std::string rindex = ars.at(i);
                                        if(!rindex.empty())
                                        {
                                                if(-1 == find_in_set(rindex, set1))
                                                {
        //                                                LOG4CPLUS_WARN(logger, "filt by regular_conf: regular_conf!"<<_regular_conf<<" request is"<<regular_conf_value);
                                                        flag = 1;
                                                }
                                        }
                                }
                        }
                        if(!_unregular_conf.empty() && !unregular_conf_value.empty())
                        {
                                std::vector<std::string> ars;
                                stringsplit(_unregular_conf, atoken, ars);
                                int size = ars.size();
                                int i = 0;
                                for(; i < size; i++)
                                {
                                        std::string rindex = ars.at(i);
                                        if(!rindex.empty())
                                        {
                                                if(-1 == find_in_set(rindex, set2))
                                                {
      //                                                  LOG4CPLUS_WARN(logger, "filt by regular_conf: unregular_conf!"<<_unregular_conf<<" request is"<<unregular_conf_value);
                                                        flag = 1;
                                                }
                                        }
                                }
                        }
                        if(!flag)
                        {
                                new_vconfigid.push_back(vconfigid.at(i));
                        }
                        flag = 0;
                }
        }
        m_rwlock.unlock();
        if(new_vconfigid.empty())
                return -1;
        vconfigid = new_vconfigid;
        return 0;
}

int filter_config::do_filt_output_limit(std::map<std::string, std::string> *response_map_param, std::vector<std::string> &vconfigid)
{
        int vconfigid_size = vconfigid.size();
        int i = 0;
        for(; i < vconfigid_size; i++)
        {
                m_rwlock.rdlock();
                boost::shared_ptr<business_config> bconfig = m_business_config[vconfigid.at(i)];
                m_rwlock.unlock();

                if(bconfig)
                {
                        std::string key = REDIS_HEAD_FOR_BUSINESS + bconfig->m_valid_start_unixtime + bconfig->m_configid;
                        int limit = SDUtility::atouint32(bconfig->m_volumn);
                        std::string incr_num = "1";

                        int result = RW_detacher::do_redis_incrby(key, incr_num);
                        if(-1 == result)
                        {
                                MY_LOGERROR("redis error! incrby  " <<key<<" "<<incr_num);
                                return -1;
                        }
                        else if(limit < result)
                        {
                                MY_LOGWARN("business configid " <<bconfig->m_configid<<" reach the top num! top = "<<limit<<" now = "<<result);
                        }
                        else
                        {
                                response_map_param->insert(make_pair("reglular_rtn_content_conf", bconfig->m_regular_rtn_content_conf));
                                response_map_param->insert(make_pair("unregular_rtn_content_conf", bconfig->m_unregular_rtn_content_conf));
                                response_map_param->insert(make_pair("task_conf", bconfig->m_task_conf));
                                break;
                        }
                }

        }
        if(vconfigid_size == i) return -1;

        return 0;
}

int filter_config::do_update_configs(void)
{
        int result;
        std::string update_sql_1 = "select uniq_tag,control_identity,estimated_value from " + m_table_restrict_cond_config;
        string_map_vector reply_1;
        result = RW_detacher::do_mysql_opt_read(update_sql_1, reply_1);
        if(-1 == result)
        {
                MY_LOGERROR("mysql error! sql = " <<update_sql_1);
                return -1;
        }

        std::string update_sql_2 = "select uniq_tag,type,estimated_value from " + m_table_fixed_content_response_config;
        string_map_vector reply_2;
        result = RW_detacher::do_mysql_opt_read(update_sql_2,reply_2);
        if(-1 == result)
        {
                MY_LOGERROR("mysql error! sql = " <<update_sql_2);
                return -1;
        }


        std::string update_sql_3 = "select configid,UNIX_TIMESTAMP(valid_start_unixtime),regular_conf,unregular_conf, volumn, regular_rtn_content_conf, unregular_rtn_content_conf, task_conf from " + m_table_business_config;
        string_map_vector reply_3;
        result = RW_detacher::do_mysql_opt_read(update_sql_3, reply_3);
        if(-1 == result)
        {
                MY_LOGERROR("mysql error! sql = " <<update_sql_3);
                return -1;
        }

        std::string update_sql_4 = "select uniq_tag,control_identity,estimated_value from " + m_table_unrestrict_cond_config;
        string_map_vector reply_4;
        result = RW_detacher::do_mysql_opt_read(update_sql_4, reply_4);
        if(-1 == result)
        {
                MY_LOGERROR("mysql error! sql = " <<update_sql_4);
                return -1;
        }

        /*update to redis */
        int size = reply_4.size();
        for(int i = 0; i < size; ++i)
        {
                std::string uniq_tag = reply_4.at(i)["uniq_tag"];
                std::string control_identity = reply_4.at(i)["control_identity"];
                std::string estimated_value = reply_4.at(i)["estimated_value"];

                std::string key = REDIS_HEAD_FOR_UNRESTRICT_COND + uniq_tag +control_identity;
                std::string select_sql = "select data from business_uid_list where bizid='" + estimated_value + "'";
                string_map_vector reply_select;
                result = RW_detacher::do_mysql_opt_read(select_sql, reply_select);
                if(-1 == result)
                {
                        MY_LOGERROR("mysql error! sql = " <<select_sql);
                        return -1;
                }

                if(!reply_select.empty())
                {
                        int size1 = reply_select.size();
                        for(int i =0; i < size1; ++i)
                        {
                                std::string value = reply_select.at(i)["data"];
                                if(-1 == RW_detacher::do_redis_sadd(key,value))
                                {
                                        MY_LOGERROR("redis error! sadd = " <<key<<" "<<value);
                                        return -1;
                                }
                        }
                        if(-1 == RW_detacher::do_redis_expire(key, 86400))
                        {
                                MY_LOGERROR("redis error! expire = " <<key<<" 86400");
                                return -1;
                        }
                }
        }


        m_rwlock.wrlock();
        if(!m_unrestrict_cond_config.empty())
        {
                m_unrestrict_cond_config.clear();
        }
        if(!m_restrict_cond_config.empty())
        {
                m_restrict_cond_config.clear();
        }
        if(!m_fixed_content_config.empty())
        {
                m_fixed_content_config.clear();
        }
        if(!m_business_config.empty())
        {
                for(std::map< std::string, boost::shared_ptr<business_config> >::const_iterator eiter = m_business_config.begin(); eiter != m_business_config.end(); ++eiter)
                {
                        boost::shared_ptr<business_config> bconfig = eiter->second;
                        std::string key = REDIS_HEAD_FOR_BUSINESS + bconfig->m_valid_start_unixtime + bconfig->m_configid;

                        if(-1 == RW_detacher::do_redis_del(key))
                        {
                                MY_LOGERROR("redis error! del = " <<key);
                                m_rwlock.unlock();
                                return -1;
                        }
                }
                m_business_config.clear();
        }

        if(!reply_4.empty())
        {
                size = reply_4.size();
                for(int i = 0; i < size; ++i)
                {
                        boost::shared_ptr<unrestrict_cond_config > aconfig = boost::shared_ptr<unrestrict_cond_config >(new unrestrict_cond_config);
                        aconfig->m_uniq_tag = reply_1.at(i)["uniq_tag"];
                        aconfig->m_control_identity = reply_1.at(i)["control_identity"];
                        aconfig->m_estimated_value = reply_1.at(i)["estimated_value"];

                        m_unrestrict_cond_config.insert(make_pair(aconfig->m_uniq_tag, aconfig));
                }
        }

        if(!reply_1.empty())
        {
                size = reply_1.size();
                for(int i = 0; i < size; ++i)
                {
                        boost::shared_ptr<restrict_cond_config > aconfig = boost::shared_ptr<restrict_cond_config >(new restrict_cond_config);
                        aconfig->m_uniq_tag = reply_1.at(i)["uniq_tag"];
                        aconfig->m_control_identity = reply_1.at(i)["control_identity"];
                        aconfig->m_estimated_value = reply_1.at(i)["estimated_value"];

                        m_restrict_cond_config.insert(make_pair(aconfig->m_uniq_tag, aconfig));
                }
        }

        if(!reply_2.empty())
        {
                size = reply_2.size();
                for(int i = 0; i < size; ++i)
                {
                        boost::shared_ptr<fixed_content_response_config > aconfig = boost::shared_ptr<fixed_content_response_config >(new fixed_content_response_config);
                        aconfig->m_uniq_tag = reply_2.at(i)["uniq_tag"];
                        aconfig->m_type = reply_2.at(i)["type"];
                        aconfig->m_estimated_value = reply_2.at(i)["estimated_value"];

                        m_fixed_content_config.insert(make_pair(aconfig->m_uniq_tag, aconfig));
                }
        }

        if(!reply_3.empty())
        {
                size = reply_3.size();
                for(int i = 0; i < size; ++i)
                {
                        boost::shared_ptr<business_config > aconfig = boost::shared_ptr<business_config >(new business_config);
                        aconfig->m_configid = reply_3.at(i)["configid"];
                        aconfig->m_valid_start_unixtime = reply_3.at(i)["UNIX_TIMESTAMP(valid_start_unixtime)"];
                        aconfig->m_regular_conf = reply_3.at(i)["regular_conf"];
                        aconfig->m_unregular_conf = reply_3.at(i)["unregular_conf"];
                        aconfig->m_volumn = reply_3.at(i)["volumn"];
                        aconfig->m_regular_rtn_content_conf = reply_3.at(i)["regular_rtn_content_conf"];
                        aconfig->m_unregular_rtn_content_conf = reply_3.at(i)["unregular_rtn_content_conf"];
                        aconfig->m_task_conf = reply_3.at(i)["task_conf"];

                        m_business_config.insert(make_pair(aconfig->m_configid, aconfig));
                }
        }

        m_rwlock.unlock();



        return 0;
}


void filter_config::INIT_FILTER_CONFIG(void)
{
        const SDConfiguration& config = SDConfigurationSingleton::get_instance()->get_config();
        m_table_business_config = config.getString("table_business_config", " ");
        m_table_fixed_content_response_config = config.getString("table_fixed_content_response_config", " ");
        m_table_restrict_cond_config = config.getString("table_restrict_cond_config", " ");
        m_table_unrestrict_cond_config = config.getString("table_unrestrict_cond_config", " ");
        m_time_stamp = config.getInt("time_stamp", 600);

}


