#include <bitset>
#include <iostream>
using namespace std;

int main()
{
	bitset<16> bs;
	cout<<bs.to_string()<<endl;
	cout<<bs.test(1)<<endl;
	return 0;
}
