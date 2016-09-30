/* 
* CurrentLimit.cpp
*
* Created: 2016/07/28 10:27:33 AM
* Author: Philip
*/


#include "../system.h"

#define MAX 0xFF

bool CurrentLimit::Enabled;
U16 CurrentLimit::_limit;

U16 CurrentLimit::StepSize;
U16 CurrentLimit::max;
U16 CurrentLimit::min;
S8 CurrentLimit::calibration_value;
bool CurrentLimit::_dirty;
RotaryEncoder* CurrentLimit::_encoder;

void CurrentLimit::init(RotaryEncoder *encoder)
{
  Enabled = true;
  _encoder = encoder;
  tc0::init(TC_WGM_PWM_FAST, TC_COM_OC2A_CLEAR, TC_CS_DIV8);
  SetLimit(0);
}

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
    tc0::SetDutyCycle(100);
    return;
  }

  double x = millivolts;
  x *= MAX;
  x /= 3300;

  U8 dc = (U8)x;
  tc0::SetDutyCycle(dc);
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

void CurrentLimit::FSM(FSM_CONTROL control)
{
  S16 change;
  static U16 previous_current;

  if(control == RESET)
  {
    _encoder->FSM(true);
    SetLimit(config.SavedCurrent());
    return;
  }

  _encoder->FSM(false);

  if(_encoder->Changed())
  {
    _dirty = true;
    change = StepSize * _encoder->Value();
    _encoder->Reset();

    if(change > 0)
    {
      _limit += change;
      if(_limit > max)
        _limit = max;
    }
    else
    {
      change = -change;
      if((U16)change > _limit)
        _limit = min;
      else
      {
        _limit -= change;
      }
    }
    change = 0;
    SetLimit(_limit);
  }
}