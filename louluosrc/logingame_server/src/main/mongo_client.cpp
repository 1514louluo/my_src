#include "mongo_client.h"

#include "global_var.h"

using namespace mongo;

IMPL_LOGGER(mongo_client, logger);

mongo_client::mongo_client(void):m_conn(true)
{
}

mongo_client::~mongo_client(void)
{
}

int mongo_client::connect( std::string conn_string )
{
	m_string_connection=conn_string;
	bool result;
	std::string errmsg;
	result = m_conn.connect(conn_string,errmsg);
	if (result == false)
	{
		MONGODB_LOG("connection db failed, errmsg="<<errmsg.c_str());
		return -1;
	}
	else
	{	
		return 0;

	}

}

int mongo_client::insert( std::string db_collectionname, mongo::BSONObjBuilder &builder )
{
	if(validate_connection() == -1)
	{
		return -1;
	}
	
	
	try
	{
		m_conn.insert(db_collectionname, builder.obj(),0);
		std::string lasterr;
		lasterr = m_conn.getLastError();
		if (lasterr.empty())
		{
			return 0;
		}
		else
		{
			MONGODB_LOG("mongo_client::insert failed, lasterr="<<lasterr.c_str());
			return -1;
		}
	}
	catch( std::exception& e ) 
	{
		//cout << "ERROR: " << e.what() << endl;
		MONGODB_LOG("ERROR: "<<e.what());
		return -1;
	}
	return 0;
	

}

int mongo_client::insert_explicit_id( std::string db_collectionname, std::string id_value, mongo::BSONObj &bsonobj )
{
	if (validate_connection() == -1)
	{
		return -1;
	}
	mongo::BSONObj after = mongo::BSONObjBuilder().append("_id" , id_value).appendElements( bsonobj ).obj();
	try
	{
		m_conn.insert(db_collectionname, after);
		std::string lasterr;
		lasterr = m_conn.getLastError();
		if (lasterr.empty())
		{
			return 0;
		}
		else
		{
			MONGODB_LOG("mongo_client::insert_explicit_id failed, lasterr="<< lasterr.c_str());
			return -1;
		}

	}
	catch( std::exception& e ) 
	{
		MONGODB_LOG("ERROR: "<<e.what());
		return -1;
	}
	return 0;

}

int mongo_client::findone( std::string db_collectionname, std::string field, std::string value, 
						  mongo::BSONObj &bsonobj )
{
	if (validate_connection() == -1)
	{
		return -1;
	}
	//mongo::BSONObj querycondition = mongo::BSONObjBuilder().append(field, value).obj();
	mongo::BSONObjBuilder query;
	query.append( field , value );
	try
	{
		bsonobj = m_conn.findOne(db_collectionname, query.obj());
	}
	catch( std::exception& e ) 
	{
		MONGODB_LOG("ERROR: "<<e.what());
		return -1;
	}

	return 0;

}

int mongo_client::query( std::string db_collectionname, mongo::BSONObjBuilder &condition_builder, 
							std::vector<mongo::BSONObj> &vec_result )
{
	if (validate_connection() == -1)
	{
		return -1;
	}
	
	try
	{
		auto_ptr<mongo::DBClientCursor> cursor = m_conn.query(db_collectionname , condition_builder.obj());
		while (cursor->more())
		{
			mongo::BSONObj obj = cursor->next();
			vec_result.push_back(obj);
		}


	}
	catch( std::exception& e ) 
	{
		MONGODB_LOG("ERROR: "<<e.what());
		return -1;
	}
	return 0;

}

int mongo_client::update( std::string db_collectionname, mongo::BSONObjBuilder &condition_builder,
							mongo::BSONObj &update_info )
{
	if (validate_connection() == -1)
	{
		return -1;
	}

	try
	{
		m_conn.update(db_collectionname, condition_builder.obj(), update_info);
		std::string err = m_conn.getLastError();
		if (err.empty())
		{
			return 0;
		}
		else
		{
			MONGODB_LOG("mongo_client::update failed err="<< err.c_str());
			return -1;
		}
	}
	catch ( std::exception& e ) 
	{
		MONGODB_LOG("ERROR: "<<e.what());
		return -1;
	}

	return 0;


}

int mongo_client::close( std::string db_collectionname )
{
	if (validate_connection()==-1)
	{
		return -1;
	}

	try
	{
		mongo::BSONObj info;
		m_conn.logout(db_collectionname,info);
		return 0;

	}
	catch ( std::exception& e ) 
	{
		MONGODB_LOG("ERROR: "<<e.what());
		return -1;
	}
	

}

int mongo_client::remove( std::string db_collectionname, std::string key, std::string value )
{
	if(validate_connection() == -1)
	{
		return -1;
	}

	try
	{
		mongo::BSONObj conditionobj = mongo::BSONObjBuilder().append(key, value).obj();
		m_conn.remove(db_collectionname, conditionobj);
		std::string err;
		err = m_conn.getLastError();
		if (err.empty())
		{
			return 0;
		}
		else
		{
			MONGODB_LOG("remove err, err="<<err.c_str());
			return -1;
		}

	}
	catch ( std::exception& e ) 
	{
		MONGODB_LOG("ERROR: "<<e.what());
		return -1;
	}



}

int mongo_client::validate_connection( void )
{
	//m_conn.autoReconnect = true;
	if (m_conn.isFailed())
	{
		MONGODB_LOG("connection is failed, try to reconnect");
		int result;
		std::string errmsg;
		try
		{
			result = m_conn.connect(m_string_connection,errmsg);
			if (result == -1)
			{
				MY_LOGERROR("retry connect to mongos failed, errmsg="<<errmsg);
				sleep(1);
				return -1;
			}
			MONGODB_LOG("reconnect success");
			return 0;
		}
		catch ( std::exception& e ) 
		{
			MONGODB_LOG("ERROR: "<<e.what());
			sleep(1);
			return -1;
		}
		sleep(1);

		
		return -1;
	}
	return 0;

}




