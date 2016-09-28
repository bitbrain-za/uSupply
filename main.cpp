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
Desktop desktop;

int main(void)
{
  U16 delay_counter = 0;
  U16 voltage_read = 0;
  char str[32];
  SystemClock::init();
  U16 voltage = 0;
  timer led_timer;
  led_timer.init(500);
  led_timer.Start();

  hal::board_init();
  twi::inititalise();
  currentEncoder.FSM(true);
  houseKeeper.Run(RESET);
  voltageControl.Run(RESET);
  desktop.FSM(RESET);

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
    }

    currentEncoder.FSM(false);
    voltageControl.Run(NO_RESET);

    if(voltageControl.Dirty())
    {
      desktop.voltage_read = voltageControl.voltage_read();
      desktop.commandQueue.Send(DSKTP_CMD_REFRESH);
   }

    desktop.FSM(NO_RESET);
    _delay_ms(1);
  }
}

