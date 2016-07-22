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
