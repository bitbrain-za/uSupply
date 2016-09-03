/* 
* VoltageControl.cpp
*
* Created: 2016/09/02 9:28:04 PM
* Author: philb
*/

#include "../system.h"

// default constructor
VoltageControl::VoltageControl(ADC_CHANNEL_MUX channel, RotaryEncoder *encoder) : dac(VREF_VREFPIN, DAC_GAIN_1X)
{
  adc_channel = channel;
  _encoder = encoder;
  StepSize = 50;
  max_voltage = 12000;
  min_voltage = 660;
  _dirty = false;
  _voltage_set = 660;
  calibration_value = 30;
} //VoltageControl

// default destructor
VoltageControl::~VoltageControl()
{
} //~VoltageControl

void VoltageControl::Run(FSM_CONTROL control)
{
  static U8 unstable_count;
  S16 change;
  static U16 previous_voltage;

  if(control == RESET)
  {
    _encoder->FSM(true);
    dac.SetReference(VREF_VREFPIN, 2048);

    SetVoltage(config.SavedVoltage());
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
      _voltage_set += change;
      if(_voltage_set > max_voltage)
        _voltage_set = max_voltage;
    }
    else
    {
      change = -change;
      if((U16)change > _voltage_set)
        _voltage_set = 660;
      else
      {
        _voltage_set -= change;
        if(_voltage_set < min_voltage)
          _voltage_set = min_voltage;
      }
    }
    change = 0;
    SetVoltage(_voltage_set);
  }
  else
  {
    read_voltage();

    if(previous_voltage > _voltage_read)
    {
      if((previous_voltage - _voltage_read) > 20)
        _dirty = true;
    }
    else if(previous_voltage < _voltage_read)
    {
      if((_voltage_read - previous_voltage) > 20)
        _dirty = true;
    }
    previous_voltage = _voltage_read;

    if(!IsVoltageCorrect())
    {
      unstable_count++;

      if(unstable_count++ > 12)
      {
        if(_stable)
        {
          _stable = false;
          _dirty = true;
        }
      }
      SetVoltage(_voltage_set);
    }
    else
    {
      if(unstable_count > 0)
        unstable_count--;
      else
      {
        if(!_stable)
        {
          _dirty = true;
          _stable = true;
        }
      }
    }
  }
}

void VoltageControl::SetVoltage(U16 val)
{
  _voltage_set = val;
  config.ram_voltage = _voltage_set;
  dac.SetVoltage(val);
}

U16 VoltageControl::read_voltage()
{
  /* ADC = Vin*MAX/Vref 
         = Vin / 2 
     
     Vin(mV) = ADC * 2
         
      Vout = Vin * 10
           = ADC * 20 mV

    10x Voltage divider, MAX/Vref = 1/2 
  */

  U16 val = adc::DoConversion(adc_channel);
  _voltage_read = val * 20;
  _voltage_read += calibration_value;
  return _voltage_read;
}

bool VoltageControl::IsVoltageCorrect()
{
  if(_voltage_set > _voltage_read)
  {
    if( (_voltage_set - _voltage_read) > 20 )
      return false;
  }
  if(_voltage_read > _voltage_set)
  {
    if( (_voltage_read - _voltage_set) > 20 )
      return false;
  }
  return true;
}