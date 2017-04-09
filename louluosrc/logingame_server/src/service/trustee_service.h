#pragma once


#include "cmysql.h"

#include "SDLogger.h"
#include "mycommon.h"
#include <list>
#include "mysql_guard.h"
#include "block_queue.h"

#include "commondef.h"
#include "SDUtility.h"
 
class trustee_service
{
public:
	trustee_service(CMysql *pmysql);
	~trustee_service(void);

	//查询自增长id对应的记录
	int get_trusteeinfo_by_id(uint64_t sequenceid, STU_TRUSTEE_INFO *ptrustee_info);

	int remove_record_by_id(uint64_t sequenceid);

	int update_trustee_info(uint64_t sequenceid, std::string &str_masteralias, std::string &str_slaveralias,
							std::string &str_gameid, std::string &str_serverid);

	int query_slaves(std::string str_master_gameinternalno, std::string str_gameid, std::string str_serverid, std::vector<STU_TRUSTEE_INFO> &vec);

	int add_trustee(STU_TRUSTEE_INFO *pstutrustee);

	// 根据小号的gameinternalno，查询大号的所有信息
	int query_masters(std::string str_slave_gameinternalno,  std::string gameid,
						std::string serverid, std::vector<STU_TRUSTEE_INFO> &vec);

	int query_slavers_count(std::string gameinternalno, int &slavecount);

	int query_record_by_seqid(STU_TRUSTEE_INFO *pstu,uint64_t seqid);

	int updateSlaverIdcardno(std::string &gameinternalno, std::string &cardno);
private:
	CMysql *m_pmysql;
	DECL_LOGGER(logger);
	
};
