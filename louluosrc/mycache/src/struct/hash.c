#include "hash.h"

ulong_t crypt_table[0x500];

void crypt_table_init(void)
{
        ulong_t seed = 0x00100001, index1 = 0, index2 = 0, i;

        for(; index1 < 0x100; index1++)
        {
                for(index2 = index1, i = 0; i < 5; i++, index2 += 0x100)
                {
                        ulong_t temp1, temp2;
                        seed = (seed * 125 + 3) % 0x2AAAAB;
                        temp1 = (seed & 0xFFFF) << 0x10;
                        seed = (seed * 125 + 3) % 0x2AAAAB;
                        temp2 = (seed & 0xFFFF);
                        crypt_table[index2] = (temp1 | temp2);
                }
        }
}

ulong_t hash_string(const char *cstring, ulong_t stringlen, int  hash_type)
{
	 int i = 0;
        ulong_t seed1 = 0x7FED7FED, seed2 = 0xEEEEEEEE;
        int ch;

	 for(; i < stringlen; ++i)
        {
                ch = toupper(*(cstring+i));

                seed1 = crypt_table[(hash_type << 8) + ch] ^ (seed1 + seed2);
                seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
        }
        return seed1;
}






