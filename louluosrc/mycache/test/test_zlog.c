#include <stdio.h> 
#ifdef _DEBUG
#include "logger.h"
DECL_LOGGER(sys_logger);
#endif

int main()
{
#ifdef _DEBUG
   int rt = LOG_INIT("../conf/zlog.conf");
   if(rt) return -1;
   if(NULL == IMPL_LOGGER(sys_logger)) return -1;
#endif

   SYS_LOG_INFO("fjdsjfsjfjdsfjsdjfj");

   LOG_FINI();

   return 0;
} 
