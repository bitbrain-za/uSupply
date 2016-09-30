/* 
* tc0.cpp
*
* Created: 2016/07/20 12:06:22 PM
* Author: Philip
*/


#include "../system.h"

void tc0::init(TC_WGM waveform_mode, TC_COM compare_mode, TC_CS clock_select)
{
  SetWGM(waveform_mode);
  SetCOM(compare_mode);
  SelectClock(clock_select);
}

void tc0::SetWGM(TC_WGM mode)
{
  TCCR0A &= ~((0x01 << WGM00) | (0x01 << WGM01));
  U8 wgm20 = (mode >> 1) & 0x01;
  U8 wgm21 = mode & 0x01;

  TCCR0A |= ((wgm20 << WGM00) | (wgm21 << WGM01));
}

void tc0::SetCOM(TC_COM mode)
{
  TCCR0A &= ~(0x03 << COM0A0);
  TCCR0A |= (mode << COM0A0);
}

void tc0::SelectClock(TC_CS clock_select)
{
  TCCR0A &= ~(0x07);
  TCCR0A |= (clock_select);
}

void tc0::ExternalClockMode(bool enable, bool xtal)
{

}

void tc0::SetDutyCycle(U8 duty)
{
  /* scale duty cycle */
  if(duty >= 100)
  {
    SetOutputCompareA(0xFF);
    return;
  }

  double scale = duty;
  scale *= 0xFF;
  scale /= 100;

  duty = (U8)(scale);

  SetOutputCompareA(duty);
}