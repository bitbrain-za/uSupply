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
EEPROM_24LC08 eeprom(&hal::portE, 3);
MCP4716A0T dac(VREF_VREFPIN, DAC_GAIN_1X);

int main(void)
{
  U16 delay_counter = 0;
  char str[32];
  char eeprom_str[32];
  LM6029ACW display;
  SystemClock::init();
  float voltage = 0;

  hal::board_init();
  twi::inititalise();
  //twi_master::USI_TWI_Master_Initialise();
  display.init();

  display.ClearScreen(false);
  currentEncoder.FSM(true);
  voltageEncoder.FSM(true);

  dac.SetReference(VREF_VREFPIN, 2.048);
  dac.SetVoltage(2.047);

  while (1) 
  {
    if(delay_counter++ >= 1000)
    {
      if(voltage < 2.048)
        voltage += 0.200;
      else
        voltage = 0;

      delay_counter = 0;
      dac.SetVoltage(voltage);
      hal::climitLed.Toggle();
    }

    currentEncoder.FSM(false);
    voltageEncoder.FSM(false);
    if(voltageEncoder.Changed() || currentEncoder.Changed() || (delay_counter == 0))
    {
      display.ClearScreen(false);
      sprintf(str, "Voltage: %i", voltageEncoder.Value());
      display.GotoXY(20, 3);
      display.PutStr(str, false);

      sprintf(str, "Current: %i", currentEncoder.Value());
      display.GotoXY(20, 4);
      display.PutStr(str, false);

      display.GotoXY(20, 2);
//      display.PutStr(eeprom_str, false);
   }

    _delay_ms(1);
  }
}

