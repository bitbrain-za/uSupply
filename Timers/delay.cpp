/* 
* delay.cpp
*
* Created: 2016/07/20 1:17:50 PM
* Author: Philip
*/

#include "../system.h"
#include "delay.h"

void delay::init(void)
{
  
}

void delay::delay_us(U16 us)
{
  
  /* 8MHz clock - 0.125us */
  asm("nop");
}