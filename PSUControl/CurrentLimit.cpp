/* 
* CurrentLimit.cpp
*
* Created: 2016/07/28 10:27:33 AM
* Author: Philip
*/


#include "../system.h"

#define MAX 0xFF

// default constructor
CurrentLimit::CurrentLimit() : tc(TC_WGM_PWM_FAST, TC_COM_OC2A_CLEAR, TC_CS_DIV8)
{
  Enabled = true;

  tc.SetWGM(TC_WGM_PWM_FAST);
  tc.SetCOM(TC_COM_OC2A_CLEAR);

  tc.SelectClock(TC_CS_DIV8);

  SetLimit(0);
} //CurrentLimit

// default destructor
CurrentLimit::~CurrentLimit()
{
} //~CurrentLimit

void CurrentLimit::SetAverageVoltage(U16 millivolts)
{
  /* 
  Vav = (Vh * x + Vl * y) / (x + y) 
  
  Vh = 3V3
  Vl = 0

  x = OCR x 2
  y = (MAX - OCR) x 2
  y = 2MAX - x

  Vav = Vh(x) / MAX
  x = Vav / Vh * MAX
  x = Vav / 3300 * MAX
  */

  if(millivolts >= 3300)
  {
    tc.SetDutyCycle(100);
    return;
  }

  double x = millivolts;
  x *= MAX;
  x /= 3300;

  U8 dc = (U8)x;
  tc.SetDutyCycle(dc);
}

void CurrentLimit::SetLimit(U16 milliamps)
{
  if(milliamps > 1055)
  {
    milliamps = 1055;
  }

  /* Voltage divider: R1 = 10k, R2 = 4k7 */
  U16 vout = (100 + 47) / 47 * milliamps;
  
  SetAverageVoltage(vout);
  _limit = milliamps;
}