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
VoltageControl voltageControl(ADC2, &voltageEncoder);
Housekeeping houseKeeper;

int main(void)
{
  U16 delay_counter = 0;
  U16 voltage_read = 0;
  char str[32];
  LM6029ACW display;
  SystemClock::init();
  U16 voltage = 0;
  timer led_timer;
  led_timer.init(500);
  led_timer.Start();

  hal::board_init();
  twi::inititalise();
  display.init();

  display.ClearScreen(false);
  currentEncoder.FSM(true);
  voltageEncoder.FSM(true);
  houseKeeper.Run(RESET);
  voltageControl.Run(RESET);

  adc::init();

  sei();

  while (1) 
  {
    houseKeeper.Run(NO_RESET);

    if(delay_counter++ >= 5000)
    {
      delay_counter = 0;
    }

    if(led_timer.Expired())
    {
      led_timer.Restart();
      //hal::climitLed.Toggle();
    }

    currentEncoder.FSM(false);
    voltageControl.Run(NO_RESET);

    if(voltageControl.Dirty() || currentEncoder.Changed() || ((delay_counter % 500) == 0))
    {
      display.ClearScreen(false);

      voltage = voltageControl.voltage_set();
      voltage_read = voltageControl.voltage_read();

      //sprintf(str, "Set: %d.%03d V", (voltage/1000), (voltage%1000));
      //display.GotoXY(20, 2);
      //display.PutStr(str, false);

      sprintf(str, "Output: %d.%02d V", voltage_read/1000, (voltage_read%1000) / 10);
      display.GotoXY(20, 3);
      display.PutStr(str, false);

      sprintf(str, "Current: %i", currentEncoder.Value());
      display.GotoXY(20, 4);
      display.PutStr(str, false);
   }

    _delay_ms(1);
  }
}

