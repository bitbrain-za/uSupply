/* 
* button.cpp
*
* Created: 2016/08/14 4:42:27 PM
* Author: philb
*/


#include "../system.h"

// default constructor
button::button(port* ptr_port, U8 pinNumberA) : _pin(ptr_port, pinNumberA, true, false)
{
} //button

// default destructor
button::~button()
{
} //~button

bool button::FSM(bool reset)
{
  static bool previous;
  if(reset)
  {
    _pin.SetValue(true);
    _pressed = false;
    previous = _pin.Value();
    return false;
  }

  if(!_pin.Value() & previous)
  {
    _pressed = true;
  }
  previous = _pin.Value();

  return _pressed;
}