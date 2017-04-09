#pragma once

#include "mycommon.h"

#pragma pack(1)  
typedef struct _STU_PROTOCOL_HEAD
{
	uint16_t version;
	uint16_t length;
}STU_PROTOCOL_HEAD;

#pragma pack()  


#include "SDLogger.h"

class protocol_codec
{
public:
	protocol_codec(void);
	~protocol_codec(void);
	int get_head_length(void);
	int get_body_length(char *pdata, int buffer_len);
	int get_body(char *pdata, int buffer_len, std::string &body);
	int encode_buffer(int version, std::string &content, std::string &str_buffer);
	int get_body_string(std::string &str_body, std::string &str_content);
	DECL_LOGGER(logger);
};
