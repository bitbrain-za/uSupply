/* 
* hal.cpp
*
* Created: 2016/07/20 10:41:39 AM
* Author: Philip
*/


#include "../system.h"

port hal::portA(&PORTA, &DDRA, &PINA);
port hal::portB(&PORTB, &DDRB, &PINB);
port hal::portC(&PORTC, &DDRC, &PINC);
port hal::portD(&PORTD, &DDRD, &PIND);
port hal::portE(&PORTE, &DDRE, &PINE);
port hal::portF(&PORTF, &DDRF, &PINF);
port hal::portG(&PORTG, &DDRG, &PING);

pin hal::climitLed(&hal::portC, 4, true, true);

ADC_CHANNEL_MUX hal::VoltageChannel = ADC0;
ADC_CHANNEL_MUX hal::TemperatureChannel = ADC1;

void hal::board_init(void)
{
  portB.SetOutputs((1 << PB4) | (1 << PB5)); 
  portB.SetInputs(1 << PB6);

  portC.SetOutputs(0x3F);
  portC.SetInputs(0xC0);

  portD.SetOutputs(0xFF);
  portD.SetInputs(0x00);

  portE.SetOutputs((1 << PE3));

  /* Programming pins? */
  portF.SetInputs((1 << PF0) | (1 << PF1) | (1 << PF2));
  
  portG.SetOutputs((1 << PG0) | (1 << PG1));
}
