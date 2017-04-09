#pragma once


#include "mongo/client/dbclient.h"

#include <string>
#include <vector>
#include "SDLogger.h"
#include "mycommon.h"
class mongo_client
{
public:
	mongo_client(void);
	~mongo_client(void);

	int connect(std::string conn_string);

	int insert(std::string db_collectionname, mongo::BSONObjBuilder &builder);

	int insert_explicit_id(std::string db_collectionname, std::string id_value, mongo::BSONObj &bsonobj);

	int findone(std::string db_collectionname, std::string field, std::string value, mongo::BSONObj &bsonobj);

	int query(std::string db_collectionname, mongo::BSONObjBuilder &condition_builder, 
		std::vector<mongo::BSONObj> &vec_result);

	// 注意：调用本update的时候，文档将被修改为update_info
	int update(std::string db_collectionname, mongo::BSONObjBuilder &condition_builder,
		mongo::BSONObj &update_info);

	int remove(std::string db_collectionname, std::string key, std::string value);

	int close(std::string  db_collectionname);

private:
	mongo::DBClientConnection m_conn;
	DECL_LOGGER(logger);
	int validate_connection(void);
	std::string m_string_connection;

};
