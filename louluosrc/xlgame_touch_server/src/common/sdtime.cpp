#include "sdtime.h"

#include <unistd.h>

#include <sys/time.h>

uint32_t get_cur_millseconds()
{
	struct timeval now;
	gettimeofday(&now, NULL);
	return now.tv_sec * 1000 + now.tv_usec / 1000;
}
