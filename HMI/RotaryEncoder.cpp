/* 
* RotaryEncoder.cpp
*
* Created: 2016/08/14 12:05:25 AM
* Author: philb
*/


#include "../system.h"

// default constructor
RotaryEncoder::RotaryEncoder(port* portA, U8 pinNumberA, port* portB, U8 pinNumberB) : pinA(portA, pinNumberA, true, false), pinB(portB, pinNumberB, true, false)
{
} //RotaryEncoder

// default destructor
RotaryEncoder::~RotaryEncoder()
{
} //~RotaryEncoder

void RotaryEncoder::FSM(bool reset)
{
  bool valA = pinA.Value();

  if(reset)
  {
    pinA.SetValue(true);
    pinB.SetValue(true);
    _value = 0;
    _changed = true;
    return;
  }

  if(!valA)
  {
    if(pinB.Value())
    {
      /* clockwise */
      if(_value < 128)
        _value++;
    }
    else
    {
      if(_value > -128)
        _value--;
    }
    _changed = true;

    while(!pinA.Value())
    {

    }
  }
}