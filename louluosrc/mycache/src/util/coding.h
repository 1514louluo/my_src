#ifndef _CODING_H_
#define _CODING_H_

#include <string.h>
#include <stdio.h>

typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef int int32_t;
typedef long long int64_t;
typedef unsigned char uchar_t;
static int little_endian;


void _bm_getmask(int *pos, uchar_t *bmask)
{
        *bmask = (uchar_t)(1 << (*pos % 8));
        *pos /= 8;
}

inline int bm_isset(uchar_t * bm, int pos)
{
        uchar_t   bmask;

        _bm_getmask(&pos, &bmask);
        return !!(bm[pos] & bmask);
}
void bm_foreach(uchar_t *bmap, int size)
{
        int i=size-1;
        for(; i >=0 ; --i)
                printf("%d", bm_isset(bmap, i));
        printf("\n");
}

/*
*	judge whether little_endian?
*/

void is_little_endian(void)
{
        int i = 1;
        char *test = (char *)&i;
        if(*test)  little_endian = 1;
        else little_endian = 0;
}

/*
*	little endian
*/
void  encode_little_endian32(char *buf, uint32_t  value)
{
        if(little_endian)
        {
                memcpy(buf, &value, sizeof(value));
        }
        else
        {
                buf[0] = value & 0xff;
                buf[1] = (value >> 8) & 0xff;
                buf[2] = (value >> 16) & 0xff;
                buf[3] = (value >> 24) & 0xff;
        }
}

void  encode_little_endian64(char *buf, uint64_t  value)
{
        if(little_endian)
        {
                memcpy(buf, &value, sizeof(value));
        }
        else
        {
                buf[0] = value & 0xff;
                buf[1] = (value >> 8) & 0xff;
                buf[2] = (value >> 16) & 0xff;
                buf[3] = (value >> 24) & 0xff;
                buf[4] = (value >> 32) & 0xff;
                buf[5] = (value >> 40) & 0xff;
                buf[6] = (value >> 48) & 0xff;
                buf[7] = (value >> 56) & 0xff;
        }
}

inline uint32_t decode_little_endian32(const char* ptr)  //将存储的4字节 缓存数组 编程32位小端数字
{
        if(little_endian)
        {
                // Load the raw bytes
                uint32_t result;
                memcpy(&result, ptr, sizeof(result));  // gcc optimizes this to a plain load
                return result;
        }
        else
        {
                return (((uint32_t)((unsigned char)(ptr[0])))
                        | ((uint32_t)((unsigned char)(ptr[1])) << 8)
                        | ((uint32_t)((unsigned char)(ptr[2])) << 16)
                        | ((uint32_t)((unsigned char)(ptr[3])) << 24));
        }
}

inline uint64_t decode_little_endian64(const char* ptr)//将存储的8字节 缓存数组 编程64位小端数字
{
        if(little_endian)
        {
                // Load the raw bytes
                uint64_t result;
                memcpy(&result, ptr, sizeof(result));  // gcc optimizes this to a plain load
                return result;
        }
        else
        {
                uint64_t lo = decode_little_endian32(ptr);
                uint64_t hi = decode_little_endian32(ptr + 4);
                return (hi << 32) | lo;
        }
}

/*
*	varint, 返回的ptr - dst 为encode后的字节长度
*/

char* encode_varint32(char* dst, uint32_t v) // 将32位数字encode位varint
{
        // Operate on characters as unsigneds
        unsigned char* ptr = (unsigned char*)(dst); // 转化指针类型
        static const int B = 128;
        if(v < (1<<7))
        {
                *(ptr++) = v;
        }
        else if(v < (1<<14))
        {
                *(ptr++) = v | B;
                *(ptr++) = v>>7;
        }
        else if(v < (1<<21))
        {
                *(ptr++) = v | B;
                *(ptr++) = (v>>7) | B;
                *(ptr++) = v>>14;
        }
        else if(v < (1<<28))
        {
                *(ptr++) = v | B;
                *(ptr++) = (v>>7) | B;
                *(ptr++) = (v>>14) | B;
                *(ptr++) = v>>21;
        }
        else
        {
                *(ptr++) = v | B;
                *(ptr++) = (v>>7) | B;
                *(ptr++) = (v>>14) | B;
                *(ptr++) = (v>>21) | B;
                *(ptr++) = v>>28;
        }
        return (char *)(ptr);
}

//varint, 返回的ptr - dst 为encode后的字节长度
char* encode_varint64(char* dst, uint64_t v) // 将64位数字encode位varint
{
        static const int B = 128;
        unsigned char* ptr = (unsigned char*)(dst);
        while(v >= B)
        {
                *(ptr++) = (v & (B-1)) | B;
                v >>= 7;
        }
        *(ptr++) = (unsigned char)(v);
        return (char *)(ptr);
}

// Returns the length of the varint32 or varint64 encoding of "v"
int encode_varint_length(uint64_t v)
{
        int len = 1;
        while(v >= 128)
        {
                v >>= 7;
                len++;
        }
        return len;
}

// limit = p + size
const char* get_varint32_fallback(const char* p,const char* limit, uint32_t* value)
{
        uint32_t result = 0;
        uint32_t shift = 0;
        for(; shift <= 28 && p < limit; shift += 7)
        {
                uint32_t byte = *((const unsigned char*)(p));
                p++;
                if(byte & 128)
                {
                        // More bytes are present
                        result |= ((byte & 127) << shift);
                }
                else
                {
                        result |= (byte << shift);
                        *value = result;
                        return (const char*)(p);
                }
        }
        return NULL;
}
// limit = p + size
const char* get_varint64_fallback(const char* p, const char* limit, uint64_t* value)
{
        uint64_t result = 0;
        uint32_t shift = 0;
        for(; shift <= 63 && p < limit; shift += 7)
        {
                uint64_t byte = *((const unsigned char*)(p));
                p++;
                if(byte & 128)
                {
                        // More bytes are present
                        result |= ((byte & 127) << shift);
                }
                else
                {
                        result |= (byte << shift);
                        *value = result;
                        return (const char*)(p);
                }
        }
        return NULL;
}
// limit = p + size
const char* get_varint32_data_ptr(const char* p,const char* limit,uint32_t* value)
{
        if(p < limit)
        {
                uint32_t result = *((const unsigned char*)(p));
                if((result & 128) == 0)
                {
                        *value = result;
                        return p + 1;
                }
        }
        return get_varint32_fallback(p, limit, value);
}
// limit = p + size， 返回值为p除去长度后的data指针
const char* get_varint64_data_ptr(const char* p,const char* limit,uint64_t* value)
{
        return get_varint64_fallback(p, limit, value);
}

// ZigZag Transform:  Encodes signed integers so that they can be
// effectively used with varint encoding.
//
// varint operates on unsigned integers, encoding smaller numbers into
// fewer bytes.  If you try to use it on a signed integer, it will treat
// this number as a very large unsigned integer, which means that even
// small signed numbers like -1 will take the maximum number of bytes
// (10) to encode.  ZigZagEncode() maps signed integers to unsigned
// in such a way that those with a small absolute value will have smaller
// encoded values, making them appropriate for encoding using varint.
//
//       int32 ->     uint32
// -------------------------
//           0 ->          0
//          -1 ->          1
//           1 ->           2
//          -2 ->          3
//         ... ->        ...
//  2147483647 -> 4294967294
// -2147483648 -> 4294967295
//
//        >> encode >>
//        << decode <<

uint32_t zigzag_encode32(int32_t n)
{
        return (n << 1) ^ (n >> 31);
}

int32_t zigzag_decode32(uint32_t n)
{
        return (n >> 1) ^ -(int32_t)(n & 1);
}

uint64_t zigzag_encode64(int64_t n)
{
        return (n << 1) ^ (n >> 63);
}

int64_t zigzag_decode64(uint64_t n)
{
        return (n >> 1) ^ -(int64_t)(n & 1);
}


#endif //_CODING_H_

