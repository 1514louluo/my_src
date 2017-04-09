#include "config_file.h"
#include <sys/time.h>
#include <iostream>
using namespace music_util;

int main(int argc, char **argv)
{
	if(argc < 2) 
	{
		std::cout<<"wrong param!!"<<std::endl;
	}

  struct timeval tv_s;
  struct timeval tv_e;
	const char *config_path = argv[1];
  gettimeofday(&tv_s, NULL);
	ConfigFile config;
	config.load(config_path);
  gettimeofday(&tv_e, NULL);

	std::cout<<"insert sec  use: "<<(tv_e.tv_sec - tv_s.tv_sec)<<std::endl;
	std::cout<<"insert usec use: "<<(tv_e.tv_usec - tv_s.tv_usec)<<std::endl;
  

  gettimeofday(&tv_s, NULL);
	std::cout<<config.get_string("public_radio.radioid_29", "149")<<std::endl;
	std::cout<<config.get_int("public_radio.radioid_29", 149)<<std::endl;
  gettimeofday(&tv_e, NULL);

	std::cout<<"insert sec  use: "<<(tv_e.tv_sec - tv_s.tv_sec)<<std::endl;
	std::cout<<"insert usec use: "<<(tv_e.tv_usec - tv_s.tv_usec)<<std::endl;
	
	std::cout<<config.get_int("public_radio.radioid_291", 149)<<std::endl;
	std::cout<<config.get_int("public_radio.radioname_32", 149)<<std::endl;
	return 0;
}
