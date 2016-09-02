/* 
* adc.cpp
*
* Created: 2016/07/20 3:18:01 PM
* Author: Philip
*/

#include "../system.h"

/*
Pin | Channel |   Signal
60  |   ADC1  |   Temperature
59  |   ADC2  |   Voltage Out
62  |   VREF  |   2.048V
*/

#define VOLTAGE_SENSE  ADC2
#define TEMPERATURE ADC1
#define VREF 2048
#define MAX 1024

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
  ChannelSelect(ADC1);
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

U16 adc::ReadVoltageOutput_mV()
{
  U16 val = DoConversion(VOLTAGE_SENSE);

  /* ADC = Vin*MAX/Vref 
         = Vin / 2 
     
     Vin(mV) = ADC * 2
         
      Vout = Vin * 10
           = ADC * 20 mV
         */

  /* 10x Voltage divider, MAX/Vref = 1/2 */
  return val * 20;
}

S16 adc::ReadTemperature()
{
  /* ADC = Vin*MAX/Vref 
         = Vin / 2 
     
     Vin = ADC * 2    
     
     Vin = Temp(10mV/Deg) + 500 mV 

     Temp(10mV/deg) = (Vin - 500mV)



     Temp = ((ADC*2) - 500) / 10

         */

  U16 val = DoConversion(TEMPERATURE);
  S16 temp = ((2 * val) - 500);
  return (temp / 10);

}