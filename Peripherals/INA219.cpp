/* 
* INA219.cpp
*
* Created: 2016/07/20 3:17:38 PM
* Author: Philip
*/


#include "../system.h"

#if 0

// default constructor
INA219::INA219(BUS_VOLTAGE brng, ADC_SETTING adc_setting, OPERATING_MODE operating_mode)
{
  bus_voltage = brng;
  adc = adc_setting;
  mode = operating_mode;
  gain = PGA_GAIN_DIV8;

  Configure();

} //INA219

// default destructor
INA219::~INA219()
{
} //~INA219

bool INA219::Configure(BUS_VOLTAGE brng, PGA_GAIN pga_gain, ADC_SETTING adc_setting, OPERATING_MODE operating_mode)
{
  bus_voltage = brng;
  adc = adc_setting;
  mode = operating_mode;
  gain = pga_gain;

  return Configure();
}

bool INA219::Configure(BUS_VOLTAGE brng, ADC_SETTING adc_setting, OPERATING_MODE operating_mode)
{
  bus_voltage = brng;
  adc = adc_setting;
  mode = operating_mode;

  return Configure();
}

bool INA219::Configure()
{
  U16 config = 0x00;

  config |= bus_voltage << 13;
  config |= gain << 11;
  config |= adc << 3;
  config |= mode;

  return WriteRegister(INA219_CONFIG, config);
}
bool INA219::SetGain(PGA_GAIN pga_gain)
{
  gain = pga_gain;
  return Configure();
}

S16 INA219::Current()
{
  S16 shunt_register;
  ReadRegister(INA219_SHUNT_VOLTAGE, (U8 *)&shunt_register);
  return shunt_register;
}

#endif