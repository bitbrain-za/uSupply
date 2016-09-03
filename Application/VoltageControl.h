/* 
* VoltageControl.h
*
* Created: 2016/09/02 9:28:04 PM
* Author: philb
*/


#ifndef __VOLTAGECONTROL_H__
#define __VOLTAGECONTROL_H__


class VoltageControl
{
//variables
public:
  U16 voltage_set(void) { return _voltage_set; }
  U16 voltage_read(void) { return _voltage_read; }
  bool Stable() { return _stable; }
  bool Dirty() { bool temp = _dirty; _dirty = false; return temp; }
  U16 StepSize;
  U16 max_voltage;
  U16 min_voltage;
  S8 calibration_value;

protected:
private:
  RotaryEncoder *_encoder;
  U16 _voltage_set;
  U16 _voltage_read;
  ADC_CHANNEL_MUX adc_channel;
  bool _stable;
  bool _dirty;
  MCP4716A0T dac;
  
//functions
public:
	VoltageControl(ADC_CHANNEL_MUX channel, RotaryEncoder *encoder);
	~VoltageControl();
  
  void Run(FSM_CONTROL control);

protected:
private:
	VoltageControl( const VoltageControl &c );
	VoltageControl& operator=( const VoltageControl &c );
  U16 read_voltage();
  bool IsVoltageCorrect();
  void SetVoltage(U16 val);

}; //VoltageControl

#endif //__VOLTAGECONTROL_H__
