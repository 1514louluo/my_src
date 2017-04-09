#include "Clock.h"
IMPL_LOGGER(Clock, logger);	
Clock::Clock(struct tm *start_tm, long sec, long circle_num_secs, long slot_num_secs):m_slot_sec(sec), m_circle_num(circle_num_secs),
	     m_slot_num(slot_num_secs), m_current_rotation_num(0), m_current_slot_num(0)
{
	m_Clock_status = CLOCK_HALT;
	pthread_rwlock_init(&m_rwlock, NULL);
	if(start_tm)
		m_start_tick_tm = *start_tm;
	else
	{
		time_t tm;
		time(&tm);
		m_start_tick_tm = *(gmtime(&tm));
	}

	for(int i = 0; i < SLOT_NUMS; i++)	
		m_slot_timer_head[i] = NULL;

	m_thread_poll_man.init_thread_poll_other();
}
Clock::~Clock(){}

long Clock::get_secs_per_slot(void)
{
	return m_slot_sec;
}
long Clock::get_secs_per_circle(void)
{
	return (m_slot_sec * SLOT_NUMS);
}

Timer * Clock::add_timer(long hours, long mins, long secs, timer_callback my_callback, void *data)
{
	Timer *newtimer = (Timer *)malloc(sizeof(Timer));
	/*首先确定这个timer在时钟的多少圈上*/
	long total_secs_4hours = hours * SECS_PER_HOUR;
	long total_secs_4mins  = mins * SECS_PER_MIN;
	long tatal_secs_4secs = secs + total_secs_4hours + total_secs_4mins;

	long secs_per_slot   = get_secs_per_slot();
	long secs_per_circle = get_secs_per_circle();

	newtimer->valid_rotation_num = tatal_secs_4secs / secs_per_circle;
	int remind = tatal_secs_4secs % secs_per_circle;

	int slotnb = remind / secs_per_slot;
	remind %= secs_per_slot;
	if(remind >= secs_per_slot/2)
		newtimer->slot_num = slotnb;
	else
		newtimer->slot_num = slotnb - 1;

	newtimer->next = NULL;
	newtimer->prev = NULL;
	newtimer->callback_func = my_callback;
	newtimer->data = data;

	int index = newtimer->slot_num;
	if(NULL == m_slot_timer_head[index])
	{
		m_slot_timer_head[index] = newtimer;
		m_slot_timer_head[index]->next = m_slot_timer_head[index];
		m_slot_timer_head[index]->prev = m_slot_timer_head[index];
	}
	else
	{
		m_slot_timer_head[index]->next->prev = newtimer;
		newtimer->next = m_slot_timer_head[index]->next;
		newtimer->prev = m_slot_timer_head[index];
		m_slot_timer_head[index]->next = newtimer;
	}

	return newtimer;
}
void Clock::delete_timer(Timer *timer)
{
	Timer *t_next = timer->next;
	if(t_next != timer) 
	{
		Timer *t_prev = timer->prev;
		t_prev->next = t_next;
		t_next->prev = t_prev;
	}
	free(timer);
	timer = NULL;
}
void Clock::start_tick(void)
{
    time_t target_time = mktime(&m_start_tick_tm);
    time_t process_start_time = time(NULL);
    long diff = (long)difftime(target_time, process_start_time);

    m_Clock_status = CLOCK_RUN;
    struct timeval secs_sep;

    if(diff >= 0)
    {
 		secs_sep.tv_sec = diff;
		secs_sep.tv_usec = 0;
   		int ret = select(0,NULL,NULL,NULL,&secs_sep);
   		if(!ret)
   		{
   			SYS_LOG_DEBUG( "start_tick timeout after secs_sep second = "<<secs_sep.tv_sec);
   		}
    	       else
    	       {
  			SYS_LOG_ERROR( "select error!!");
    	       }
    }else{
    
  	SYS_LOG_ERROR( "start time < now!!");
	return;
    }

    int ret;
    while(1)
    {		
    	while(pthread_rwlock_tryrdlock(&m_rwlock))
			usleep(3000);
    	if(CLOCK_HALT == m_Clock_status) break;
    	pthread_rwlock_unlock(&m_rwlock);

    	secs_sep.tv_sec = m_slot_sec;
	secs_sep.tv_usec = 0;
    	ret = select(0,NULL,NULL,NULL,&secs_sep);
    	if(!ret)
    	{
   			SYS_LOG_DEBUG( "start_tick timeout after secs_sep second = "<<m_slot_sec);
   			/*for test*/
   			printf("m_current_slot_num = %d, m_current_rotation_num = %d\n", m_current_slot_num, m_current_rotation_num);

	        	int i = m_current_slot_num;	
			Timer *htimer = m_slot_timer_head[i];
			Timer *etimer = htimer;
			if(etimer)
			{
				do
				{
					if(etimer->valid_rotation_num == m_current_rotation_num && etimer->slot_num == m_current_slot_num)	
					{
   						/*thread pool*/
						boost::function<void(void)> task = NULL;
						task = boost::bind(etimer->callback_func, etimer->data);	
						if (task != NULL)
						{
							m_thread_poll_man.commit_task(task);		
						}
					}
					etimer = etimer->next;	
				}while(etimer && etimer!= htimer);
			}

   			if(SLOT_NUMS - 1 <= m_current_slot_num)	
   			{
   				m_current_rotation_num++;
   				m_current_slot_num = 0;
   			}
   			else
   				m_current_slot_num++;

			if(m_current_rotation_num == m_circle_num && m_current_slot_num == m_slot_num)
			{
				m_current_rotation_num = 0;
				m_current_slot_num = 0;
			}
    	}
        else
   			SYS_LOG_ERROR("select error!!");
    }
    /*free all timer!*/
    clear_all_timer();
}
void Clock::clear_all_timer(void)
{

    /*free all timer!*/
    for(int i = 0; i < SLOT_NUMS; i++)	
    {
	Timer *htimer = m_slot_timer_head[i];
	if(NULL == htimer) continue;
	Timer *t_next = htimer->next;
	Timer *tmp;
	while(t_next && t_next != htimer)
	{
		tmp = t_next;
		t_next = t_next->next;	
		free(tmp);
		tmp = NULL;
	}
	free(htimer);
	m_slot_timer_head[i] = NULL;
    }	
}
void Clock::stop_tick(void)
{
	while(pthread_rwlock_trywrlock(&m_rwlock))
		usleep(3000);
	m_Clock_status = CLOCK_HALT;
	pthread_rwlock_unlock(&m_rwlock);
}






