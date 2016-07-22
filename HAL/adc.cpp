/* 
* adc.cpp
*
* Created: 2016/07/20 3:18:01 PM
* Author: Philip
*/

#include "../system.h"

void adc::init(void)
{
  /* Power ADC */
  PRR &= ~(1 << PRADC);

  /* Enable ADC */
  ADCSRA |= (1 << ADEN);

  DIDR0 = (1 << ADC2D) | (1 << ADC0D);

  /* Use AREF @ 2.048V */
  ReferenceSelection(REF_AREF);

  /* Right adjust result */
  ADMUX &= ~(1 << ADLAR);

  /* 500 kHz */
  PrescalerSelect(DIV16);
  ChannelSelect(ADC0);
}

void adc::ReferenceSelection(ADC_REFRENCE ref)
{
  ADMUX &= 0x3F;
  ADMUX |= ref;
}

void adc::ChannelSelect(ADC_CHANNEL_MUX channel)
{
  ADMUX &= ~0x1F;
  ADMUX |= channel;
}

void adc::PrescalerSelect(ADC_PRESCALER prescaler)
{
  ADCSRA &= ~0x07;
  ADCSRA |= prescaler;
}

U16 adc::Result(void)
{
  U16 res = 0;
  if((ADMUX & (1 << ADLAR)) == 0x00)
  {
    res = ADCL;
    res += (ADCH & 0x03) << 8;
  }
  else
  {
    res = (ADCH << 2);
    res += (ADCL >> 6) & 0x03;
  }
  return res;
}

U16 adc::DoConversion(ADC_CHANNEL_MUX channel)
{
  U16 result = 0;

  ChannelSelect(channel);
  ClearInterruptFlag();

  StartConversion();
  while(!ConversionComplete()) {;}

  result = Result();

  ClearInterruptFlag();

  return result;
}
