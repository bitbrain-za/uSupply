/* 
* twi.cpp
*
* Created: 2016/07/20 10:41:52 AM
* Author: Philip
*/
#if 0
#include "../system.h"

#define DUMMY 0xFF

TWI_STATE twi::_error_state;
pin twi::SCL(&hal::portE, 4, true, true);
pin twi::SDA(&hal::portE, 5, true, true);

void twi::inititalise(void)
{
  SCL.Set();
  SDA.Set();

  SCL.set_output();
  SDA.set_output();

  USIDR = 0xFF;

  USICR = 0x00 |
          /* TWI Mode */
          (1 << USIWM1) | (0 << USIWM0) |
          /* Software strobe as clock source */
          (1<<USICS1) | (0 << USICS0) | (1 << USICLK) |
          (0<<USITC);

  counter_overflow_interrupt_disable();
  start_condition_interrupt_disable();

  ClearFlag(0xF0);
}

bool twi::SetAddress(U8 slave_address, bool ReadNotWrite)
{
  slave_address &= 0xFE;
  if(ReadNotWrite)
    slave_address |= 0x01;

  if(!SendStart())
  {
    return false;
  }
  SCL.Clear();

  TransferByte(slave_address);

  if(!ACK_received())
  {
    _error_state = USI_TWI_NO_ACK_ON_ADDRESS;
    return false;
  }

  return true;
}

bool twi::WriteBytes(U8 slave_address, U8 length, U8 *bytes)
{
  _error_state = USI_TWI_OKAY;
  if((bytes + length) > (unsigned char*)RAMEND)
  {
    _error_state = USI_TWI_DATA_OUT_OF_BOUND;
    return false;
  }

  if(length <= 0)
  {
    _error_state = USI_TWI_NO_DATA;
    return false;
  }

  if(!CheckForBusErrors())
    return false;

  if(!SetAddress(slave_address, false))
  {
    return false;
  }  

  do
  {
    SCL.Clear();
    TransferByte(*(bytes++));

    if(!ACK_received())
      return false;

  }while( --length);

  SendStop();

  return true;
}

bool twi::ReadBytes(U8 slave_address, U8 length, U8 *bytes)
{
  _error_state = USI_TWI_OKAY;

  if(!CheckForBusErrors())
    return false;

  if(!SetAddress(slave_address, true))
    return false;

  do
  {
    SDA.set_input();
    *(bytes++) = TransferByte(DUMMY);

    if( length == 1)
    {
      SendNACK();
    }
    else
    {
      SendACK();
    }
  }while( --length);
  
  SendStop();

  return true;
}

bool twi::WriteBytesToRegister(U8 slave_address, U8 reg, U8 length, U8 *bytes)
{
  _error_state = USI_TWI_OKAY;

  if((bytes + length) > (unsigned char*)RAMEND)
  {
    _error_state = USI_TWI_DATA_OUT_OF_BOUND;
    return false;
  }

  if(length <= 0)
  {
    _error_state = USI_TWI_NO_DATA;
    return false;
  }

  if(!CheckForBusErrors())
    return false;

  if(!SetAddress(slave_address, false))
  {
    return false;
  }  

  SCL.Clear();
  TransferByte(reg);

  if(!ACK_received())
    return false;

  do
  {
    SCL.Clear();
    TransferByte(*(bytes++));

    if(!ACK_received())
      return false;

  }while( --length);

  SendStop();

  return true;
}

bool twi::ReadBytesFromRegister(U8 slave_address, U8 reg, U8 length, U8 *bytes)
{
  _error_state = USI_TWI_OKAY;

  if(!CheckForBusErrors())
    return false;

  if(!SetAddress(slave_address, false))
  {
    return false;
  }  

  SCL.Clear();
  TransferByte(reg);

  if(!ACK_received())
    return false;

  if(!SetAddress(slave_address, true))
    return false;

  do
  {
    SDA.set_input();
    *(bytes++) = TransferByte(DUMMY);

    if( length == 1)
    {
      SendNACK();
    }
    else
    {
      SendACK();
    }
  }while( --length);
  
  SendStop();

  return true;
}

U8 twi::TransferBit(U8 b)
{
  USIDR = b;
  one_bit_mode();

  do
  {
    _delay_ms(10);              
    toggleClockPort();
    while(!SCL.Value());
    _delay_ms(10);              
    toggleClockPort();
  }while(!CheckFlag(TWI_FLAG_COUNTER_OVERFLOW));
  
  _delay_ms(10);
  U8 rx  = USIDR;
  USIDR = 0xFF;
  SDA.set_output();

  return rx;
}

bool twi::SendStop(void)
{
  SDA.Clear();
  SCL.Set();
  while(!SCL.Value());
  _delay_ms(10);               
  SDA.Set();
  _delay_ms(10);               
  
  if(!CheckFlag(TWI_FLAG_STOP_DETECTED))
  {
    _error_state = USI_TWI_MISSING_STOP_CON;    
    return false;
  }

  return true;
}

bool twi::SendStart(void)
{
  SCL.Set();

  while(!SCL.Value());
  _delay_ms(10);

  SDA.Clear();
  _delay_ms(10);                         
  SCL.Clear();
  SDA.Set();

  if(!CheckFlag(TWI_FLAG_START_DETECTED))
  {
    _error_state = USI_TWI_MISSING_START_CON;  
    return false;
  }

  return true;
}

bool twi::CheckAndClearFlag(TWI_FLAGS flag)
{
  if(CheckFlag(flag))
  {
    ClearFlag(flag);
    return true;
  }
  return false;
}

bool twi::ACK_received(void)
{
  SDA.set_input();

  if(TransferBit(DUMMY) & 0x01) 
  {
    _error_state = USI_TWI_NO_ACK_ON_DATA;
    return false;
  }
  return true;
}

void twi::SendACK()
{
  TransferBit(0x00);
}

void twi::SendNACK()
{
  TransferBit(0xFF);
}

U8 twi::TransferByte(U8 b)
{
  USIDR = b;
  eight_bit_mode();

  do
  {
    _delay_ms(10);               
    toggleClockPort();
    while(!SCL.Value());
    _delay_ms(10);
    toggleClockPort();
  }while(!CheckFlag(TWI_FLAG_COUNTER_OVERFLOW));
  
  _delay_ms(10);                
  U8 rx  = USIDR;
  USIDR = DUMMY;
  SDA.set_output();

  return rx;
}

bool twi::CheckForBusErrors()
{
  return true;

  if(CheckFlag(TWI_FLAG_START_DETECTED))
  {
    _error_state = USI_TWI_UE_START_CON;
    return false;
  }

  if(CheckFlag(TWI_FLAG_STOP_DETECTED))
  {
    _error_state = USI_TWI_UE_STOP_CON;
    return false;
  }

  if(CheckFlag(TWI_FLAG_DATA_COLLISION))
  {
    _error_state = USI_TWI_UE_DATA_COL;
    return false;
  }
  
  return true;
}

#endif