#include "timer.h"

Timer::Timer() 
{ 			//constructor
  Reset();
}

//Set the timer value to the current time
void Timer::Reset()
{
  timerval = (TimerType) time((time_t *) 0);
}

//Return the amount of time that has elapsed since the last timer reset
TimerType Timer::ElapsedTime()
{
  return((TimerType) time((time_t *) 0) - timerval);
}
  


