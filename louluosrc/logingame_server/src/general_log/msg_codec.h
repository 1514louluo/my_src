#ifndef 	_MSG_CODEC_H_
#define 	_MSG_CODEC_H_

#pragma once
#include <map>
#include <string>
#include "SDLogger.h"
#include "cJSON.h"

class msg_codec
{
public:
	msg_codec(void);
	~msg_codec(void);
	
	bool setData(const std::string  &sKey,  const std::string  &sVal);
	bool setData(const char  *sKey,  const std::string  &sVal);
	std::string getSendData(void);
	
private:

	bool encode_info();
	bool insert_map(const std::string& sKey,  const std::string& sVal);

private:

	//send data
	std::map<std::string, std::string> m_data_map;
	std::string m_send_data;

	DECL_LOGGER(logger);
};

#endif 

