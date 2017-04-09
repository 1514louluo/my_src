#include "cstring.h"

int print()
{
	char b[] = "hello world!!";

	string a = new_string(b,0, 10);
	
	string c = new_string(b,6,3);

	printf("a = %s\n", a->value);
	printf("c = %s\n", c->value);

	a = append_realloc(a, c);

	printf("a = %s\n", a->value);

	string xx = append(a, c);
	printf("xx = %s\n", xx->value);

/*
	a->value[1] = (char *)malloc(strlen(b)+1);
	memcpy(a->value[1], b, strlen(b)); 
	a->value[1][strlen(b)] = '\0';
	a->grouplen++;
	a->strlen += strlen(b) + 1;
*/
//	string x = substr(a, 0, 2);

//	printf("%lu\n", x->refcount);
//	printf("%s\n", c_str(x));

//	printf("%c\n", at(a, 14));
//
//	string xx = append(a, x);
//	printf("%s\n", c_str(xx));

	return 0;
}

int main()
{
	print();
	return 0;
}
