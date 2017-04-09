#include "cdef.h"
#include "minipwr_mem_pool.h"
#include "logger.h"
#include "kvcache.h"

#ifdef _DEBUG
DECL_LOGGER(sys_logger);
#endif

void EnableCoreFile(void)
{
	struct rlimit rl, rl_new;

	if (getrlimit(RLIMIT_CORE, &rl) == 0)
	{
		rl_new.rlim_cur = rl_new.rlim_max = RLIM_INFINITY;
		if (setrlimit(RLIMIT_CORE, &rl_new))
		{
			// failed.
			rl_new.rlim_cur = rl_new.rlim_max = rl.rlim_max;
			setrlimit(RLIMIT_CORE, &rl_new);
		}
	}

	// Check again
	if (getrlimit(RLIMIT_CORE, &rl) || rl.rlim_cur == 0)
	{
		//PRINTLOG_ERROR("Failed to ensure core file creation.");
		exit(1);
	}
	else
	{
		//PRINTLOG_INFO("enable dore dump finished");
	}
}

int main()
{
#ifdef _DEBUG
   int rt = LOG_INIT("../conf/zlog.conf");
   if(rt) return -1;
   if(NULL == IMPL_LOGGER(sys_logger)) return -1;
#endif

   struct timeval tv_s;
   struct timeval tv_e;
   EnableCoreFile();
   crypt_table_init();
   kvcache_init();

   char key[] = "he";
   char key1[] = "hel";
   char value[] = "xxxxxxxxxxxx";
   char value1[] = "xxxxxxyyyyyyyyyyyyyyyyy";


   int rt;
   gettimeofday(&tv_s, NULL);
   for(rt=0; rt < 100000; ++rt)
   {
	if(rt/2)
   		kvcache_set(key1,  value, 4, sizeof(value), 1000);
	else
   		kvcache_set(key,  value1, 3, sizeof(value1), 1000);
   }
   gettimeofday(&tv_e, NULL);
   printf("sec: %u\t", tv_e.tv_sec - tv_s.tv_sec);
   printf("usec:%u\n", tv_e.tv_usec - tv_s.tv_usec);

//   kvcache_del(key, sizeof(key));

/*
   gettimeofday(&tv_s, NULL);
   for(rt=0; rt < 10; rt++)
   {
   	char *kkk = kvcache_get(key1,   4);
   //	printf("%s\n", kkk);
   	char *kkk1 = kvcache_get(key,   3);
   //	printf("%s\n", kkk1);
   }
   gettimeofday(&tv_e, NULL);
   printf("sec: %u\t", tv_e.tv_sec - tv_s.tv_sec);
   printf("usec:%u\n", tv_e.tv_usec - tv_s.tv_usec);
*/
/*
   printf("!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
   for(rt=0; rt < 10; rt++)
   {
   	kvcache_del((char *)&rt,   4);
   }

   for(rt=0; rt < 10; rt++)
   {
   	printf("%s\n", kvcache_get((char *)&rt,   4));
   }
*/
/*
   minipwr_create_pool(1);
   struct timeval tv_s;
   struct timeval tv_e;

   gettimeofday(&tv_s, NULL);
   gettimeofday(&tv_e, NULL);
   malloc(100);
   printf("sec: %u\t", tv_e.tv_sec - tv_s.tv_sec);
   printf("usec:%u\n", tv_e.tv_usec - tv_s.tv_usec);

   gettimeofday(&tv_s, NULL);
   gettimeofday(&tv_e, NULL);
   minipwr_palloc(100);
   printf("search sec: %u\t", tv_e.tv_sec - tv_s.tv_sec);
   printf("search usec:%u\n", tv_e.tv_usec - tv_s.tv_usec);
*/

   kvcache_free();
//   LOG_FINI();
   return 0;
}
