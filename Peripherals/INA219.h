/* 
* INA219.h
*
* Created: 2016/07/20 3:17:38 PM
* Author: Philip
*/


#ifndef __INA219_H__
#define __INA219_H__

typedef enum
{
  INA219_CONFIG = 0x00,
  INA219_SHUNT_VOLTAGE = 0x01,
  INA219_BUS_VOLTAGE = 0x02,
  INA219_POWER = 0x03,
  INA219_CURRENT = 0x04,
  INA219_CALIBRATION = 0x05,
}INA219_REGISTER;

typedef enum
{
  PGA_GAIN_DIV1 = 0x0,
  PGA_GAIN_DIV2 = 0x1,
  PGA_GAIN_DIV4 = 0x2,
  PGA_GAIN_DIV8 = 0x3,
}PGA_GAIN;

typedef enum
{
  BUSV_16V = 0,
  BUSV_32V = 1,
}BUS_VOLTAGE;

typedef enum
{
  ADC_BIT_9,
  ADC_BIT_10,
  ADC_BIT_11,
  ADC_BIT_12,
  ADC_SAMPLES_2,
  ADC_SAMPLES_4,
  ADC_SAMPLES_8,
  ADC_SAMPLES_16,
  ADC_SAMPLES_32,
  ADC_SAMPLES_64,
  ADC_SAMPLES_128,
}ADC_SETTING;

typedef enum
{
  POWER_DOWN = 0x0,
  SHUNT_VOLTAGE_TRIGGERED = 0x01,
  BUS_VOLTAGE_TRIGGERED = 0x02,
  SHUNT_AND_BUS_TRIGGERED = 0x03,
  ADC_OFF = 0x4,
  SHUNT_VOLTAGE_CONTINUOUS = 0x5,
  BUS_VOLTAGE_CONTINUOUS = 0x6,
  SHUNT_AND_BUS_CONTINUOUS = 0x7,
}OPERATING_MODE;

typedef enum
{
  RANGE_40MA = PGA_GAIN_DIV1,
  RANGE_80MA = PGA_GAIN_DIV2,
  RANGE_160MA = PGA_GAIN_DIV4,
  RANGE_320MA = PGA_GAIN_DIV8,
}RANGE;

class INA219
{
//variables
public:
protected:
private:
  static const U8 address = 0x80;
  
  PGA_GAIN gain;
  BUS_VOLTAGE bus_voltage;
  ADC_SETTING adc;
  OPERATING_MODE mode;

//functions
public:
	INA219(BUS_VOLTAGE brng, ADC_SETTING adc_setting, OPERATING_MODE operating_mode);
	~INA219();
protected:
private:
	INA219( const INA219 &c );
	INA219& operator=( const INA219 &c );

  bool ReadRegister(INA219_REGISTER reg, U8 *buffer) { return twi::ReadBytesFromRegister(address, reg, 2, buffer); }
  bool WriteRegister(INA219_REGISTER reg, U16 data) { return twi::WriteBytesToRegister(address, reg, 2, (U8 *)&data); }

  bool Reset() { return WriteRegister(INA219_CONFIG, 0x8000); }
  bool Configure(BUS_VOLTAGE brng, PGA_GAIN pga_gain, ADC_SETTING adc_setting, OPERATING_MODE operating_mode);
  bool Configure(BUS_VOLTAGE brng, ADC_SETTING adc_setting, OPERATING_MODE operating_mode);
  bool Configure();

  bool SetGain(PGA_GAIN pga_gain);

  S16 Current();

}; //INA219

#endif //__INA219_H__
