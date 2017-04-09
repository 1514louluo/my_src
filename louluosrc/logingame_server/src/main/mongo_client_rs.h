#pragma once

#include "mongo/client/dbclient.h"

#include <string>
#include <vector>

class mongo_client_rs
{
public:
	mongo_client_rs(void);
	~mongo_client_rs(void);

	int connect(std::string conn_string);

	int connect_cluster(std::string clustername, std::vector<mongo::HostAndPort> &vec_hostport);

	int insert(std::string db_collectionname, mongo::BSONObjBuilder &builder);

	int insert_explicit_id(std::string db_collectionname, std::string id_value, mongo::BSONObj &bsonobj);

	int findone(std::string db_collectionname, std::string field, std::string value, mongo::BSONObj &bsonobj);

	int query(std::string db_collectionname, mongo::BSONObjBuilder &condition_builder, 
				std::vector<mongo::BSONObj> &vec_result);

	int update(std::string db_collectionname, mongo::BSONObjBuilder &condition_builder,
				mongo::BSONObj &update_info);

	int close(std::string  db_collectionname);
	mongo::DBClientReplicaSet *m_connection;
private:

	std::string m_conn_string;
	
	//bool m_is_connected;


};
