#ifndef _CSTRING_H
#define _CSTRING_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "atomic.h"
/*
 * advise that don't use in struct, if you want to use in struct, must be careful free yourself!
 *
 */

struct cstring
{
	long refcount;
	uint32_t strlen;
	char value[0];
};

#define GET_CSTR_LEN(pcstr) strlen(pcstr)
#define CSTRING_SIZE          sizeof(cstring)
#define _shared_ptr_ __attribute__((cleanup(cstring_free)))

typedef struct cstring cstring;
#define string _shared_ptr_ cstring *

static inline void cstring_free(void* pcstring)
{
	void** paddress = (void**) pcstring;

	cstring *p = (cstring *)(*paddress);

	int curref = atomic_fetch_sub_long(&p->refcount, 1);

	if(1 == curref)
	{
		printf("%p\tcurref = %lu\n", p, p->refcount);
		printf("%s\n", p->value);
		free(p);
	}	
}

/*
 * C function reload!
 *
 * */
cstring *new_string(const char *pcstr, uint32_t index, uint32_t len) 
{
	if(NULL == pcstr) return NULL;
	if(index > 0xfffffff || len > 0xfffffff) return NULL;

	string p = (cstring *)malloc(CSTRING_SIZE + len + 1);
	p->refcount = 2;	
	p->strlen = len + 1;
	memcpy(p->value, pcstr+index, len); 
	p->value[len] = '\0';
	return p;
}

char at(cstring *p, int pos)
{
	if(NULL == p || pos < 0 || pos >= p->strlen)
	{
		return '\0';
	}
	return p->value[pos];
}

int empty(cstring *p)
{
	return (p->strlen > 1?1:0);
}


uint32_t length(cstring *p)
{
	return (p->strlen - 1);
}

cstring *substr(cstring *p, uint32_t pos, uint32_t num)
{
	string newp = new_string(p->value, pos, num);

	int curref = atomic_fetch_add_long(&newp->refcount, 1);

	if(1 >  curref)
	{
		free(p);
		return NULL;
	}

	return newp;
}

cstring *append(cstring *p, cstring *ip)
{
	uint32_t newstrlen = p->strlen + ip->strlen - 1;
	string newp = (cstring *)malloc(CSTRING_SIZE + newstrlen);
	newp->refcount = 2;	
	newp->strlen = newstrlen;
	memcpy(newp->value, p->value, p->strlen - 1); 
	memcpy(newp->value + p->strlen -1, ip->value, ip->strlen - 1); 
	newp->value[newstrlen] = '\0';
	return newp;
}

cstring *append_realloc(cstring *p, cstring *ip)
{
	uint32_t plen = p->strlen;
	uint32_t iplen = ip->strlen;
	p = (cstring *)realloc(p, CSTRING_SIZE + plen + iplen - 1);
	if(NULL == p) return NULL;
	p->refcount = 1;	
	p->strlen = plen + iplen -1;
	memcpy(p->value + plen -1, ip->value, iplen - 1); 
	p->value[p->strlen] = '\0';
	return p;
}

char *c_str(cstring *p)
{
	if(NULL == p) return NULL;
	return p->value;
}

#endif //_CSTRING_H
