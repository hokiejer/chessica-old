#include <time.h>

typedef unsigned long int TimerType;


class Timer {

  TimerType timerval;

public:
  Timer();
  void Reset();
  TimerType ElapsedTime();
};


