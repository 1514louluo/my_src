#ifndef _GENERAL_LOG_H
#define _GENERAL_LOG_H
#include <sys/time.h>
#include "msg_codec.h"
#include "mycommon.h"
#include "Singleton.h"
#include "SDLogger.h"
#include "zmq_client_pub.h"
#include "SDConfigurationSingleton.h"
#include "cJSON.h"

enum DB
{
        LOG_MYSQL=1,
        LOG_MONGO=2,
};

enum CACHE
{
        LOG_REDIS=3,
        LOG_MEMCACHE=4,
};

#define LOG_SERVER_3RD       5
enum OPT_TYPE
{
        OPT_NONE=0,
        OPT_WRITE=1,
        OPT_READ=2,
};

#define GENERAL_OPT_MYSQL_WRITE(log, loginfo, rttype, func,  ...)                                                             \
    ({                                          															\
    rttype ret;                                          														\
    struct timeval tv_s;                         														\
    struct timeval tv_e;                          														\
    gettimeofday(&tv_s, NULL);                   														\
    ret =        func(__VA_ARGS__);                       												\
    gettimeofday(&tv_e, NULL);                    													\
    long cost = (tv_e.tv_sec - tv_s.tv_sec)*1000000 + tv_e.tv_usec - tv_s.tv_usec;    					\
    log.attach_http_other_type_log(LOG_MYSQL, OPT_WRITE, loginfo, cost);                                                                                   \
    ret;                                                     														\
   })

#define GENERAL_OPT_MYSQL_READ(log, loginfo, rttype, func, ...)                                                              \
    ({                                          															\
    rttype ret;                                          														\
    struct timeval tv_s;                         														\
    struct timeval tv_e;                          														\
    gettimeofday(&tv_s, NULL);                   														\
    ret =        func(__VA_ARGS__);                       												\
    gettimeofday(&tv_e, NULL);                    													\
    long cost = (tv_e.tv_sec - tv_s.tv_sec)*1000000 + tv_e.tv_usec - tv_s.tv_usec;    					\
    log.attach_http_other_type_log(LOG_MYSQL, OPT_READ, loginfo, cost);                                                                                   \
    ret;                                                     														\
   })

#define GENERAL_OPT_REDIS_WRITE(log, loginfo, rttype, func, ...)                                                              \
    ({                                          															\
    rttype ret;                                          														\
    struct timeval tv_s;                         														\
    struct timeval tv_e;                          														\
    gettimeofday(&tv_s, NULL);                   														\
    ret =        func(__VA_ARGS__);                       												\
    gettimeofday(&tv_e, NULL);                    													\
    long cost = (tv_e.tv_sec - tv_s.tv_sec)*1000000 + tv_e.tv_usec - tv_s.tv_usec;    					\
    log.attach_http_other_type_log(LOG_REDIS,OPT_WRITE, loginfo, cost);                                                                                   \
    ret;                                                     														\
   })

#define GENERAL_OPT_REDIS_READ(log, loginfo, rttype, func, ...)                                                              \
    ({                                          															\
    rttype ret;                                          														\
    struct timeval tv_s;                         														\
    struct timeval tv_e;                          														\
    gettimeofday(&tv_s, NULL);                   														\
    ret =        func(__VA_ARGS__);                       												\
    gettimeofday(&tv_e, NULL);                    													\
    long cost = (tv_e.tv_sec - tv_s.tv_sec)*1000000 + tv_e.tv_usec - tv_s.tv_usec;    					\
    log.attach_http_other_type_log(LOG_REDIS, OPT_READ, loginfo, cost);                                                                                   \
    ret;                                                     														\
   })

#define GENERAL_OPT_MONGO_WRITE(log, loginfo, rttype, func, ...)                                                              \
    ({                                          															\
    rttype ret;                                          														\
    struct timeval tv_s;                         														\
    struct timeval tv_e;                          														\
    gettimeofday(&tv_s, NULL);                   														\
    ret =        func(__VA_ARGS__);                       												\
    gettimeofday(&tv_e, NULL);                    													\
    long cost = (tv_e.tv_sec - tv_s.tv_sec)*1000000 + tv_e.tv_usec - tv_s.tv_usec;    					\
    log.attach_http_other_type_log(LOG_MONGO,OPT_WRITE, loginfo, cost);                                                                                   \
    ret;                                                     														\
   })

#define GENERAL_OPT_MONGO_READ(log, loginfo, rttype, func, ...)                                                              \
    ({                                          															\
    rttype ret;                                          														\
    struct timeval tv_s;                         														\
    struct timeval tv_e;                          														\
    gettimeofday(&tv_s, NULL);                   														\
    ret =        func(__VA_ARGS__);                       												\
    gettimeofday(&tv_e, NULL);                    													\
    long cost = (tv_e.tv_sec - tv_s.tv_sec)*1000000 + tv_e.tv_usec - tv_s.tv_usec;    					\
    log.attach_http_other_type_log(LOG_MONGO, OPT_READ, loginfo, cost);                                                                                   \
    ret;                                                     														\
   })

#define GENERAL_OPT_MEMCACHE_WRITE(log, loginfo, rttype, func, ...)                                                              \
    ({                                          															\
    rttype ret;                                          														\
    struct timeval tv_s;                         														\
    struct timeval tv_e;                          														\
    gettimeofday(&tv_s, NULL);                   														\
    ret =        func(__VA_ARGS__);                       												\
    gettimeofday(&tv_e, NULL);                    													\
    long cost = (tv_e.tv_sec - tv_s.tv_sec)*1000000 + tv_e.tv_usec - tv_s.tv_usec;    					\
    log.attach_http_other_type_log(LOG_MEMCACHE, OPT_WRITE, loginfo, cost);                                                                                   \
    ret;                                                     														\
   })

#define GENERAL_OPT_MEMCACHE_READ(log, loginfo, rttype, func, ...)                                                              \
    ({                                          															\
    rttype ret;                                          														\
    struct timeval tv_s;                         														\
    struct timeval tv_e;                          														\
    gettimeofday(&tv_s, NULL);                   														\
    ret =        func(__VA_ARGS__);                       												\
    gettimeofday(&tv_e, NULL);                    													\
    long cost = (tv_e.tv_sec - tv_s.tv_sec)*1000000 + tv_e.tv_usec - tv_s.tv_usec;    					\
    log.attach_http_other_type_log(LOG_MEMCACHE, OPT_READ, loginfo, cost);                                                                                   \
    ret;                                                     														\
   })

#define GENERAL_OPT_3RD_SERVICE_WRITE(log, loginfo, rttype, func, ...)                                                              \
    ({                                          															\
    rttype ret;                                          														\
    struct timeval tv_s;                         														\
    struct timeval tv_e;                          														\
    gettimeofday(&tv_s, NULL);                   														\
    ret =        func(__VA_ARGS__);                       												\
    gettimeofday(&tv_e, NULL);                    													\
    long cost = (tv_e.tv_sec - tv_s.tv_sec)*1000000 + tv_e.tv_usec - tv_s.tv_usec;    					\
    log.attach_http_other_type_log(LOG_SERVER_3RD, OPT_WRITE, loginfo, cost);                                                                                   \
    ret;                                                     														\
   })


#define GENERAL_OPT_3RD_SERVICE_READ(log, loginfo, rttype, func, ...)                                                              \
    ({                                          															\
    rttype ret;                                          														\
    struct timeval tv_s;                         														\
    struct timeval tv_e;                          														\
    gettimeofday(&tv_s, NULL);                   														\
    ret =        func(__VA_ARGS__);                       												\
    gettimeofday(&tv_e, NULL);                    													\
    long cost = (tv_e.tv_sec - tv_s.tv_sec)*1000000 + tv_e.tv_usec - tv_s.tv_usec;    					\
    log.attach_http_other_type_log(LOG_SERVER_3RD, OPT_READ, loginfo, cost);                                                                                   \
    ret;                                                     														\
   })

class general_log
{
public:
        general_log();
        ~general_log();

        void set_log_tag(std::string service_name);

        void attach_http_business_log(std::map<std::string, std::string> &input, std::map<std::string, std::string> &output);
        void attach_http_other_type_log(int type,  int opt_type,  std::string info, long cost_time);/*ОўГо*/
        void attach_http_main_log(std::string cmd_path, std::string start_time, std::string process_time, std::string result);

        void set_stat_log_tag(std::string service_name);
        void attach_stat_log(std::string key, std::string value);

        void produce_http_log(void);
        void produce_stat_log(void);

        static void INIT_GENERAL_LOG(void);
        static void zmq_pub_func(std::string &);
private:

        std::string m_http_business_log;
        std::string m_http_db_log;
        std::string m_http_cache_log;
        std::string m_http_3rd_service_log;
        std::string m_http_main_log;

        std::string m_stat_log;

        std::map<std::string, std::string> m_http_main_log_map;
        std::map<std::string, std::string> m_stat_log_map;

        static std::string m_zmq_log_destination;
        static std::string m_server_name;
        static std::string	m_server_ip;
        static std::string m_server_port;

        DECL_LOGGER(logger);
};
#endif //_GENERAL_LOG_H








