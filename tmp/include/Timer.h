/*
 *purpose:to compute the timer
 *author:junlin zhang
 *date:2010-11-15
 */

#ifndef TIMER_H
#define TIMER_H

#include <time.h>
#include <sys/timeb.h>

class Timer{

	public:
		Timer();
		~Timer();

        clock_t  GetCurrentTime();//read current time
		float  TimeDuration(clock_t & begin,clock_t & end);//return the time duration

};

#endif

