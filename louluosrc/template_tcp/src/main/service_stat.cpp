#include "service_stat.h"


service_stat *service_stat::m_pthis=NULL;


service_stat::service_stat(void)
{
	m_tcp_curent_conn=0;
	m_tcp_history_conn = 0;
	m_cpu_usage=0;
	m_mem_usage=0;
	m_overload=0;
	m_cmd_counts_60seconds = 0;
	m_cmd_counts_60seconds_stat=0;
}

service_stat::~service_stat(void)
{
}

service_stat * service_stat::singleton( void )
{
	if (m_pthis == NULL)
	{
		m_pthis = new service_stat;
		m_pthis->init_stat_service();

	}
	return m_pthis;

}

void service_stat::timer_callback( void )
{
	get_cpu(m_cpu_usage);
	get_mem(m_mem_usage);
	get_overload(m_overload);
	{
		scopedlock mylock1(m_mutex_cmd_count.get());
		m_cmd_counts_60seconds_stat = m_cmd_counts_60seconds;
		m_cmd_counts_60seconds=0;
	}



}

void service_stat::init_stat_service( void )
{
	m_str_tmp_path = "/tmp/store_tmp";
	boost::function<void (void)> timercallback = boost::bind(&service_stat::timer_callback, this);
	m_timer_stat.startimer(timercallback, 60);

	

	

}


int service_stat::get_cpu(int &cpu_usage)
{
	std::string str_random;
	str_random = (boost::format("%d") %random()).str();
	std::string str_file_path;
	str_file_path = m_str_tmp_path+str_random;
	pid_t thispid;
	thispid = getpid();
	std::string cmdline;
	cmdline = ((boost::format("rm -f %s")  %str_file_path.c_str())).str();
	::system(cmdline.c_str());
	cmdline = ((boost::format("top -bn 1 -p %d | grep %d | awk '{print $9}' > %s") %thispid  %thispid %str_file_path.c_str())).str();
	::system(cmdline.c_str());

	//int fd = ::open(m_str_cpu_path.c_str(), O_CREAT);
	std::ifstream in(str_file_path.c_str());
	std::string str_value;
	//std::getline(fin, line)
	std::getline(in,str_value);
	double fvalue = atof(str_value.c_str());
	cpu_usage = fvalue;
	in.close();

	//printf("cpu_usage = %d\n",cpu_usage);

	cmdline = (boost::format("rm -f %s")  %str_file_path.c_str()).str();
	::system(cmdline.c_str());
	return 0;

}


int service_stat::get_mem(float &mem_usage)
{

	std::string str_random;
	str_random = (boost::format("%d") %random()).str();
	std::string str_file_path;
	str_file_path = m_str_tmp_path+str_random;

	pid_t thispid;
	thispid = getpid();
	std::string cmdline;
	cmdline = ((boost::format("rm -f %s")  %str_file_path.c_str())).str();
	::system(cmdline.c_str());
	cmdline = ((boost::format("top -bn 1 -p %d | grep %d | awk '{print $10}' > %s") %thispid  %thispid %str_file_path.c_str())).str();
	::system(cmdline.c_str());

	//int fd = ::open(m_str_cpu_path.c_str(), O_CREAT);
	std::ifstream in(str_file_path.c_str());
	std::string str_value;
	std::getline(in, str_value);
	double fvalue = atof(str_value.c_str());
	mem_usage = fvalue;
	in.close();

	//printf("mem_usage = %d\n",mem_usage);

	cmdline = (boost::format("rm -f %s")  %str_file_path.c_str()).str();
	::system(cmdline.c_str());
	return 0;

}


int service_stat::get_overload(int &overload)
{

	std::string str_random;
	str_random = (boost::format("%d") %random()).str();
	std::string str_file_path;
	str_file_path = m_str_tmp_path+str_random;

	pid_t thispid;
	thispid = getpid();
	std::string cmdline;
	cmdline = ((boost::format("rm -f %s")  %str_file_path.c_str())).str();
	::system(cmdline.c_str());
	cmdline = ((boost::format("top -bn 1 |grep \"load average\"| cut -d \",\" -f 4| awk '{print $3}' > %s")  %str_file_path.c_str()) ).str();
	::system(cmdline.c_str());

	//int fd = ::open(m_str_cpu_path.c_str(), O_CREAT);
	std::ifstream in(str_file_path.c_str());
	std::string str_value;
	std::getline(in,str_value);
	double fvalue = atof(str_value.c_str());
	overload = fvalue;
	in.close();

	//printf("overload = %d\n",overload);

	cmdline = (boost::format("rm -f %s")  %str_file_path.c_str()).str();
	::system(cmdline.c_str());
	return 0;

}

void service_stat::add_cmd_count( void )
{
	scopedlock mylock(m_mutex_cmd_count.get());
	m_cmd_counts_60seconds++;

}
