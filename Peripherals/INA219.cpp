/* 
* INA219.cpp
*
* Created: 2016/07/20 3:17:38 PM
* Author: Philip
*/


#include "../system.h"

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
  S16 shunt_register = ReadRegister(INA219_SHUNT_VOLTAGE);
  return shunt_register;
}

U16 INA219::ReadRegister(INA219_REGISTER reg)
{ 
  U8 buffer[2];

  twi::WriteBytes(DeviceAddress, (U8 *)&reg, 1);
  twi::ReadBytes(DeviceAddress, buffer, 2);

  U16 val = buffer[0] << 8;
  val |= buffer[1];

  return val;
}

bool INA219::WriteRegister(INA219_REGISTER reg, U16 data) 
{ 
  if( !((reg == INA219_CONFIG) || (reg == INA219_CALIBRATION)) )
    return false;
  
  U8 buffer[3];
  buffer[0] = (U8)reg;
  buffer[1] = ((data >> 8) & 0xFF);
  buffer[2] = (data & 0xFF);

  return twi::WriteBytes(DeviceAddress, buffer, 3);
}
