/* 
* SystemClock.cpp
*
* Created: 2016/07/22 1:20:42 PM
* Author: Philip
*/


#include "SystemClock.h"

tc2 hal::sys_timer(TC_WGM_NORMAL, TC_COM_OC2A_DISCONNECTED, TC_CS_DIV8);

void SystemClock::init(void)
{

}