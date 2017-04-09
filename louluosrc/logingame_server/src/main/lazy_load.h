#pragma once




/*
说明：该模块主要用于处理惰性加载流程的固化
程序中大量的惰性加载，导致处理流程代码copy，paste，增加隐性bug
采用boost::function机制，实现惰性加载流程代码的唯一性以及与业务代码的隔离

对加载函数的返回需要按照规定给予返回：

读取类型：
-1： 加载错误或失败
0：  加载成功，读取数据个数为0
>0:  加载成功，读取数据个数
写入类型：
-1:  写入失败
0：  写入成功


注：对于m_func_dbread，与m_func_dbwrite，可以不同时具有

*/
#include "mycommon.h"
#include "boost/lambda/lambda.hpp"
#include "boost/bind.hpp"
#include "boost/lambda/bind.hpp"

class lazy_load
{
public:
	lazy_load(void);
	~lazy_load(void);

	// 
	void bind_function(boost::function<int(void)> dbread, boost::function<int(void)> dbwrite,
					   boost::function<int(void)> cacheread, boost::function<int(void)> cachewrite);

	int read(void);
	int write(void);

private:
	boost::function<int(void)> m_func_dbread;
	boost::function<int(void)> m_func_dbwrite;
	boost::function<int(void)> m_func_cacheread;
	boost::function<int(void)> m_func_cachewrite;

};
