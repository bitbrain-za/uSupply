/* 
* timer.cpp
*
* Created: 2016/07/29 10:36:37 AM
* Author: Philip
*/


#include "../system.h"

timer *timer_list::timers[MAX_TIMERS];

void timer_list::init()
{
  int i = 0;

  for( i = 0 ; i < MAX_TIMERS ; i++ )
  {
    timers[i] = NULL;
  }
}

void timer_list::tick(U16 ms)
{
  int i = 0;

  for(i = 0 ; i < MAX_TIMERS ; i++)
  {
    if(timers[i] != NULL)
    {
      timers[i]->tick(ms);
    }
  }
}

U8 timer_list::RegisterTimer(timer *t)
{
  U8 i = 0;

  for(i = 0 ; i < MAX_TIMERS ; i++)
  {
    if(timers[i] == NULL)
    {
      timers[i] = t;
      return i;
    }
  }
  return 0xFF;
}

void timer_list::DeregisterTimer(U8 id)
{
  timers[id] = NULL;
}




// default constructor
timer::timer()
{
  _id = timer_list::RegisterTimer(this);
  _callback_set = false;
  _running = false;
  _interval = 0;
  _count = 0;
} //timer

timer::timer(U16 milliseconds)
{
  _id = timer_list::RegisterTimer(this);
  _callback_set = false;
  _running = false;
  _interval = milliseconds;
  _count = 0;
} //timer

// default destructor
timer::~timer()
{
  timer_list::DeregisterTimer(_id);
} //~timer

void timer::tick(U16 ms)
{
  if(!_running)
    return;

  _count += ms;

  if(_callback_set & !_isr_flag)
  {
    if(_count >= ms)
    {
      _isr_flag = true;
      CallBack();
    }
  }
}

void timer::Start()
{
  _running = true;
}

void timer::Start(U16 milliseconds)
{
  _interval = milliseconds;
  _running = true;
}

void timer::Restart()
{
  _count = 0;
  Start();
}

void timer::Restart(U16 milliseconds)
{
  _interval = milliseconds;
  Restart();
}

