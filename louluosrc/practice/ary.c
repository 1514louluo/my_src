/*
 *	输入一个表示十六进制的字符串，转换为十进制的整数输出。
 *	author: louluo
 */
#include <stdio.h>
#include <string.h>

int ary_change(char *string, int len, int *ret)
{
	int i = 0;
	*ret = 0;
	for(; i<len; ++i)	
	{
		char chr = string[i];
		if('\0' == chr) break;
		if(chr <= '9' && chr >= '0')
			*ret += (chr-'0')<<(4*(len-i-1));
		else if(chr <= 'F' && chr >= 'A')
			*ret += (10 + chr-'A')<<(4*(len-i-1));
		else if(chr <= 'f' && chr >= 'a')
			*ret += (10 + chr-'a')<<(4*(len-i-1));
		else
			return -1;
	}
	return 0;
}

int main()
{
	char a[] = "f103a8b";
	int ret;
	int status = ary_change(a, strlen(a), &ret);
	if(!status)
		printf("%d\n", ret);
	return 0;
}
