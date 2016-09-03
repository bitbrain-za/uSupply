/* 
* adc.h
*
* Created: 2016/07/20 3:18:01 PM
* Author: Philip
*/


#ifndef __ADC_H__
#define __ADC_H__

typedef enum
{
  REF_AREF = 0x0,
  REF_AVCC = (1 << REFS0),
  REF_INTERNAL = (1 << REFS1) | (1 << REFS0),
}ADC_REFRENCE;

typedef enum
{
  ADC0 = 0x00,
  ADC1 = 0x01,
  ADC2 = 0x02,
  ADC3 = 0x03,
  ADC4 = 0x04,
  ADC5 = 0x05,
  ADC6 = 0x06,
  ADC7 = 0x07,
  DIF_0_1 = 0x10,
  DIF_1_1 = 0x11,
  DIF_2_1 = 0x12,
  DIF_3_1 = 0x13,
  DIF_4_1 = 0x14,
  DIF_5_1 = 0x15,
  DIF_6_1 = 0x16,
  DIF_7_1 = 0x17,
  DIF_0_2 = 0x18,
  DIF_1_2 = 0x19,
  DIF_2_2 = 0x1A,
  DIF_3_2 = 0x1B,
  DIF_4_2 = 0x1C,
  DIF_5_2 = 0x1D,
  ADC_VBG = 0x1E,
  ADC_GND = 0x1F, 
}ADC_CHANNEL_MUX;

typedef enum
{
  DIV2  = 0x0,
  DIV2b = 0x1,
  DIV4  = 0x2,
  DIV8  = 0x3,
  DIV16 = 0x4,
  DIV32 = 0x5,
  DIV64 = 0x6,
  DIV128 = 0x7,
}ADC_PRESCALER;

typedef enum
{
  ADC_TRIG_FREE_RUNNING = 0x0,
  ADC_TRIG_ANALOG_COMPARATOR = 0x1,
  ADC_TRIG_EXTERNAL_IRQ0 = 0x2,
  ADC_TRIG_TC0_COMPARE_MATCH = 0x3,
  ADC_TRIG_TC0_OVERFLOW = 0x4,
  ADC_TRIG_TC1_COMPARE_MATCH = 0x5,
  ADC_TRIG_TC1_OVERFLOW = 0x6,
  ADC_TRIG_TC1_CAPTURE_EVENT = 0x7,
}ADC_TRIGGER;

class adc
{
//variables
public:
protected:
private:

//functions
public:
  static void init(void);
  static U16 DoConversion(ADC_CHANNEL_MUX channel);

  static S16 ReadTemperature();
protected:
private:
  /* ADMUX */
  static void ReferenceSelection(ADC_REFRENCE ref);
  static void ChannelSelect(ADC_CHANNEL_MUX channel);

  /* ADCSRA */
  static void StartConversion(void) { ADCSRA |= (1 << ADSC); }
  static void AutoTrigger(bool enable) { if(enable) ADCSRA |= (1 << ADATE); else ADCSRA &= ~(1 << ADATE); }
  static bool ConversionComplete(void) { return ((ADCSRA & (1 << ADIF)) == (1 << ADIF)); }
  static void ClearInterruptFlag(void) { ADCSRA |= (1 << ADIF); }
  static void EnableInterrupt(void) { ADCSRA |= (1 << ADIE); }
  static void DisableInterrupt(void) { ADCSRA &= ~(1 << ADIE); }
  static void PrescalerSelect(ADC_PRESCALER prescaler);

  /* ADCL & ADCH */
  static U16 Result(void);

  /* ADCSRB */
  static void SetTrigger(ADC_TRIGGER trigger) { ADCSRB &= ~0x03; ADCSRB |= trigger; }


}; //adc

#endif //__ADC_H__
