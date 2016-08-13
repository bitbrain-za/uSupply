/* 
* tc2.cpp
*
* Created: 2016/07/20 12:07:43 PM
* Author: Philip
*/


#include "../system.h"

bool tc2::ocrCallbackSet = false;
void (*tc2::ocrCallBack)(void);

void tc2::init(TC_WGM waveform_mode, TC_COM compare_mode, TC_CS clock_select)
{
  SetWGM(waveform_mode);
  SetCOM(compare_mode);
  SelectClock(clock_select);
}

void tc2::SetWGM(TC_WGM mode)
{
  TCCR2A &= ~((0x01 << WGM20) | (0x01 << WGM21));
  U8 wgm20 = (mode >> 1) & 0x01;
  U8 wgm21 = mode & 0x01;

  TCCR2A |= ((wgm20 << WGM20) | (wgm21 << WGM21));
}

void tc2::SetCOM(TC_COM mode)
{
  TCCR2A &= ~(0x03 << COM2A0);
  TCCR2A |= (mode << COM2A0);
}

void tc2::SelectClock(TC_CS clock_select)
{
  TCCR2A &= ~(0x07);
  TCCR2A |= (clock_select);
}

void tc2::ExternalClockMode(bool enable, bool xtal)
{
  ASSR &= 0x03;
  if(enable)
    ASSR |= (1 << EXCLK);
  if(xtal)
    ASSR |= (1 << AS2);
}

ISR(TIMER2_COMP_vect)
{
  tc2::TriggerCompareMatchCallback();
}