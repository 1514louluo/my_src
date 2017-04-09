/*
*	protocol: just same as redis! "*3\r\n$3\r\nSET\r\n$5\r\nmykey\r\n$7\r\nmyvalue\r\n"
*	Copyright (C) 2014 louluo  <18915413902@163.com>
*/
#ifndef _PROTOCOL_H
#define _PROTOCOL_H
#include "coding.h"

#define KVCACHE_CMD_SET             "SET"
#define KVCACHE_CMD_GET             "GET"
#define KVCACHE_CMD_DEL              "DEL"
#define KVCACHE_CMD_TTL               "TTL"
#define KVCACHE_CMD_EXPIRE         "EXPIRE"

typedef struct kvcache_opt
{
	char  num;
	char *argv[0];
}kvcache_opt;

/*jump pointer after '\r\n' */
static inline const char *enter_jump(const char *in)
{
	if('\r' != *in || '\n' != *(in+1)) return NULL;
	return (in + 2);
}

int protocol_parse(const char *in, uint32_t len)
{
	const char *base = in;
	while(in - base < len)
	{
		if('*' == *in)
		{
			uint32_t  argc;
			in = get_varint32_data_ptr(in+1, in+5, &argc);
			if(!argc) return -1;
			kvcache_opt  *kv_opt = (kvcache_opt *)P_MALLOC(sizeof(kvcache_opt) + argc * sizeof(kvcache_opt *));
			if(NULL == kv_opt) return -1;

			in = enter_jump(in);
			int i = 0;
			for(; i < argc; ++i)
			{
				if('$' == *in)
				{
					uint32_t argv_len;
					in = get_varint32_data_ptr(in+1, in+5, &argv_len);
					if(!argv_len) return -1;
					
					in = enter_jump(in);
					kv_opt->argv[i] = (char *)P_MALLOC(argv_len);
					memcpy(kv_opt->argv[i], in, argv_len);
					in = enter_jump(in);
				}
			}
		}
		// here  do something!
	}
}
#endif //_PROTOCOL_H

