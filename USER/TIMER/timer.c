/*******************************************************************************

功能：定时器相关

创建时间：2016/03/03

修改历史：

********************************************************************************/

#include "timer.h"

#include <applicfg.h>

extern u32 lwip_localtime;  //lwip本地时间计数器,单位:ms
extern bool FlashFlag;
/*********************************************************************************

说明：定时器3中断服务程序，每隔1ms，对小车进行纠偏

**********************************************************************************/
u32 tempCnt1ms;
#define LEDCycT 400//300ms
void TIM3_IRQHandler(void)
{ 	
	extern u16 Count_Timer1ms;
	
	if(TIM3->SR&0X0001)//溢出中断
	{

   
	TIM3->SR&=~(1<<0);//清除中断标志位 	    
}


/*********************************************************************************

说明：通用定时器3的初始化
	  这里时钟选择为APB1的2倍，而APB1为42M
	  arr：自动重装值。
	  psc：时钟预分频数
	  定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
	  Ft=定时器工作频率,单位:Mhz
	  这里使用的是定时器3!

**********************************************************************************/
void TIM3_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;	//TIM3时钟使能    
 	TIM3->ARR=arr;  	//设定计数器自动重装值 
	TIM3->PSC=psc;  	//预分频器	  
	TIM3->DIER|=1<<0;   //允许更新中断	  
	TIM3->CR1|=0x01;    //使能定时器3
  MY_NVIC_Init(1,3,TIM3_IRQn,2);	//抢占1，子优先级3，组2									 
}




/***************************************************************************************

以下用于canopen_master模块

*****************************************************************************************/

/*  ---------  The timer table --------- */
s_timer_entry timers[MAX_NB_TIMER] = {{TIMER_FREE, NULL, NULL, 0, 0, 0},};

TIMEVAL total_sleep_time = TIMEVAL_MAX;
TIMER_HANDLE last_timer_raw = -1;

#define min_val(a,b) ((a<b)?a:b)

/*!
** -------  Use this to declare a new alarm ------
**
** @param d
** @param id
** @param callback
** @param value
** @param period
**
** @return
**/
TIMER_HANDLE SetAlarm(CO_Data* d, UNS32 id, TimerCallback_t callback, TIMEVAL value, TIMEVAL period)
{
	TIMER_HANDLE row_number;
	s_timer_entry *row;

	/* in order to decide new timer setting we have to run over all timer rows */
	for(row_number=0, row=timers; row_number <= last_timer_raw + 1 && row_number < MAX_NB_TIMER; row_number++, row++)
	{
		if (callback && 	/* if something to store */
		   row->state == TIMER_FREE) /* and empty row */
		{	/* just store */
			TIMEVAL real_timer_value;
			TIMEVAL elapsed_time;

			if (row_number == (TIMER_HANDLE)last_timer_raw + 1) last_timer_raw++;

			elapsed_time = getElapsedTime();
			/* set next wakeup alarm if new entry is sooner than others, or if it is alone */
			real_timer_value = value;
			real_timer_value = min_val(real_timer_value, TIMEVAL_MAX);

			if (total_sleep_time > elapsed_time && total_sleep_time - elapsed_time > real_timer_value)
			{
				total_sleep_time = elapsed_time + real_timer_value;
				setTimer(real_timer_value);
			}
			row->callback = callback;
			row->d = d;
			row->id = id;
			row->val = value + elapsed_time;
			row->interval = period;
			row->state = TIMER_ARMED;
			return row_number;
		}
	}

	return TIMER_NONE;
}

/*!
**  -----  Use this to remove an alarm ----
**
** @param handle
**
** @return
**/
TIMER_HANDLE DelAlarm(TIMER_HANDLE handle)
{
	/* Quick and dirty. system timer will continue to be trigged, but no action will be preformed. */
	MSG_WAR(0x3320, "DelAlarm. handle = ", handle);
	if(handle != TIMER_NONE)
	{
		if(handle == last_timer_raw)
			last_timer_raw--;
		timers[handle].state = TIMER_FREE;
	}
	return TIMER_NONE;
}

/*!
** ------  TimeDispatch is called on each timer expiration ----
**
**/
int tdcount=0;
void TimeDispatch(void)
{
	TIMER_HANDLE i;
	TIMEVAL next_wakeup = TIMEVAL_MAX; /* used to compute when should normaly occur next wakeup */
	/* First run : change timer state depending on time */
	/* Get time since timer signal */
	UNS32 overrun = (UNS32)getElapsedTime();

	TIMEVAL real_total_sleep_time = total_sleep_time + overrun;

	s_timer_entry *row;

	for(i=0, row = timers; i <= last_timer_raw; i++, row++)
	{
		if (row->state & TIMER_ARMED) /* if row is active */
		{
			if (row->val <= real_total_sleep_time) /* to be trigged */
			{
				if (!row->interval) /* if simply outdated */
				{
					row->state = TIMER_TRIG; /* ask for trig */
				}
				else /* or period have expired */
				{
					/* set val as interval, with 32 bit overrun correction, */
					/* modulo for 64 bit not available on all platforms     */
					row->val = row->interval - (overrun % (UNS32)row->interval);
					row->state = TIMER_TRIG_PERIOD; /* ask for trig, periodic */
					/* Check if this new timer value is the soonest */
					if(row->val < next_wakeup)
						next_wakeup = row->val;
				}
			}
			else
			{
				/* Each armed timer value in decremented. */
				row->val -= real_total_sleep_time;

				/* Check if this new timer value is the soonest */
				if(row->val < next_wakeup)
					next_wakeup = row->val;
			}
		}
	}

	/* Remember how much time we should sleep. */
	total_sleep_time = next_wakeup;

	/* Set timer to soonest occurence */
	setTimer(next_wakeup);

	/* Then trig them or not. */
	for(i=0, row = timers; i<=last_timer_raw; i++, row++)
	{
		if (row->state & TIMER_TRIG)
		{
			row->state &= ~TIMER_TRIG; /* reset trig state (will be free if not periodic) */
			if(row->callback)
				(*row->callback)(row->d, row->id); /* trig ! */
		}
	}
}














