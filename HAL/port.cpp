/* 
* port.cpp
*
* Created: 2016/07/20 11:14:52 AM
* Author: Philip
*/

#include "../system.h"

// default constructor
port::port(U8 addr, U8 ddr_addr, U8 pin)
{
  port_address = addr;
  ddr = ddr_addr; 
  PIN = pin;
} //port

// default destructor
port::~port()
{
} //~port

void port::SetInputs(U8 pins)
{
  ddr &= ~(pins);
}

void port::SetOutputs(U8 pins)
{
  ddr |= pins;
}

void port::SetPin(U8 pin)
{
  port_address |= (1<< pin);
}

void port::ClearPin(U8 pin)
{
  port_address &= ~(1 << pin);
}