


//#include "module_test.h"
#include "SDConfigurationSingleton.h"
#include <log4cplus/configurator.h>
using namespace log4cplus;
#include "common/SDLogger.h"
#include "dispatcher.h"
#include "service_stat.h"
#include "http_server.h"
#include <sys/types.h>
#include <sys/wait.h>
//#include "log_codec_wrapper.h"
void init_config(void)
{
	std::string config_file;
	config_file ="../conf/logingame.conf";
	SDConfiguration* config = new SDConfiguration(config_file.c_str());
	if (!config->load()) 
	{
		//LOG4CPLUS_ERROR(logger, "read config_file \"" << m_conf_file << "\" fail: " << strerror(errno));
		printf("failed to load config file path=%s\n", config_file.c_str());
		delete config;
		exit(0);
	}
	SDConfigurationSingleton::get_instance()->init(config);



}

void sign_t(int sign)
{
	printf("error!! in thread %u\n", (unsigned int) pthread_self());
	abort();
}


int main(int argc, char* argv[])
{


	
	

	if (argc >= 2)
	{
		char *pparam = argv[1];
		std::string str_param=pparam;
		if (!str_param.compare("daemon"))
		{
			printf("become guard\n");
			printf("become daemon\n");
			daemon(1, 1);
			do 
			{
				pid_t pid;
				pid = fork();
				if (pid == 0)
				{// child process
					break;
				}
				else
				{
					int status=0;
					waitpid(pid, &status, 0);
					printf("child process exit, restart child process\n");
				}
			} while (true);
		}
		else
		{
			printf("becom app \n");
		}
	}

	
	
	


	PropertyConfigurator::doConfigure("../conf/log4cplus.conf");
	Logger logger=Logger::getInstance("MY");
	LOG4CPLUS_INFO(logger,"======main======");
	EnableCoreFile();
	//EnableFileLimit();
	init_config();
//	module_test mytest;
//	mytest.general_test();
	/*if (argc < 2)
	{
		printf("username is needed \n");
		exit(0);
	}
	mytest.usernametouid(argv[1]);
	exit(0);*/
	
	//log_codec_wrapper::log_codec_wrapper_init();
	//general_log_client *plogclient = general_log_client::get_instance();
	//plogclient->init();
	

	

	
	signal(SIGPIPE, SIG_IGN);
	//debug
	signal(SIGSEGV, sign_t);

	signal(SIGIO, SIG_IGN);
	//signal(SIGINT, SIG_IGN);
	signal(SIGALRM, SIG_IGN);

	


	
	http_server diagnose_server;
	diagnose_server.init();

	dispatcher gamebox_gate_server;
	gamebox_gate_server.init();

	

	while(true)
	{
		sleep(1000000);
	}
	
	return 0;
}
