#ifndef _KVCACHE_H
#define _KVCACHE_H
#include "cdef.h"
#include "hash.h"

/*
*  	 set nmykey nmyvalue =>	"*3\r\n$3\r\nSET\r\n$5\r\nmykey\r\n$7\r\nmyvalue\r\n"
*/

#define KVCACHE_CMD_SET             "SET"
#define KVCACHE_CMD_GET             "GET"
#define KVCACHE_CMD_DEL              "DEL"
#define KVCACHE_CMD_TTL               "TTL"
#define KVCACHE_CMD_EXPIRE         "EXPIRE"

#define MAX_AREA_SIZE              (1024<<5)
typedef void (*operate_func)();

typedef struct cache_ackn kvcache_ackn;
typedef struct cache_node kvcache_node;
typedef struct cache_head kvcache_head;

/*size*/
#define  MAX_PRIORITY   ULONG_MAX

#define KVCACHE_HEAD_SIZE sizeof(kvcache_head)
#define KVCACHE_HEAD_PSIZE sizeof(kvcache_head *)

#define KVCACHE_NODE_SIZE sizeof(kvcache_node)
#define KVCACHE_NODE_PSIZE sizeof(kvcache_node *)

/*mem*/
#define NIL NULL
#define NEW_KVCACHE_NODE_NODE(level)       (kvcache_node *)P_MALLOC(KVCACHE_NODE_SIZE + (level) * KVCACHE_NODE_PSIZE)


/*level*/
#define MAX_SK_LEVEL                                            16
#define MAX_SK_LEVEL_INDEX                                (MAX_SK_LEVEL-1)

struct cache_ackn
{
        ulong_t 		   ack_hash_1;
        ulong_t 		   ack_hash_2;
} ;

struct cache_node
{
	/*data area*/
	char *key;
	char *value;
	
	ulong_t klen;	
	ulong_t vlen;	
	
	/*info area*/
	char 			type;

	/*time info*/
	long_t                   ttl;
	long_t                   tms;
	
	/*ptr area*/
	 ulong_t  		            priority;
        kvcache_ackn     	     ack;
        kvcache_node            *elem[0];
//}__attribute__((aligned(sizeof(long_t))));
};

struct cache_head
{
	 int  				  level;
        kvcache_node 	*head;
};

#ifdef __cplusplus
extern "C" {
#endif

int kvcache_init(void);
int kvcache_set(const char *key, const char *value,  ulong_t klen, ulong_t vlen, long_t ttl);
char *kvcache_get(const char *key, ulong_t klen);
int kvcache_del(const char *key, ulong_t klen);
int kvcache_ttl(const char *key, ulong_t klen);
int kvcache_expire(const char *key, ulong_t klen, long_t ttl);
void kvcache_free(void);


#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_KVCACHE_H

