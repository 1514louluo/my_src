#include "mongo_client_rs.h"


using namespace mongo;


mongo_client_rs::mongo_client_rs(void)
{
	m_connection = NULL;
	//m_is_connected = false;
}

mongo_client_rs::~mongo_client_rs(void)
{
}

int mongo_client_rs::connect( std::string conn_string )
{
	
	std::string errmsg;
	mongo::ConnectionString cs = mongo::ConnectionString::parse( conn_string , errmsg );
	if ( ! cs.isValid() ) 
	{
		printf("error parsing url: errmsg=%s\n",errmsg.c_str());
		//cout << "error parsing url: " << errmsg << endl;
		return -1;
	}
	m_connection = static_cast<DBClientReplicaSet*>( cs.connect( errmsg, true ? 10 : 0 ) );
	if (  m_connection == NULL ) 
	{
		
		printf("error connecting: errmsg=%s\n", errmsg.c_str());
		return -1;
	}

	

	return 0;
}


int mongo_client_rs::insert( std::string db_collectionname, mongo::BSONObjBuilder &builder )
{
	if (m_connection == NULL)
	{
		printf("m_connection == NULL, connection down\n");
		return -1;
	}
	try
	{
		m_connection->insert( db_collectionname , builder.obj() );
	}
	catch( std::exception& e ) 
	{
		cout << "ERROR: " << e.what() << endl;
		return -1;
	}
	
	return 0;

}

int mongo_client_rs::findone( std::string db_collectionname, std::string field, std::string value, mongo::BSONObj &bsonobj )
{

	if (m_connection == NULL)
	{
		printf("m_connection == NULL, connection down\n");
		return -1;
	}
	mongo::BSONObjBuilder query;
	query.append( field , value );
	try
	{
		bsonobj = m_connection->findOne(db_collectionname , query.obj() );

	}
	catch( std::exception& e ) 
	{
		cout << "ERROR: " << e.what() << endl;
		return -1;
	}
	

	return 0;


}

int mongo_client_rs::insert_explicit_id( std::string db_collectionname, std::string id_value, 
											mongo::BSONObj &bsonobj )
{
	if (m_connection == NULL)
	{
		printf("m_connection == NULL, connection down\n");
		return -1;
	}

	mongo::BSONObj after = mongo::BSONObjBuilder().append( "_id" , id_value ).appendElements( bsonobj ).obj();
	
	try
	{
		m_connection->insert( db_collectionname , after );
	}
	catch( std::exception& e ) 
	{
		cout << "ERROR: " << e.what() << endl;
		return -1;
	}
	
	return 0;

}

int mongo_client_rs::query( std::string db_collectionname, mongo::BSONObjBuilder &condition_builder, 
						   std::vector<mongo::BSONObj> &vec_result )
{
	if (m_connection == NULL)
	{
		printf("m_connection == NULL, connection down\n");
		return -1;
	}

	try
	{
		auto_ptr<mongo::DBClientCursor> cursor = m_connection->query(db_collectionname , condition_builder.obj());
		while (cursor->more())
		{
			mongo::BSONObj obj = cursor->next();
			vec_result.push_back(obj);
		}
		
		
	}
	catch( std::exception& e ) 
	{
		cout << "ERROR: " << e.what() << endl;
		return -1;
	}
	return 0;

}

int mongo_client_rs::close( std::string  db_collectionname)
{
	if (m_connection == NULL)
	{
		return 0;
	}
	try
	{
		mongo::BSONObj info;
		m_connection->logout(db_collectionname,info);

	}
	catch ( std::exception& e ) 
	{
		cout << "ERROR: " << e.what() << endl;
		return -1;
	}

	
	return 0;

}

int mongo_client_rs::connect_cluster( std::string clustername, std::vector<mongo::HostAndPort> &vec_hostport )
{

	m_connection = new mongo::DBClientReplicaSet(clustername,vec_hostport, 10);
	bool result = m_connection->connect();
	if (result == false)
	{
		return -1;
	}
	return 0;
}


int mongo_client_rs::update( std::string db_collectionname, mongo::BSONObjBuilder &condition_builder, mongo::BSONObj &update_info )
{

	if (m_connection == NULL)
	{
		return 0;
	}

	try
	{
		m_connection->update(db_collectionname, condition_builder.obj(), update_info);
	}
	catch ( std::exception& e ) 
	{
		cout << "ERROR: " << e.what() << endl;
		return -1;
	}

	return 0;

}


//mongo::BSONObjBuilder query;

//mongo::BSONObj res = conn.findOne( "test.people" , query.obj() );




//while ( cursor->more() ) {
//	mongo::BSONObj obj = cursor->next();
//	cout << "\t" << obj.jsonString() << endl;
//}
