/*
 * uSupply.cpp
 *
 * Created: 2016/07/20 10:40:38 AM
 * Author : Philip
 */ 

#define MAIN_C

#include "system.h"

RotaryEncoder voltageEncoder(&hal::portA, 4, &hal::portA, 2);
RotaryEncoder currentEncoder(&hal::portA, 5, &hal::portA, 0);

int main(void)
{
  char str[32];
  LM6029ACW display;
  SystemClock::init();

  hal::board_init();
  display.init();

  display.ClearScreen(false);                  //clear all the pixels on the display
  currentEncoder.FSM(true);
  voltageEncoder.FSM(true);
  while (1) 
  {
    currentEncoder.FSM(false);
    voltageEncoder.FSM(false);
    if(voltageEncoder.Changed() || currentEncoder.Changed())
    {
      display.ClearScreen(false);
      sprintf(str, "Voltage: %i", voltageEncoder.Value());
      display.GotoXY(20, 3);
      display.PutStr(str, false);

      sprintf(str, "Current: %i", currentEncoder.Value());
      display.GotoXY(20, 4);
      display.PutStr(str, false);
    }
   
    _delay_ms(1);
  }
}

