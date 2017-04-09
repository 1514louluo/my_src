#include <boost/pool/singleton_pool.hpp>
#include <iostream>
using namespace std;
using namespace boost;

struct A{};

typedef singleton_pool<struct A, 16> mypool;

int main()
{
	int i = 0;
	int bytes = 0;
	for(; i < 100000000; ++i)
	{
		int *tmp = (int *)mypool::malloc();
		if(tmp)
		{
			bytes += 16;
			cout<<"now: Bytes = "<<bytes<<endl;	
		}

		
	//	mypool::free(tmp);
	}
	mypool::purge_memory();
	return 0;
}

