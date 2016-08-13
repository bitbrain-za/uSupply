/* 
* SystemClock.cpp
*
* Created: 2016/07/22 1:20:42 PM
* Author: Philip
*/


#include "../system.h"


void SystemClock::init(void)
{
  timer_list::init();

  /* 125 kHz base */
  tc2::init(TC_WGM_CTC, TC_COM_OC2A_DISCONNECTED, TC_CS_DIV64);

  /* 1ms */
  tc2::SetOutputCompareA(124);
  tc2::SetOutputCompareCallback(&tick);
  tc2::EnableOutputCompareInterrupt();

}

void SystemClock::tick(void)
{
  timer_list::tick(1);
}