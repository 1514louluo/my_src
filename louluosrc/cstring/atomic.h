#ifndef _ATOMIC_H
#define _ATOMIC_H
#include <stdint.h>

#define atomic_add_fetch_long(p, v)  _atomic_add_fetch_long(p, v)
#define atomic_fetch_add_long(p, v)  _atomic_fetch_add_long(p, v)
#define atomic_sub_fetch_long(p, v)  _atomic_sub_fetch_long(p, v)
#define atomic_fetch_sub_long(p, v)  _atomic_fetch_sub_long(p, v)


#define atomic_add_fetch_ulong(p, v)  _atomic_add_fetch_ulong(p, v)
#define atomic_fetch_add_ulong(p, v)  _atomic_fetch_add_ulong(p, v)
#define atomic_sub_fetch_ulong(p, v)  _atomic_sub_fetch_ulong(p, v)
#define atomic_fetch_sub_ulong(p, v)  _atomic_fetch_sub_ulong(p, v)

#define atomic_add_fetch_uint64(p, v)  _atomic_add_fetch_uint64(p, v)
#define atomic_add_fetch_uint32(p, v)  _atomic_add_fetch_uint32(p, v)
#define atomic_add_fetch_uint16(p, v)  _atomic_add_fetch_uint16(p, v)
#define atomic_add_fetch_uint8(p, v)   _atomic_add_fetch_uint8(p, v)

#define atomic_fetch_add_uint64(p, v)  _atomic_fetch_add_uint64(p, v)
#define atomic_fetch_add_uint32(p, v)  _atomic_fetch_add_uint32(p, v)
#define atomic_fetch_add_uint16(p, v)  _atomic_fetch_add_uint16(p, v)
#define atomic_fetch_add_uint8(p, v)   _atomic_fetch_add_uint8(p, v)


#define atomic_sub_fetch_uint64(p, v)  _atomic_sub_fetch_uint64(p, v)
#define atomic_sub_fetch_uint32(p, v)  _atomic_sub_fetch_uint32(p, v)
#define atomic_sub_fetch_uint16(p, v)  _atomic_sub_fetch_uint16(p, v)
#define atomic_sub_fetch_uint8(p, v)   _atomic_sub_fetch_uint8(p, v)

#define atomic_fetch_sub_uint64(p, v)  _atomic_fetch_sub_uint64(p, v)
#define atomic_fetch_sub_uint32(p, v)  _atomic_fetch_sub_uint32(p, v)
#define atomic_fetch_sub_uint16(p, v)  _atomic_fetch_sub_uint16(p, v)
#define atomic_fetch_sub_uint8(p, v)   _atomic_fetch_sub_uint8(p, v)

static inline long _atomic_add_fetch_long(long *p, long v)
{
        return (__sync_add_and_fetch(p, v));
}

static inline long _atomic_fetch_add_long(long *p, long v)
{
        return (__sync_fetch_and_add(p, v));
}

static inline long _atomic_fetch_sub_long(long *p, long v)
{
        return (__sync_fetch_and_sub(p, v));
}              

static inline long _atomic_sub_fetch_long(long *p, long v)
{
        return (__sync_sub_and_fetch(p, v));
}

static inline unsigned long _atomic_add_fetch_ulong(unsigned long *p, unsigned long v)
{
	return (__sync_add_and_fetch(p, v));
}

static inline unsigned long _atomic_fetch_add_ulong(unsigned long *p, unsigned long v)
{
	return (__sync_fetch_and_add(p, v));
}

static inline unsigned long _atomic_fetch_sub_ulong(unsigned long *p, unsigned long v)
{
	return (__sync_fetch_and_sub(p, v));
}

static inline unsigned long _atomic_sub_fetch_ulong(unsigned long *p, unsigned long v)
{
	return (__sync_sub_and_fetch(p, v));
}

static inline uint64_t _atomic_add_fetch_uint64(uint64_t *p, uint64_t v)
{
	return (__sync_add_and_fetch(p, v));
}
static inline uint32_t _atomic_add_fetch_uint32(uint32_t *p, uint32_t v)
{
	return (__sync_add_and_fetch(p, v));
}
static inline uint16_t _atomic_add_fetch_uint16(uint16_t *p, uint16_t v)
{
	return (__sync_add_and_fetch(p, v));
}
static inline uint8_t _atomic_add_fetch_uint8(uint8_t *p, uint8_t v)
{
	return (__sync_add_and_fetch(p, v));
}

static inline uint64_t _atomic_fetch_add_uint64(uint64_t *p, uint64_t v)
{
	return (__sync_fetch_and_add(p, v));
}
static inline uint32_t _atomic_fetch_add_uint32(uint32_t *p, uint32_t v)
{
	return (__sync_fetch_and_add(p, v));
}
static inline uint16_t _atomic_fetch_add_uint16(uint16_t *p, uint16_t v)
{
	return (__sync_fetch_and_add(p, v));
}
static inline uint8_t _atomic_fetch_add_uint8(uint8_t *p, uint8_t v)
{
	return (__sync_fetch_and_add(p, v));
}

static inline uint64_t _atomic_sub_fetch_uint64(uint64_t *p, uint64_t v)
{
	return (__sync_sub_and_fetch(p, v));
}
static inline uint32_t _atomic_sub_fetch_uint32(uint32_t *p, uint32_t v)
{
	return (__sync_sub_and_fetch(p, v));
}
static inline uint16_t _atomic_sub_fetch_uint16(uint16_t *p, uint16_t v)
{
	return (__sync_sub_and_fetch(p, v));
}
static inline uint8_t _atomic_sub_fetch_uint8(uint8_t *p, uint8_t v)
{
	return (__sync_sub_and_fetch(p, v));
}

static inline uint64_t _atomic_fetch_sub_uint64(uint64_t *p, uint64_t v)
{
	return (__sync_fetch_and_sub(p, v));
}
static inline uint32_t _atomic_fetch_sub_uint32(uint32_t *p, uint32_t v)
{
	return (__sync_fetch_and_sub(p, v));
}
static inline uint16_t _atomic_fetch_sub_uint16(uint16_t *p, uint16_t v)
{
	return (__sync_fetch_and_sub(p, v));
}
static inline uint8_t _atomic_fetch_sub_uint8(uint8_t *p, uint8_t v)
{
	return (__sync_fetch_and_sub(p, v));
}

#endif //_ATOMIC_H

