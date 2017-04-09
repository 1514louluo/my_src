#include "protocol_codec.h"

IMPL_LOGGER(protocol_codec, logger);

protocol_codec::protocol_codec(void)
{
}

protocol_codec::~protocol_codec(void)
{
}

int protocol_codec::get_head_length( void )
{
	return sizeof(STU_PROTOCOL_HEAD);

}

int protocol_codec::get_body_length( char *pdata, int buffer_len )
{
	if (sizeof(STU_PROTOCOL_HEAD) > buffer_len)
	{
		MY_LOGERROR("sizeof(STU_PROTOCOL_HEAD) > buffer_len sizeof(STU_PROTOCOL_HEAD)="<<sizeof(STU_PROTOCOL_HEAD)<<"  buffer_len="<<buffer_len);
		return -1;
	}
	STU_PROTOCOL_HEAD *pstu_head = (STU_PROTOCOL_HEAD *)pdata;
	uint16_t length = pstu_head->length;
	
	//MY_LOGDEBUG("get body length, length="<<length);
	hton16(&length);

	//MY_LOGDEBUG("get body length, length="<<length);
	if (length >= 5000)
	{
		return -1;
	}
	if (length == 0)
	{
		return -1;
	}
	return length;

}

int protocol_codec::get_body( char *pdata, int buffer_len, std::string &body )
{
	int body_length;
	body_length = get_body_length(pdata, buffer_len);
	if (body_length == -1)
	{
		return -1;
	}
	if ( buffer_len != body_length+get_head_length() )
	{
		MY_LOGERROR("buffer_len != body_length+get_head_length() ");
		return -1;
	}
	body.append(pdata+get_head_length(), body_length);
	return 0;

}

int protocol_codec::encode_buffer( int version, std::string &content, std::string &str_buffer )
{
	STU_PROTOCOL_HEAD stu_head;
	stu_head.version = version;
	uint16_t length = content.length();
	stu_head.length = length;

	hton16(&(stu_head.version));
	hton16(&(stu_head.length));

	//uint32_t bodylength = content.length();
	//hton32(&bodylength);


	str_buffer.append((char *)&stu_head, sizeof(stu_head));
	//str_buffer.append((char *)&bodylength, sizeof(bodylength));
	str_buffer=str_buffer+content;
	return 0;

}

int protocol_codec::get_body_string( std::string &str_body, std::string &str_content )
{
	if (str_body.size() <=4)
	{
		return -1;
	}
	uint32_t *pcontent_length;
	pcontent_length = (uint32_t *)str_body.c_str();
	uint32_t content_length = *pcontent_length;
	hton32(&content_length);
	if (content_length + 4 != str_body.size())
	{
		std::string printstr;
		printstring2str(str_body, printstr);
		MY_LOGERROR("body content error, buffer= "<<printstr);
		return -1;
	}
	str_content = str_body.substr(4);
	return 0;



}
