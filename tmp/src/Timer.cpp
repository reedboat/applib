/*
 *
 */
#include"../include/Timer.h"

Timer::Timer()
{


}

Timer::~Timer()
{


}


clock_t Timer::GetCurrentTime()
{
     return clock();

}

float Timer::TimeDuration(clock_t & begin,clock_t & end)
{
      return (float)((float)(end-begin)/(float)CLOCKS_PER_SEC);

}
