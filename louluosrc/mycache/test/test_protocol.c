#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct kvcache_opt
{
	char  num;
	char *argv[0];
}kvcache_opt;
static inline const char *enter_jump(const char *in)
{
	if('\r' != *in || '\n' != *(in+1)) return NULL;
	return (in + 2);
}

const char *get_varint32_data_ptr(const char *in, const char *limit, uint32_t *value)
{
	*value = 0;
	while('\r' != *in)
	{
		*value *= 10;
		int inc = *in-'0';
		*value += inc;
		in++;
	}
	return in;
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
			printf("a cmd is comming num = %u: ", argc);
			kvcache_opt  *kv_opt = (kvcache_opt *)malloc(sizeof(kvcache_opt) + argc * sizeof(kvcache_opt *));
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
					kv_opt->argv[i] = (char *)malloc(argv_len + 1);
					*(kv_opt->argv[i] + argv_len) = '\0';
					memcpy(kv_opt->argv[i], in, argv_len);
					in += argv_len;
					in = enter_jump(in);
					printf("%s\t", kv_opt->argv[i]);
				}
			}
			printf("\n");
		}
	}
	return 0;
}

int main()
{
	const char *in = "*3\r\n$3\r\nSET\r\n$5\r\nmykey\r\n$7\r\nmyvalue\r\n*2\r\n$3\r\nSET\r\n$5\r\nmykey\r\n*3\r\n$3\r\nSET\r\n$5\r\nmykey\r\n$7\r\nmyvalue\r\n";
	protocol_parse(in, strlen(in));
	
	return 0;
}
