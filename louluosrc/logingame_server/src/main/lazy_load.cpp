#include "lazy_load.h"

lazy_load::lazy_load(void)
{
	 m_func_dbread = NULL;
	 m_func_dbwrite = NULL;
	 m_func_cacheread = NULL;
	 m_func_cachewrite =NULL;
}

lazy_load::~lazy_load(void)
{
}

void lazy_load::bind_function( boost::function<int(void)> dbread, boost::function<int(void)> dbwrite, 
							  boost::function<int(void)> cacheread, boost::function<int(void)>  cachewrite )
{

	m_func_dbread = dbread;
	m_func_dbwrite = dbwrite;
	m_func_cacheread = cacheread;
	m_func_cachewrite = cachewrite;
}


using namespace std;
using namespace boost::lambda;

int lazy_load::read( void )
{
	if (m_func_cacheread == NULL || m_func_cachewrite == NULL || m_func_dbread == NULL)
	{
		return -1;
	}
	int result;
	result = m_func_cacheread();
	if (result == -1)
	{
		return -1;
	}
	if (result > 0)
	{
		return result;
	}

	// result = 0
	result = m_func_dbread();
	if (result == -1)
	{
		return -1;
	}
	
	if (result > 0)
	{
		m_func_cachewrite();
	}
	else
	{

		return result;
	}

	//int i = 0; int j;
	//var_type<int>::type delayed_i(var(i));
	//var_type<int>::type delayed_j(var(j));
	////constant_type<int>::type delayed_c(constant(c));

	//std::vector<int> a;

	//
	////std::for_each(a.begin(), a.end(), (var(j) = _1)); 

	////list<int> v(10);
	//for_each(a.begin(), a.end(), (var(j) = _1, _1 = var(i), var(i) = var(j))); 

	//-------------
//var(i)=_1;
	//int i = 1; int j = 2; 
	//boost::lambda(_1 + _2)(i, j); // ok

	

}
