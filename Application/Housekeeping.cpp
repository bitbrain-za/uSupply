/* 
* Housekeeping.cpp
*
* Created: 2016/09/03 7:33:26 PM
* Author: philb
*/


#include "../system.h"

// default constructor
Housekeeping::Housekeeping()
{
} //Housekeeping

// default destructor
Housekeeping::~Housekeeping()
{
} //~Housekeeping

void Housekeeping::Run(FSM_CONTROL control)
{
  static U8 config_counter;

  if(control == RESET)
  {
    if(!house_keeping_tick.Registered())
      house_keeping_tick.init(500);
    house_keeping_tick.Start();
    config_counter = 0;
    config.Load();
  }

  if(house_keeping_tick.Expired())
  {
    if(config_counter++ >= 10)
    {
      config_counter = 0;
      config.Save();
    }
    house_keeping_tick.Restart();
    hal::climitLed.Toggle();
  }
}