#include "coding.h"
#include <stdio.h>

int main()
{
	is_little_endian();
	uint64_t m = 103218;
	bm_foreach((uchar_t *)&m, 64);


	char buf[4] = {0};

	bm_foreach((uchar_t *)buf, 64);
	char *tmp = encode_varint64(buf, m);
	bm_foreach((uchar_t *)buf, (tmp-buf)*8);

	uint64_t m1=0;
	const char *data = get_varint64_data_ptr(buf, buf+64, &m1);
	bm_foreach((uchar_t *)data, (8 - (data-buf))*8);

	int32_t i = -2;
	bm_foreach((uchar_t *)&i, 32);

	
	uint32_t fi = zigzag_encode32(i);
	bm_foreach((uchar_t *)&fi, 32);


	int32_t ai = zigzag_decode32(fi);
	bm_foreach((uchar_t *)&ai, 32);

	return 0;
}
