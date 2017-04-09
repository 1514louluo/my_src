#include <boost/scoped_ptr.hpp>
#include <iostream>
using namespace std;

int main()
{
	boost::scoped_ptr<int> sptr(new int);
	*sptr = 100;
	cout<<*sptr<<endl;
	
	return 0;
}
