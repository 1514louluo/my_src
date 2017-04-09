#ifndef _CLOCK_H
#define _CLOCK_H
/*只精确到秒*/
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>
#include <pthread.h>
#include "logdef.h"
#include "mycommon.h"
#include "threadpoll_manager.h"

#define SECS_PER_MIN    60 
#define SECS_PER_HOUR   3600
#define SECS_PER_DAY    86400
//#define SECS_PER_MONTH  86400
//#define SECS_PER_YEAR   86400

#define SLOT_NUMS       60
#define CLOCK_RUN         '1'
#define CLOCK_HALT        '0'

typedef void (*timer_callback)(void *ctx);
typedef struct timer
{
	int valid_rotation_num;
	int slot_num;
	void *data;
	timer_callback callback_func;
	struct timer *next;
	struct timer *prev;
}Timer;

class Clock
{
public:
	Clock(struct tm *, long , long, long);
	void start_tick(void);
	Timer *add_timer(long hours, long mins, long secs, timer_callback my_callback, void *data);
	void delete_timer(Timer *timer);
	void clear_all_timer(void);
	void stop_tick(void);
	~Clock();

private:
	/* data */
	long get_secs_per_circle(void);
	long get_secs_per_slot(void);

	const long m_slot_sec; /*每次滴答的时间间隔*/

	const long m_circle_num; /*每次钟面周期，比如设置每年，每月，每天, 每小时，每分钟，每秒, 重置m_current_rotation_num和m_current_slot_num,以秒为单位*/
	const long m_slot_num; /*每次钟面周期，比如设置每年，每月，每天, 每小时，每分钟，每秒, 重置m_current_rotation_num和m_current_slot_num,以秒为单位*/

	Timer *m_slot_timer_head[SLOT_NUMS]; /*每个槽对应的定时器链表*/
	int m_current_rotation_num; /*当前时钟已经旋转的圈数*/
	int m_current_slot_num; /*当前时针指向的槽号*/
	struct tm m_start_tick_tm; /*开始正式滴答前的等待时间,可以设置从什么时候正式开始时钟*/
	volatile char m_Clock_status; /*Clock status*/
	pthread_rwlock_t m_rwlock; /*lock m_Clock_status*/

	threadpoll_manager m_thread_poll_man;
	DECL_LOGGER(logger);
};
#endif
