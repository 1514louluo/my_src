#ifndef _RW_DETACHSER_H
#define _RW_DETACHSER_H
#include "SDConfigurationSingleton.h"
#include "mycommon.h"
#include "redis_pool.h"
#include "mysql_pool.h"
#include "cJSON.h"

//#include "mongodb_pool.h"
//#include "memcache_pool.h"
enum init_type
{
	RW_DETACHER_ALL=1,
	RW_DETACHER_ONLY_REDIS=2,
	RW_DETACHER_ONLY_MYSQL=3,
};
 
class RW_detacher
{
	public:
		RW_detacher();
		~RW_detacher();
		static int init_RW_detacher(int inittype);
/*for redis*/		
		static int do_redis_hget(const string_type &key,const string_type &field, string_type &value);
		static int do_redis_hgetall( const string_type & key, string_pair_vector & out );
		static int do_redis_hset( const string_type & key, const string_type & field, const string_type & value );
		static int do_redis_hdel( const string_type & key, const string_type &field );

		static int do_redis_expire(const string_type & key, unsigned int secs);

		static int do_redis_set(const string_type & key, const string_type & value);
		static int do_redis_setex(const string_type & key, const string_type & value, unsigned int secs);
		static int do_redis_get(const string_type & key, string_type &value);
		static int do_redis_del(const string_type & key);

		static int do_redis_incrby(const string_type &key, const string_type &value);

		static int do_redis_sadd(const string_type &key, const string_type &value);
		static int do_redis_sismember(const string_type &key, const string_type &value);
/*for redis*/	

/*for mysql*/
		static int do_mysql_opt_write(string_type &sql);
		static int do_mysql_opt_read(string_type &sql, string_map_vector &reply);
/*for mysql*/

	private:
/*for redis*/		
		static int init_RW_datacher_for_redis(void);
		static redis_client *get_redis_reader(void);
		static redis_client *get_redis_writer(void);
		static int reback_redis_reader(redis_client *pclient);
		static int reback_redis_writer(redis_client *pclient);
		
		static redis_pool m_redis_rpool;
		static redis_pool m_redis_wpool;
/*for redis*/		

/*for mysql*/
		static int init_RW_datacher_for_mysql(void);
		static CMysql *get_mysql_reader(void);
		static CMysql *get_mysql_writer(void);
		static int reback_mysql_reader(CMysql *pclient);
		static int reback_mysql_writer(CMysql *pclient);
		
		static mysql_pool m_mysql_rpool;
		static mysql_pool m_mysql_wpool;
/*for mysql*/
		DECL_LOGGER(logger);
};
#endif //_XLGAME_PROPERTY_SERVER_SRC_RWDETACH_RW_DETACHSER_H


